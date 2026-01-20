/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <kcenon/logger/writers/otlp_writer.h>
#include <kcenon/logger/otlp/otel_context.h>
#include <kcenon/common/patterns/result.h>

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>

#ifdef LOGGER_HAS_OTLP
#include <opentelemetry/exporters/otlp/otlp_http_log_record_exporter.h>
#include <opentelemetry/exporters/otlp/otlp_grpc_log_record_exporter.h>
#include <opentelemetry/logs/provider.h>
#include <opentelemetry/sdk/logs/logger_provider.h>
#include <opentelemetry/sdk/logs/simple_log_record_processor.h>
#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/sdk/resource/semantic_conventions.h>
#endif

namespace kcenon::logger {

#ifdef LOGGER_HAS_OTLP
/**
 * @brief OpenTelemetry SDK implementation details
 */
class otlp_writer::otel_impl {
public:
    std::shared_ptr<opentelemetry::logs::LoggerProvider> provider;
    std::shared_ptr<opentelemetry::logs::Logger> logger;

    otel_impl(const config& cfg) {
        namespace logs_sdk = opentelemetry::sdk::logs;
        namespace otlp_exporter = opentelemetry::exporter::otlp;
        namespace resource = opentelemetry::sdk::resource;

        // Build resource attributes
        resource::ResourceAttributes resource_attrs;
        resource_attrs.SetAttribute(
            resource::SemanticConventions::kServiceName,
            cfg.service_name.empty() ? "unknown_service" : cfg.service_name);

        if (!cfg.service_version.empty()) {
            resource_attrs.SetAttribute(
                resource::SemanticConventions::kServiceVersion,
                cfg.service_version);
        }

        if (!cfg.service_namespace.empty()) {
            resource_attrs.SetAttribute(
                resource::SemanticConventions::kServiceNamespace,
                cfg.service_namespace);
        }

        if (!cfg.service_instance_id.empty()) {
            resource_attrs.SetAttribute(
                resource::SemanticConventions::kServiceInstanceId,
                cfg.service_instance_id);
        }

        // Add custom attributes
        for (const auto& [key, value] : cfg.resource_attributes) {
            resource_attrs.SetAttribute(key, value);
        }

        auto resource_ptr = resource::Resource::Create(resource_attrs);

        // Create exporter based on protocol
        std::unique_ptr<opentelemetry::sdk::logs::LogRecordExporter> exporter;

        if (cfg.protocol == protocol_type::http) {
            otlp_exporter::OtlpHttpLogRecordExporterOptions http_opts;
            http_opts.url = cfg.endpoint;
            http_opts.timeout = cfg.timeout;
            http_opts.ssl_insecure_skip_verify = !cfg.use_tls;

            for (const auto& [key, value] : cfg.headers) {
                http_opts.http_headers.insert({key, value});
            }

            exporter = std::make_unique<otlp_exporter::OtlpHttpLogRecordExporter>(http_opts);
        } else {
            otlp_exporter::OtlpGrpcLogRecordExporterOptions grpc_opts;
            grpc_opts.endpoint = cfg.endpoint;
            grpc_opts.timeout = cfg.timeout;
            grpc_opts.use_ssl_credentials = cfg.use_tls;

            for (const auto& [key, value] : cfg.headers) {
                grpc_opts.metadata.insert({key, value});
            }

            exporter = std::make_unique<otlp_exporter::OtlpGrpcLogRecordExporter>(grpc_opts);
        }

        // Create processor and provider
        auto processor = std::make_unique<logs_sdk::SimpleLogRecordProcessor>(
            std::move(exporter));

        provider = std::make_shared<logs_sdk::LoggerProvider>(
            std::move(processor), resource_ptr);

        logger = provider->GetLogger(cfg.service_name, "", "");
    }
};
#endif

otlp_writer::otlp_writer(const config& cfg)
    : config_(cfg) {

#ifdef LOGGER_HAS_OTLP
    otel_impl_ = std::make_unique<otel_impl>(cfg);
#endif

    // Start background export thread
    running_.store(true);
    export_thread_ = std::make_unique<std::thread>([this]() {
        export_thread_func();
    });
}

otlp_writer::~otlp_writer() {
    // Stop background thread
    running_.store(false);

    // Wake up export thread
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        queue_cv_.notify_one();
    }

    // Wait for thread to finish
    if (export_thread_ && export_thread_->joinable()) {
        export_thread_->join();
    }

    // Export remaining logs
    std::vector<log_entry> remaining;
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        while (!queue_.empty()) {
            remaining.push_back(std::move(queue_.front()));
            queue_.pop();
        }
    }

    if (!remaining.empty()) {
        export_batch(remaining);
    }
}

