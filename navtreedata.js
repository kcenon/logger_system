/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Logger System", "index.html", [
    [ "Documentation (GitHub Pages)", "index.html#autotoc_md1", null ],
    [ "🚀 Recent Improvements", "index.html#autotoc_md2", [
      [ "Phase 2 - Core Systems (Complete - 100% ✅)", "index.html#autotoc_md3", null ],
      [ "Phase 1 - Foundation (Complete)", "index.html#autotoc_md4", null ]
    ] ],
    [ "🔗 Project Ecosystem Integration", "index.html#autotoc_md5", [
      [ "Project Dependencies", "index.html#autotoc_md6", null ],
      [ "Related Projects", "index.html#autotoc_md7", null ],
      [ "Integration Architecture", "index.html#autotoc_md8", null ],
      [ "Integration Benefits", "index.html#autotoc_md9", null ]
    ] ],
    [ "Features", "index.html#autotoc_md10", [
      [ "Core Features", "index.html#autotoc_md11", null ],
      [ "Enhanced Architecture (New in Phase 1)", "index.html#autotoc_md12", null ],
      [ "Advanced Features", "index.html#autotoc_md13", null ]
    ] ],
    [ "Build Configuration", "index.html#autotoc_md14", [
      [ "CMake Feature Flags", "index.html#autotoc_md15", null ],
      [ "Using as a Package", "index.html#autotoc_md16", null ]
    ] ],
    [ "Integration with Thread System", "index.html#autotoc_md17", null ],
    [ "Quick Start", "index.html#autotoc_md18", [
      [ "Quick Start with Builder Pattern (Recommended)", "index.html#autotoc_md19", null ],
      [ "Configuration Templates", "index.html#autotoc_md20", null ],
      [ "Traditional API (Legacy Support)", "index.html#autotoc_md21", null ],
      [ "Error Handling with Result Pattern", "index.html#autotoc_md22", null ],
      [ "Interface Architecture", "index.html#autotoc_md23", null ],
      [ "Performance Metrics", "index.html#autotoc_md24", null ],
      [ "Structured Logging", "index.html#autotoc_md25", null ],
      [ "Advanced Filtering and Routing", "index.html#autotoc_md26", null ],
      [ "File Writers", "index.html#autotoc_md27", null ],
      [ "Distributed Logging", "index.html#autotoc_md28", null ],
      [ "Real-time Analysis", "index.html#autotoc_md29", null ],
      [ "Security Features", "index.html#autotoc_md30", [
        [ "Log Encryption", "index.html#autotoc_md31", null ],
        [ "Sensitive Data Sanitization", "index.html#autotoc_md32", null ],
        [ "Access Control", "index.html#autotoc_md33", null ]
      ] ],
      [ "Custom Writers", "index.html#autotoc_md34", null ]
    ] ],
    [ "Building", "index.html#autotoc_md35", [
      [ "Build Options", "index.html#autotoc_md36", null ]
    ] ],
    [ "Testing", "index.html#autotoc_md37", null ],
    [ "Platform Support", "index.html#autotoc_md38", null ],
    [ "FAQ", "index.html#autotoc_md39", null ],
    [ "Further Reading", "index.html#autotoc_md40", null ],
    [ "Installation", "index.html#autotoc_md41", null ],
    [ "CMake Integration", "index.html#autotoc_md42", null ],
    [ "License", "index.html#autotoc_md43", null ],
    [ "Threading Ecosystem Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html", [
      [ "🏗️ Ecosystem Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md45", null ],
      [ "📋 Project Roles & Responsibilities", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md46", [
        [ "1. thread_system (Foundation)", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md47", [
          [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md48", null ],
          [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md49", null ],
          [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md50", null ]
        ] ],
        [ "2. logger_system (Logging)", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md52", [
          [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md53", null ],
          [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md54", null ],
          [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md55", null ]
        ] ],
        [ "3. monitoring_system (Metrics)", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md57", [
          [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md58", null ],
          [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md59", null ],
          [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md60", null ]
        ] ],
        [ "4. integrated_thread_system (Integration Hub)", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md62", [
          [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md63", null ],
          [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md64", null ],
          [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md65", null ]
        ] ]
      ] ],
      [ "🔄 Dependency Flow & Interface Contracts", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md66", [
        [ "Interface Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md67", null ],
        [ "Dependency Graph", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md68", null ],
        [ "Build Order Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md69", null ]
      ] ],
      [ "🔧 Integration Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md70", [
        [ "1. Interface-Based Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md71", null ],
        [ "2. Dependency Injection Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md72", null ],
        [ "3. Configuration Management", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md73", null ]
      ] ],
      [ "📊 Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md74", [
        [ "Design Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md75", null ],
        [ "Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md76", null ]
      ] ],
      [ "🔄 Evolution: Monolithic → Modular", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md77", [
        [ "Before: Monolithic Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md78", null ],
        [ "After: Modular Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md79", null ],
        [ "Migration Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md80", null ]
      ] ],
      [ "🚀 Getting Started", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md81", [
        [ "1. Development Environment Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md82", null ],
        [ "2. Build Order (Local Development)", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md83", null ],
        [ "3. Verification", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md84", null ]
      ] ],
      [ "📚 Documentation Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md85", [
        [ "thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md86", null ],
        [ "logger_system", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md87", null ],
        [ "monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md88", null ],
        [ "integrated_thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md89", null ]
      ] ],
      [ "🔮 Future Roadmap", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md90", [
        [ "Phase 1: Stabilization (Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md91", null ],
        [ "Phase 2: Enhancement", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md92", null ],
        [ "Phase 3: Ecosystem Expansion", "md__2home_2runner_2work_2logger__system_2logger__system_2ARCHITECTURE.html#autotoc_md93", null ]
      ] ]
    ] ],
    [ "Security Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2SECURITY.html", [
      [ "Threat Model (Scope)", "md__2home_2runner_2work_2logger__system_2logger__system_2SECURITY.html#autotoc_md96", null ],
      [ "Current Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2SECURITY.html#autotoc_md97", null ],
      [ "Important Limitations", "md__2home_2runner_2work_2logger__system_2logger__system_2SECURITY.html#autotoc_md98", null ],
      [ "Recommendations (Production)", "md__2home_2runner_2work_2logger__system_2logger__system_2SECURITY.html#autotoc_md99", null ],
      [ "Secure Configuration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2SECURITY.html#autotoc_md100", null ],
      [ "Roadmap", "md__2home_2runner_2work_2logger__system_2logger__system_2SECURITY.html#autotoc_md101", null ]
    ] ],
    [ "Logger System API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md103", null ],
      [ "Core Classes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md104", [
        [ "logger_module::logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md105", null ]
      ] ],
      [ "Enumerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md106", [
        [ "thread_module::log_level", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md107", null ]
      ] ],
      [ "Logger Class", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md108", [
        [ "Constructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md109", null ],
        [ "Logging Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md110", [
          [ "log (simple)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md111", null ],
          [ "log (with source location)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md112", null ]
        ] ],
        [ "Configuration Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md113", [
          [ "set_min_level", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md114", null ],
          [ "get_min_level", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md115", null ],
          [ "is_enabled", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md116", null ]
        ] ],
        [ "Writer Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md117", [
          [ "add_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md118", null ],
          [ "clear_writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md119", null ]
        ] ],
        [ "Lifecycle Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md120", [
          [ "start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md121", null ],
          [ "stop", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md122", null ],
          [ "is_running", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md123", null ],
          [ "flush", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md124", null ]
        ] ]
      ] ],
      [ "Writer Classes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md125", [
        [ "base_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md126", null ],
        [ "console_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md127", null ]
      ] ],
      [ "Log Collector", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md128", [
        [ "Features:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md129", null ]
      ] ],
      [ "Macros and Helpers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md130", null ],
      [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md131", null ],
      [ "Example Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md132", [
        [ "Complete Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md133", null ]
      ] ]
    ] ],
    [ "Logger System API Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md135", null ],
      [ "Introduction", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md136", [
        [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md137", null ]
      ] ],
      [ "Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md138", [
        [ "Basic Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md139", null ],
        [ "File Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md140", null ]
      ] ],
      [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md141", [
        [ "Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md142", null ],
        [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md143", [
          [ "Console Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md144", null ],
          [ "File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md145", null ],
          [ "Async Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md146", null ],
          [ "Batch Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md147", null ]
        ] ],
        [ "Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md148", null ],
        [ "Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md149", null ]
      ] ],
      [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md150", [
        [ "Logger Builder", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md151", null ],
        [ "Configuration Templates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md152", null ],
        [ "Environment-based Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md153", null ]
      ] ],
      [ "Advanced Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md154", [
        [ "Structured Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md155", null ],
        [ "Contextual Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md156", null ],
        [ "Performance Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md157", null ],
        [ "Crash Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md158", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md159", [
        [ "1. Use Appropriate Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md160", null ],
        [ "2. Structured Logging for Analytics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md161", null ],
        [ "3. Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md162", null ],
        [ "4. Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md163", null ],
        [ "5. Resource Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md164", null ]
      ] ],
      [ "Performance Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md165", [
        [ "Memory Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md166", null ],
        [ "Throughput Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md167", null ],
        [ "Latency Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md168", null ]
      ] ],
      [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md169", [
        [ "From Version 1.x to 2.x", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md170", null ],
        [ "From Other Logging Libraries", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md171", [
          [ "From spdlog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md172", null ],
          [ "From Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md173", null ]
        ] ]
      ] ],
      [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md174", [
        [ "Enumerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md175", null ],
        [ "Type Definitions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md176", null ],
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__DOCUMENTATION.html#autotoc_md177", null ]
      ] ]
    ] ],
    [ "Logger System Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md180", null ],
      [ "Architecture Diagram", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md181", null ],
      [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md182", [
        [ "1. Logger Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md183", null ],
        [ "2. Logger Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md184", [
          [ "Key Features:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md185", null ]
        ] ],
        [ "3. Log Collector (Async Mode)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md186", [
          [ "Design Decisions:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md187", null ]
        ] ],
        [ "4. Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md188", [
          [ "Base Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md189", null ],
          [ "Console Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md190", null ],
          [ "Extensibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md191", null ]
        ] ]
      ] ],
      [ "Threading Model", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md192", [
        [ "Synchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md193", null ],
        [ "Asynchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md194", null ],
        [ "Thread Safety Guarantees", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md195", null ]
      ] ],
      [ "Memory Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md196", [
        [ "Buffer Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md197", null ],
        [ "Object Lifetime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md198", null ],
        [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md199", null ]
      ] ],
      [ "Integration Points", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md200", [
        [ "Service Container Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md201", null ],
        [ "Direct Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md202", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md203", [
        [ "Synchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md204", null ],
        [ "Asynchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md205", null ]
      ] ],
      [ "Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md206", [
        [ "Planned Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md207", null ]
      ] ],
      [ "Platform Notes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md208", [
        [ "Extension Points", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md209", null ]
      ] ],
      [ "Design Patterns Used", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md210", null ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md211", [
        [ "For Library Users", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md212", null ],
        [ "For Contributors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2architecture.html#autotoc_md213", null ]
      ] ]
    ] ],
    [ "Logger System Best Practices Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md215", null ],
      [ "Design Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md216", [
        [ "1. Single Responsibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md217", null ],
        [ "2. Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md218", null ],
        [ "3. Interface Segregation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md219", null ]
      ] ],
      [ "Configuration Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md220", [
        [ "Environment-Specific Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md221", null ],
        [ "Configuration Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md222", null ],
        [ "Dynamic Reconfiguration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md223", null ]
      ] ],
      [ "Performance Guidelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md224", [
        [ "1. Minimize Allocations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md225", null ],
        [ "2. Lazy Evaluation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md226", null ],
        [ "3. Batch Operations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md227", null ],
        [ "4. Async Logging Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md228", null ]
      ] ],
      [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md229", [
        [ "Graceful Degradation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md230", null ],
        [ "Error Recovery", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md231", null ],
        [ "Circuit Breaker Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md232", null ]
      ] ],
      [ "Security Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md233", [
        [ "1. Sanitize Sensitive Data", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md234", null ],
        [ "2. Access Control", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md235", null ],
        [ "3. Log Injection Prevention", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md236", null ]
      ] ],
      [ "Testing Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md237", [
        [ "1. Mock Logger for Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md238", null ],
        [ "2. Performance Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md239", null ],
        [ "3. Stress Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md240", null ]
      ] ],
      [ "Production Deployment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md241", [
        [ "1. Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md242", null ],
        [ "2. Log Rotation Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md243", null ],
        [ "3. Graceful Shutdown", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md244", null ]
      ] ],
      [ "Common Pitfalls", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md245", [
        [ "1. Memory Leaks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md246", null ],
        [ "2. Deadlocks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md247", null ],
        [ "3. Performance Bottlenecks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md248", null ]
      ] ],
      [ "Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BEST__PRACTICES.html#autotoc_md249", null ]
    ] ],
    [ "Logger System CI/CD Dashboard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html", [
      [ "🚀 Build Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md251", [
        [ "Main Branch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md252", null ]
      ] ],
      [ "📊 Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md253", [
        [ "Latest Benchmark Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md254", null ],
        [ "Performance Trends", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md255", null ]
      ] ],
      [ "🔍 Code Quality", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md256", [
        [ "Static Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md257", null ],
        [ "Test Coverage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md258", null ]
      ] ],
      [ "🧪 Test Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md259", [
        [ "Test Execution Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md260", null ],
        [ "Recent Test Failures", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md261", null ]
      ] ],
      [ "🔥 Sanitizer Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md262", null ],
      [ "📈 Build Time Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md263", [
        [ "Average Build Times", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md264", null ]
      ] ],
      [ "🚨 Recent Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md265", [
        [ "Build Failures (Last 7 Days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md266", null ],
        [ "Performance Regressions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md267", null ]
      ] ],
      [ "📝 Dependency Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md268", [
        [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md269", null ]
      ] ],
      [ "🔄 CI/CD Pipeline Health", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md270", [
        [ "Workflow Execution Stats (Last 30 Days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md271", null ],
        [ "Resource Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md272", null ]
      ] ],
      [ "📅 Release History", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md273", [
        [ "Recent Releases", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md274", null ]
      ] ],
      [ "🔗 Quick Links", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md275", null ],
      [ "📊 Dashboard Update", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CI__CD__DASHBOARD.html#autotoc_md276", null ]
    ] ],
    [ "Creating Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md279", null ],
      [ "Base Writer Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md280", null ],
      [ "Simple Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md281", [
        [ "1. File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md282", null ],
        [ "2. Rotating File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md283", null ],
        [ "3. Network Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md284", null ],
        [ "4. Database Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md285", null ]
      ] ],
      [ "Advanced Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md286", [
        [ "1. Filtering Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md287", null ],
        [ "2. Async Writer Wrapper", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md288", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md289", null ],
      [ "Testing Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2custom-writers.html#autotoc_md290", null ]
    ] ],
    [ "Getting Started with Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md292", null ],
      [ "Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md293", null ],
      [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md294", [
        [ "Using CMake FetchContent (Recommended)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md295", null ],
        [ "Building from Source", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md296", null ],
        [ "Using as Installed Package", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md297", null ]
      ] ],
      [ "Basic Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md298", [
        [ "Simple Console Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md299", null ],
        [ "Logging with Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md300", null ],
        [ "Structured JSON Output", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md301", null ],
        [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md302", null ],
        [ "Filtering by Level", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md303", null ]
      ] ],
      [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md304", [
        [ "Synchronous vs Asynchronous Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md305", null ],
        [ "Multiple Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md306", null ],
        [ "Windows Notes (Networking)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md307", null ],
        [ "Console Writer Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md308", null ]
      ] ],
      [ "Integration with Thread System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md309", [
        [ "Using Service Container", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md310", null ],
        [ "Direct Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md311", null ]
      ] ],
      [ "Next Steps", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2getting-started.html#autotoc_md312", null ]
    ] ],
    [ "Logger System Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md314", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md315", [
        [ "Breaking Changes Summary (v2.0+)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md316", null ]
      ] ],
      [ "Version Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md317", [
        [ "From v1.x to v2.x", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md318", [
          [ "1. Error Handling Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md319", null ],
          [ "2. Memory Management Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md320", null ],
          [ "3. Configuration Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md321", null ],
          [ "4. Logging API Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md322", null ]
        ] ]
      ] ],
      [ "API Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md323", [
        [ "Core Logger API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md324", null ],
        [ "Writer API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md325", null ],
        [ "Configuration API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md326", null ]
      ] ],
      [ "Configuration Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md327", [
        [ "From INI/XML Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md328", null ],
        [ "Environment-based Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md329", null ]
      ] ],
      [ "Migration from Other Libraries", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md330", [
        [ "From spdlog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md331", [
          [ "spdlog Pattern Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md332", null ]
        ] ],
        [ "From Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md333", null ],
        [ "From Google glog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md334", null ],
        [ "From log4cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md335", null ]
      ] ],
      [ "Compatibility Wrappers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md336", [
        [ "Legacy API Wrapper", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md337", null ],
        [ "Wrapper for Other Libraries", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md338", null ]
      ] ],
      [ "Step-by-Step Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md339", [
        [ "Phase 1: Preparation (1-2 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md340", null ],
        [ "Phase 2: Core Migration (2-3 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md341", null ],
        [ "Phase 3: Feature Migration (2-3 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md342", null ],
        [ "Phase 4: Optimization (1-2 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md343", null ],
        [ "Phase 5: Validation (1 day)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md344", null ]
      ] ],
      [ "Common Issues and Solutions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md345", [
        [ "Issue 1: Memory Leaks with Raw Pointers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md346", null ],
        [ "Issue 2: Exception Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md347", null ],
        [ "Issue 3: Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md348", null ],
        [ "Issue 4: Configuration Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md349", null ],
        [ "Issue 5: Custom Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md350", null ]
      ] ],
      [ "Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md351", null ],
      [ "Support and Resources", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2MIGRATION__GUIDE.html#autotoc_md352", null ]
    ] ],
    [ "Logger System Performance Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md354", null ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md355", [
        [ "Synchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md356", null ],
        [ "Asynchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md357", null ]
      ] ],
      [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md358", [
        [ "Test Environment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md359", null ],
        [ "Single Thread Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md360", null ],
        [ "Multi-threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md361", null ],
        [ "Memory Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md362", null ]
      ] ],
      [ "Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md363", [
        [ "1. Choose the Right Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md364", null ],
        [ "2. Buffer Size Tuning", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md365", null ],
        [ "2.1 Batch Size Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md366", null ],
        [ "3. Level Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md367", null ],
        [ "4. Message Construction", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md368", null ],
        [ "5. Writer Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md369", [
          [ "Console Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md370", null ],
          [ "Custom High-Performance Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md371", null ]
        ] ],
        [ "6. Batch Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md372", null ]
      ] ],
      [ "Performance Anti-patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md373", [
        [ "1. Synchronous I/O in Hot Paths", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md374", null ],
        [ "2. Excessive String Formatting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md375", null ],
        [ "3. Logging in Tight Loops", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md376", null ]
      ] ],
      [ "Profiling and Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md377", [
        [ "Built-in Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md378", null ],
        [ "External Profiling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md379", null ]
      ] ],
      [ "Best Practices Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md380", null ],
      [ "Platform-Specific Optimizations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md381", [
        [ "Linux", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md382", null ],
        [ "Windows", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md383", null ],
        [ "macOS", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance.html#autotoc_md384", null ]
      ] ]
    ] ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", "functions_vars" ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ],
    [ "Examples", "examples.html", "examples" ]
  ] ]
];

var NAVTREEINDEX =
[
"Advanced-example.html",
"classlogger__module_1_1compact__formatter.html",
"classlogger__module_1_1file__writer.html#a23076abfb99a7c2f26a6f0736bbf33c5",
"classlogger__module_1_1log__aggregator.html#a9d4263c4f3d89de4d811196a61952058",
"classlogger__module_1_1logger.html#af0f6ca9bc91d3ebebb46de27822f52bb",
"classlogger__module_1_1overflow__policy__base.html#afdfa24948eb8a8044b6b5692144090e3",
"classlogger__module_1_1small__string.html#af0a65072bbcbbbf65c8e5f912c041e86",
"error__codes_8h.html#a23f0da6f87d99d4e4c13ef24c21135eba9cc57114b319994e90322314a00521d4",
"logger__types_8h.html#ae3766003c8285b1cba6e1d4ee4475a19ad969190b2d0c64b046cf1a95545d4f56",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2api-reference.html#autotoc_md126",
"namespacelogger__system.html#ad8e3c64f7744daa2d80e68c884e342d1a1ea4c3ab05ee0c6d4de30740443769cb",
"structlogger__module_1_1log__route.html#aadfb464af1c631c27fbd5c7fcd8bfaac",
"structlogger__module_1_1small__string_1_1memory__stats.html#a9161e171fcf49191b0109471913009d9"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';