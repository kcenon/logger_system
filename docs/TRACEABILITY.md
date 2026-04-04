---
doc_id: "LOG-QUAL-004"
doc_title: "Feature-Test-Module Traceability Matrix"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "QUAL"
---

# Traceability Matrix

> **SSOT**: This document is the single source of truth for **Logger System Feature-Test-Module Traceability**.

## Feature -> Test -> Module Mapping

### Core Logger

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-001 | Logger Core | tests/unit/logger_test/logger_test.cpp, tests/integration_test.cpp | include/kcenon/logger/core/, src/core/ | Covered |
| LOG-FEAT-002 | Logger Builder | tests/unit/logger_test/logger_test.cpp | include/kcenon/logger/core/ | Covered |
| LOG-FEAT-003 | Logger Config & Strategies | tests/unit/config_test/config_test.cpp, tests/unit/config_test/strategy_test.cpp | include/kcenon/logger/core/strategies/ | Covered |
| LOG-FEAT-004 | Error Codes | tests/unit/core_test/error_codes_test.cpp | include/kcenon/logger/core/ | Covered |
| LOG-FEAT-005 | Logger Context | tests/unit/core_test/logger_context_test.cpp, tests/unit/core_test/scoped_context_guard_test.cpp, tests/unit/core_test/unified_log_context_test.cpp | include/kcenon/logger/core/ | Covered |
| LOG-FEAT-006 | ILogger Interface | tests/unit/ilogger_interface_test.cpp | include/kcenon/logger/interfaces/ | Covered |
| LOG-FEAT-007 | Log Entry | tests/unit/interfaces_test/log_entry_test.cpp | include/kcenon/logger/interfaces/ | Covered |

### Writer Types

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-008 | Console/File Writers | tests/unit/writers_test/writers_test.cpp | include/kcenon/logger/writers/, src/impl/writers/ | Covered |
| LOG-FEAT-009 | Async Writer | tests/unit/writers_test/async_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-010 | Batch Writer | tests/unit/writers_test/batch_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-011 | Buffered Writer | tests/unit/writers_test/buffered_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-012 | Critical Writer | tests/unit/writers_test/critical_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-013 | Composite Writer | tests/unit/writers_test/composite_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-014 | Encrypted Writer | tests/encrypted_writer_test.cpp, tests/unit/writers_test/encrypted_writer_extended_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-015 | Filtered Writer | tests/unit/writers_test/filtered_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-016 | Formatted Writer | tests/unit/writers_test/formatted_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-017 | Network Writer | tests/unit/writers_test/network_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-018 | Rotating File Writer | tests/unit/writers_test/rotating_file_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-019 | Thread-Safe Writer | tests/unit/writers_test/thread_safe_writer_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-020 | Decorator Writer Base | tests/unit/writers_test/decorator_writer_base_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-021 | Queued Writer Base | tests/unit/writers_test/queued_writer_base_test.cpp | include/kcenon/logger/writers/ | Covered |
| LOG-FEAT-022 | Writer Categories | tests/unit/writers_test/writer_category_test.cpp | include/kcenon/logger/writers/ | Covered |

### Formatting & Filtering

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-023 | Formatters (Plain, JSON, Logfmt) | tests/unit/formatters_test/formatters_test.cpp | include/kcenon/logger/formatters/ | Covered |
| LOG-FEAT-024 | Filters (Level, Regex, Function, Composite) | tests/unit/filters_test/filters_test.cpp, tests/min_level_filter_test.cpp | include/kcenon/logger/filters/, src/impl/filters/ | Covered |
| LOG-FEAT-025 | Overflow Policy | tests/unit/flow_test/overflow_policy_test.cpp | include/kcenon/logger/core/ | Covered |

### Security Features

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-026 | Secure Key Storage | tests/unit/security_test/security_test.cpp | include/kcenon/logger/security/, src/security/ | Covered |
| LOG-FEAT-027 | Path Validator | tests/unit/security_test/path_validator_test.cpp | include/kcenon/logger/security/ | Covered |
| LOG-FEAT-028 | Log Sanitizer | tests/unit/security_test/log_sanitizer_test.cpp | include/kcenon/logger/security/ | Covered |
| LOG-FEAT-029 | Signal Handler Safety | tests/unit/security_test/signal_manager_test.cpp, tests/unit/safety_test/crash_safety_test.cpp | include/kcenon/logger/safety/ | Covered |