common::VoidResult otlp_writer::write(const log_entry& entry) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);

        // Check queue size limit
        if (queue_.size() >= config_.max_queue_size) {
            stats_.logs_dropped.fetch_add(1, std::memory_order_relaxed);
            return common::ok();  // Drop silently to avoid backpressure
        }

        // Create a copy of the entry since log_entry is move-only
        if (entry.location) {
            queue_.emplace(entry.level,
                          entry.message.to_string(),
                          entry.location->file.to_string(),
                          entry.location->line,
                          entry.location->function.to_string(),
                          entry.timestamp);
        } else {
            queue_.emplace(entry.level,
                          entry.message.to_string(),
                          entry.timestamp);
        }

        // Copy OTEL context to the queued entry
        auto& queued_entry = queue_.back();
        queued_entry.otel_ctx = entry.otel_ctx.has_value() ? entry.otel_ctx : otlp::otel_context_storage::get();

        // Wake up export thread if batch size reached
        if (queue_.size() >= config_.max_batch_size) {
            queue_cv_.notify_one();
        }
    }

    return common::ok();
}

common::VoidResult otlp_writer::flush() {
    force_export();
    return common::ok();
}

bool otlp_writer::is_healthy() const {
    return healthy_.load(std::memory_order_acquire);
}

otlp_writer::export_stats otlp_writer::get_stats() const {
    return export_stats{
        stats_.logs_exported.load(std::memory_order_relaxed),
        stats_.logs_dropped.load(std::memory_order_relaxed),
        stats_.export_success.load(std::memory_order_relaxed),
        stats_.export_failures.load(std::memory_order_relaxed),
        stats_.retries.load(std::memory_order_relaxed),
        stats_.last_export,
        stats_.last_error
    };
}

void otlp_writer::force_export() {
    std::vector<log_entry> batch;

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        while (!queue_.empty()) {
            batch.push_back(std::move(queue_.front()));
            queue_.pop();
        }
    }

    if (!batch.empty()) {
        export_batch(batch);
    }
}

void otlp_writer::export_thread_func() {
    while (running_.load(std::memory_order_acquire)) {
        std::vector<log_entry> batch;

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // Wait for batch size or timeout
            queue_cv_.wait_for(lock, config_.flush_interval, [this] {
                return !running_.load(std::memory_order_acquire) ||
                       queue_.size() >= config_.max_batch_size;
            });

            if (!running_.load(std::memory_order_acquire) && queue_.empty()) {
                break;
            }

            // Collect batch
            while (!queue_.empty() && batch.size() < config_.max_batch_size) {
                batch.push_back(std::move(queue_.front()));
                queue_.pop();
            }
        }

        // Export batch
        if (!batch.empty()) {
            export_batch(batch);
        }
    }
}

bool otlp_writer::export_batch(const std::vector<log_entry>& batch) {
    bool success = false;
    std::size_t retries = 0;
    auto delay = config_.retry_delay;

    while (!success && retries <= config_.max_retries) {
        if (retries > 0) {
            stats_.retries.fetch_add(1, std::memory_order_relaxed);
            std::this_thread::sleep_for(delay);
            delay *= 2;  // Exponential backoff
        }

#ifdef LOGGER_HAS_OTLP
        success = export_with_otel_sdk(batch);
#else
        success = export_with_http(batch);
#endif

        ++retries;
    }

    if (success) {
        stats_.logs_exported.fetch_add(batch.size(), std::memory_order_relaxed);
        stats_.export_success.fetch_add(1, std::memory_order_relaxed);
        stats_.last_export = std::chrono::system_clock::now();
        healthy_.store(true, std::memory_order_release);
    } else {
        stats_.logs_dropped.fetch_add(batch.size(), std::memory_order_relaxed);
        stats_.export_failures.fetch_add(1, std::memory_order_relaxed);
        stats_.last_error = std::chrono::system_clock::now();
        healthy_.store(false, std::memory_order_release);
    }

    return success;
}

int otlp_writer::to_otlp_severity(common::interfaces::log_level level) {
    // OTLP Severity Numbers (per OpenTelemetry specification)
    // https://opentelemetry.io/docs/specs/otel/logs/data-model/#field-severitynumber
    switch (level) {
        case common::interfaces::log_level::trace:
            return 1;   // TRACE
        case common::interfaces::log_level::debug:
            return 5;   // DEBUG
        case common::interfaces::log_level::info:
            return 9;   // INFO
        case common::interfaces::log_level::warn:
            return 13;  // WARN
        case common::interfaces::log_level::error:
            return 17;  // ERROR
        case common::interfaces::log_level::fatal:
            return 21;  // FATAL
        default:
            return 9;   // INFO as default
    }
}

