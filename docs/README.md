# Logger System Documentation

Welcome to the Logger System documentation. This guide provides comprehensive information about the high-performance C++20 logging system.

## Table of Contents

### Getting Started
- [Quick Start Guide](GETTING-STARTED.md) - Get up and running quickly
- [Installation](GETTING-STARTED.md#installation) - Detailed installation instructions
- [Basic Usage](GETTING-STARTED.md#basic-usage) - Simple examples to begin with

### Core Concepts
- [Architecture Overview](ARCHITECTURE.md) - System design and components
- [API Reference](API-REFERENCE.md) - Complete API documentation
- [Performance Guide](PERFORMANCE.md) - Performance characteristics and benchmarks

### Advanced Topics
- [Custom Writers](CUSTOM-WRITERS.md) - Creating custom output destinations
- [Asynchronous Pipeline](ARCHITECTURE.md#asynchronous-mode) - Queueing and batching model
- [Integration Guide](integration.md) - Integrating with other systems

### Development
- [Building from Source](building.md) - Build instructions for all platforms
- [Contributing](../CONTRIBUTING.md) - Guidelines for contributors
- [Testing](testing.md) - Running tests and benchmarks

### Examples
- [Basic Examples](../samples/) - Sample code demonstrations
- [Thread System Integration](integration.md#thread-system) - Using with thread_system

## Quick Links

- [GitHub Repository](https://github.com/kcenon/logger_system)
- [Issue Tracker](https://github.com/kcenon/logger_system/issues)
- [Thread System](https://github.com/kcenon/thread_system) - Parent project
- [Security Guide](../SECURITY.md) - Threat model and hardening

## Documentation (Pages) Checklist

- Enable GitHub Pages with source set to `gh-pages` branch (root).
- Ensure `Doxyfile` exists at repository root (already added).
- The workflow `Generate-Documentation` will build and publish to `documents/html` on `gh-pages`.
- Visit Actions → Generate-Documentation to run manually if needed (workflow_dispatch supported).
