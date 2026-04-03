---
doc_id: "LOG-PROJ-005"
doc_title: "SOUP List &mdash; logger_system"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "PROJ"
---

# SOUP List &mdash; logger_system

> **SSOT**: This document is the single source of truth for **SOUP List &mdash; logger_system**.

> **Software of Unknown Provenance (SOUP) Register per IEC 62304:2006+AMD1:2015 &sect;8.1.2**
>
> This document is the authoritative reference for all external software dependencies.
> Every entry must include: title, manufacturer, unique version identifier, license, and known anomalies.

| Document | Version |
|----------|---------|
| IEC 62304 Reference | &sect;8.1.2 Software items from SOUP |
| Last Reviewed | 2026-03-16 |
| logger_system Version | 0.1.2 |

---

## Production SOUP

| ID | Name | Manufacturer | Version | License | Usage | Safety Class | Linking | Known Anomalies |
|----|------|-------------|---------|---------|-------|-------------|---------|-----------------|
| SOUP-P01 | [kcenon-common-system](https://github.com/kcenon/common_system) | kcenon | 0.2.0 | BSD-3-Clause | Core foundation library providing Result&lt;T&gt; pattern, ILogger interface, and common utilities | A | Static or dynamic | None known at pinned version |

---

## Optional SOUP

| ID | Name | Manufacturer | Version | License | Usage | Safety Class | Linking | Known Anomalies |
|----|------|-------------|---------|---------|-------|-------------|---------|-----------------|
| SOUP-001 | [OpenSSL](https://www.openssl.org/) | OpenSSL Software Foundation | 3.3.0 | Apache-2.0 | AES-256-GCM encrypted log writer (`encryption` feature) | C | Dynamic | None known at pinned version |
| SOUP-002 | [OpenTelemetry C++ SDK](https://github.com/open-telemetry/opentelemetry-cpp) | CNCF / OpenTelemetry | 1.14.2 | Apache-2.0 | Distributed tracing and metrics export via OTLP (`otlp` feature) | B | Dynamic | None |
| SOUP-003 | [gRPC](https://grpc.io/) | Google / CNCF | 1.51.1 | Apache-2.0 | High-performance RPC for OTLP telemetry export (`otlp` feature) | B | Dynamic | None |
| SOUP-004 | [Protocol Buffers](https://protobuf.dev/) | Google | 3.21.12 | BSD-3-Clause | Serialization format for gRPC and telemetry (`otlp` feature) | B | Dynamic | None |
| SOUP-005 | [spdlog](https://github.com/gabime/spdlog) | Gabi Melman | 1.13.0 | MIT | Fast C++ logging library for benchmark comparison (`benchmarks` feature) | A | Header-only or shared | None |
| SOUP-006 | [kcenon-thread-system](https://github.com/kcenon/thread_system) | kcenon | 0.1.0 | BSD-3-Clause | Thread pool and async executor integration (`thread-system` feature) | A | Static or dynamic | None known at pinned version |

---

## Development/Test SOUP (Not Deployed)

| ID | Name | Manufacturer | Version | License | Usage | Qualification |
|----|------|-------------|---------|---------|-------|--------------|
| SOUP-T01 | [Google Test](https://github.com/google/googletest) | Google | 1.14.0 | BSD-3-Clause | Unit testing framework (includes GMock) | Required |
| SOUP-T02 | [Google Benchmark](https://github.com/google/benchmark) | Google | 1.8.3 | Apache-2.0 | Performance benchmarking framework | Not required |

---

## Safety Classification Key

| Class | Definition | Example |
|-------|-----------|---------|
| **A** | No contribution to hazardous situation | Logging, formatting, benchmarking |
| **B** | Non-serious injury possible | Telemetry export, data serialization |
| **C** | Death or serious injury possible | Encryption, access control |

---

## Version Pinning (IEC 62304 Compliance)

All SOUP versions are pinned in `vcpkg.json` via the `overrides` field:

```json
{
  "overrides": [
    { "name": "openssl", "version": "3.3.0" },
    { "name": "spdlog", "version": "1.13.0" },
    { "name": "opentelemetry-cpp", "version": "1.14.2" },
    { "name": "protobuf", "version": "3.21.12" },
    { "name": "grpc", "version": "1.51.1" },
    { "name": "gtest", "version": "1.14.0" },
    { "name": "benchmark", "version": "1.8.3" }
  ]
}
```

The vcpkg baseline is locked in `vcpkg-configuration.json` to ensure reproducible builds.

---

## Version Update Process

When updating any SOUP dependency:

1. Update the version in `vcpkg.json` (overrides section)
2. Update the corresponding row in this document
3. Verify no new known anomalies (check CVE databases)
4. Run full CI/CD pipeline to confirm compatibility
5. Document the change in the PR description

---

## License Compliance Summary

| License | Count | Copyleft | Obligation |
|---------|-------|----------|------------|
| Apache-2.0 | 4 | No | Include license + NOTICE file |
| BSD-3-Clause | 2 | No | Include copyright + no-endorsement clause |
| MIT | 1 | No | Include copyright notice |

> **No LGPL or copyleft dependencies** in logger_system. All licenses are permissive and BSD-3-Clause compatible.
