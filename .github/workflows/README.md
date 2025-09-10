# CI/CD Workflows

This directory contains GitHub Actions workflows for continuous integration and deployment of the Logger System project.

## Workflow Organization

The CI/CD pipeline is organized by platform and purpose to maximize parallelization and maintainability:

### Platform-Specific Build Workflows

#### 🐧 Linux Builds
- **`build-linux-gcc.yml`**: GCC compiler builds (versions 9-13)
  - Matrix builds for Debug/Release configurations
  - AddressSanitizer and ThreadSanitizer testing
  - Code coverage generation with gcovr
  - Supports vcpkg for dependency management

- **`build-linux-clang.yml`**: Clang compiler builds (versions 12-16)
  - Matrix builds for Debug/Release configurations
  - MemorySanitizer and UndefinedBehaviorSanitizer testing
  - Static analysis with clang-tidy and cppcheck
  - libc++ standard library support

#### 🍎 macOS Builds
- **`build-macos.yml`**: macOS builds for multiple versions
  - Supports macOS 12, 13, and 14
  - Xcode 13.4, 14.3, and 15.0
  - Universal binary creation (x86_64 + arm64)
  - AddressSanitizer support on Apple Silicon
  - Homebrew package management

#### 🪟 Windows Builds
- **`build-windows-msvc.yml`**: Visual Studio builds
  - Visual Studio 2019 and 2022
  - x64 and Win32 architectures
  - vcpkg integration for dependencies
  - Release package creation

- **`build-windows-mingw.yml`**: MinGW builds
  - MinGW versions 11-13
  - MSYS2 environment
  - Cross-platform compatibility testing
  - Both 32-bit and 64-bit builds

### 📚 Documentation Workflow
- **`documentation.yml`**: Documentation generation and quality checks
  - Doxygen API documentation
  - Sphinx documentation with Breathe integration
  - PDF generation from Markdown
  - Documentation coverage reporting
  - GitHub Pages deployment
  - Broken link checking
  - Markdown linting

## Workflow Features

### Quality Assurance
- **Sanitizers**: Address, Thread, Memory, and Undefined Behavior sanitizers
- **Static Analysis**: clang-tidy, cppcheck, and PVS-Studio (optional)
- **Code Coverage**: gcovr/lcov with Codecov integration
- **Compiler Warnings**: Comprehensive warning flags with -Werror option

### Build Matrix
- **Compilers**: GCC 9-13, Clang 12-16, MSVC 2019/2022, MinGW 11-13
- **Platforms**: Ubuntu, macOS (Intel/ARM), Windows
- **Configurations**: Debug and Release
- **Architectures**: x86, x64, ARM64

### Testing Strategy
- **Unit Tests**: Fast, isolated component tests
- **Integration Tests**: End-to-end scenario testing
- **Stress Tests**: Performance and stability under load
- **Benchmarks**: Performance regression testing

## Triggering Workflows

### Automatic Triggers
- **Push to main/develop**: All workflows except documentation
- **Pull Request**: All workflows with reduced matrix
- **Schedule**: Weekly documentation builds
- **Path filters**: Skip workflows when only docs change

### Manual Triggers
All workflows support `workflow_dispatch` for manual triggering with custom parameters.

## Artifacts

Each workflow produces various artifacts:

- **Test Results**: JUnit XML format test results
- **Coverage Reports**: HTML, XML, and JSON coverage reports
- **Static Analysis**: clang-tidy and cppcheck reports
- **Documentation**: HTML docs, PDF manuals, API references
- **Release Packages**: Platform-specific binary packages

## Environment Variables

Common environment variables used across workflows:

- `BUILD_TYPE`: Debug or Release configuration
- `CMAKE_VERSION`: Minimum CMake version required
- `CODECOV_TOKEN`: Token for Codecov integration (secret)

## Secrets Required

The following secrets should be configured in the repository:

- `CODECOV_TOKEN`: For uploading coverage reports to Codecov
- `GITHUB_TOKEN`: Automatically provided by GitHub Actions

## Performance Optimizations

- **Parallel Jobs**: Matrix builds run in parallel
- **Caching**: Dependencies cached between runs
- **Path Filters**: Skip irrelevant workflows
- **Shallow Clones**: Use `fetch-depth: 0` only when needed
- **Ninja Build System**: Faster than Make on all platforms

## Maintenance

### Adding a New Compiler Version
1. Add the version to the appropriate matrix in the workflow
2. Update tool installation steps if needed
3. Test the workflow in a feature branch

### Updating Dependencies
1. Update version numbers in installation steps
2. Test compatibility with all compiler versions
3. Update documentation if APIs change

### Workflow Debugging
- Use `actions/upload-artifact` to save logs
- Enable debug logging with `ACTIONS_RUNNER_DEBUG=true`
- Use `continue-on-error: true` for non-critical steps

## Best Practices

1. **Keep workflows focused**: One workflow per platform/purpose
2. **Use matrix builds**: Test multiple configurations efficiently
3. **Fail fast**: Set `fail-fast: false` for better debugging
4. **Cache dependencies**: Reduce build times
5. **Upload artifacts**: Preserve test results and logs
6. **Use path filters**: Skip unnecessary builds
7. **Document changes**: Update this README when modifying workflows

## Troubleshooting

### Common Issues

1. **Sanitizer failures on macOS**: Disable leak detection
2. **Windows path issues**: Use forward slashes or `${/}`
3. **MinGW linking errors**: Check MSYS2 package versions
4. **Documentation build failures**: Ensure all tools are installed

### Getting Help

- Check workflow run logs in the Actions tab
- Review similar issues in thread_system repository
- Open an issue with workflow logs attached