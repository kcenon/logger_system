---
doc_id: "LOG-API-002b"
doc_title: "API Reference - Writers"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "API"
---

> **Language:** **English** | [한국어](API_REFERENCE.kr.md)

# API Reference - Writers

> **Split from**: [API_REFERENCE.md](API_REFERENCE.md)

**Version**: 0.5.0.0
**Last Updated**: 2026-02-08

## Table of Contents
- [Writers](#writers)
- [Filters](#filters)
- [Formatters](#formatters)
- [Log Sampling (v3.3.0)](#log-sampling-v330)
- [Real-time Analysis (v3.2.0)](#real-time-analysis-v320)
- [Backend Integration](#backend-integration)
- [Monitoring Integration](#monitoring-integration)
- [Usage Examples](#usage-examples)
- [Thread Safety](#thread-safety)
- [Performance Considerations](#performance-considerations)
- [Migration from v2.x](#migration-from-v2x)

## Writers

### `kcenon::logger::console_writer`

```cpp
class console_writer : public base_writer {
public:
    console_writer(bool use_color = true);
    void set_use_color(bool use_color);
};
```

### `kcenon::logger::file_writer`

```cpp
class file_writer : public base_writer {
public:
    file_writer(const std::string& filename);
    common::VoidResult open();
    common::VoidResult close();
};
```

### `kcenon::logger::rotating_file_writer`

```cpp
class rotating_file_writer : public base_writer {
public:
    enum class rotation_type {
        size_based,
        time_based,
        daily,
        hourly
    };

    rotating_file_writer(const std::string& filename,
                         std::size_t max_size,
                         std::size_t max_files);

    rotating_file_writer(const std::string& filename,
                         rotation_type type,
                         std::size_t max_files);
};
```

### `kcenon::logger::network_writer`

```cpp
class network_writer : public base_writer {
public:
    enum class protocol_type { tcp, udp };

    network_writer(const std::string& host,
                   uint16_t port,
                   protocol_type protocol = protocol_type::tcp);
};
```

### `kcenon::logger::critical_writer`

Synchronous writer for critical messages that bypass async queue:

```cpp
class critical_writer : public base_writer {
public:
    critical_writer(std::unique_ptr<base_writer> underlying_writer);
};
```

### `kcenon::logger::batch_writer`

Wraps another writer with batching support:

```cpp
class batch_writer : public base_writer {
public:
    struct config {
        std::size_t max_batch_size = 100;
        std::chrono::milliseconds flush_interval{1000};
    };

    batch_writer(std::unique_ptr<base_writer> underlying_writer,
                 const config& cfg = {});
};
```

## Filters

### `kcenon::logger::filters::level_filter`

```cpp
class level_filter : public log_filter_interface {
public:
    level_filter(logger_system::log_level min_level);
};
```

### `kcenon::logger::filters::regex_filter`

```cpp
class regex_filter : public log_filter_interface {
public:
    regex_filter(const std::string& pattern, bool include = true);
};
```

### `kcenon::logger::filters::function_filter`

```cpp
class function_filter : public log_filter_interface {
public:
    using filter_func = std::function<bool(const log_entry&)>;
    function_filter(filter_func func);
};
```

### `kcenon::logger::filters::composite_filter`

```cpp
class composite_filter : public log_filter_interface {
public:
    enum class logic_type { AND, OR };

    composite_filter(logic_type logic = logic_type::AND);
    void add_filter(std::unique_ptr<log_filter_interface> filter);
};
```

## Formatters

### `kcenon::logger::plain_formatter`

```cpp
class plain_formatter : public base_formatter {
public:
    std::string format(const log_entry& entry) const override;
};
```

### `kcenon::logger::json_formatter`

```cpp
class json_formatter : public base_formatter {
public:
    json_formatter(bool pretty_print = false);
    std::string format(const log_entry& entry) const override;
};
```

## Log Sampling (v3.3.0)

**Headers**: `<kcenon/logger/sampling/log_sampler.h>`, `<kcenon/logger/sampling/sampling_config.h>`

### Sampling Strategies

```cpp
namespace kcenon::logger::sampling;

enum class sampling_strategy : std::uint8_t {
    random,         // Simple random sampling based on probability
    rate_limiting,  // Rate-based sampling (N logs per time window)
    adaptive,       // Adaptive sampling that adjusts based on volume
    hash_based      // Deterministic sampling based on message hash
};
```

### `kcenon::logger::sampling::sampling_config`

Configuration structure for log sampling behavior.

```cpp
struct sampling_config {
    bool enabled = false;                          // Enable/disable sampling
    double rate = 1.0;                             // Base rate (0.0-1.0, 1.0 = keep all)
    sampling_strategy strategy = sampling_strategy::random;

    // Levels that bypass sampling (always logged)
    std::vector<log_level> always_log_levels = {
        log_level::error,
        log_level::critical
    };

    // Per-category sampling rates
    std::unordered_map<std::string, double> category_rates;

    // Per-field sampling rates (field_name -> value -> rate) (v3.4.0)
    std::unordered_map<std::string, std::unordered_map<std::string, double>> field_rates;

    // Fields whose presence bypasses sampling (v3.4.0)
    std::vector<std::string> always_log_fields;

    // Rate limiting settings
    std::size_t rate_limit_per_second = 1000;
    std::size_t rate_limit_window_ms = 1000;

    // Adaptive sampling settings
    bool adaptive_enabled = false;
    std::size_t adaptive_threshold = 10000;        // Messages/second to trigger adaptation
    double adaptive_min_rate = 0.01;               // Minimum rate under extreme load

    // Hash-based settings
    std::uint64_t hash_seed = 0;                   // Seed for reproducible sampling
};
```

#### Static Factory Methods

```cpp
static sampling_config disabled();                             // Disabled (pass-through)
static sampling_config random_sampling(double sample_rate);    // Random sampling
static sampling_config rate_limited(std::size_t max_per_second); // Rate limiting
static sampling_config adaptive(std::size_t threshold,
                                double min_rate = 0.01);       // Adaptive
static sampling_config hash_based(double sample_rate,
                                  std::uint64_t seed = 0);     // Hash-based

// Fluent method for setting bypass levels (v3.4.0)
sampling_config with_always_log(std::vector<log_level> levels) &&;
sampling_config with_always_log(std::vector<log_level> levels) const &;
```

### `kcenon::logger::sampling::sampling_stats`

```cpp
struct sampling_stats {
    std::uint64_t total_count = 0;      // Total messages considered
    std::uint64_t sampled_count = 0;    // Messages that passed sampling
    std::uint64_t dropped_count = 0;    // Messages dropped by sampling
    std::uint64_t bypassed_count = 0;   // Messages that bypassed sampling
    double effective_rate = 1.0;         // Current effective rate
    bool is_throttling = false;          // Whether adaptive sampling is reducing rate

    [[nodiscard]] double actual_ratio() const;  // Actual sampled/total ratio
    void reset();                                // Reset all counters
};
```

### `kcenon::logger::sampling::log_sampler`

Thread-safe log sampler with multiple strategy support. Uses fast PRNG (xorshift64) for minimal overhead.

#### Key Methods

```cpp
// Constructor
explicit log_sampler(const sampling_config& config = sampling_config{});

// Core sampling decision
[[nodiscard]] bool should_sample(const log_entry& entry);
[[nodiscard]] bool should_sample(log_level level, const std::string& message);
[[nodiscard]] bool should_sample(log_level level, const std::string& message,
                                 const std::optional<std::string>& category);

// Configuration management
void set_config(const sampling_config& config);
[[nodiscard]] sampling_config get_config() const;

// Statistics
[[nodiscard]] sampling_stats get_stats() const;
void reset_stats();

// Enable/disable
[[nodiscard]] bool is_enabled() const;
void set_enabled(bool enabled);

// Effective rate (may differ from configured rate in adaptive mode)
[[nodiscard]] double get_effective_rate() const;
```

### `kcenon::logger::sampling::sampler_factory`

Factory for creating pre-configured samplers.

```cpp
static std::unique_ptr<log_sampler> create_disabled();
static std::unique_ptr<log_sampler> create_random(double rate);
static std::unique_ptr<log_sampler> create_rate_limited(std::size_t max_per_second);
static std::unique_ptr<log_sampler> create_adaptive(
    std::size_t threshold = 10000, double min_rate = 0.01);
static std::unique_ptr<log_sampler> create_production(
    double base_rate = 0.1,
    std::vector<log_level> critical_levels = {
        log_level::warn, log_level::error, log_level::fatal
    });
```

#### Usage Examples

```cpp
#include <kcenon/logger/sampling/log_sampler.h>

using namespace kcenon::logger::sampling;

// Random sampling: keep 10% of logs, always keep errors/fatal
auto config = sampling_config::random_sampling(0.1)
    .with_always_log({log_level::error, log_level::fatal});

log_sampler sampler(config);

log_entry entry(log_level::info, "Message");
if (sampler.should_sample(entry)) {
    // Log the message
}

// Per-category sampling
config.category_rates["database"] = 0.01;  // 1% of database logs
config.category_rates["security"] = 1.0;   // All security logs
config.category_rates["http"] = 0.05;      // 5% of HTTP logs
sampler.set_config(config);

// Per-field sampling (v3.4.0)
config.field_rates["severity"]["high"] = 1.0;     // 100% of high severity
config.field_rates["severity"]["low"] = 0.1;      // 10% of low severity
config.field_rates["endpoint"]["/health"] = 0.01;  // 1% of health checks
config.always_log_fields = {"error_id", "transaction_id"};  // Always log these

// Adaptive sampling: auto-reduce when exceeding 10k msgs/sec
auto adaptive_sampler = sampler_factory::create_adaptive(10000, 0.01);

// Production-ready: 10% base rate, always log warn/error/fatal
auto prod_sampler = sampler_factory::create_production(0.1);

// Monitor sampling behavior
auto stats = sampler.get_stats();
std::cout << "Sampled: " << stats.sampled_count
          << "/" << stats.total_count
          << " (" << (stats.actual_ratio() * 100) << "%)"
          << std::endl;
```

---

## Real-time Analysis (v3.2.0)

**Headers**: `<kcenon/logger/analysis/realtime_log_analyzer.h>`, `<kcenon/logger/analysis/log_analyzer.h>`

### `kcenon::logger::analysis::anomaly_event`

Represents an anomaly detected during real-time analysis.

```cpp
struct anomaly_event {
    enum class type : std::uint8_t {
        error_spike,      // Sudden increase in errors
        pattern_match,    // Configured pattern detected
        rate_anomaly,     // Unusual log rate (too high or too low)
        new_error_type    // Previously unseen error message
    };

    type anomaly_type;
    std::chrono::system_clock::time_point detected_at;
    std::string description;
    std::vector<analyzed_log_entry> related_entries;
    std::string pattern;           // Pattern that triggered (for pattern_match)
    size_t current_count = 0;      // Current count (for spike/rate anomalies)
    size_t threshold = 0;          // Threshold that was exceeded
};
```

### `kcenon::logger::analysis::realtime_analysis_config`

```cpp
struct realtime_analysis_config {
    size_t error_spike_threshold = 100;           // Errors/minute to trigger spike alert
    size_t rate_anomaly_high_threshold = 1000;    // Logs/minute considered high
    size_t rate_anomaly_low_threshold = 0;        // Logs/minute considered low (0 = disabled)
    std::chrono::seconds window_duration{60};     // Sliding window for rate calculation
    std::chrono::seconds baseline_duration{300};  // Duration for baseline calculation
    bool track_new_errors = true;                 // Enable new error type detection
    bool enable_rate_anomaly_detection = true;    // Enable rate anomaly detection
    double rate_deviation_factor = 2.0;           // Factor for dynamic rate anomaly detection
    size_t max_related_entries = 10;              // Max entries stored per anomaly
};
```

### `kcenon::logger::analysis::realtime_log_analyzer`

Real-time log analyzer with anomaly detection. Designed to be integrated directly into the logging pipeline with minimal performance impact (< 5% overhead).

#### Callback Type

```cpp
using anomaly_callback = std::function<void(const anomaly_event&)>;
```

#### Constructor

```cpp
realtime_log_analyzer() = default;
explicit realtime_log_analyzer(const realtime_analysis_config& config);
```

#### Key Methods

```cpp
// Set callback for anomaly notifications (invoked synchronously)
void set_anomaly_callback(anomaly_callback cb);

// Feed a log entry for real-time analysis (thread-safe)
void analyze(const analyzed_log_entry& entry);

// Configure error spike threshold
void set_error_spike_threshold(size_t errors_per_minute);

// Pattern-based alerting
void add_pattern_alert(const std::string& pattern, log_level min_level);
bool remove_pattern_alert(const std::string& pattern);
void clear_pattern_alerts();

// Rate anomaly thresholds
void set_rate_thresholds(size_t high_threshold, size_t low_threshold = 0);

// New error type tracking
void set_track_new_errors(bool enable);

// Rate queries
double get_error_rate() const;   // Current errors per minute
double get_log_rate() const;     // Current logs per minute

// Configuration
const realtime_analysis_config& get_config() const;
void set_config(const realtime_analysis_config& config);

// Reset all statistics and tracked state
void reset();
```

#### Statistics

```cpp
struct statistics {
    size_t total_analyzed = 0;
    size_t total_errors = 0;
    size_t anomalies_detected = 0;
    size_t error_spikes = 0;
    size_t pattern_matches = 0;
    size_t rate_anomalies = 0;
    size_t new_error_types = 0;
    double current_log_rate = 0.0;
    double current_error_rate = 0.0;
};

statistics get_statistics() const;
```

### `kcenon::logger::analysis::realtime_analyzer_factory`

Factory for creating pre-configured analyzers.

```cpp
static std::unique_ptr<realtime_log_analyzer> create_basic();

static std::unique_ptr<realtime_log_analyzer> create(
    const realtime_analysis_config& config);

static std::unique_ptr<realtime_log_analyzer> create_production(
    size_t error_threshold = 50,
    realtime_log_analyzer::anomaly_callback callback = nullptr);
```

#### Usage Example

```cpp
#include <kcenon/logger/analysis/realtime_log_analyzer.h>

using namespace kcenon::logger::analysis;

// Create a production analyzer
auto analyzer = realtime_analyzer_factory::create_production(50,
    [](const anomaly_event& event) {
        switch (event.anomaly_type) {
            case anomaly_event::type::error_spike:
                send_pagerduty_alert(event.description);
                break;
            case anomaly_event::type::pattern_match:
                send_slack_notification(event.description);
                break;
            case anomaly_event::type::rate_anomaly:
                log_warning(event.description);
                break;
            case anomaly_event::type::new_error_type:
                track_new_error(event.description);
                break;
        }
    });

// Add pattern-based alerts
analyzer->add_pattern_alert("Connection refused", log_level::error);
analyzer->add_pattern_alert("OOM", log_level::fatal);
analyzer->add_pattern_alert("deadlock", log_level::error);

// Configure rate thresholds
analyzer->set_rate_thresholds(
    1000,  // High: 1000 logs/minute
    10     // Low: 10 logs/minute (detects service going quiet)
);

// Feed log entries during logging
analyzed_log_entry entry;
entry.level = log_level::error;
entry.message = "Connection refused to db-primary";
entry.timestamp = std::chrono::system_clock::now();
analyzer->analyze(entry);

// Query statistics
auto stats = analyzer->get_statistics();
std::cout << "Analyzed: " << stats.total_analyzed
          << ", Anomalies: " << stats.anomalies_detected
          << ", Error rate: " << stats.current_error_rate << "/min"
          << std::endl;
```

#### Thread Safety

The `analyze()` method is thread-safe and can be called from multiple threads concurrently. The anomaly callback is invoked synchronously; for non-blocking operation, dispatch to a separate thread within the callback.

---

## Backend Integration

### Integration Backend Interface

```cpp
namespace backends {

class integration_backend {
public:
    virtual ~integration_backend() = default;

    // Convert between log level types
    virtual common::interfaces::log_level to_common_level(
        logger_system::log_level level) const = 0;
    virtual logger_system::log_level from_common_level(
        common::interfaces::log_level level) const = 0;
};

}  // namespace backends
```

### Standalone Backend (Default)

```cpp
class standalone_backend : public integration_backend {
public:
    standalone_backend() = default;
    // Uses std::jthread for async processing
    // No external dependencies
};
```

## Monitoring Integration

### IMonitor Integration (Phase 2.2.4)

```cpp
logger_builder& with_monitoring(
    std::shared_ptr<common::interfaces::IMonitor> monitor);
logger_builder& with_health_check_interval(std::chrono::milliseconds interval);
```

## Usage Examples

### Basic Usage with ILogger Interface

```cpp
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>

using namespace kcenon::logger;

int main() {
    auto result = logger_builder()
        .use_template("production")
        .add_writer("console", std::make_unique<console_writer>())
        .build();

    if (!result) {
        std::cerr << "Failed: " << result.error().message() << "\n";
        return 1;
    }

    auto logger = std::move(result.value());

    // Using ILogger interface (recommended)
    logger->log(common::interfaces::log_level::info, "Application started");

    // Using C++20 source_location (automatic)
    logger->log(common::interfaces::log_level::debug, "Debug message");
    // Source location captured automatically!

    logger->flush();
    return 0;
}
```

### Native API (Backward Compatible)

```cpp
// Using native logger_system types
logger->log(log_level::info, "Message with native types");

// Note: The source location is auto-captured internally in v2.x+
// The legacy API with __FILE__, __LINE__, __func__ is deprecated and will be removed in v3.0.0
logger->log(log_level::error, "Error occurred");
```

### Configuration Strategies

```cpp
auto logger = logger_builder()
    .for_environment(deployment_env::production)
    .with_performance_tuning(performance_level::high_throughput)
    .auto_configure()  // Override with environment variables
    .add_writer("file", std::make_unique<rotating_file_writer>(
        "logs/app.log", 10 * 1024 * 1024, 5))
    .build()
    .value();
```

### Custom Writer

```cpp
class custom_writer : public kcenon::logger::log_writer_interface {
public:
    common::VoidResult write(const kcenon::logger::log_entry& entry) override {
        // Custom implementation
        return common::ok();
    }

    common::VoidResult flush() override {
        return common::ok();
    }
};
```

### Filtering

```cpp
auto logger = logger_builder()
    .add_level_filter(log_level::warning)
    .add_regex_filter("critical|important", true)
    .add_function_filter([](const log_entry& entry) {
        return entry.file.find("debug") == std::string::npos;
    })
    .build();
```

## Thread Safety

All public methods of the logger class are thread-safe. Writers are called sequentially (no concurrent writes to the same writer).

## Performance Considerations

- Use asynchronous mode for better performance
- Batch size affects latency vs throughput trade-off
- Larger buffer sizes reduce contention but increase memory usage
- Use `is_enabled()` check before expensive message construction
- Consider `critical_writer` only for truly critical messages

## Migration from v2.x

See [MIGRATION_GUIDE.md](guides/MIGRATION_GUIDE.md) for detailed migration instructions.

Key changes in v3.0:
- Namespace: `logger_module` -> `kcenon::logger`
- Interface: Implements `common::interfaces::ILogger`
- thread_system: Now optional (standalone mode is default)
- C++20 source_location support

---

*Last Updated: 2026-02-08*