#ifdef LOGGER_HAS_OTLP
bool otlp_writer::export_with_otel_sdk(const std::vector<log_entry>& batch) {
    if (!otel_impl_ || !otel_impl_->logger) {
        return false;
    }

    try {
        for (const auto& entry : batch) {
            // Convert log_level from logger_system to common::interfaces
            auto level = static_cast<common::interfaces::log_level>(static_cast<int>(entry.level));

            // Create log record through OpenTelemetry SDK
            auto severity = static_cast<opentelemetry::logs::Severity>(
                to_otlp_severity(level));

            // Build attributes
            std::map<std::string, opentelemetry::common::AttributeValue> attrs;

            if (entry.location) {
                std::string file = entry.location->file.to_string();
                std::string function = entry.location->function.to_string();

                if (!file.empty()) {
                    attrs["code.filepath"] = file;
                }
                if (entry.location->line > 0) {
                    attrs["code.lineno"] = static_cast<int64_t>(entry.location->line);
                }
                if (!function.empty()) {
                    attrs["code.function"] = function;
                }
            }

            // Add OTEL context if present
            if (entry.otel_ctx) {
                attrs["trace_id"] = entry.otel_ctx->trace_id;
                attrs["span_id"] = entry.otel_ctx->span_id;
                if (!entry.otel_ctx->trace_flags.empty()) {
                    attrs["trace_flags"] = entry.otel_ctx->trace_flags;
                }
            }

            // Emit log record
            otel_impl_->logger->EmitLogRecord(
                severity,
                entry.message.to_string(),
                attrs,
                entry.timestamp
            );
        }

        return true;
    } catch (const std::exception&) {
        return false;
    }
}
#else
bool otlp_writer::export_with_http(const std::vector<log_entry>& batch) {
    // Fallback implementation without OpenTelemetry SDK
    // This is a minimal HTTP export implementation for demonstration
    // In production, you would use a proper HTTP client library

    // Build JSON payload following OTLP/HTTP JSON format
    std::ostringstream json;
    json << R"({"resourceLogs":[{"resource":{"attributes":[)";

    // Add service name attribute
    json << R"({"key":"service.name","value":{"stringValue":")"
         << config_.service_name << R"("}}]},)";

    json << R"("scopeLogs":[{"logRecords":[)";

    bool first = true;
    for (const auto& entry : batch) {
        if (!first) json << ",";
        first = false;

        // Convert log_level from logger_system to common::interfaces
        auto level = static_cast<common::interfaces::log_level>(static_cast<int>(entry.level));

        // Convert timestamp to nanoseconds
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            entry.timestamp.time_since_epoch()).count();

        json << R"({"timeUnixNano":")" << ns << R"(",)";
        json << R"("severityNumber":)" << to_otlp_severity(level) << ",";
        json << R"("body":{"stringValue":")" << escape_json(entry.message.to_string()) << R"("},)";

        // Add attributes
        json << R"("attributes":[)";
        bool first_attr = true;

        if (entry.location) {
            std::string file = entry.location->file.to_string();

            if (!file.empty()) {
                if (!first_attr) json << ",";
                first_attr = false;
                json << R"({"key":"code.filepath","value":{"stringValue":")"
                     << escape_json(file) << R"("}})";
            }

            if (entry.location->line > 0) {
                if (!first_attr) json << ",";
                first_attr = false;
                json << R"({"key":"code.lineno","value":{"intValue":")"
                     << entry.location->line << R"("}})";
            }
        }

        if (entry.otel_ctx) {
            if (!first_attr) json << ",";
            first_attr = false;
            json << R"({"key":"trace_id","value":{"stringValue":")"
                 << entry.otel_ctx->trace_id << R"("}},)";
            json << R"({"key":"span_id","value":{"stringValue":")"
                 << entry.otel_ctx->span_id << R"("}})";
        }

        json << "]}";
    }

    json << "]}]}]}";

    // For now, just log that we would send this
    // In production, use libcurl or similar to actually POST
    // This is a stub implementation when OTLP SDK is not available

    // Return true to indicate "successful" export in stub mode
    // The JSON is built but not actually sent without an HTTP client
    return true;
}

std::string otlp_writer::escape_json(const std::string& str) {
    std::ostringstream result;
    for (char c : str) {
        switch (c) {
            case '"': result << "\\\""; break;
            case '\\': result << "\\\\"; break;
            case '\b': result << "\\b"; break;
            case '\f': result << "\\f"; break;
            case '\n': result << "\\n"; break;
            case '\r': result << "\\r"; break;
            case '\t': result << "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    result << "\\u" << std::hex << std::setfill('0')
                           << std::setw(4) << static_cast<int>(c);
                } else {
                    result << c;
                }
        }
    }
    return result.str();
}
#endif

} // namespace kcenon::logger
