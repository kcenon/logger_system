/**
 * @file log_entry_fuzzer.cpp
 * @brief libFuzzer harness for the log_entry parse/serialize surface.
 *
 * @details
 * This harness exercises a deterministic, self-contained parse + serialize
 * round-trip over the public ::kcenon::logger::log_entry type defined in
 * include/kcenon/logger/interfaces/log_entry.h.
 *
 * The fuzzer:
 *   1. Deserializes arbitrary fuzzer-provided bytes into a log_entry
 *      (field splitting, log_level parsing, line-number parsing, optional
 *       file/function handling) -- the "parser" surface.
 *   2. Serializes the resulting log_entry back into a single structured
 *      text line with escaping of control / delimiter characters
 *      -- the "serialization" surface.
 *
 * Both routines are implemented locally against the real public type so the
 * harness compiles against the repository's tracked headers only and does not
 * depend on internal sink/formatter sources that are not part of this module's
 * public surface.
 *
 * Build is gated behind BUILD_FUZZERS and is compiled with
 * clang -fsanitize=fuzzer,address. It does not affect the default build.
 */

#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/logger_types.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace {

using ::kcenon::logger::log_entry;
using ::kcenon::logger::log_level;

// Field delimiter used by the toy structured format exercised here.
constexpr char kFieldDelimiter = '\x1f';  // ASCII unit separator.

// Map an arbitrary integer onto a valid log_level, saturating into range so
// the parser never produces an out-of-range enum value.
log_level parse_level(long value) {
    if (value < static_cast<long>(log_level::trace)) {
        return log_level::trace;
    }
    if (value > static_cast<long>(log_level::off)) {
        return log_level::off;
    }
    return static_cast<log_level>(value);
}

// Parse a (possibly malformed) decimal integer from a view without throwing.
// Returns 0 on any parse failure; clamps to int range.
int parse_int(std::string_view sv) {
    long acc = 0;
    bool negative = false;
    std::size_t i = 0;
    if (i < sv.size() && (sv[i] == '-' || sv[i] == '+')) {
        negative = (sv[i] == '-');
        ++i;
    }
    bool any = false;
    for (; i < sv.size(); ++i) {
        const char c = sv[i];
        if (c < '0' || c > '9') {
            break;
        }
        any = true;
        acc = acc * 10 + (c - '0');
        // Saturate to avoid signed overflow UB.
        if (acc > 2000000000L) {
            acc = 2000000000L;
        }
    }
    if (!any) {
        return 0;
    }
    return static_cast<int>(negative ? -acc : acc);
}

// Deserialize fuzzer bytes into a log_entry.
//
// Wire shape (delimiter = 0x1f):
//   level <DEL> line <DEL> logger_name <DEL> message <DEL> file <DEL> function
// Missing trailing fields are treated as empty / absent. This intentionally
// tolerates truncated and oversized inputs the way a real parser must.
log_entry deserialize(const std::uint8_t* data, std::size_t size) {
    const std::string_view input(reinterpret_cast<const char*>(data), size);

    std::vector<std::string_view> fields;
    std::size_t start = 0;
    for (std::size_t i = 0; i <= input.size(); ++i) {
        if (i == input.size() || input[i] == kFieldDelimiter) {
            fields.emplace_back(input.substr(start, i - start));
            start = i + 1;
        }
    }

    log_entry entry{};
    entry.timestamp = std::chrono::system_clock::time_point{};

    const auto field = [&](std::size_t idx) -> std::string_view {
        return idx < fields.size() ? fields[idx] : std::string_view{};
    };

    entry.level = parse_level(parse_int(field(0)));
    entry.line = parse_int(field(1));
    entry.logger_name = std::string(field(2));
    entry.message = std::string(field(3));

    const std::string_view file_sv = field(4);
    if (!file_sv.empty()) {
        entry.file = std::string(file_sv);
    }
    const std::string_view function_sv = field(5);
    if (!function_sv.empty()) {
        entry.function = std::string(function_sv);
    }

    return entry;
}

// Append a string with control / delimiter characters escaped so the produced
// line is unambiguous. This is the serialization / formatting surface.
void append_escaped(std::string& out, std::string_view in) {
    static const char* const hex = "0123456789abcdef";
    for (const char ch : in) {
        const unsigned char uc = static_cast<unsigned char>(ch);
        if (uc < 0x20 || uc == 0x7f || ch == kFieldDelimiter || ch == '\\') {
            out += '\\';
            out += 'x';
            out += hex[(uc >> 4) & 0xf];
            out += hex[uc & 0xf];
        } else {
            out += ch;
        }
    }
}

// Serialize a log_entry to a single structured line.
std::string serialize(const log_entry& entry) {
    std::string out;
    out.reserve(entry.message.size() + entry.logger_name.size() + 64);

    out += "level=";
    out += std::to_string(static_cast<int>(entry.level));
    out += " line=";
    out += std::to_string(entry.line);
    out += " logger=\"";
    append_escaped(out, entry.logger_name);
    out += "\" msg=\"";
    append_escaped(out, entry.message);
    out += '"';
    if (entry.file.has_value()) {
        out += " file=\"";
        append_escaped(out, *entry.file);
        out += '"';
    }
    if (entry.function.has_value()) {
        out += " func=\"";
        append_escaped(out, *entry.function);
        out += '"';
    }
    return out;
}

}  // namespace

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data,
                                      std::size_t size) {
    const log_entry entry = deserialize(data, size);
    const std::string line = serialize(entry);

    // Touch the result so the optimizer cannot elide the work, and assert a
    // basic structural invariant the serializer must always uphold.
    if (!line.empty() && line.find("msg=") == std::string::npos) {
        // Unreachable by construction; serves as a self-check for the harness.
        __builtin_trap();
    }
    return 0;
}
