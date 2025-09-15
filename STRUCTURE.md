# Logger System - New Structure

## Directory Layout

```
logger_system/
├── include/kcenon/logger/       # Public headers
│   ├── core/                   # Core APIs (logger, config, builder)
│   ├── interfaces/             # Interface definitions
│   ├── writers/                # Writer interfaces
│   ├── formatters/             # Formatter interfaces
│   └── utils/                  # Utilities
├── src/                        # Implementation
│   ├── core/                   # Core implementation
│   ├── impl/                   # Private implementations
│   │   ├── writers/            # Writer implementations
│   │   ├── formatters/         # Formatter implementations
│   │   ├── filters/            # Filter implementations
│   │   ├── async/              # Async components
│   │   ├── di/                 # DI container
│   │   └── monitoring/         # Monitoring adapters
│   └── utils/                  # Utility implementations
├── tests/                      # All tests
│   ├── unit/                   # Unit tests
│   ├── integration/            # Integration tests
│   └── benchmarks/             # Performance tests
├── examples/                   # Usage examples
├── docs/                       # Documentation
└── CMakeLists.txt             # Build configuration
```

## Namespace Structure

- Root: `kcenon::logger`
- Core functionality: `kcenon::logger::core`
- Interfaces: `kcenon::logger::interfaces`
- Writers: `kcenon::logger::writers`
- Formatters: `kcenon::logger::formatters`
- Implementation details: `kcenon::logger::impl`
- Utilities: `kcenon::logger::utils`

## Key Components

### Public API (include/kcenon/logger/)
- `core/logger.h` - Main logger class
- `core/logger_config.h` - Configuration
- `core/logger_builder.h` - Builder pattern
- `interfaces/logger_interface.h` - Logger interface
- `writers/*.h` - Writer interfaces
- `formatters/*.h` - Formatter interfaces

### Implementation (src/)
- Async pipeline in `impl/async/`
- DI container in `impl/di/`
- Writer implementations in `impl/writers/`
- Formatter implementations in `impl/formatters/`

## Migration Notes

1. Old structure backed up in `old_structure/` directory
2. Compatibility header at `include/kcenon/logger/compatibility.h`
3. Run `./migrate_namespaces.sh` to update all namespaces
4. Update CMakeLists.txt for new structure
