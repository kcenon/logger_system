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
  [ "logger_system", "index.html", [
    [ "Logger System", "index.html", "index" ],
    [ "Threading Ecosystem Architecture", "md_ARCHITECTURE.html", [
      [ "🏗️ Ecosystem Overview", "md_ARCHITECTURE.html#autotoc_md45", null ],
      [ "📋 Project Roles & Responsibilities", "md_ARCHITECTURE.html#autotoc_md46", [
        [ "1. thread_system (Foundation)", "md_ARCHITECTURE.html#autotoc_md47", [
          [ "Responsibilities:", "md_ARCHITECTURE.html#autotoc_md48", null ],
          [ "Key Components:", "md_ARCHITECTURE.html#autotoc_md49", null ],
          [ "Dependencies:", "md_ARCHITECTURE.html#autotoc_md50", null ]
        ] ],
        [ "2. logger_system (Logging)", "md_ARCHITECTURE.html#autotoc_md52", [
          [ "Responsibilities:", "md_ARCHITECTURE.html#autotoc_md53", null ],
          [ "Key Components:", "md_ARCHITECTURE.html#autotoc_md54", null ],
          [ "Dependencies:", "md_ARCHITECTURE.html#autotoc_md55", null ]
        ] ],
        [ "3. monitoring_system (Metrics)", "md_ARCHITECTURE.html#autotoc_md57", [
          [ "Responsibilities:", "md_ARCHITECTURE.html#autotoc_md58", null ],
          [ "Key Components:", "md_ARCHITECTURE.html#autotoc_md59", null ],
          [ "Dependencies:", "md_ARCHITECTURE.html#autotoc_md60", null ]
        ] ],
        [ "4. integrated_thread_system (Integration Hub)", "md_ARCHITECTURE.html#autotoc_md62", [
          [ "Responsibilities:", "md_ARCHITECTURE.html#autotoc_md63", null ],
          [ "Key Components:", "md_ARCHITECTURE.html#autotoc_md64", null ],
          [ "Dependencies:", "md_ARCHITECTURE.html#autotoc_md65", null ]
        ] ]
      ] ],
      [ "🔄 Dependency Flow & Interface Contracts", "md_ARCHITECTURE.html#autotoc_md66", [
        [ "Interface Hierarchy", "md_ARCHITECTURE.html#autotoc_md67", null ],
        [ "Dependency Graph", "md_ARCHITECTURE.html#autotoc_md68", null ],
        [ "Build Order Requirements", "md_ARCHITECTURE.html#autotoc_md69", null ]
      ] ],
      [ "🔧 Integration Patterns", "md_ARCHITECTURE.html#autotoc_md70", [
        [ "1. Interface-Based Integration", "md_ARCHITECTURE.html#autotoc_md71", null ],
        [ "2. Dependency Injection Pattern", "md_ARCHITECTURE.html#autotoc_md72", null ],
        [ "3. Configuration Management", "md_ARCHITECTURE.html#autotoc_md73", null ]
      ] ],
      [ "📊 Performance Characteristics", "md_ARCHITECTURE.html#autotoc_md74", [
        [ "Design Principles", "md_ARCHITECTURE.html#autotoc_md75", null ],
        [ "Performance Metrics", "md_ARCHITECTURE.html#autotoc_md76", null ]
      ] ],
      [ "🔄 Evolution: Monolithic → Modular", "md_ARCHITECTURE.html#autotoc_md77", [
        [ "Before: Monolithic Architecture", "md_ARCHITECTURE.html#autotoc_md78", null ],
        [ "After: Modular Architecture", "md_ARCHITECTURE.html#autotoc_md79", null ],
        [ "Migration Benefits", "md_ARCHITECTURE.html#autotoc_md80", null ]
      ] ],
      [ "🚀 Getting Started", "md_ARCHITECTURE.html#autotoc_md81", [
        [ "1. Development Environment Setup", "md_ARCHITECTURE.html#autotoc_md82", null ],
        [ "2. Build Order (Local Development)", "md_ARCHITECTURE.html#autotoc_md83", null ],
        [ "3. Verification", "md_ARCHITECTURE.html#autotoc_md84", null ]
      ] ],
      [ "📚 Documentation Structure", "md_ARCHITECTURE.html#autotoc_md85", [
        [ "thread_system", "md_ARCHITECTURE.html#autotoc_md86", null ],
        [ "logger_system", "md_ARCHITECTURE.html#autotoc_md87", null ],
        [ "monitoring_system", "md_ARCHITECTURE.html#autotoc_md88", null ],
        [ "integrated_thread_system", "md_ARCHITECTURE.html#autotoc_md89", null ]
      ] ],
      [ "🔮 Future Roadmap", "md_ARCHITECTURE.html#autotoc_md90", [
        [ "Phase 1: Stabilization (Current)", "md_ARCHITECTURE.html#autotoc_md91", null ],
        [ "Phase 2: Enhancement", "md_ARCHITECTURE.html#autotoc_md92", null ],
        [ "Phase 3: Ecosystem Expansion", "md_ARCHITECTURE.html#autotoc_md93", null ]
      ] ]
    ] ],
    [ "Security Guide", "md_SECURITY.html", [
      [ "Threat Model (Scope)", "md_SECURITY.html#autotoc_md96", null ],
      [ "Current Capabilities", "md_SECURITY.html#autotoc_md97", null ],
      [ "Important Limitations", "md_SECURITY.html#autotoc_md98", null ],
      [ "Recommendations (Production)", "md_SECURITY.html#autotoc_md99", null ],
      [ "Secure Configuration Checklist", "md_SECURITY.html#autotoc_md100", null ],
      [ "Roadmap", "md_SECURITY.html#autotoc_md101", null ]
    ] ],
    [ "Logger System API Reference", "md_docs_2api-reference.html", [
      [ "Table of Contents", "md_docs_2api-reference.html#autotoc_md103", null ],
      [ "Core Classes", "md_docs_2api-reference.html#autotoc_md104", [
        [ "logger_module::logger", "md_docs_2api-reference.html#autotoc_md105", null ]
      ] ],
      [ "Enumerations", "md_docs_2api-reference.html#autotoc_md106", [
        [ "thread_module::log_level", "md_docs_2api-reference.html#autotoc_md107", null ]
      ] ],
      [ "Logger Class", "md_docs_2api-reference.html#autotoc_md108", [
        [ "Constructor", "md_docs_2api-reference.html#autotoc_md109", null ],
        [ "Logging Methods", "md_docs_2api-reference.html#autotoc_md110", [
          [ "log (simple)", "md_docs_2api-reference.html#autotoc_md111", null ],
          [ "log (with source location)", "md_docs_2api-reference.html#autotoc_md112", null ]
        ] ],
        [ "Configuration Methods", "md_docs_2api-reference.html#autotoc_md113", [
          [ "set_min_level", "md_docs_2api-reference.html#autotoc_md114", null ],
          [ "get_min_level", "md_docs_2api-reference.html#autotoc_md115", null ],
          [ "is_enabled", "md_docs_2api-reference.html#autotoc_md116", null ]
        ] ],
        [ "Writer Management", "md_docs_2api-reference.html#autotoc_md117", [
          [ "add_writer", "md_docs_2api-reference.html#autotoc_md118", null ],
          [ "clear_writers", "md_docs_2api-reference.html#autotoc_md119", null ]
        ] ],
        [ "Lifecycle Methods", "md_docs_2api-reference.html#autotoc_md120", [
          [ "start", "md_docs_2api-reference.html#autotoc_md121", null ],
          [ "stop", "md_docs_2api-reference.html#autotoc_md122", null ],
          [ "is_running", "md_docs_2api-reference.html#autotoc_md123", null ],
          [ "flush", "md_docs_2api-reference.html#autotoc_md124", null ]
        ] ]
      ] ],
      [ "Writer Classes", "md_docs_2api-reference.html#autotoc_md125", [
        [ "base_writer", "md_docs_2api-reference.html#autotoc_md126", null ],
        [ "console_writer", "md_docs_2api-reference.html#autotoc_md127", null ]
      ] ],
      [ "Log Collector", "md_docs_2api-reference.html#autotoc_md128", [
        [ "Features:", "md_docs_2api-reference.html#autotoc_md129", null ]
      ] ],
      [ "Macros and Helpers", "md_docs_2api-reference.html#autotoc_md130", null ],
      [ "Thread Safety", "md_docs_2api-reference.html#autotoc_md131", null ],
      [ "Example Usage", "md_docs_2api-reference.html#autotoc_md132", [
        [ "Complete Example", "md_docs_2api-reference.html#autotoc_md133", null ]
      ] ]
    ] ],
    [ "Logger System Architecture", "md_docs_2architecture.html", [
      [ "Overview", "md_docs_2architecture.html#autotoc_md135", null ],
      [ "Architecture Diagram", "md_docs_2architecture.html#autotoc_md136", null ],
      [ "Core Components", "md_docs_2architecture.html#autotoc_md137", [
        [ "1. Logger Interface", "md_docs_2architecture.html#autotoc_md138", null ],
        [ "2. Logger Implementation", "md_docs_2architecture.html#autotoc_md139", [
          [ "Key Features:", "md_docs_2architecture.html#autotoc_md140", null ]
        ] ],
        [ "3. Log Collector (Async Mode)", "md_docs_2architecture.html#autotoc_md141", [
          [ "Design Decisions:", "md_docs_2architecture.html#autotoc_md142", null ]
        ] ],
        [ "4. Writers", "md_docs_2architecture.html#autotoc_md143", [
          [ "Base Writer", "md_docs_2architecture.html#autotoc_md144", null ],
          [ "Console Writer", "md_docs_2architecture.html#autotoc_md145", null ],
          [ "Extensibility", "md_docs_2architecture.html#autotoc_md146", null ]
        ] ]
      ] ],
      [ "Threading Model", "md_docs_2architecture.html#autotoc_md147", [
        [ "Synchronous Mode", "md_docs_2architecture.html#autotoc_md148", null ],
        [ "Asynchronous Mode", "md_docs_2architecture.html#autotoc_md149", null ],
        [ "Thread Safety Guarantees", "md_docs_2architecture.html#autotoc_md150", null ]
      ] ],
      [ "Memory Management", "md_docs_2architecture.html#autotoc_md151", [
        [ "Buffer Management", "md_docs_2architecture.html#autotoc_md152", null ],
        [ "Object Lifetime", "md_docs_2architecture.html#autotoc_md153", null ],
        [ "Performance Considerations", "md_docs_2architecture.html#autotoc_md154", null ]
      ] ],
      [ "Integration Points", "md_docs_2architecture.html#autotoc_md155", [
        [ "Service Container Integration", "md_docs_2architecture.html#autotoc_md156", null ],
        [ "Direct Integration", "md_docs_2architecture.html#autotoc_md157", null ]
      ] ],
      [ "Performance Characteristics", "md_docs_2architecture.html#autotoc_md158", [
        [ "Synchronous Mode", "md_docs_2architecture.html#autotoc_md159", null ],
        [ "Asynchronous Mode", "md_docs_2architecture.html#autotoc_md160", null ]
      ] ],
      [ "Future Enhancements", "md_docs_2architecture.html#autotoc_md161", [
        [ "Planned Features", "md_docs_2architecture.html#autotoc_md162", null ]
      ] ],
      [ "Platform Notes", "md_docs_2architecture.html#autotoc_md163", [
        [ "Extension Points", "md_docs_2architecture.html#autotoc_md164", null ]
      ] ],
      [ "Design Patterns Used", "md_docs_2architecture.html#autotoc_md165", null ],
      [ "Best Practices", "md_docs_2architecture.html#autotoc_md166", [
        [ "For Library Users", "md_docs_2architecture.html#autotoc_md167", null ],
        [ "For Contributors", "md_docs_2architecture.html#autotoc_md168", null ]
      ] ]
    ] ],
    [ "Creating Custom Writers", "md_docs_2custom-writers.html", [
      [ "Overview", "md_docs_2custom-writers.html#autotoc_md170", null ],
      [ "Base Writer Interface", "md_docs_2custom-writers.html#autotoc_md171", null ],
      [ "Simple Examples", "md_docs_2custom-writers.html#autotoc_md172", [
        [ "1. File Writer", "md_docs_2custom-writers.html#autotoc_md173", null ],
        [ "2. Rotating File Writer", "md_docs_2custom-writers.html#autotoc_md174", null ],
        [ "3. Network Writer", "md_docs_2custom-writers.html#autotoc_md175", null ],
        [ "4. Database Writer", "md_docs_2custom-writers.html#autotoc_md176", null ]
      ] ],
      [ "Advanced Patterns", "md_docs_2custom-writers.html#autotoc_md177", [
        [ "1. Filtering Writer", "md_docs_2custom-writers.html#autotoc_md178", null ],
        [ "2. Async Writer Wrapper", "md_docs_2custom-writers.html#autotoc_md179", null ]
      ] ],
      [ "Best Practices", "md_docs_2custom-writers.html#autotoc_md180", null ],
      [ "Testing Custom Writers", "md_docs_2custom-writers.html#autotoc_md181", null ]
    ] ],
    [ "Getting Started with Logger System", "md_docs_2getting-started.html", [
      [ "Table of Contents", "md_docs_2getting-started.html#autotoc_md183", null ],
      [ "Requirements", "md_docs_2getting-started.html#autotoc_md184", null ],
      [ "Installation", "md_docs_2getting-started.html#autotoc_md185", [
        [ "Using CMake FetchContent (Recommended)", "md_docs_2getting-started.html#autotoc_md186", null ],
        [ "Building from Source", "md_docs_2getting-started.html#autotoc_md187", null ],
        [ "Using as Installed Package", "md_docs_2getting-started.html#autotoc_md188", null ]
      ] ],
      [ "Basic Usage", "md_docs_2getting-started.html#autotoc_md189", [
        [ "Simple Console Logging", "md_docs_2getting-started.html#autotoc_md190", null ],
        [ "Logging with Source Location", "md_docs_2getting-started.html#autotoc_md191", null ],
        [ "Structured JSON Output", "md_docs_2getting-started.html#autotoc_md192", null ],
        [ "Log Levels", "md_docs_2getting-started.html#autotoc_md193", null ],
        [ "Filtering by Level", "md_docs_2getting-started.html#autotoc_md194", null ]
      ] ],
      [ "Configuration", "md_docs_2getting-started.html#autotoc_md195", [
        [ "Synchronous vs Asynchronous Logging", "md_docs_2getting-started.html#autotoc_md196", null ],
        [ "Multiple Writers", "md_docs_2getting-started.html#autotoc_md197", null ],
        [ "Windows Notes (Networking)", "md_docs_2getting-started.html#autotoc_md198", null ],
        [ "Console Writer Options", "md_docs_2getting-started.html#autotoc_md199", null ]
      ] ],
      [ "Integration with Thread System", "md_docs_2getting-started.html#autotoc_md200", [
        [ "Using Service Container", "md_docs_2getting-started.html#autotoc_md201", null ],
        [ "Direct Integration", "md_docs_2getting-started.html#autotoc_md202", null ]
      ] ],
      [ "Next Steps", "md_docs_2getting-started.html#autotoc_md203", null ]
    ] ],
    [ "Logger System Performance Guide", "md_docs_2performance.html", [
      [ "Overview", "md_docs_2performance.html#autotoc_md205", null ],
      [ "Performance Characteristics", "md_docs_2performance.html#autotoc_md206", [
        [ "Synchronous Mode", "md_docs_2performance.html#autotoc_md207", null ],
        [ "Asynchronous Mode", "md_docs_2performance.html#autotoc_md208", null ]
      ] ],
      [ "Benchmarks", "md_docs_2performance.html#autotoc_md209", [
        [ "Test Environment", "md_docs_2performance.html#autotoc_md210", null ],
        [ "Single Thread Performance", "md_docs_2performance.html#autotoc_md211", null ],
        [ "Multi-threaded Performance", "md_docs_2performance.html#autotoc_md212", null ],
        [ "Memory Usage", "md_docs_2performance.html#autotoc_md213", null ]
      ] ],
      [ "Optimization Strategies", "md_docs_2performance.html#autotoc_md214", [
        [ "1. Choose the Right Mode", "md_docs_2performance.html#autotoc_md215", null ],
        [ "2. Buffer Size Tuning", "md_docs_2performance.html#autotoc_md216", null ],
        [ "2.1 Batch Size Considerations", "md_docs_2performance.html#autotoc_md217", null ],
        [ "3. Level Filtering", "md_docs_2performance.html#autotoc_md218", null ],
        [ "4. Message Construction", "md_docs_2performance.html#autotoc_md219", null ],
        [ "5. Writer Optimization", "md_docs_2performance.html#autotoc_md220", [
          [ "Console Writer", "md_docs_2performance.html#autotoc_md221", null ],
          [ "Custom High-Performance Writer", "md_docs_2performance.html#autotoc_md222", null ]
        ] ],
        [ "6. Batch Processing", "md_docs_2performance.html#autotoc_md223", null ]
      ] ],
      [ "Performance Anti-patterns", "md_docs_2performance.html#autotoc_md224", [
        [ "1. Synchronous I/O in Hot Paths", "md_docs_2performance.html#autotoc_md225", null ],
        [ "2. Excessive String Formatting", "md_docs_2performance.html#autotoc_md226", null ],
        [ "3. Logging in Tight Loops", "md_docs_2performance.html#autotoc_md227", null ]
      ] ],
      [ "Profiling and Monitoring", "md_docs_2performance.html#autotoc_md228", [
        [ "Built-in Metrics", "md_docs_2performance.html#autotoc_md229", null ],
        [ "External Profiling", "md_docs_2performance.html#autotoc_md230", null ]
      ] ],
      [ "Best Practices Summary", "md_docs_2performance.html#autotoc_md231", null ],
      [ "Platform-Specific Optimizations", "md_docs_2performance.html#autotoc_md232", [
        [ "Linux", "md_docs_2performance.html#autotoc_md233", null ],
        [ "Windows", "md_docs_2performance.html#autotoc_md234", null ],
        [ "macOS", "md_docs_2performance.html#autotoc_md235", null ]
      ] ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
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
        [ "Variables", "functions_vars.html", null ],
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
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"classlogger__module_1_1file__writer.html#a8690be70d550d6938599fbfef00ae3b7",
"classlogger__module_1_1log__writer__interface.html#a9ab1ce565b731e06d1fec66588d420bf",
"classlogger__module_1_1network__writer.html",
"classthread__module_1_1logger__registry.html",
"md_ARCHITECTURE.html#autotoc_md69",
"network__writer_8h.html",
"structlogger__module_1_1performance__metrics.html#ab86d1395f3cec32af67c20f3a4c71275"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';