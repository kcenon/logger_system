# Fuzzing

libFuzzer harnesses for `logger_system`. These targets are **off by default**
and are gated behind the `BUILD_FUZZERS` CMake option. They require a Clang
toolchain (libFuzzer is a Clang feature) and build with
`-fsanitize=fuzzer,address`.

## Harnesses

| Target             | Surface fuzzed                                                       |
|--------------------|---------------------------------------------------------------------|
| `log_entry_fuzzer` | Deserialize bytes -> `kcenon::logger::log_entry`, then serialize it back to a structured, escaped text line. Exercises field splitting, `log_level`/line-number parsing, optional-field handling, and control-character escaping. |

The harness compiles only against the module's tracked public headers
(`include/kcenon/logger/interfaces/`), so it does not depend on internal
sink/formatter sources.

## Build

```sh
cmake -B build -G Ninja \
  -DBUILD_FUZZERS=ON \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++
cmake --build build --target log_entry_fuzzer
```

## Run

```sh
# Replay/extend the checked-in corpus, capped at 60s.
./build/fuzz/log_entry_fuzzer -max_total_time=60 fuzz/corpus
```

Any crash input is written to the working directory as `crash-<hash>`; attach it
to a bug report to reproduce deterministically.

## Corpus

`corpus/` holds small seed inputs covering: a fully-populated entry, an entry
with no optional fields, and an entry with out-of-range level and embedded
control characters. The field delimiter in the toy wire format is the ASCII
unit separator (`0x1f`).
