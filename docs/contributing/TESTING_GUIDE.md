---
doc_id: "LOG-QUAL-003"
doc_title: "Testing Guide"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "QUAL"
---

## Test Coverage

To enable code coverage measurement:

```bash
cmake -B build -DENABLE_COVERAGE=ON
cmake --build build
cmake --build build --target coverage
```

Coverage reports will be generated in `build/coverage/`.

Target: 80%+ code coverage for all modules.

