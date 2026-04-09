---
doc_id: "LOG-API-002"
doc_title: "API Reference"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "API"
---

> **Language:** **English** | [한국어](API_REFERENCE.kr.md)

# API Reference

> **SSOT**: This document is the single source of truth for **API Reference**.

**Version**: 0.5.0.0
**Last Updated**: 2026-02-08

This document has been split into focused sub-documents for easier navigation.

## Sub-Documents

| Document | Description |
|----------|-------------|
| [API_REFERENCE_CORE.md](API_REFERENCE_CORE.md) | Core logger class, ILogger interface, unified log context, scoped context guard, structured log builder, writer builder, configuration, builder pattern, interfaces, and error handling |
| [API_REFERENCE_WRITERS.md](API_REFERENCE_WRITERS.md) | All writer classes (console, file, rotating file, network, critical, batch), filters, formatters, log sampling, real-time analysis, backend integration, monitoring, usage examples, and migration guide |
| [API_REFERENCE_SECURITY.md](API_REFERENCE_SECURITY.md) | Security error codes, encrypted writer, secure key storage, critical logger interface, log sanitization, path validation, security filters, and audit logging |

## Overview

The Logger System (v5.0) provides a high-performance, thread-safe logging framework with:

- **Dual API Design**: Supports both `common::interfaces::ILogger` (standardized) and native `logger_system::log_level` (backward compatible)
- **Standalone Mode**: No external dependencies required (uses `std::jthread` internally)
- **Optional Integration**: Thread_system integration is optional since v3.0
- **C++20 Features**: Leverages Concepts and `source_location` for enhanced developer experience
- **Unified Log Context** (v3.3.0): Thread-safe, category-based context management with type-safe storage
- **Structured Logging** (v3.1.0): Fluent builder API for creating structured log entries with arbitrary fields
- **Writer Builder** (v4.1.0): Fluent decorator composition for building complex writer configurations
- **Log Sampling** (v3.3.0): Multiple sampling strategies (random, rate limiting, adaptive, hash-based) for high-volume scenarios
- **Real-time Analysis** (v3.2.0): Anomaly detection with error spike, pattern matching, and rate anomaly alerts

## Quick Links

### Core
- [Logger Class](API_REFERENCE_CORE.md#core-classes)
- [ILogger Interface](API_REFERENCE_CORE.md#ilogger-interface-phase-20)
- [Unified Log Context](API_REFERENCE_CORE.md#unified-log-context-v330)
- [Scoped Context Guard](API_REFERENCE_CORE.md#scoped-context-guard-v330)
- [Structured Log Builder](API_REFERENCE_CORE.md#structured-log-builder-v310)
- [Writer Builder](API_REFERENCE_CORE.md#writer-builder-v410)
- [Configuration](API_REFERENCE_CORE.md#configuration)
- [Builder Pattern](API_REFERENCE_CORE.md#builder-pattern)
- [Interfaces](API_REFERENCE_CORE.md#interfaces)
- [Error Handling](API_REFERENCE_CORE.md#error-handling)

### Writers and Filters
- [Writers](API_REFERENCE_WRITERS.md#writers)
- [Filters](API_REFERENCE_WRITERS.md#filters)
- [Formatters](API_REFERENCE_WRITERS.md#formatters)
- [Log Sampling](API_REFERENCE_WRITERS.md#log-sampling-v330)
- [Real-time Analysis](API_REFERENCE_WRITERS.md#real-time-analysis-v320)
- [Backend Integration](API_REFERENCE_WRITERS.md#backend-integration)
- [Monitoring Integration](API_REFERENCE_WRITERS.md#monitoring-integration)
- [Usage Examples](API_REFERENCE_WRITERS.md#usage-examples)

### Security
- [Security Error Codes](API_REFERENCE_SECURITY.md#security-error-codes)
- [Encrypted Writer](API_REFERENCE_SECURITY.md#encrypted-writer)
- [Secure Key Storage](API_REFERENCE_SECURITY.md#secure-key-storage)
- [Log Sanitization](API_REFERENCE_SECURITY.md#log-sanitization)
- [Path Validation](API_REFERENCE_SECURITY.md#path-validation)
- [Audit Logging](API_REFERENCE_SECURITY.md#audit-logging)

---

*Last Updated: 2026-02-08*
