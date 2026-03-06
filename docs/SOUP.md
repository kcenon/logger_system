# SOUP List &mdash; logger_system

> **Software of Unknown Provenance (SOUP) Register per IEC 62304:2006+AMD1:2015 &sect;8.1.2**
>
> This document is the authoritative reference for all external software dependencies.
> Every entry must include: title, manufacturer, unique version identifier, license, and known anomalies.

| Document | Version |
|----------|---------|
| IEC 62304 Reference | &sect;8.1.2 Software items from SOUP |
| Last Reviewed | 2026-03-06 |
| logger_system Version | 1.0.0 |

---

## Internal Ecosystem Dependencies

| ID | Name | Manufacturer | Version | License | Usage | Safety Class | Known Anomalies |
|----|------|-------------|---------|---------|-------|-------------|-----------------|
| INT-001 | [common_system](https://github.com/kcenon/common_system) | kcenon | Latest (embedded) | BSD-3-Clause | Result&lt;T&gt; pattern, error handling primitives | B | None |
| INT-002 | [thread_system](https://github.com/kcenon/thread_system) | kcenon | Latest (embedded) | BSD-3-Clause | Thread pool, async task scheduling for log writers | B | None |

> **Note**: common_system and thread_system are embedded as source-level dependencies within the logger_system repository.

---

## Production SOUP

| ID | Name | Manufacturer | Version | License | Usage | Safety Class | Known Anomalies |
|----|------|-------------|---------|---------|-------|-------------|-----------------|
| SOUP-001 | [fmt](https://github.com/fmtlib/fmt) | Victor Zverovich | 10.2.1 | MIT | String formatting library for log message formatting | A | None |

---

## Optional SOUP

### Encryption Feature (`encryption`)

| ID | Name | Manufacturer | Version | License | Usage | Safety Class | Known Anomalies |
|----|------|-------------|---------|---------|-------|-------------|-----------------|
| SOUP-002 | [OpenSSL](https://www.openssl.org/) | OpenSSL Project | 3.3.0 | Apache-2.0 | AES-256-GCM encrypted log writer | C | CVE tracking via vendor advisories required |

### OTLP Feature (`otlp`)

| ID | Name | Manufacturer | Version | License | Usage | Safety Class | Known Anomalies |
|----|------|-------------|---------|---------|-------|-------------|-----------------|
| SOUP-003 | [opentelemetry-cpp](https://github.com/open-telemetry/opentelemetry-cpp) | OpenTelemetry Authors (CNCF) | 1.14.2 | Apache-2.0 | OTLP log export for observability integration | A | None |
| SOUP-004 | [Protocol Buffers](https://github.com/protocolbuffers/protobuf) | Google | 3.21.12 | BSD-3-Clause | Serialization for OTLP transport | A | None |
| SOUP-005 | [gRPC](https://github.com/grpc/grpc) | Google | 1.51.1 | Apache-2.0 | RPC transport for OTLP export | A | None |

---

## Development/Test SOUP (Not Deployed)

| ID | Name | Manufacturer | Version | License | Usage | Qualification |
|----|------|-------------|---------|---------|-------|--------------|
| SOUP-T01 | [Google Test](https://github.com/google/googletest) | Google | 1.14.0 | BSD-3-Clause | Unit testing framework | Required |
| SOUP-T02 | [Google Benchmark](https://github.com/google/benchmark) | Google | 1.8.3 | Apache-2.0 | Performance benchmarking framework | Not required |
| SOUP-T03 | [spdlog](https://github.com/gabime/spdlog) | Gabi Melman | 1.13.0 | MIT | Benchmark comparison baseline (not used in production) | Not required |

---

## Safety Classification Key

| Class | Definition | Example |
|-------|-----------|---------|
| **A** | No contribution to hazardous situation | Logging, formatting, test frameworks |
| **B** | Non-serious injury possible | Data processing, network communication |
| **C** | Death or serious injury possible | Encryption, access control |

---

## Version Pinning (IEC 62304 Compliance)

All SOUP versions are pinned in `vcpkg.json` via the `overrides` field:

```json
{
  "overrides": [
    { "name": "fmt", "version": "10.2.1" },
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
3. Verify no new known anomalies (check CVE databases, especially for OpenSSL)
4. Run full CI/CD pipeline to confirm compatibility
5. Document the change in the PR description

---

## License Compliance Summary

| License | Count | Copyleft | Obligation |
|---------|-------|----------|------------|
| MIT | 2 | No | Include copyright notice |
| Apache-2.0 | 3 | No | Include license + NOTICE file |
| BSD-3-Clause | 2 | No | Include copyright + no-endorsement clause |

> **GPL contamination**: None detected. All dependencies are permissively licensed.