### Advanced Capabilities

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-030 | Structured Logging | tests/structured_logging_test.cpp | include/kcenon/logger/structured/ | Covered |
| LOG-FEAT-031 | Log Routing | tests/routing_integration_test.cpp | include/kcenon/logger/routing/ | Covered |
| LOG-FEAT-032 | Log Analysis | tests/unit/analysis_test/log_analyzer_test.cpp | include/kcenon/logger/analysis/ | Covered |
| LOG-FEAT-033 | Real-time Anomaly Detection | tests/realtime_analyzer_test.cpp | include/kcenon/logger/analysis/ | Covered |
| LOG-FEAT-034 | Log Server (Distributed) | tests/unit/server_test/log_server_test.cpp | include/kcenon/logger/server/ | Covered |
| LOG-FEAT-035 | Log Collector | tests/unit/collectors_test/log_collector_test.cpp | include/kcenon/logger/core/ | Covered |

### OTLP Integration

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-036 | OTLP Writer | tests/otlp_test.cpp | include/kcenon/logger/otlp/, include/kcenon/logger/writers/ | Covered |

### Log Sampling

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-037 | Log Sampling | tests/log_sampling_test.cpp | include/kcenon/logger/sampling/, src/sampling/ | Covered |

### Monitoring & Health

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-038 | Monitoring Integration | tests/unit/monitoring_test/monitoring_test.cpp, tests/integration/monitoring_integration_test.cpp | include/kcenon/logger/core/ | Covered |
| LOG-FEAT-039 | Health Checks | tests/unit/health_test/health_check_test.cpp | include/kcenon/logger/core/ | Covered |

### Integration & DI

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-040 | DI Container | tests/unit/di_test/di_container_test.cpp | include/kcenon/logger/di/ | Covered |
| LOG-FEAT-041 | Executor Integration | tests/unit/executor_integration_test.cpp | include/kcenon/logger/adapters/ | Covered |
| LOG-FEAT-042 | Thread System Integration | tests/integration/thread_system_integration_test.cpp, tests/unit/thread_system_integration_module_test.cpp | include/kcenon/logger/integration/ | Covered |
| LOG-FEAT-043 | Version Compatibility | tests/compatibility/version_compatibility_test.cpp | (cross-cutting) | Covered |

### Utilities

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-044 | File Utilities | tests/unit/utils_test/file_utils_test.cpp | include/kcenon/logger/utils/ | Covered |
| LOG-FEAT-045 | String Utilities | tests/unit/utils_test/string_utils_test.cpp | include/kcenon/logger/utils/ | Covered |
| LOG-FEAT-046 | Time Utilities | tests/unit/utils_test/time_utils_test.cpp | include/kcenon/logger/utils/ | Covered |

### Production Quality

| Feature ID | Feature | Test File(s) | Module/Directory | Status |
|-----------|---------|-------------|-----------------|--------|
| LOG-FEAT-047 | Thread Safety | tests/thread_safety_tests.cpp | (cross-cutting) | Covered |
| LOG-FEAT-048 | Stress Testing | tests/unit/stress_test/stress_test.cpp | (cross-cutting) | Covered |

## Coverage Summary

| Category | Total Features | Covered | Partial | Uncovered |
|----------|---------------|---------|---------|-----------|
| Core Logger | 7 | 7 | 0 | 0 |
| Writer Types | 15 | 15 | 0 | 0 |
| Formatting & Filtering | 3 | 3 | 0 | 0 |
| Security Features | 4 | 4 | 0 | 0 |
| Advanced Capabilities | 6 | 6 | 0 | 0 |
| OTLP Integration | 1 | 1 | 0 | 0 |
| Log Sampling | 1 | 1 | 0 | 0 |
| Monitoring & Health | 2 | 2 | 0 | 0 |
| Integration & DI | 4 | 4 | 0 | 0 |
| Utilities | 3 | 3 | 0 | 0 |
| Production Quality | 2 | 2 | 0 | 0 |
| **Total** | **48** | **48** | **0** | **0** |

## See Also

- [FEATURES.md](FEATURES.md) -- Detailed feature documentation
- [README.md](README.md) -- SSOT Documentation Registry
