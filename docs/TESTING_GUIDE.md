## Test Coverage

To enable code coverage measurement:

```bash
cmake -B build -DENABLE_COVERAGE=ON
cmake --build build
cmake --build build --target coverage
```

Coverage reports will be generated in `build/coverage/`.

Target: 80%+ code coverage for all modules.

