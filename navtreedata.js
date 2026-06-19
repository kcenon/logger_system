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
    [ "System Overview", "index.html#overview", null ],
    [ "Key Features", "index.html#features", null ],
    [ "Architecture Diagram", "index.html#architecture", null ],
    [ "Learning Resources", "index.html#learning", null ],
    [ "Quick Start", "index.html#quickstart", null ],
    [ "Installation", "index.html#installation", [
      [ "CMake FetchContent (Recommended)", "index.html#install_fetchcontent", null ],
      [ "vcpkg", "index.html#install_vcpkg", null ]
    ] ],
    [ "Module Overview", "index.html#modules", null ],
    [ "Examples", "index.html#examples", null ],
    [ "Related Systems", "index.html#related", null ],
    [ "README", "md_README.html", [
      [ "Logger System", "md_README.html#autotoc_md0", [
        [ "Table of Contents", "md_README.html#autotoc_md1", null ],
        [ "Overview", "md_README.html#autotoc_md2", null ],
        [ "Project Layout", "md_README.html#autotoc_md4", null ],
        [ "Quick Start", "md_README.html#autotoc_md6", [
          [ "Basic Example", "md_README.html#autotoc_md7", null ],
          [ "Composing Writers with Decorator Pattern", "md_README.html#autotoc_md8", [
            [ "Decorator Application Order", "md_README.html#autotoc_md9", null ],
            [ "Common Usage Patterns", "md_README.html#autotoc_md10", null ]
          ] ],
          [ "Installation", "md_README.html#autotoc_md11", null ],
          [ "Requirements", "md_README.html#autotoc_md12", [
            [ "Optional Feature Dependencies", "md_README.html#autotoc_md13", null ],
            [ "Development and Benchmark Dependencies", "md_README.html#autotoc_md14", null ],
            [ "Dependency Flow", "md_README.html#autotoc_md15", null ],
            [ "Building with Dependencies", "md_README.html#autotoc_md16", null ]
          ] ]
        ] ],
        [ "Core Features", "md_README.html#autotoc_md18", [
          [ "Asynchronous Logging", "md_README.html#autotoc_md19", null ],
          [ "Multiple Writer Types", "md_README.html#autotoc_md20", null ],
          [ "OpenTelemetry Integration (v3.0.0)", "md_README.html#autotoc_md21", null ],
          [ "Security Features (v3.0.0)", "md_README.html#autotoc_md22", null ],
          [ "Structured Logging (v3.1.0)", "md_README.html#autotoc_md23", null ]
        ] ],
        [ "Performance Highlights", "md_README.html#autotoc_md25", [
          [ "Throughput", "md_README.html#autotoc_md26", null ],
          [ "Latency", "md_README.html#autotoc_md27", null ],
          [ "Memory Efficiency", "md_README.html#autotoc_md28", null ]
        ] ],
        [ "Architecture Overview", "md_README.html#autotoc_md30", [
          [ "Modular Design", "md_README.html#autotoc_md31", null ],
          [ "Key Components", "md_README.html#autotoc_md32", null ]
        ] ],
        [ "Ecosystem Integration", "md_README.html#autotoc_md34", [
          [ "Ecosystem Dependency Map", "md_README.html#autotoc_md35", null ],
          [ "Dependencies", "md_README.html#autotoc_md36", null ],
          [ "Integration Pattern", "md_README.html#autotoc_md37", null ]
        ] ],
        [ "C++20 Module Support", "md_README.html#autotoc_md39", [
          [ "Requirements for Modules", "md_README.html#autotoc_md40", null ],
          [ "Building with Modules", "md_README.html#autotoc_md41", null ],
          [ "Using Modules", "md_README.html#autotoc_md42", null ],
          [ "Module Structure", "md_README.html#autotoc_md43", null ]
        ] ],
        [ "", "md_README.html#autotoc_md44", null ],
        [ "Documentation", "md_README.html#autotoc_md45", [
          [ "Getting Started", "md_README.html#autotoc_md46", null ],
          [ "Core Documentation", "md_README.html#autotoc_md47", null ],
          [ "Advanced Topics", "md_README.html#autotoc_md48", null ],
          [ "Development", "md_README.html#autotoc_md49", null ]
        ] ],
        [ "Compliance", "md_README.html#autotoc_md51", null ],
        [ "Configuration Templates", "md_README.html#autotoc_md53", [
          [ "Advanced Configuration", "md_README.html#autotoc_md54", null ]
        ] ],
        [ "Build Configuration", "md_README.html#autotoc_md56", [
          [ "CMake Feature Flags", "md_README.html#autotoc_md57", null ]
        ] ],
        [ "Platform Support", "md_README.html#autotoc_md59", [
          [ "Officially Supported", "md_README.html#autotoc_md60", null ]
        ] ],
        [ "Testing", "md_README.html#autotoc_md62", [
          [ "Test Coverage", "md_README.html#autotoc_md63", null ],
          [ "Running Tests", "md_README.html#autotoc_md64", null ],
          [ "CI/CD Status", "md_README.html#autotoc_md65", null ]
        ] ],
        [ "Contributing", "md_README.html#autotoc_md67", [
          [ "Development Workflow", "md_README.html#autotoc_md68", null ],
          [ "Code Standards", "md_README.html#autotoc_md69", null ]
        ] ],
        [ "Support", "md_README.html#autotoc_md71", null ],
        [ "License", "md_README.html#autotoc_md73", null ],
        [ "Acknowledgments", "md_README.html#autotoc_md75", null ]
      ] ]
    ] ],
    [ "Security Policy", "md_SECURITY.html", [
      [ "Supported Versions", "md_SECURITY.html#autotoc_md78", null ],
      [ "Reporting a Vulnerability", "md_SECURITY.html#autotoc_md79", [
        [ "What to Include", "md_SECURITY.html#autotoc_md80", null ],
        [ "Response Timeline", "md_SECURITY.html#autotoc_md81", null ],
        [ "After Reporting", "md_SECURITY.html#autotoc_md82", null ]
      ] ],
      [ "Security Best Practices", "md_SECURITY.html#autotoc_md83", null ]
    ] ],
    [ "ADR-001-decorator-pattern-adoption", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html", [
      [ "ADR-001: Decorator Pattern for Writer Composition", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md89", [
        [ "autotoc_md87", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md87", null ],
        [ "category: \"ADR\"", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md88", null ],
        [ "Context", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md90", null ],
        [ "Decision", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md91", null ],
        [ "Alternatives Considered", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md92", [
          [ "Monolithic Writer Classes", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md93", null ],
          [ "Template-Based Policy Composition (Mixin)", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md94", null ],
          [ "Strategy Pattern with Feature Flags", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md95", null ]
        ] ],
        [ "Consequences", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md96", [
          [ "Positive", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md97", null ],
          [ "Negative", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md98", null ]
        ] ]
      ] ]
    ] ],
    [ "ADR-002-async-writer-architecture", "md_docs_2adr_2ADR-002-async-writer-architecture.html", [
      [ "ADR-002: Async Writer Architecture", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md101", [
        [ "autotoc_md99", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md99", null ],
        [ "category: \"ADR\"", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md100", null ],
        [ "Context", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md102", null ],
        [ "Decision", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md103", null ],
        [ "Alternatives Considered", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md104", [
          [ "Always Use std::jthread", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md105", null ],
          [ "Always Require thread_system", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md106", null ],
          [ "Lock-Free Ring Buffer", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md107", null ]
        ] ],
        [ "Consequences", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md108", [
          [ "Positive", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md109", null ],
          [ "Negative", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md110", null ]
        ] ]
      ] ]
    ] ],
    [ "ARCHITECTURE", "md_docs_2advanced_2ARCHITECTURE.html", [
      [ "Architecture - Logger System", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md113", [
        [ "autotoc_md111", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md111", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md112", null ],
        [ "Overview", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md114", null ],
        [ "Table of Contents", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md116", null ],
        [ "Design Philosophy", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md118", [
          [ "Core Principles", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md119", null ]
        ] ],
        [ "Layered Architecture", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md121", [
          [ "Layer Responsibilities", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md122", null ]
        ] ],
        [ "Core Components", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md124", [
          [ "Logger (core/logger.h, core/logger.cpp)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md125", null ],
          [ "Logger Builder (core/logger_builder.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md127", null ],
          [ "Log Collector (core/log_collector.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md129", null ],
          [ "Writers", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md131", [
            [ "Console Writer (writers/console_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md132", null ],
            [ "File Writer (writers/file_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md133", null ],
            [ "Rotating File Writer (writers/rotating_file_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md134", null ],
            [ "Network Writer (writers/network_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md135", null ],
            [ "Critical Writer (writers/critical_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md136", null ],
            [ "Async Writer (writers/async_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md137", null ]
          ] ],
          [ "Formatters", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md139", [
            [ "Base Formatter (formatters/base_formatter.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md140", null ],
            [ "JSON Formatter", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md141", null ],
            [ "XML Formatter", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md142", null ]
          ] ],
          [ "Filters", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md144", [
            [ "Level Filter (filters/log_filter.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md145", null ],
            [ "Regex Filter", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md146", null ],
            [ "Function Filter", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md147", null ]
          ] ]
        ] ],
        [ "Integration Architecture", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md149", [
          [ "Common System Integration", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md150", null ],
          [ "Thread System Integration", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md151", null ],
          [ "Monitoring System Integration", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md152", null ]
        ] ],
        [ "Threading Model", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md154", [
          [ "Asynchronous Processing", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md155", null ],
          [ "Thread Safety Guarantees", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md156", null ]
        ] ],
        [ "Memory Management", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md158", [
          [ "Small String Optimization (SSO)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md159", null ],
          [ "Log Entry Structure", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md160", null ]
        ] ],
        [ "Performance Characteristics", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md162", [
          [ "Benchmarks", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md163", null ],
          [ "Optimization Techniques", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md164", null ]
        ] ],
        [ "Design Patterns", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md166", [
          [ "Observer Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md167", null ],
          [ "Strategy Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md168", null ],
          [ "Builder Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md169", null ],
          [ "Template Method Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md170", null ],
          [ "Dependency Injection", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md171", null ],
          [ "Factory Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md172", null ]
        ] ],
        [ "Build Configuration", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md174", [
          [ "CMake Options", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md175", null ],
          [ "Dependency Resolution", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md176", null ],
          [ "Compiler Requirements", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md177", null ]
        ] ],
        [ "Future Enhancements", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md179", [
          [ "Planned Features", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md180", null ],
          [ "Research Areas", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md181", null ]
        ] ],
        [ "Error Code Allocation", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md183", null ],
        [ "References", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md185", null ]
      ] ]
    ] ],
    [ "ASYNC_WRITERS.kr", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html", [
      [ "비동기 Writer 구현체 가이드", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md189", [
        [ "autotoc_md187", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md187", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md188", null ],
        [ "개요", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md190", null ],
        [ "목차", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md192", null ],
        [ "빠른 선택 가이드", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md194", [
          [ "선택 플로우차트", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md195", null ]
        ] ],
        [ "구현체 비교", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md197", null ],
        [ "상세 설명", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md199", [
          [ "async_writer (공개 API)", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md200", [
            [ "주요 특징", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md201", null ],
            [ "아키텍처", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md202", null ],
            [ "API 레퍼런스", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md203", null ],
            [ "사용 시기", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md204", null ]
          ] ],
          [ "async_worker (내부)", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md206", [
            [ "주요 특징", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md207", null ],
            [ "아키텍처", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md208", null ],
            [ "사용 시기", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md209", null ]
          ] ]
        ] ],
        [ "", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md210", [
          [ "high_performance_async_writer (내부)", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md211", [
            [ "주요 특징", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md212", null ],
            [ "아키텍처", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md213", null ],
            [ "설정", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md214", null ],
            [ "사용 시기", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md215", null ]
          ] ]
        ] ],
        [ "", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md216", null ],
        [ "성능 특성", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md217", [
          [ "벤치마크 비교", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md218", null ],
          [ "지연 시간 분포", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md219", null ]
        ] ],
        [ "사용 예제", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md221", [
          [ "예제 1: 기본 async_writer 사용", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md222", null ],
          [ "예제 2: 콘솔 출력과 async_writer", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md223", null ],
          [ "예제 3: 큐 가득 참 처리", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md224", null ],
          [ "예제 4: 정상 종료 패턴", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md225", null ]
        ] ],
        [ "모범 사례", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md227", [
          [ "큐 크기 선택", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md228", null ],
          [ "플러시 타임아웃 설정", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md229", null ],
          [ "에러 처리", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md230", null ],
          [ "생명주기 관리", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md231", null ],
          [ "스레드 안전성 고려사항", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md232", null ]
        ] ],
        [ "마이그레이션 가이드", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md234", [
          [ "동기에서 async_writer로", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md235", null ],
          [ "큐 가득 참 마이그레이션", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md236", null ]
        ] ],
        [ "관련 문서", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md238", null ]
      ] ]
    ] ],
    [ "ASYNC_WRITERS", "md_docs_2advanced_2ASYNC__WRITERS.html", [
      [ "Asynchronous Writer Implementations Guide", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md242", [
        [ "autotoc_md240", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md240", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md241", null ],
        [ "Overview", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md243", null ],
        [ "Table of Contents", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md245", null ],
        [ "Quick Decision Guide", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md247", [
          [ "Decision Flowchart", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md248", null ]
        ] ],
        [ "Implementation Comparison", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md250", null ],
        [ "Detailed Descriptions", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md252", [
          [ "async_writer (Public API)", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md253", [
            [ "Key Features", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md254", null ],
            [ "Architecture", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md255", null ],
            [ "API Reference", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md256", null ],
            [ "When to Use", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md257", null ]
          ] ],
          [ "async_worker (Internal)", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md259", [
            [ "Key Features", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md260", null ],
            [ "Architecture", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md261", null ],
            [ "When to Use", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md262", null ]
          ] ]
        ] ],
        [ "", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md263", [
          [ "high_performance_async_writer (Internal)", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md264", [
            [ "Key Features", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md265", null ],
            [ "Architecture", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md266", null ],
            [ "Configuration", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md267", null ],
            [ "When to Use", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md268", null ]
          ] ]
        ] ],
        [ "", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md269", null ],
        [ "Performance Characteristics", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md270", [
          [ "Benchmark Comparison", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md271", null ],
          [ "Latency Distribution", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md272", null ]
        ] ],
        [ "Usage Examples", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md274", [
          [ "Example 1: Basic async_writer Usage", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md275", null ],
          [ "Example 2: async_writer with Console Output", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md276", null ],
          [ "Example 3: Queue Full Handling", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md277", null ],
          [ "Example 4: Graceful Shutdown Pattern", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md278", null ]
        ] ],
        [ "Best Practices", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md280", [
          [ "Queue Size Selection", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md281", null ],
          [ "Flush Timeout Configuration", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md282", null ],
          [ "Error Handling", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md283", null ],
          [ "Lifecycle Management", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md284", null ],
          [ "Thread Safety Considerations", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md285", null ]
        ] ],
        [ "Migration Guide", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md287", [
          [ "From Synchronous to async_writer", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md288", null ],
          [ "Queue Full Migration", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md289", null ]
        ] ],
        [ "Related Documentation", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md291", null ]
      ] ]
    ] ],
    [ "CI_CD_DASHBOARD.kr", "md_docs_2advanced_2CI__CD__DASHBOARD_8kr.html", [
      [ "Logger System CI/CD 대시보드", "md_docs_2advanced_2CI__CD__DASHBOARD_8kr.html#autotoc_md295", null ]
    ] ],
    [ "CI_CD_DASHBOARD", "md_docs_2advanced_2CI__CD__DASHBOARD.html", [
      [ "Logger System CI/CD Dashboard", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md300", [
        [ "autotoc_md298", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md298", null ],
        [ "category: \"PROJ\"", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md299", null ],
        [ "🚀 Build Status", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md301", [
          [ "Main Branch", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md302", null ]
        ] ],
        [ "📊 Performance Metrics", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md303", [
          [ "Latest Benchmark Results", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md304", null ],
          [ "Performance Trends", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md305", null ]
        ] ],
        [ "🔍 Code Quality", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md306", [
          [ "Static Analysis", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md307", null ],
          [ "Test Coverage", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md308", null ]
        ] ],
        [ "🧪 Test Results", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md309", [
          [ "Test Execution Summary", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md310", null ],
          [ "Recent Test Failures", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md311", null ]
        ] ],
        [ "🔥 Sanitizer Results", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md312", null ],
        [ "📈 Build Time Analysis", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md313", [
          [ "Average Build Times", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md314", null ]
        ] ],
        [ "🚨 Recent Issues", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md315", [
          [ "Build Failures (Last 7 Days)", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md316", null ],
          [ "Performance Regressions", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md317", null ]
        ] ],
        [ "📝 Dependency Status", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md318", [
          [ "External Dependencies", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md319", null ]
        ] ],
        [ "🔄 CI/CD Pipeline Health", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md320", [
          [ "Workflow Execution Stats (Last 30 Days)", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md321", null ],
          [ "Resource Usage", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md322", null ]
        ] ],
        [ "📅 Release History", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md323", [
          [ "Recent Releases", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md324", null ]
        ] ],
        [ "🔗 Quick Links", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md325", null ],
        [ "📊 Dashboard Update", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md326", null ],
        [ "This dashboard is automatically updated by CI/CD workflows. For real-time data, check the GitHub Acti...", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md328", null ]
      ] ]
    ] ],
    [ "CONDITIONAL_COMPILATION_REFACTORING", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html", [
      [ "Conditional Compilation Refactoring Strategy", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md331", [
        [ "autotoc_md329", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md329", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md330", null ],
        [ "Problem Statement", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md332", [
          [ "Current State", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md333", null ],
          [ "Issues", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md334", null ],
          [ "Example of Current Pattern", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md335", null ]
        ] ],
        [ "Proposed Solution: Adapter Pattern", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md336", [
          [ "Architecture", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md337", null ],
          [ "Benefits", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md338", null ],
          [ "Trade-offs", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md339", null ]
        ] ],
        [ "Implementation Plan", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md340", [
          [ "Phase 3.1: Analysis and Documentation ✅", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md341", null ],
          [ "Phase 3.2: Core Interface Design", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md342", null ],
          [ "Phase 3.3: Adapter Implementation", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md343", null ],
          [ "Phase 3.4: Migration", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md344", null ],
          [ "Phase 3.5: Validation", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md345", null ]
        ] ],
        [ "Detailed Design", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md346", [
          [ "Core Logger Interface", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md347", null ],
          [ "Adapter Interface", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md348", null ],
          [ "Thread System Adapter", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md349", null ],
          [ "Common System Adapter", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md350", null ],
          [ "Factory Pattern", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md351", null ]
        ] ],
        [ "Migration Strategy", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md352", [
          [ "Step 1: Extract Core (1 week)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md353", null ],
          [ "Step 2: Implement Adapters (1 week)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md354", null ],
          [ "Step 3: Update Public Interface (3 days)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md355", null ],
          [ "Step 4: Remove Conditional Compilation (2 days)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md356", null ],
          [ "Step 5: Testing and Validation (3 days)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md357", null ]
        ] ],
        [ "Performance Considerations", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md358", [
          [ "Virtual Function Overhead", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md359", null ],
          [ "Binary Size Impact", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md360", null ]
        ] ],
        [ "Risk Mitigation", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md361", [
          [ "Risks", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md362", null ],
          [ "Rollback Plan", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md363", null ]
        ] ],
        [ "Success Criteria", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md364", null ],
        [ "References", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md365", null ]
      ] ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION.kr", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html", [
      [ "크리티컬 로그 손실 방지 가이드", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html#autotoc_md369", null ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html", [
      [ "Critical Log Loss Prevention Guide", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md375", [
        [ "autotoc_md372", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md372", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md373", null ],
        [ "Table of Contents", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md374", null ],
        [ "Overview", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md376", [
          [ "Problem", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md377", null ],
          [ "Solution", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md378", null ]
        ] ],
        [ "Architecture", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md380", null ],
        [ "Class Structure", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md382", [
          [ "critical_writer", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md383", [
            [ "Key Features:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md384", null ],
            [ "Configuration:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md385", null ],
            [ "Usage Example:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md386", null ]
          ] ],
          [ "hybrid_writer", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md387", [
            [ "Features:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md388", null ],
            [ "Usage Example:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md389", null ]
          ] ]
        ] ],
        [ "How It Works", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md391", [
          [ "Level-Based Routing", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md392", null ],
          [ "Critical Write Flow", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md393", [
            [ "Step-by-Step Explanation:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md394", null ]
          ] ],
          [ "Write-Ahead Logging (WAL)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md395", [
            [ "WAL Format:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md396", null ],
            [ "WAL Usage:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md397", null ]
          ] ],
          [ "Signal Handler", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md398", [
            [ "Handled Signals:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md399", null ],
            [ "Signal Handler Implementation:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md400", null ],
            [ "Important Notes:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md401", null ]
          ] ],
          [ "File Descriptor Sync", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md402", [
            [ "Buffer Layers:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md403", null ],
            [ "Implementation:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md404", null ]
          ] ]
        ] ],
        [ "Performance Impact", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md406", [
          [ "Critical Log Overhead", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md407", null ],
          [ "Normal Log Impact", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md408", null ],
          [ "Optimization Strategies", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md409", [
            [ "Strategy 1: Disable sync_on_critical", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md410", null ],
            [ "Strategy 2: Disable force_flush_on_error", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md411", null ],
            [ "Strategy 3: Use Hybrid Writer", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md412", null ]
          ] ],
          [ "Benchmark Results (Estimated)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md413", null ]
        ] ],
        [ "Production Recommendations", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md415", [
          [ "Default Configuration (General Services)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md416", null ],
          [ "High Reliability Configuration (Finance/Medical)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md417", null ],
          [ "High Performance Configuration (Games/Real-time Systems)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md418", null ]
        ] ],
        [ "Testing & Verification", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md420", [
          [ "Critical Log Loss Test", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md421", [
            [ "Verification Method:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md422", null ]
          ] ],
          [ "WAL Recovery Test", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md423", null ],
          [ "Performance Benchmark", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md424", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md426", [
          [ "Issue 1: WAL File Not Created", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md427", null ],
          [ "Issue 2: Signal Handler Not Working", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md428", null ],
          [ "Issue 3: Performance Degradation", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md429", null ]
        ] ],
        [ "FAQ", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md431", [
          [ "Q1: Should I wrap all logs with critical_writer?", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md432", null ],
          [ "Q2: When should I use WAL?", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md433", null ],
          [ "Q3: fsync() overhead is too high.", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md434", null ],
          [ "Q4: What can I do in a signal handler?", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md435", null ]
        ] ],
        [ "References", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md437", null ],
        [ "Version History", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md439", null ],
        [ "License", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md441", null ]
      ] ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START.kr", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html", [
      [ "크리티컬 로깅 빠른 시작", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html#autotoc_md445", null ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html", [
      [ "Critical Logging Quick Start", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md451", [
        [ "autotoc_md448", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md448", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md449", null ],
        [ "Table of Contents", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md450", null ],
        [ "🎯 Goal", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md453", null ],
        [ "🚀 Quick Start", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md455", [
          [ "Step 1: Include Headers", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md456", null ],
          [ "Step 2: Create Hybrid Writer", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md457", null ],
          [ "Step 3: Verify", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md458", null ]
        ] ],
        [ "📊 3 Usage Scenarios", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md460", [
          [ "Scenario 1: General Web Services (Recommended)", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md461", null ],
          [ "Scenario 2: Finance/Medical Systems (Maximum Reliability)", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md462", null ],
          [ "Scenario 3: Games/Real-time Systems (High Performance)", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md463", null ]
        ] ],
        [ "🔧 Configuration Options", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md465", null ],
        [ "✅ Verification Methods", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md467", [
          [ "Test 1: Check Critical Logs", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md468", null ],
          [ "Test 2: Crash Simulation", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md469", null ],
          [ "Test 3: Ctrl+C Test", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md470", null ]
        ] ],
        [ "📚 Learn More", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md472", null ],
        [ "🐛 Troubleshooting", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md474", [
          [ "\"WAL file not created\"", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md475", null ],
          [ "\"Performance too slow\"", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md476", null ],
          [ "\"Signal handler not working\"", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md477", null ]
        ] ],
        [ "💡 Key Points", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md479", null ]
      ] ]
    ] ],
    [ "CUSTOM_WRITERS.kr", "md_docs_2advanced_2CUSTOM__WRITERS_8kr.html", [
      [ "커스텀 작성기 생성", "md_docs_2advanced_2CUSTOM__WRITERS_8kr.html#autotoc_md484", null ]
    ] ],
    [ "CUSTOM_WRITERS", "md_docs_2advanced_2CUSTOM__WRITERS.html", [
      [ "Creating Custom Writers", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md489", [
        [ "autotoc_md487", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md487", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md488", null ],
        [ "Overview", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md490", null ],
        [ "Writer Hierarchy", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md491", null ],
        [ "Recommended: Using thread_safe_writer (Since v1.3.0)", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md492", [
          [ "Benefits of thread_safe_writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md493", null ]
        ] ],
        [ "Base Writer Interface", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md494", null ],
        [ "Simple Examples", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md495", [
          [ "Simple File Writer (Using thread_safe_writer)", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md496", null ],
          [ "Rotating File Writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md497", null ],
          [ "Network Writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md498", null ],
          [ "Database Writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md499", null ]
        ] ],
        [ "Advanced Patterns", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md500", [
          [ "Filtering Writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md501", null ],
          [ "Async Writer Wrapper", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md502", null ]
        ] ],
        [ "Choosing the Right Base Class", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md503", [
          [ "When to Use thread_safe_writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md504", null ],
          [ "When to Use base_writer Directly", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md505", null ]
        ] ],
        [ "Best Practices", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md506", null ],
        [ "Testing Custom Writers", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md507", null ]
      ] ]
    ] ],
    [ "LOG_LEVEL_SEMANTIC_STANDARD", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html", [
      [ "Log Level Semantic Standard", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md511", [
        [ "autotoc_md509", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md509", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md510", null ],
        [ "Problem Statement", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md513", [
          [ "logger_system (Ascending: Low → High)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md514", null ],
          [ "thread_system (Descending: High → Low)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md515", null ],
          [ "Impact", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md516", null ]
        ] ],
        [ "Decision", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md518", [
          [ "Rationale", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md519", null ]
        ] ],
        [ "Migration Plan", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md521", [
          [ "Phase 1: Documentation (Sprint 1 - Current)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md522", null ],
          [ "Phase 2: thread_system Unification (Sprint 2)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md523", null ],
          [ "Phase 3: Adapter Simplification (Sprint 3)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md524", null ]
        ] ],
        [ "Implementation Details", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md526", [
          [ "Standard Definition (logger_system - already compliant)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md527", null ],
          [ "thread_system Changes (Sprint 2)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md528", null ],
          [ "Adapter Simplification (Sprint 3)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md529", null ]
        ] ],
        [ "Compatibility Considerations", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md531", [
          [ "Breaking Change Assessment", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md532", null ],
          [ "Mitigation Strategy", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md533", null ],
          [ "User Impact", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md534", null ]
        ] ],
        [ "Testing Requirements", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md536", [
          [ "Unit Tests", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md537", null ],
          [ "Integration Tests", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md538", null ],
          [ "Regression Tests", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md539", null ]
        ] ],
        [ "Documentation Updates Required", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md541", null ],
        [ "Success Criteria", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md543", null ],
        [ "Timeline", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md545", null ],
        [ "References", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md547", null ],
        [ "Approval", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md549", null ]
      ] ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE.kr", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html", [
      [ "로거 시스템 아키텍처", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md553", [
        [ "autotoc_md551", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md551", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md552", null ],
        [ "개요", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md554", [
          [ "주요 기능 (v3.0)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md555", null ]
        ] ],
        [ "아키텍처 다이어그램", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md556", null ],
        [ "핵심 컴포넌트", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md557", [
          [ "ILogger 인터페이스 구현", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md558", null ],
          [ "Dual API 설계", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md559", null ],
          [ "백엔드 추상화", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md560", null ]
        ] ],
        [ "성능 특성", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md561", [
          [ "벤치마크 (v3.0)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md562", null ],
          [ "멀티스레드 성능", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md563", null ]
        ] ]
      ] ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html", [
      [ "Logger System Architecture", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md568", [
        [ "autotoc_md566", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md566", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md567", null ],
        [ "Table of Contents", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md569", null ],
        [ "Overview", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md570", [
          [ "Key Features (v3.0)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md571", null ]
        ] ],
        [ "Architecture Diagram", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md572", null ],
        [ "Core Components", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md573", [
          [ "ILogger Interface Implementation", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md574", null ],
          [ "Dual API Design", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md575", null ],
          [ "Configuration Management", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md576", [
            [ "Logger Configuration with Validation", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md577", null ],
            [ "Configuration Templates", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md578", null ]
          ] ],
          [ "Builder Pattern with Validation", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md579", null ],
          [ "Backend Abstraction", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md580", null ],
          [ "Interface Segregation", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md581", [
            [ "Writer Interface", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md582", null ],
            [ "Filter Interface", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md583", null ],
            [ "Formatter Interface", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md584", null ]
          ] ],
          [ "Log Entry Structure", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md585", null ]
        ] ],
        [ "Advanced Features", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md586", [
          [ "Asynchronous Pipeline", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md587", null ],
          [ "Error Handling with Result Pattern", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md588", null ],
          [ "C++20 Source Location", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md589", null ],
          [ "Performance Monitoring", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md590", null ],
          [ "Configuration Strategies", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md591", null ]
        ] ],
        [ "Threading Model", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md592", [
          [ "Synchronous Mode", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md593", null ],
          [ "Asynchronous Mode (Default)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md594", null ],
          [ "Thread Safety Guarantees", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md595", null ]
        ] ],
        [ "Memory Management", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md596", [
          [ "Buffer Management", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md597", null ],
          [ "Object Lifetime", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md598", null ]
        ] ],
        [ "Performance Characteristics", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md599", [
          [ "Benchmarks (v3.0)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md600", null ],
          [ "Multi-threaded Performance", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md601", null ],
          [ "Optimization Strategies", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md602", null ]
        ] ],
        [ "Integration Patterns", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md603", [
          [ "ILogger Interface Integration", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md604", null ],
          [ "Dependency Injection", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md605", null ],
          [ "With Monitoring (Phase 2.2.4)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md606", null ]
        ] ],
        [ "Extension Points", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md607", [
          [ "Custom Writers", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md608", null ],
          [ "Custom Filters", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md609", null ],
          [ "Custom Formatters", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md610", null ]
        ] ],
        [ "Future Enhancements", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md611", [
          [ "Performance Improvements", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md612", null ],
          [ "Feature Additions", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md613", null ],
          [ "Platform Extensions", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md614", null ]
        ] ],
        [ "Best Practices", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md615", [
          [ "For Library Users", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md616", null ],
          [ "For Contributors", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md617", null ]
        ] ],
        [ "Platform Notes", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md618", null ]
      ] ]
    ] ],
    [ "MIGRATION", "md_docs_2advanced_2MIGRATION.html", [
      [ "Migration Guide - Logger System", "md_docs_2advanced_2MIGRATION.html#autotoc_md622", [
        [ "autotoc_md620", "md_docs_2advanced_2MIGRATION.html#autotoc_md620", null ],
        [ "category: \"MIGR\"", "md_docs_2advanced_2MIGRATION.html#autotoc_md621", null ],
        [ "Overview", "md_docs_2advanced_2MIGRATION.html#autotoc_md623", null ],
        [ "Table of Contents", "md_docs_2advanced_2MIGRATION.html#autotoc_md625", null ],
        [ "Quick Migration Checklist", "md_docs_2advanced_2MIGRATION.html#autotoc_md627", null ],
        [ "Migration Path 1: From Integrated thread_system", "md_docs_2advanced_2MIGRATION.html#autotoc_md629", [
          [ "Background", "md_docs_2advanced_2MIGRATION.html#autotoc_md630", null ],
          [ "Step 1: Update CMakeLists.txt", "md_docs_2advanced_2MIGRATION.html#autotoc_md631", null ],
          [ "Step 2: Update Header Includes", "md_docs_2advanced_2MIGRATION.html#autotoc_md632", null ],
          [ "Step 3: Update Namespace", "md_docs_2advanced_2MIGRATION.html#autotoc_md633", null ],
          [ "Step 4: Migrate Logger Creation", "md_docs_2advanced_2MIGRATION.html#autotoc_md634", null ],
          [ "Step 5: Update Thread System Integration (Optional)", "md_docs_2advanced_2MIGRATION.html#autotoc_md635", null ]
        ] ],
        [ "Migration Path 2: From spdlog", "md_docs_2advanced_2MIGRATION.html#autotoc_md637", [
          [ "Overview", "md_docs_2advanced_2MIGRATION.html#autotoc_md638", null ],
          [ "API Mapping", "md_docs_2advanced_2MIGRATION.html#autotoc_md639", null ],
          [ "Example Migration", "md_docs_2advanced_2MIGRATION.html#autotoc_md640", null ],
          [ "Pattern Support", "md_docs_2advanced_2MIGRATION.html#autotoc_md641", null ]
        ] ],
        [ "Migration Path 3: From Custom Logger", "md_docs_2advanced_2MIGRATION.html#autotoc_md643", [
          [ "Common Custom Logger Patterns", "md_docs_2advanced_2MIGRATION.html#autotoc_md644", [
            [ "Pattern 1: Direct File Writing", "md_docs_2advanced_2MIGRATION.html#autotoc_md645", null ],
            [ "Pattern 2: Multiple Output Targets", "md_docs_2advanced_2MIGRATION.html#autotoc_md646", null ],
            [ "Pattern 3: Thread-Safe Singleton", "md_docs_2advanced_2MIGRATION.html#autotoc_md647", null ]
          ] ]
        ] ],
        [ "Common Migration Issues", "md_docs_2advanced_2MIGRATION.html#autotoc_md649", [
          [ "Issue 1: Namespace Conflicts", "md_docs_2advanced_2MIGRATION.html#autotoc_md650", null ],
          [ "Issue 2: CMake Cache Issues", "md_docs_2advanced_2MIGRATION.html#autotoc_md651", null ],
          [ "Issue 3: Header Not Found", "md_docs_2advanced_2MIGRATION.html#autotoc_md652", null ],
          [ "Issue 4: Result<T> Pattern Unfamiliar", "md_docs_2advanced_2MIGRATION.html#autotoc_md653", null ],
          [ "Issue 5: Performance Regression", "md_docs_2advanced_2MIGRATION.html#autotoc_md654", null ]
        ] ],
        [ "Testing Your Migration", "md_docs_2advanced_2MIGRATION.html#autotoc_md656", [
          [ "Compile Test", "md_docs_2advanced_2MIGRATION.html#autotoc_md657", null ],
          [ "Unit Tests", "md_docs_2advanced_2MIGRATION.html#autotoc_md658", null ],
          [ "Integration Tests", "md_docs_2advanced_2MIGRATION.html#autotoc_md659", null ],
          [ "Performance Benchmark", "md_docs_2advanced_2MIGRATION.html#autotoc_md660", null ]
        ] ],
        [ "Rollback Plan", "md_docs_2advanced_2MIGRATION.html#autotoc_md662", [
          [ "Option 1: Revert Changes", "md_docs_2advanced_2MIGRATION.html#autotoc_md663", null ],
          [ "Option 2: Gradual Migration", "md_docs_2advanced_2MIGRATION.html#autotoc_md664", null ]
        ] ],
        [ "Migration Assistance", "md_docs_2advanced_2MIGRATION.html#autotoc_md666", [
          [ "Automated Migration Script", "md_docs_2advanced_2MIGRATION.html#autotoc_md667", null ]
        ] ],
        [ "Support", "md_docs_2advanced_2MIGRATION.html#autotoc_md669", null ],
        [ "Next Steps After Migration", "md_docs_2advanced_2MIGRATION.html#autotoc_md671", null ]
      ] ]
    ] ],
    [ "STRUCTURE.kr", "md_docs_2advanced_2STRUCTURE_8kr.html", [
      [ "Logger System - 프로젝트 구조", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md675", [
        [ "autotoc_md673", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md673", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md674", null ],
        [ "📁 디렉토리 레이아웃", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md677", null ],
        [ "🏗️ Namespace 구조", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md678", [
          [ "Core Namespaces", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md679", null ],
          [ "Nested Namespaces", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md680", null ]
        ] ],
        [ "🔧 주요 컴포넌트 개요", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md681", [
          [ "🎯 Public API Layer (include/kcenon/logger/)", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md682", null ],
          [ "⚙️ Implementation Layer (src/)", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md683", null ]
        ] ],
        [ "📊 성능 특성", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md684", null ],
        [ "🔄 마이그레이션 가이드", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md685", [
          [ "Step 1: 현재 설정 백업", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md686", null ],
          [ "Step 2: Include 경로 업데이트", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md687", null ],
          [ "Step 3: Namespace 사용 업데이트", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md688", null ],
          [ "Step 4: 마이그레이션 스크립트 실행", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md689", null ]
        ] ],
        [ "🚀 새로운 구조로 빠른 시작", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md690", null ]
      ] ]
    ] ],
    [ "STRUCTURE", "md_docs_2advanced_2STRUCTURE.html", [
      [ "Logger System - Project Structure", "md_docs_2advanced_2STRUCTURE.html#autotoc_md693", [
        [ "autotoc_md691", "md_docs_2advanced_2STRUCTURE.html#autotoc_md691", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2STRUCTURE.html#autotoc_md692", null ],
        [ "Table of Contents", "md_docs_2advanced_2STRUCTURE.html#autotoc_md695", null ],
        [ "Directory Layout", "md_docs_2advanced_2STRUCTURE.html#autotoc_md696", null ],
        [ "Namespace Organization", "md_docs_2advanced_2STRUCTURE.html#autotoc_md697", [
          [ "Primary Namespace Hierarchy", "md_docs_2advanced_2STRUCTURE.html#autotoc_md698", null ],
          [ "Namespace Usage Examples", "md_docs_2advanced_2STRUCTURE.html#autotoc_md699", null ]
        ] ],
        [ "Component Layout", "md_docs_2advanced_2STRUCTURE.html#autotoc_md700", [
          [ "Core Components", "md_docs_2advanced_2STRUCTURE.html#autotoc_md701", [
            [ "Logger Core (core/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md702", null ],
            [ "Interfaces (interfaces/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md703", null ],
            [ "Writers (writers/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md704", null ],
            [ "Formatters (formatters/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md705", null ],
            [ "Filters (filters/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md706", null ],
            [ "Routing (routing/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md707", null ],
            [ "Analysis (analysis/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md708", null ],
            [ "Structured Logging (structured/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md709", null ],
            [ "Safety (safety/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md710", null ],
            [ "Server (server/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md711", null ],
            [ "Adapters (adapters/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md712", null ]
          ] ],
          [ "Monitoring (core/monitoring/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md713", null ]
        ] ],
        [ "Module Dependencies", "md_docs_2advanced_2STRUCTURE.html#autotoc_md714", [
          [ "Internal Dependencies", "md_docs_2advanced_2STRUCTURE.html#autotoc_md715", null ],
          [ "External Dependencies", "md_docs_2advanced_2STRUCTURE.html#autotoc_md716", null ],
          [ "Compilation Units", "md_docs_2advanced_2STRUCTURE.html#autotoc_md717", null ]
        ] ],
        [ "Key Components Overview", "md_docs_2advanced_2STRUCTURE.html#autotoc_md718", [
          [ "Public API Layer (include/kcenon/logger/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md719", null ],
          [ "Implementation Layer (src/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md720", null ],
          [ "Test Organization", "md_docs_2advanced_2STRUCTURE.html#autotoc_md721", null ]
        ] ],
        [ "Performance Characteristics", "md_docs_2advanced_2STRUCTURE.html#autotoc_md722", [
          [ "Throughput", "md_docs_2advanced_2STRUCTURE.html#autotoc_md723", null ],
          [ "Memory", "md_docs_2advanced_2STRUCTURE.html#autotoc_md724", null ],
          [ "Threading", "md_docs_2advanced_2STRUCTURE.html#autotoc_md725", null ],
          [ "Resource Management", "md_docs_2advanced_2STRUCTURE.html#autotoc_md726", null ]
        ] ],
        [ "Build Configuration", "md_docs_2advanced_2STRUCTURE.html#autotoc_md727", [
          [ "CMake Targets", "md_docs_2advanced_2STRUCTURE.html#autotoc_md728", null ],
          [ "Feature Flags", "md_docs_2advanced_2STRUCTURE.html#autotoc_md729", null ]
        ] ],
        [ "References", "md_docs_2advanced_2STRUCTURE.html#autotoc_md730", null ]
      ] ]
    ] ],
    [ "WRITER_HIERARCHY", "md_docs_2advanced_2WRITER__HIERARCHY.html", [
      [ "Writer Hierarchy and Categories", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md733", [
        [ "autotoc_md731", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md731", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md732", null ],
        [ "Overview", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md734", null ],
        [ "Category System", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md735", [
          [ "Writer Categories", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md736", null ],
          [ "Category Tags", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md737", null ]
        ] ],
        [ "Complete Writer Hierarchy", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md738", null ],
        [ "Category Characteristics", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md739", [
          [ "Synchronous Writers (sync_writer_tag)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md740", null ],
          [ "Asynchronous Writers (async_writer_tag)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md741", null ],
          [ "Decorator Writers (decorator_writer_tag)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md742", null ],
          [ "Composite Writers (composite_writer_tag)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md743", null ]
        ] ],
        [ "Using Category Information", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md744", [
          [ "Compile-Time Type Traits", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md745", null ],
          [ "C++20 Concepts", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md746", null ],
          [ "Runtime Category Query", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md747", null ]
        ] ],
        [ "Design Patterns Used", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md748", [
          [ "Template Method Pattern (thread_safe_writer)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md749", null ],
          [ "Decorator Pattern", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md750", null ],
          [ "Pipeline Pattern (composite_writer)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md751", null ]
        ] ],
        [ "Related Documentation", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md752", null ]
      ] ]
    ] ],
    [ "WRITER_SELECTION_GUIDE", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html", [
      [ "Writer Selection Guide", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md756", [
        [ "autotoc_md754", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md754", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md755", null ],
        [ "Quick Selection Flowchart", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md757", null ],
        [ "Writer Selection by Use Case", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md758", [
          [ "Simple Console Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md759", null ],
          [ "Basic File Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md761", null ],
          [ "Production File Logging with Rotation", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md763", null ],
          [ "High-Throughput Logging (>10K msg/sec)", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md765", null ],
          [ "Network/Remote Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md767", null ],
          [ "Encrypted/Secure Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md769", null ],
          [ "Critical/Safety-Critical Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md771", null ],
          [ "Custom Pipeline (Formatter + Sink)", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md773", null ]
        ] ],
        [ "Decorator Stacking Patterns", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md775", [
          [ "Pattern 1: Async + Encrypted", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md776", null ],
          [ "Pattern 2: Critical + Async + Rotating", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md777", null ],
          [ "Pattern 3: Batch + Network", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md778", null ]
        ] ],
        [ "Performance Comparison", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md779", null ],
        [ "Decision Matrix", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md780", null ],
        [ "Related Documentation", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md781", null ]
      ] ]
    ] ],
    [ "API_QUICK_REFERENCE", "md_docs_2API__QUICK__REFERENCE.html", [
      [ "API Quick Reference", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md785", [
        [ "autotoc_md783", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md783", null ],
        [ "category: \"API\"", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md784", null ],
        [ "Logger Creation", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md786", [
          [ "Logger Builder", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md787", null ],
          [ "Config Builder", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md788", null ]
        ] ],
        [ "Logging Messages", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md789", [
          [ "Log Levels (low to high severity)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md790", null ]
        ] ],
        [ "Writer Builder (Decorator Chain)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md791", [
          [ "Adding Writers to Logger", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md792", null ],
          [ "Starting Async Writers", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md793", null ]
        ] ],
        [ "Core Writers", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md794", null ],
        [ "Structured Logging", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md795", [
          [ "Logger Context (applies to all subsequent logs)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md796", null ],
          [ "RAII Context Scopes (thread-local)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md797", null ],
          [ "Trace Context (distributed tracing)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md798", null ]
        ] ],
        [ "Filters", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md799", [
          [ "Per-Writer Filtering (via decorator)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md800", null ]
        ] ],
        [ "Formatters", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md801", null ],
        [ "Metrics", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md802", null ],
        [ "Critical Logging", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md803", [
          [ "Hybrid Writer (async normal + sync critical)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md804", null ]
        ] ],
        [ "Custom Writer", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md805", null ],
        [ "Composite Writer (SRP Architecture)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md806", null ],
        [ "Header Quick Index", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md807", null ]
      ] ]
    ] ],
    [ "API_REFERENCE.kr", "md_docs_2API__REFERENCE_8kr.html", [
      [ "API 레퍼런스", "md_docs_2API__REFERENCE_8kr.html#autotoc_md810", [
        [ "autotoc_md808", "md_docs_2API__REFERENCE_8kr.html#autotoc_md808", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE_8kr.html#autotoc_md809", null ],
        [ "v3.0 주요 변경사항", "md_docs_2API__REFERENCE_8kr.html#autotoc_md811", [
          [ "네임스페이스 변경", "md_docs_2API__REFERENCE_8kr.html#autotoc_md812", null ],
          [ "인터페이스 변경", "md_docs_2API__REFERENCE_8kr.html#autotoc_md813", null ],
          [ "헤더 경로 변경", "md_docs_2API__REFERENCE_8kr.html#autotoc_md814", null ],
          [ "주요 기능", "md_docs_2API__REFERENCE_8kr.html#autotoc_md815", null ]
        ] ],
        [ "빠른 시작", "md_docs_2API__REFERENCE_8kr.html#autotoc_md816", null ]
      ] ]
    ] ],
    [ "API_REFERENCE", "md_docs_2API__REFERENCE.html", [
      [ "API Reference", "md_docs_2API__REFERENCE.html#autotoc_md821", [
        [ "autotoc_md819", "md_docs_2API__REFERENCE.html#autotoc_md819", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE.html#autotoc_md820", null ],
        [ "Sub-Documents", "md_docs_2API__REFERENCE.html#autotoc_md822", null ],
        [ "Overview", "md_docs_2API__REFERENCE.html#autotoc_md823", null ],
        [ "Quick Links", "md_docs_2API__REFERENCE.html#autotoc_md824", [
          [ "Core", "md_docs_2API__REFERENCE.html#autotoc_md825", null ],
          [ "Writers and Filters", "md_docs_2API__REFERENCE.html#autotoc_md826", null ],
          [ "Security", "md_docs_2API__REFERENCE.html#autotoc_md827", null ]
        ] ]
      ] ]
    ] ],
    [ "API_REFERENCE_CORE", "md_docs_2API__REFERENCE__CORE.html", [
      [ "API Reference - Core", "md_docs_2API__REFERENCE__CORE.html#autotoc_md831", [
        [ "autotoc_md829", "md_docs_2API__REFERENCE__CORE.html#autotoc_md829", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE__CORE.html#autotoc_md830", null ],
        [ "Table of Contents", "md_docs_2API__REFERENCE__CORE.html#autotoc_md832", null ],
        [ "Overview", "md_docs_2API__REFERENCE__CORE.html#autotoc_md833", [
          [ "Namespace", "md_docs_2API__REFERENCE__CORE.html#autotoc_md834", null ]
        ] ],
        [ "Core Classes", "md_docs_2API__REFERENCE__CORE.html#autotoc_md835", [
          [ "kcenon::logger::logger", "md_docs_2API__REFERENCE__CORE.html#autotoc_md836", [
            [ "Constructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md837", null ],
            [ "Destructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md838", null ]
          ] ]
        ] ],
        [ "ILogger Interface (Phase 2.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md839", [
          [ "Log Methods (ILogger Interface)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md840", null ],
          [ "Level Management (ILogger Interface)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md841", null ],
          [ "Native API (Backward Compatible)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md842", null ],
          [ "Writer Management", "md_docs_2API__REFERENCE__CORE.html#autotoc_md843", null ],
          [ "Lifecycle Management", "md_docs_2API__REFERENCE__CORE.html#autotoc_md844", null ],
          [ "Metrics", "md_docs_2API__REFERENCE__CORE.html#autotoc_md845", null ],
          [ "Filtering", "md_docs_2API__REFERENCE__CORE.html#autotoc_md846", null ],
          [ "Emergency Flush (Signal Safety)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md847", null ]
        ] ],
        [ "Unified Log Context (v3.3.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md848", [
          [ "kcenon::logger::unified_log_context", "md_docs_2API__REFERENCE__CORE.html#autotoc_md849", [
            [ "Value Type", "md_docs_2API__REFERENCE__CORE.html#autotoc_md850", null ],
            [ "Context Categories", "md_docs_2API__REFERENCE__CORE.html#autotoc_md851", null ],
            [ "Constructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md852", null ],
            [ "Setters", "md_docs_2API__REFERENCE__CORE.html#autotoc_md853", null ],
            [ "Getters", "md_docs_2API__REFERENCE__CORE.html#autotoc_md854", null ],
            [ "Query Methods", "md_docs_2API__REFERENCE__CORE.html#autotoc_md855", null ],
            [ "Removal and Export", "md_docs_2API__REFERENCE__CORE.html#autotoc_md856", null ],
            [ "Usage Example", "md_docs_2API__REFERENCE__CORE.html#autotoc_md857", null ],
            [ "Thread Safety", "md_docs_2API__REFERENCE__CORE.html#autotoc_md858", null ]
          ] ]
        ] ],
        [ "Scoped Context Guard (v3.3.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md860", [
          [ "kcenon::logger::scoped_context_guard", "md_docs_2API__REFERENCE__CORE.html#autotoc_md861", [
            [ "Constructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md862", null ],
            [ "Setters (Chainable)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md863", null ],
            [ "Usage Example", "md_docs_2API__REFERENCE__CORE.html#autotoc_md864", null ]
          ] ]
        ] ],
        [ "Structured Log Builder (v3.1.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md866", [
          [ "kcenon::logger::structured_log_builder", "md_docs_2API__REFERENCE__CORE.html#autotoc_md867", [
            [ "Constructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md868", null ],
            [ "Builder Methods", "md_docs_2API__REFERENCE__CORE.html#autotoc_md869", null ],
            [ "Logger Integration (log_structured)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md870", null ],
            [ "Usage Example", "md_docs_2API__REFERENCE__CORE.html#autotoc_md871", null ],
            [ "Thread Safety", "md_docs_2API__REFERENCE__CORE.html#autotoc_md872", null ]
          ] ]
        ] ],
        [ "Writer Builder (v4.1.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md874", [
          [ "kcenon::logger::writer_builder", "md_docs_2API__REFERENCE__CORE.html#autotoc_md875", [
            [ "Core Writers (Terminal Nodes)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md876", null ],
            [ "Decorator Writers (Middleware)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md877", null ],
            [ "Build and Utility", "md_docs_2API__REFERENCE__CORE.html#autotoc_md878", null ],
            [ "Usage Examples", "md_docs_2API__REFERENCE__CORE.html#autotoc_md879", null ]
          ] ]
        ] ],
        [ "Configuration", "md_docs_2API__REFERENCE__CORE.html#autotoc_md881", [
          [ "kcenon::logger::logger_config", "md_docs_2API__REFERENCE__CORE.html#autotoc_md882", null ],
          [ "Static Factory Methods", "md_docs_2API__REFERENCE__CORE.html#autotoc_md883", null ]
        ] ],
        [ "Builder Pattern", "md_docs_2API__REFERENCE__CORE.html#autotoc_md884", [
          [ "kcenon::logger::logger_builder", "md_docs_2API__REFERENCE__CORE.html#autotoc_md885", [
            [ "Basic Configuration", "md_docs_2API__REFERENCE__CORE.html#autotoc_md886", null ],
            [ "Feature Flags", "md_docs_2API__REFERENCE__CORE.html#autotoc_md887", null ],
            [ "Output Configuration", "md_docs_2API__REFERENCE__CORE.html#autotoc_md888", null ],
            [ "Writers and Filters", "md_docs_2API__REFERENCE__CORE.html#autotoc_md889", null ],
            [ "Backend Selection", "md_docs_2API__REFERENCE__CORE.html#autotoc_md890", null ],
            [ "Environment and Error Handling", "md_docs_2API__REFERENCE__CORE.html#autotoc_md891", null ],
            [ "Build", "md_docs_2API__REFERENCE__CORE.html#autotoc_md892", null ]
          ] ]
        ] ],
        [ "Configuration Strategies", "md_docs_2API__REFERENCE__CORE.html#autotoc_md893", [
          [ "Deployment Strategy", "md_docs_2API__REFERENCE__CORE.html#autotoc_md894", null ],
          [ "Performance Strategy", "md_docs_2API__REFERENCE__CORE.html#autotoc_md895", null ],
          [ "Environment Strategy", "md_docs_2API__REFERENCE__CORE.html#autotoc_md896", null ],
          [ "Composite Strategy", "md_docs_2API__REFERENCE__CORE.html#autotoc_md897", null ]
        ] ],
        [ "Interfaces", "md_docs_2API__REFERENCE__CORE.html#autotoc_md898", [
          [ "kcenon::logger::log_writer_interface", "md_docs_2API__REFERENCE__CORE.html#autotoc_md899", null ],
          [ "kcenon::logger::log_filter_interface", "md_docs_2API__REFERENCE__CORE.html#autotoc_md900", null ],
          [ "kcenon::logger::log_formatter_interface", "md_docs_2API__REFERENCE__CORE.html#autotoc_md901", null ],
          [ "kcenon::logger::log_entry", "md_docs_2API__REFERENCE__CORE.html#autotoc_md902", null ]
        ] ],
        [ "Error Handling", "md_docs_2API__REFERENCE__CORE.html#autotoc_md903", [
          [ "Error Codes", "md_docs_2API__REFERENCE__CORE.html#autotoc_md904", [
            [ "Error Code Categories Summary", "md_docs_2API__REFERENCE__CORE.html#autotoc_md905", null ]
          ] ],
          [ "Result Types", "md_docs_2API__REFERENCE__CORE.html#autotoc_md906", null ],
          [ "Helper Functions", "md_docs_2API__REFERENCE__CORE.html#autotoc_md907", [
            [ "kcenon::logger::result<T> Wrapper", "md_docs_2API__REFERENCE__CORE.html#autotoc_md908", null ]
          ] ]
        ] ]
      ] ]
    ] ],
    [ "API_REFERENCE_SECURITY", "md_docs_2API__REFERENCE__SECURITY.html", [
      [ "API Reference - Security", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md912", [
        [ "autotoc_md910", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md910", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md911", null ],
        [ "Table of Contents", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md913", null ],
        [ "Overview", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md914", null ],
        [ "Security Error Codes", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md916", null ],
        [ "Encrypted Writer", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md918", [
          [ "Writer Builder Integration", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md919", null ],
          [ "writer_builder::encrypted()", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md920", null ]
        ] ],
        [ "Secure Key Storage", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md922", [
          [ "Key Generation", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md923", null ],
          [ "Usage Example", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md924", null ]
        ] ],
        [ "Security-Critical Logger Interface", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md926", null ],
        [ "Log Sanitization", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md928", [
          [ "Common Sanitization Patterns", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md929", null ],
          [ "Integration with Filters", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md930", null ]
        ] ],
        [ "Path Validation", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md932", [
          [ "Validation Checks", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md933", null ],
          [ "Error Codes", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md934", null ],
          [ "Example", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md935", null ]
        ] ],
        [ "Security Filters", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md937", [
          [ "Level-Based Security Filtering", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md938", null ],
          [ "Sampling Bypass for Security Events", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md939", null ]
        ] ],
        [ "Audit Logging", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md941", [
          [ "Best Practices for Audit Logs", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md942", null ],
          [ "Example: Audit Trail", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md943", null ]
        ] ],
        [ "Related Documentation", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md945", null ]
      ] ]
    ] ],
    [ "API_REFERENCE_WRITERS", "md_docs_2API__REFERENCE__WRITERS.html", [
      [ "API Reference - Writers", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md949", [
        [ "autotoc_md947", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md947", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md948", null ],
        [ "Table of Contents", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md950", null ],
        [ "Writers", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md951", [
          [ "kcenon::logger::console_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md952", null ],
          [ "kcenon::logger::file_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md953", null ],
          [ "kcenon::logger::rotating_file_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md954", null ],
          [ "kcenon::logger::network_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md955", null ],
          [ "kcenon::logger::critical_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md956", null ],
          [ "kcenon::logger::batch_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md957", null ]
        ] ],
        [ "Filters", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md958", [
          [ "kcenon::logger::filters::level_filter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md959", null ],
          [ "kcenon::logger::filters::regex_filter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md960", null ],
          [ "kcenon::logger::filters::function_filter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md961", null ],
          [ "kcenon::logger::filters::composite_filter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md962", null ]
        ] ],
        [ "Formatters", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md963", [
          [ "kcenon::logger::plain_formatter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md964", null ],
          [ "kcenon::logger::json_formatter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md965", null ]
        ] ],
        [ "Log Sampling (v3.3.0)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md966", [
          [ "Sampling Strategies", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md967", null ],
          [ "kcenon::logger::sampling::sampling_config", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md968", [
            [ "Static Factory Methods", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md969", null ]
          ] ],
          [ "kcenon::logger::sampling::sampling_stats", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md970", null ],
          [ "kcenon::logger::sampling::log_sampler", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md971", [
            [ "Key Methods", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md972", null ]
          ] ],
          [ "kcenon::logger::sampling::sampler_factory", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md973", [
            [ "Usage Examples", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md974", null ]
          ] ]
        ] ],
        [ "Real-time Analysis (v3.2.0)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md976", [
          [ "kcenon::logger::analysis::anomaly_event", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md977", null ],
          [ "kcenon::logger::analysis::realtime_analysis_config", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md978", null ],
          [ "kcenon::logger::analysis::realtime_log_analyzer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md979", [
            [ "Callback Type", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md980", null ],
            [ "Constructor", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md981", null ],
            [ "Key Methods", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md982", null ],
            [ "Statistics", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md983", null ]
          ] ],
          [ "kcenon::logger::analysis::realtime_analyzer_factory", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md984", [
            [ "Usage Example", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md985", null ],
            [ "Thread Safety", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md986", null ]
          ] ]
        ] ],
        [ "Backend Integration", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md988", [
          [ "Integration Backend Interface", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md989", null ],
          [ "Standalone Backend (Default)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md990", null ]
        ] ],
        [ "Monitoring Integration", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md991", [
          [ "IMonitor Integration (Phase 2.2.4)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md992", null ]
        ] ],
        [ "Usage Examples", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md993", [
          [ "Basic Usage with ILogger Interface", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md994", null ],
          [ "Native API (Backward Compatible)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md995", null ],
          [ "Configuration Strategies", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md996", null ],
          [ "Custom Writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md997", null ],
          [ "Filtering", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md998", null ]
        ] ],
        [ "Thread Safety", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md999", null ],
        [ "Performance Considerations", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md1000", null ],
        [ "Migration from v2.x", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md1001", null ]
      ] ]
    ] ],
    [ "ARCHITECTURE.kr", "md_docs_2ARCHITECTURE_8kr.html", [
      [ "Logger System 아키텍처", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1005", [
        [ "autotoc_md1003", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1003", null ],
        [ "category: \"ARCH\"", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1004", null ],
        [ "Logger 파이프라인 아키텍처", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1006", [
          [ "파이프라인 흐름", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1007", null ],
          [ "핵심 아키텍처 결정", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1008", null ],
          [ "OTLP 관찰성 파이프라인", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1009", null ],
          [ "샘플링 및 분석 파이프라인", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1010", null ]
        ] ],
        [ "v3.0 주요 변경사항", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1012", [
          [ "의존성 구조 변경", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1013", null ],
          [ "핵심 변경사항", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1014", null ],
          [ "독립 실행 모드 vs 통합 모드", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1015", null ],
          [ "성능 비교", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1016", null ]
        ] ]
      ] ]
    ] ],
    [ "ARCHITECTURE", "md_docs_2ARCHITECTURE.html", [
      [ "Logger System Architecture", "md_docs_2ARCHITECTURE.html#autotoc_md1021", [
        [ "autotoc_md1019", "md_docs_2ARCHITECTURE.html#autotoc_md1019", null ],
        [ "category: \"ARCH\"", "md_docs_2ARCHITECTURE.html#autotoc_md1020", null ],
        [ "Table of Contents", "md_docs_2ARCHITECTURE.html#autotoc_md1022", null ],
        [ "Overview", "md_docs_2ARCHITECTURE.html#autotoc_md1023", null ],
        [ "", "md_docs_2ARCHITECTURE.html#autotoc_md1024", null ],
        [ "Logger Pipeline Architecture", "md_docs_2ARCHITECTURE.html#autotoc_md1025", [
          [ "Pipeline Flow", "md_docs_2ARCHITECTURE.html#autotoc_md1026", null ],
          [ "Backend Selection Strategy", "md_docs_2ARCHITECTURE.html#autotoc_md1027", null ],
          [ "Key Architectural Decisions", "md_docs_2ARCHITECTURE.html#autotoc_md1028", null ]
        ] ],
        [ "✍️ Writer Architecture", "md_docs_2ARCHITECTURE.html#autotoc_md1030", null ],
        [ "🔭 OTLP & Observability Pipeline", "md_docs_2ARCHITECTURE.html#autotoc_md1032", null ],
        [ "📊 Sampling & Analysis Pipeline", "md_docs_2ARCHITECTURE.html#autotoc_md1034", null ],
        [ "🏗️ Ecosystem Overview", "md_docs_2ARCHITECTURE.html#autotoc_md1036", null ],
        [ "📋 Project Roles & Responsibilities", "md_docs_2ARCHITECTURE.html#autotoc_md1037", [
          [ "common_system (Foundation) - Required", "md_docs_2ARCHITECTURE.html#autotoc_md1038", [
            [ "Responsibilities:", "md_docs_2ARCHITECTURE.html#autotoc_md1039", null ],
            [ "Key Components:", "md_docs_2ARCHITECTURE.html#autotoc_md1040", null ],
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1041", null ]
          ] ],
          [ "logger_system (Logging) - Standalone Capable", "md_docs_2ARCHITECTURE.html#autotoc_md1043", [
            [ "Responsibilities:", "md_docs_2ARCHITECTURE.html#autotoc_md1044", null ],
            [ "Key Components:", "md_docs_2ARCHITECTURE.html#autotoc_md1045", null ],
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1046", null ]
          ] ],
          [ "thread_system (Threading) - Optional", "md_docs_2ARCHITECTURE.html#autotoc_md1048", [
            [ "Responsibilities:", "md_docs_2ARCHITECTURE.html#autotoc_md1049", null ],
            [ "Key Components:", "md_docs_2ARCHITECTURE.html#autotoc_md1050", null ],
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1051", null ]
          ] ],
          [ "monitoring_system (Metrics)", "md_docs_2ARCHITECTURE.html#autotoc_md1053", [
            [ "Responsibilities:", "md_docs_2ARCHITECTURE.html#autotoc_md1054", null ],
            [ "Key Components:", "md_docs_2ARCHITECTURE.html#autotoc_md1055", null ],
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1056", null ]
          ] ],
          [ "integrated_thread_system (Integration Hub)", "md_docs_2ARCHITECTURE.html#autotoc_md1058", [
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1059", null ]
          ] ]
        ] ],
        [ "🔄 Dependency Flow & Interface Contracts", "md_docs_2ARCHITECTURE.html#autotoc_md1060", [
          [ "Interface Hierarchy (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1061", null ],
          [ "Dependency Graph (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1062", null ],
          [ "Build Order Requirements", "md_docs_2ARCHITECTURE.html#autotoc_md1063", null ],
          [ "Standalone vs Integrated Mode", "md_docs_2ARCHITECTURE.html#autotoc_md1064", null ]
        ] ],
        [ "🔧 Integration Patterns", "md_docs_2ARCHITECTURE.html#autotoc_md1065", [
          [ "Interface-Based Integration (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1066", null ],
          [ "Dependency Injection Pattern", "md_docs_2ARCHITECTURE.html#autotoc_md1067", null ],
          [ "Configuration Management", "md_docs_2ARCHITECTURE.html#autotoc_md1068", null ],
          [ "Backend Selection Pattern", "md_docs_2ARCHITECTURE.html#autotoc_md1069", null ]
        ] ],
        [ "📊 Performance Characteristics", "md_docs_2ARCHITECTURE.html#autotoc_md1070", [
          [ "Design Principles", "md_docs_2ARCHITECTURE.html#autotoc_md1071", null ],
          [ "Performance Metrics (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1072", null ],
          [ "Standalone vs Thread System Performance", "md_docs_2ARCHITECTURE.html#autotoc_md1073", null ]
        ] ],
        [ "🔄 Evolution: Monolithic → Modular → Standalone", "md_docs_2ARCHITECTURE.html#autotoc_md1074", [
          [ "Phase 1: Monolithic (v1.x)", "md_docs_2ARCHITECTURE.html#autotoc_md1075", null ],
          [ "Phase 2: Modular (v2.x)", "md_docs_2ARCHITECTURE.html#autotoc_md1076", null ],
          [ "Phase 3: Standalone (v3.0) - Current", "md_docs_2ARCHITECTURE.html#autotoc_md1077", null ],
          [ "Migration Benefits (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1078", null ]
        ] ],
        [ "🚀 Getting Started", "md_docs_2ARCHITECTURE.html#autotoc_md1079", [
          [ "Minimal Setup (Standalone)", "md_docs_2ARCHITECTURE.html#autotoc_md1080", null ],
          [ "Full Ecosystem Setup", "md_docs_2ARCHITECTURE.html#autotoc_md1081", null ],
          [ "Build Order", "md_docs_2ARCHITECTURE.html#autotoc_md1082", null ],
          [ "CMake Integration", "md_docs_2ARCHITECTURE.html#autotoc_md1083", null ]
        ] ],
        [ "📚 Documentation Structure", "md_docs_2ARCHITECTURE.html#autotoc_md1084", [
          [ "common_system", "md_docs_2ARCHITECTURE.html#autotoc_md1085", null ],
          [ "logger_system", "md_docs_2ARCHITECTURE.html#autotoc_md1086", null ],
          [ "thread_system", "md_docs_2ARCHITECTURE.html#autotoc_md1087", null ],
          [ "monitoring_system", "md_docs_2ARCHITECTURE.html#autotoc_md1088", null ]
        ] ],
        [ "Ecosystem Dependencies", "md_docs_2ARCHITECTURE.html#autotoc_md1089", null ],
        [ "", "md_docs_2ARCHITECTURE.html#autotoc_md1090", null ],
        [ "Future Roadmap", "md_docs_2ARCHITECTURE.html#autotoc_md1091", [
          [ "Phase 3.1: Enhancement", "md_docs_2ARCHITECTURE.html#autotoc_md1092", null ],
          [ "Phase 3.2: Optimization (Current)", "md_docs_2ARCHITECTURE.html#autotoc_md1093", null ],
          [ "Phase 4: Ecosystem Expansion", "md_docs_2ARCHITECTURE.html#autotoc_md1094", null ]
        ] ]
      ] ]
    ] ],
    [ "BENCHMARKS.kr", "md_docs_2BENCHMARKS_8kr.html", [
      [ "Logger System 성능 벤치마크", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1099", [
        [ "autotoc_md1097", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1097", null ],
        [ "category: \"PERF\"", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1098", null ],
        [ "목차", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1101", null ],
        [ "요약", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1103", [
          [ "핵심 하이라이트", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1104", null ],
          [ "경쟁 우위", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1105", null ]
        ] ],
        [ "테스트 환경", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1107", [
          [ "하드웨어 구성", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1108", null ],
          [ "소프트웨어 구성", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1109", null ]
        ] ],
        [ "핵심 성능 메트릭", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1111", [
          [ "단일 스레드 성능", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1112", null ],
          [ "멀티스레드 성능", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1113", [
            [ "스레드 수별 처리량", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1114", null ]
          ] ]
        ] ],
        [ "업계 비교", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1116", [
          [ "vs spdlog (가장 인기 있는 C++ 로거)", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1117", [
            [ "단일 스레드 비교", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1118", null ],
            [ "멀티스레드 비교 (4 스레드)", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1119", null ]
          ] ],
          [ "vs Boost.Log", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1120", null ],
          [ "vs glog (Google 로깅 라이브러리)", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1121", null ]
        ] ],
        [ "지연시간 벤치마크", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1123", [
          [ "지연시간 분포 (단일 스레드, 비동기)", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1124", null ],
          [ "부하 시 지연시간", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1125", null ]
        ] ],
        [ "메모리 프로파일링", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1127", [
          [ "구성별 메모리 풋프린트", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1128", null ],
          [ "경쟁사와 메모리 비교", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1129", null ]
        ] ],
        [ "성능 회귀 기준", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1131", null ]
      ] ]
    ] ],
    [ "BENCHMARKS", "md_docs_2BENCHMARKS.html", [
      [ "Logger System Performance Benchmarks", "md_docs_2BENCHMARKS.html#autotoc_md1136", [
        [ "autotoc_md1134", "md_docs_2BENCHMARKS.html#autotoc_md1134", null ],
        [ "category: \"PERF\"", "md_docs_2BENCHMARKS.html#autotoc_md1135", null ],
        [ "Table of Contents", "md_docs_2BENCHMARKS.html#autotoc_md1138", null ],
        [ "Executive Summary", "md_docs_2BENCHMARKS.html#autotoc_md1140", [
          [ "Key Highlights", "md_docs_2BENCHMARKS.html#autotoc_md1141", null ],
          [ "Competitive Advantages", "md_docs_2BENCHMARKS.html#autotoc_md1142", null ]
        ] ],
        [ "Test Environment", "md_docs_2BENCHMARKS.html#autotoc_md1144", [
          [ "Hardware Configuration", "md_docs_2BENCHMARKS.html#autotoc_md1145", null ],
          [ "Software Configuration", "md_docs_2BENCHMARKS.html#autotoc_md1146", null ]
        ] ],
        [ "Core Performance Metrics", "md_docs_2BENCHMARKS.html#autotoc_md1148", [
          [ "Single-Threaded Performance", "md_docs_2BENCHMARKS.html#autotoc_md1149", null ],
          [ "Multi-Threaded Performance", "md_docs_2BENCHMARKS.html#autotoc_md1150", [
            [ "Throughput by Thread Count", "md_docs_2BENCHMARKS.html#autotoc_md1151", null ],
            [ "Performance Scaling Graph", "md_docs_2BENCHMARKS.html#autotoc_md1152", null ]
          ] ]
        ] ],
        [ "Industry Comparisons", "md_docs_2BENCHMARKS.html#autotoc_md1154", [
          [ "vs spdlog (Most Popular C++ Logger)", "md_docs_2BENCHMARKS.html#autotoc_md1155", [
            [ "Single-Threaded Comparison", "md_docs_2BENCHMARKS.html#autotoc_md1156", null ],
            [ "Multi-Threaded Comparison (4 Threads)", "md_docs_2BENCHMARKS.html#autotoc_md1157", null ]
          ] ],
          [ "vs Boost.Log", "md_docs_2BENCHMARKS.html#autotoc_md1158", null ],
          [ "vs glog (Google's Logging Library)", "md_docs_2BENCHMARKS.html#autotoc_md1159", null ],
          [ "vs log4cxx", "md_docs_2BENCHMARKS.html#autotoc_md1160", null ]
        ] ],
        [ "Scalability Analysis", "md_docs_2BENCHMARKS.html#autotoc_md1162", [
          [ "Thread Scaling Efficiency", "md_docs_2BENCHMARKS.html#autotoc_md1163", null ],
          [ "Queue Utilization vs Thread Count", "md_docs_2BENCHMARKS.html#autotoc_md1164", null ],
          [ "CPU Utilization Efficiency", "md_docs_2BENCHMARKS.html#autotoc_md1165", null ]
        ] ],
        [ "Latency Benchmarks", "md_docs_2BENCHMARKS.html#autotoc_md1167", [
          [ "Latency Distribution (Single Thread, Async)", "md_docs_2BENCHMARKS.html#autotoc_md1168", null ],
          [ "Latency Comparison (p99)", "md_docs_2BENCHMARKS.html#autotoc_md1169", null ],
          [ "Latency Under Load", "md_docs_2BENCHMARKS.html#autotoc_md1170", null ]
        ] ],
        [ "Memory Profiling", "md_docs_2BENCHMARKS.html#autotoc_md1172", [
          [ "Memory Footprint by Configuration", "md_docs_2BENCHMARKS.html#autotoc_md1173", null ],
          [ "Memory Comparison with Competitors", "md_docs_2BENCHMARKS.html#autotoc_md1174", null ],
          [ "Memory Allocation Patterns", "md_docs_2BENCHMARKS.html#autotoc_md1175", null ]
        ] ],
        [ "Benchmark Methodology", "md_docs_2BENCHMARKS.html#autotoc_md1177", [
          [ "Test Scenarios", "md_docs_2BENCHMARKS.html#autotoc_md1178", [
            [ "Single-Threaded Throughput Test", "md_docs_2BENCHMARKS.html#autotoc_md1179", null ],
            [ "Multi-Threaded Contention Test", "md_docs_2BENCHMARKS.html#autotoc_md1180", null ],
            [ "Latency Measurement Test", "md_docs_2BENCHMARKS.html#autotoc_md1181", null ]
          ] ],
          [ "Measurement Tools", "md_docs_2BENCHMARKS.html#autotoc_md1182", null ],
          [ "Benchmark Caveats", "md_docs_2BENCHMARKS.html#autotoc_md1183", null ]
        ] ],
        [ "Performance Regression Baselines", "md_docs_2BENCHMARKS.html#autotoc_md1185", null ],
        [ "See Also", "md_docs_2BENCHMARKS.html#autotoc_md1187", null ]
      ] ]
    ] ],
    [ "CHANGELOG.kr", "md_docs_2CHANGELOG_8kr.html", [
      [ "변경 이력 - Logger System", "md_docs_2CHANGELOG_8kr.html#autotoc_md1190", [
        [ "autotoc_md1188", "md_docs_2CHANGELOG_8kr.html#autotoc_md1188", null ],
        [ "category: \"PROJ\"", "md_docs_2CHANGELOG_8kr.html#autotoc_md1189", null ],
        [ "[3.0.0] - 2025-12-31", "md_docs_2CHANGELOG_8kr.html#autotoc_md1192", [
          [ "제거됨 - Deprecated API 정리 (Issues #268, #324)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1193", [
            [ "Breaking Changes", "md_docs_2CHANGELOG_8kr.html#autotoc_md1194", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1195", null ]
          ] ]
        ] ],
        [ "[Unreleased]", "md_docs_2CHANGELOG_8kr.html#autotoc_md1197", [
          [ "실시간 로그 분석 통합 (Issue #281)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1198", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1199", null ],
            [ "예제", "md_docs_2CHANGELOG_8kr.html#autotoc_md1200", null ]
          ] ],
          [ "고용량 시나리오를 위한 로그 샘플링 (Issue #282)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1202", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1203", null ],
            [ "예제", "md_docs_2CHANGELOG_8kr.html#autotoc_md1204", null ]
          ] ],
          [ "Phase 3.4: 구조화된 로깅 통합 (Issue #311) - 2026-01-11", "md_docs_2CHANGELOG_8kr.html#autotoc_md1206", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1207", null ],
            [ "예제", "md_docs_2CHANGELOG_8kr.html#autotoc_md1208", null ]
          ] ],
          [ "Coverage 빌드 수정 (PR #291) - 2026-01-08", "md_docs_2CHANGELOG_8kr.html#autotoc_md1210", [
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1211", null ]
          ] ],
          [ "CI 워크플로우 수정 (PR #290) - 2026-01-08", "md_docs_2CHANGELOG_8kr.html#autotoc_md1213", [
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1214", null ]
          ] ],
          [ "OpenTelemetry 통합 수정 (Issue #283) - 2026-01-08", "md_docs_2CHANGELOG_8kr.html#autotoc_md1216", [
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1217", null ]
          ] ],
          [ "C++20 모듈 파일 (Issue #275) - 2026-01-03", "md_docs_2CHANGELOG_8kr.html#autotoc_md1219", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1220", null ],
            [ "모듈 구조", "md_docs_2CHANGELOG_8kr.html#autotoc_md1221", null ],
            [ "CMake 사용법", "md_docs_2CHANGELOG_8kr.html#autotoc_md1222", null ]
          ] ],
          [ "중복 Writer 추가 로직 수정 (Issue #270) - 2025-12-31", "md_docs_2CHANGELOG_8kr.html#autotoc_md1224", [
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1225", null ],
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1226", null ],
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1227", null ]
          ] ],
          [ "KCENON 기능 감지 사용 (Issue #250) - 2025-12-22", "md_docs_2CHANGELOG_8kr.html#autotoc_md1229", [
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1230", null ],
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1231", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1232", null ]
          ] ],
          [ "Deprecated common_system API 마이그레이션 (Issue #248) - 2025-12-22", "md_docs_2CHANGELOG_8kr.html#autotoc_md1234", [
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1235", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1236", null ]
          ] ],
          [ "thread_system v3.0 호환성 (Issue #244) - 2025-12-19", "md_docs_2CHANGELOG_8kr.html#autotoc_md1238", [
            [ "주요 변경 (Breaking Changes)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1239", null ],
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1240", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1241", null ]
          ] ],
          [ "#226 API 변경 후 손상된 예제 파일 수정 (Issue #228) - 2025-12-06", "md_docs_2CHANGELOG_8kr.html#autotoc_md1243", [
            [ "제거됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1244", null ],
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1245", null ],
            [ "비활성화됨 (기능 구현 대기)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1246", null ],
            [ "관련 이슈", "md_docs_2CHANGELOG_8kr.html#autotoc_md1247", null ]
          ] ],
          [ "common::interfaces::ILogger 구현 - 2025-12-06", "md_docs_2CHANGELOG_8kr.html#autotoc_md1249", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1250", null ],
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1251", null ],
            [ "사용 중단됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1252", null ],
            [ "하위 호환성", "md_docs_2CHANGELOG_8kr.html#autotoc_md1253", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1254", null ],
            [ "관련 이슈", "md_docs_2CHANGELOG_8kr.html#autotoc_md1255", null ]
          ] ],
          [ "독립형 비동기 구현 (Issue #222) - 2025-12-06", "md_docs_2CHANGELOG_8kr.html#autotoc_md1257", [
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1258", null ],
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1259", null ],
            [ "이점", "md_docs_2CHANGELOG_8kr.html#autotoc_md1260", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1261", null ],
            [ "관련 이슈", "md_docs_2CHANGELOG_8kr.html#autotoc_md1262", null ]
          ] ],
          [ "CMake fmt Fallback 제거 - 2025-12-03", "md_docs_2CHANGELOG_8kr.html#autotoc_md1264", [
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1265", null ],
            [ "제거됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1266", null ],
            [ "이점", "md_docs_2CHANGELOG_8kr.html#autotoc_md1267", null ],
            [ "관련 이슈", "md_docs_2CHANGELOG_8kr.html#autotoc_md1268", null ]
          ] ]
        ] ],
        [ "[1.0.0] - 2025-10-22", "md_docs_2CHANGELOG_8kr.html#autotoc_md1270", [
          [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1271", null ],
          [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1272", null ],
          [ "사용 중단됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1273", null ],
          [ "제거됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1274", null ],
          [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1275", null ],
          [ "보안", "md_docs_2CHANGELOG_8kr.html#autotoc_md1276", null ]
        ] ],
        [ "프로젝트 정보", "md_docs_2CHANGELOG_8kr.html#autotoc_md1278", null ],
        [ "버전 지원 매트릭스", "md_docs_2CHANGELOG_8kr.html#autotoc_md1280", null ],
        [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1282", null ]
      ] ]
    ] ],
    [ "CHANGELOG", "md_docs_2CHANGELOG.html", [
      [ "Changelog - Logger System", "md_docs_2CHANGELOG.html#autotoc_md1286", [
        [ "autotoc_md1284", "md_docs_2CHANGELOG.html#autotoc_md1284", null ],
        [ "category: \"PROJ\"", "md_docs_2CHANGELOG.html#autotoc_md1285", null ],
        [ "[4.1.0] - Unreleased", "md_docs_2CHANGELOG.html#autotoc_md1288", [
          [ "Changed - Doxygen Modernization (Issue #581)", "md_docs_2CHANGELOG.html#autotoc_md1289", null ],
          [ "Added - Writer Builder for Fluent Decorator Composition (Issue #413, #415, #416)", "md_docs_2CHANGELOG.html#autotoc_md1290", [
            [ "New Components", "md_docs_2CHANGELOG.html#autotoc_md1291", null ],
            [ "Usage Examples", "md_docs_2CHANGELOG.html#autotoc_md1292", null ],
            [ "Benefits", "md_docs_2CHANGELOG.html#autotoc_md1293", null ],
            [ "Documentation", "md_docs_2CHANGELOG.html#autotoc_md1294", null ]
          ] ],
          [ "Deprecated - Legacy Writer Patterns (Issue #418)", "md_docs_2CHANGELOG.html#autotoc_md1295", [
            [ "Deprecated Classes and Patterns", "md_docs_2CHANGELOG.html#autotoc_md1296", null ],
            [ "Deprecation Timeline", "md_docs_2CHANGELOG.html#autotoc_md1297", null ],
            [ "Migration Support", "md_docs_2CHANGELOG.html#autotoc_md1298", null ]
          ] ]
        ] ],
        [ "[4.0.0] - Unreleased", "md_docs_2CHANGELOG.html#autotoc_md1300", [
          [ "Removed - Deprecated Context ID Convenience Methods (Issue #326)", "md_docs_2CHANGELOG.html#autotoc_md1301", [
            [ "Breaking Changes", "md_docs_2CHANGELOG.html#autotoc_md1302", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1303", null ],
            [ "Benefits", "md_docs_2CHANGELOG.html#autotoc_md1304", null ]
          ] ],
          [ "Added - Decorator Pattern Writer Architecture (Issue #356)", "md_docs_2CHANGELOG.html#autotoc_md1306", [
            [ "New Components", "md_docs_2CHANGELOG.html#autotoc_md1307", null ],
            [ "Usage Examples", "md_docs_2CHANGELOG.html#autotoc_md1308", null ],
            [ "Architecture Benefits", "md_docs_2CHANGELOG.html#autotoc_md1309", null ],
            [ "Custom Writer Migration", "md_docs_2CHANGELOG.html#autotoc_md1310", null ]
          ] ]
        ] ],
        [ "[3.0.0] - 2025-12-31", "md_docs_2CHANGELOG.html#autotoc_md1312", [
          [ "Removed - Deprecated log_level Types and Converters (Issue #339)", "md_docs_2CHANGELOG.html#autotoc_md1313", [
            [ "Breaking Changes", "md_docs_2CHANGELOG.html#autotoc_md1314", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1315", null ]
          ] ],
          [ "Removed - Deprecated API Cleanup (Issues #268, #324)", "md_docs_2CHANGELOG.html#autotoc_md1317", [
            [ "Breaking Changes", "md_docs_2CHANGELOG.html#autotoc_md1318", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1319", null ]
          ] ]
        ] ],
        [ "[Unreleased]", "md_docs_2CHANGELOG.html#autotoc_md1321", [
          [ "Real-time Log Analysis Integration (Issue #281)", "md_docs_2CHANGELOG.html#autotoc_md1322", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1323", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1324", null ]
          ] ],
          [ "Log Sampling for High-Volume Scenarios (Issue #282)", "md_docs_2CHANGELOG.html#autotoc_md1326", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1327", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1328", null ]
          ] ],
          [ "Phase 3.4: Structured Logging Integration (Issue #311) - 2026-01-11", "md_docs_2CHANGELOG.html#autotoc_md1330", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1331", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1332", null ]
          ] ],
          [ "Phase 3.3: Structured Logging Formatters (Issue #310) - 2026-01-11", "md_docs_2CHANGELOG.html#autotoc_md1334", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1335", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1336", null ]
          ] ],
          [ "Phase 3.1: Structured Logging API Design (Issue #308) - 2026-01-11", "md_docs_2CHANGELOG.html#autotoc_md1338", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1339", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1340", null ]
          ] ],
          [ "Phase 3.2: Structured Logging Core Implementation (Issue #309) - 2026-01-11", "md_docs_2CHANGELOG.html#autotoc_md1342", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1343", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1344", null ]
          ] ],
          [ "Coverage Build Fix (PR #291) - 2026-01-08", "md_docs_2CHANGELOG.html#autotoc_md1346", [
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1347", null ]
          ] ],
          [ "CI Workflow Fix (PR #290) - 2026-01-08", "md_docs_2CHANGELOG.html#autotoc_md1349", [
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1350", null ]
          ] ],
          [ "OpenTelemetry Integration Fix (Issue #283) - 2026-01-08", "md_docs_2CHANGELOG.html#autotoc_md1352", [
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1353", null ]
          ] ],
          [ "C++20 Module Files (Issue #275) - 2026-01-03", "md_docs_2CHANGELOG.html#autotoc_md1355", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1356", null ],
            [ "Module Structure", "md_docs_2CHANGELOG.html#autotoc_md1357", null ],
            [ "CMake Usage", "md_docs_2CHANGELOG.html#autotoc_md1358", null ]
          ] ],
          [ "Fix Duplicate Writer Add Logic (Issue #270) - 2025-12-31", "md_docs_2CHANGELOG.html#autotoc_md1360", [
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1361", null ],
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1362", null ],
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1363", null ]
          ] ],
          [ "Use KCENON Feature Detection (Issue #250) - 2025-12-22", "md_docs_2CHANGELOG.html#autotoc_md1365", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1366", null ],
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1367", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1368", null ]
          ] ],
          [ "Migrate from Deprecated common_system APIs (Issue #248) - 2025-12-22", "md_docs_2CHANGELOG.html#autotoc_md1370", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1371", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1372", null ]
          ] ],
          [ "thread_system v3.0 Compatibility (Issue #244) - 2025-12-19", "md_docs_2CHANGELOG.html#autotoc_md1374", [
            [ "Breaking Changes", "md_docs_2CHANGELOG.html#autotoc_md1375", null ],
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1376", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1377", null ]
          ] ],
          [ "Fix broken example files after #226 API changes (Issue #228) - 2025-12-06", "md_docs_2CHANGELOG.html#autotoc_md1379", [
            [ "Removed", "md_docs_2CHANGELOG.html#autotoc_md1380", null ],
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1381", null ],
            [ "Disabled (awaiting feature implementation)", "md_docs_2CHANGELOG.html#autotoc_md1382", null ],
            [ "Related Issues", "md_docs_2CHANGELOG.html#autotoc_md1383", null ]
          ] ],
          [ "common::interfaces::ILogger Implementation - 2025-12-06", "md_docs_2CHANGELOG.html#autotoc_md1385", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1386", null ],
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1387", null ],
            [ "Deprecated", "md_docs_2CHANGELOG.html#autotoc_md1388", null ],
            [ "Backward Compatibility", "md_docs_2CHANGELOG.html#autotoc_md1389", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1390", null ],
            [ "Related Issues", "md_docs_2CHANGELOG.html#autotoc_md1391", null ]
          ] ],
          [ "Standalone async implementation (Issue #222) - 2025-12-06", "md_docs_2CHANGELOG.html#autotoc_md1393", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1394", null ],
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1395", null ],
            [ "Benefits", "md_docs_2CHANGELOG.html#autotoc_md1396", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1397", null ],
            [ "Related Issues", "md_docs_2CHANGELOG.html#autotoc_md1398", null ]
          ] ],
          [ "CMake fmt Fallback Removal - 2025-12-03", "md_docs_2CHANGELOG.html#autotoc_md1400", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1401", null ],
            [ "Removed", "md_docs_2CHANGELOG.html#autotoc_md1402", null ],
            [ "Benefits", "md_docs_2CHANGELOG.html#autotoc_md1403", null ],
            [ "Related Issues", "md_docs_2CHANGELOG.html#autotoc_md1404", null ]
          ] ],
          [ "thread_system Required Dependency - 2025-12-03", "md_docs_2CHANGELOG.html#autotoc_md1406", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1407", null ]
          ] ]
        ] ]
      ] ]
    ] ],
    [ "iso-27001", "md_docs_2compliance_2iso-27001.html", [
      [ "ISO/IEC 27001 Control Mapping", "md_docs_2compliance_2iso-27001.html#autotoc_md1410", [
        [ "autotoc_md1408", "md_docs_2compliance_2iso-27001.html#autotoc_md1408", null ],
        [ "category: \"Compliance\"", "md_docs_2compliance_2iso-27001.html#autotoc_md1409", null ],
        [ "Table of Contents", "md_docs_2compliance_2iso-27001.html#autotoc_md1411", null ],
        [ "Overview", "md_docs_2compliance_2iso-27001.html#autotoc_md1412", null ],
        [ "Applicability", "md_docs_2compliance_2iso-27001.html#autotoc_md1413", null ],
        [ "Control Mapping", "md_docs_2compliance_2iso-27001.html#autotoc_md1414", [
          [ "A.5 Organizational controls", "md_docs_2compliance_2iso-27001.html#autotoc_md1415", null ],
          [ "A.8 Technological controls", "md_docs_2compliance_2iso-27001.html#autotoc_md1416", null ]
        ] ],
        [ "Configuration Checklist", "md_docs_2compliance_2iso-27001.html#autotoc_md1417", null ],
        [ "Evidence Collection", "md_docs_2compliance_2iso-27001.html#autotoc_md1418", null ],
        [ "Related Standards", "md_docs_2compliance_2iso-27001.html#autotoc_md1419", null ],
        [ "References", "md_docs_2compliance_2iso-27001.html#autotoc_md1420", null ]
      ] ]
    ] ],
    [ "CONFIG_STRATEGIES_ADVANCED", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html", [
      [ "Configuration Strategies - Advanced", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1423", [
        [ "autotoc_md1421", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1421", null ],
        [ "category: \"GUID\"", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1422", null ],
        [ "Table of Contents", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1425", null ],
        [ "Composite Strategy", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1427", [
          [ "How It Works", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1428", null ],
          [ "API", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1429", null ],
          [ "Method Details", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1430", [
            [ "add(std::unique_ptr<config_strategy_interface> strategy)", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1431", null ],
            [ "add<Strategy, Args...>(Args&&... args)", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1432", null ]
          ] ],
          [ "Priority-Based Application", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1433", null ],
          [ "Explicit Composite Strategy Example", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1434", null ]
        ] ],
        [ "Custom Strategy Implementation", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1436", [
          [ "Basic Custom Strategy", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1437", null ],
          [ "Conditional Strategy (is_applicable)", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1438", null ]
        ] ],
        [ "Strategy Priority and Conflict Resolution", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1440", [
          [ "Priority Values", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1441", null ],
          [ "Conflict Resolution Rules", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1442", null ],
          [ "Best Practice: Environment Variables as Final Override", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1443", null ]
        ] ],
        [ "Best Practices", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1445", [
          [ "Start with Defaults, Override as Needed", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1446", null ],
          [ "Use Environment Variables for Runtime Configuration", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1447", null ],
          [ "Combine Strategies for Complex Requirements", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1448", null ],
          [ "Test Strategies in Isolation", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1449", null ],
          [ "Document Custom Strategies", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1450", null ]
        ] ],
        [ "Summary", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1452", null ]
      ] ]
    ] ],
    [ "CONFIG_STRATEGIES_BASIC", "md_docs_2CONFIG__STRATEGIES__BASIC.html", [
      [ "Configuration Strategies - Getting Started and Common Patterns", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1456", [
        [ "autotoc_md1454", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1454", null ],
        [ "category: \"GUID\"", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1455", null ],
        [ "Table of Contents", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1458", null ],
        [ "Overview", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1460", [
          [ "What are Configuration Strategies?", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1461", null ],
          [ "Architecture", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1462", null ]
        ] ],
        [ "Strategy Pattern Benefits", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1464", [
          [ "Why Use Strategies?", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1465", null ],
          [ "Example: Multi-Environment Deployment", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1466", null ]
        ] ],
        [ "Strategy Interface", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1468", [
          [ "Base Interface", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1469", null ],
          [ "Strategy Lifecycle", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1470", null ],
          [ "Method Details", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1471", [
            [ "get_name()", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1472", null ],
            [ "apply(logger_config& config)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1473", null ],
            [ "is_applicable()", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1474", null ],
            [ "priority()", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1475", null ]
          ] ]
        ] ],
        [ "Built-in Strategies", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1477", [
          [ "Deployment Strategy", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1478", [
            [ "Supported Environments", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1479", null ],
            [ "Environment Profiles", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1480", [
              [ "Development (deployment_env::development)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1481", null ],
              [ "Staging (deployment_env::staging)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1482", null ],
              [ "Production (deployment_env::production)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1483", null ],
              [ "Testing (deployment_env::testing)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1484", null ]
            ] ],
            [ "API", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1485", null ]
          ] ],
          [ "Environment Strategy", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1487", [
            [ "Supported Environment Variables", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1488", null ],
            [ "API", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1489", null ],
            [ "Docker/Kubernetes Example", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1490", null ]
          ] ],
          [ "Performance Strategy", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1492", [
            [ "Performance Levels", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1493", null ],
            [ "Performance Profiles", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1494", [
              [ "Low Latency (performance_level::low_latency)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1495", null ],
              [ "Balanced (performance_level::balanced)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1496", null ],
              [ "High Throughput (performance_level::high_throughput)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1497", null ],
              [ "Minimal Overhead (performance_level::minimal_overhead)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1498", null ]
            ] ],
            [ "API", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1499", null ]
          ] ]
        ] ],
        [ "Usage Examples", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1501", [
          [ "Example 1: Simple Deployment Strategy", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1502", null ],
          [ "Example 2: Combined Deployment + Performance", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1503", null ],
          [ "Example 3: Environment Variables + Deployment", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1504", null ],
          [ "Example 4: Conditional Strategy (Docker)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1505", null ],
          [ "Example 5: Multi-Logger Configuration", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1506", null ]
        ] ],
        [ "Related Documentation", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1508", null ]
      ] ]
    ] ],
    [ "CONFIGURATION_STRATEGIES", "md_docs_2CONFIGURATION__STRATEGIES.html", [
      [ "Configuration Strategies Guide", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1512", [
        [ "autotoc_md1510", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1510", null ],
        [ "category: \"GUID\"", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1511", null ],
        [ "Sub-Documents", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1513", null ],
        [ "Quick Links", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1514", [
          [ "Getting Started", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1515", null ],
          [ "Built-in Strategies", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1516", null ],
          [ "Advanced Topics", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1517", null ]
        ] ],
        [ "Related Documentation", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1518", null ]
      ] ]
    ] ],
    [ "CONTRIBUTING.kr", "md_docs_2contributing_2CONTRIBUTING_8kr.html", [
      [ "Logger System 기여하기", "md_docs_2contributing_2CONTRIBUTING_8kr.html#autotoc_md1522", null ]
    ] ],
    [ "CONTRIBUTING", "md_docs_2contributing_2CONTRIBUTING.html", [
      [ "Contributing to Logger System", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1527", [
        [ "autotoc_md1525", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1525", null ],
        [ "category: \"PROJ\"", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1526", null ],
        [ "Table of Contents", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1528", null ],
        [ "Code of Conduct", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1529", null ],
        [ "Getting Started", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1530", null ],
        [ "Development Process", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1531", [
          [ "Before You Start", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1532", null ],
          [ "Making Changes", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1533", null ],
          [ "Commit Message Format", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1534", null ]
        ] ],
        [ "Code Style", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1535", [
          [ "C++ Guidelines", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1536", null ],
          [ "Code Formatting", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1537", null ],
          [ "Naming Conventions", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1538", null ]
        ] ],
        [ "Testing", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1539", [
          [ "Unit Tests", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1540", null ],
          [ "Running Tests", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1541", null ],
          [ "Benchmarks", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1542", null ]
        ] ],
        [ "Documentation", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1543", [
          [ "Code Documentation", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1544", null ],
          [ "Documentation Updates", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1545", null ]
        ] ],
        [ "Submitting Changes", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1546", [
          [ "Pull Request Process", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1547", null ],
          [ "Review Process", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1548", null ]
        ] ],
        [ "Questions?", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1549", null ],
        [ "Thank you for contributing!", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1550", null ]
      ] ]
    ] ],
    [ "TESTING_GUIDE", "md_docs_2contributing_2TESTING__GUIDE.html", null ],
    [ "TRANSLATION_SUMMARY", "md_docs_2contributing_2TRANSLATION__SUMMARY.html", [
      [ "Korean Translation Summary", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1556", [
        [ "autotoc_md1554", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1554", null ],
        [ "category: \"PROJ\"", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1555", null ],
        [ "Completion Status", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1557", null ],
        [ "Statistics", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1558", null ],
        [ "Files Created", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1559", [
          [ "Root Directory (19 files)", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1560", null ],
          [ "advanced/ Directory (2 files)", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1561", null ],
          [ "guides/ Directory (4 files)", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1562", null ]
        ] ],
        [ "Language Switcher Format", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1563", [
          [ "English Files", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1564", null ],
          [ "Korean Files", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1565", null ]
        ] ],
        [ "Translation Approach", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1566", null ],
        [ "File Structure", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1567", null ],
        [ "Verification", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1568", null ],
        [ "Notes", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1569", null ],
        [ "Next Steps (Optional)", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1570", null ]
      ] ]
    ] ],
    [ "CRASH_SAFETY_GUIDE", "md_docs_2CRASH__SAFETY__GUIDE.html", [
      [ "Crash-Safe Logger Guide", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1574", [
        [ "autotoc_md1572", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1572", null ],
        [ "category: \"GUID\"", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1573", null ],
        [ "Table of Contents", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1575", null ],
        [ "Overview", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1577", null ],
        [ "Crash Safety Mechanism", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1579", [
          [ "How It Works", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1580", null ],
          [ "Signal Handlers", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1581", null ],
          [ "Async-Signal-Safe Constraints", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1582", null ],
          [ "Recovery Procedure", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1583", null ]
        ] ],
        [ "Configuration", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1585", [
          [ "Basic Setup", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1586", null ],
          [ "Advanced Configuration", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1587", null ]
        ] ],
        [ "API Reference", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1589", [
          [ "crash_safe_logger Class", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1590", null ],
          [ "Method Details", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1591", [
            [ "create()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1592", null ],
            [ "install_crash_handlers()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1593", null ],
            [ "uninstall_crash_handlers()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1594", null ],
            [ "flush_with_timeout()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1595", null ],
            [ "emergency_flush()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1596", null ],
            [ "set_auto_flush_interval()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1597", null ],
            [ "set_min_level() / get_min_level()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1598", null ]
          ] ]
        ] ],
        [ "Performance Overhead", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1600", [
          [ "Benchmarks", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1601", null ],
          [ "Overhead Sources", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1602", null ],
          [ "Optimization Tips", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1603", null ]
        ] ],
        [ "When to Use", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1605", [
          [ "Use Crash-Safe Logger When:", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1606", null ],
          [ "Do NOT Use When:", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1607", null ]
        ] ],
        [ "Combined Usage", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1609", [
          [ "Local Crash Safety + Network Forwarding", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1610", null ],
          [ "Server-Side Crash Safety", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1611", null ],
          [ "Complete Production Topology", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1612", null ]
        ] ],
        [ "Best Practices", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1614", [
          [ "Choose Appropriate Auto-Flush Interval", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1615", null ],
          [ "Test Signal Handlers", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1616", null ],
          [ "Avoid Signal Handler Conflicts", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1617", null ],
          [ "Graceful Shutdown", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1618", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1620", [
          [ "Problem: Emergency flush not working", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1621", null ],
          [ "Problem: High CPU usage from auto-flush", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1622", null ],
          [ "Problem: Signal handler conflicts", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1623", null ]
        ] ],
        [ "Related Documentation", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1625", [
          [ "Header Files", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1626", null ],
          [ "External Resources", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1627", null ]
        ] ]
      ] ]
    ] ],
    [ "DEPENDENCY_ARCHITECTURE", "md_docs_2DEPENDENCY__ARCHITECTURE.html", [
      [ "Dependency Architecture", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1631", [
        [ "autotoc_md1629", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1629", null ],
        [ "category: \"ARCH\"", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1630", null ],
        [ "Overview", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1632", null ],
        [ "Dependency Tiers", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1633", null ],
        [ "Abstraction Interfaces", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1634", null ],
        [ "Resolved: Bidirectional Dependency (Issue #252)", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1635", [
          [ "Previous State", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1636", null ],
          [ "Current State (Resolved)", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1637", null ],
          [ "CMake Guard", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1638", null ]
        ] ],
        [ "Configuration", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1639", [
          [ "Recommended (Default)", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1640", null ],
          [ "With thread_pool Backend", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1641", null ],
          [ "Deprecated (Avoid)", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1642", null ]
        ] ],
        [ "Migration Guide", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1643", null ],
        [ "Timeline", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1644", null ],
        [ "Related", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1645", null ]
      ] ]
    ] ],
    [ "Ecosystem Integration", "md_docs_2ECOSYSTEM.html", [
      [ "Dependencies", "md_docs_2ECOSYSTEM.html#autotoc_md1647", null ],
      [ "Dependent Systems", "md_docs_2ECOSYSTEM.html#autotoc_md1648", null ],
      [ "All Systems", "md_docs_2ECOSYSTEM.html#autotoc_md1649", null ]
    ] ],
    [ "Frequently Asked Questions", "faq.html", [
      [ "How do I configure log rotation?", "faq.html#faq_rotation", null ],
      [ "What are the tradeoffs between async and sync logging?", "faq.html#faq_async_vs_sync", null ],
      [ "How do I add a custom writer?", "faq.html#faq_custom_writer", null ],
      [ "How does logger_system integrate with monitoring_system?", "faq.html#faq_monitoring", null ],
      [ "Is logger_system thread-safe?", "faq.html#faq_thread_safety", null ],
      [ "How do I tune logger_system for maximum performance?", "faq.html#faq_perf", null ],
      [ "How do I set up OpenTelemetry (OTLP) export?", "faq.html#faq_otlp", null ],
      [ "Is decorator order important, and what is the recommended order?", "faq.html#faq_decorator_order", null ],
      [ "What naming conventions should I use for structured fields?", "faq.html#faq_field_conventions", null ],
      [ "How do I encrypt log files or protect sensitive data?", "faq.html#faq_security", null ],
      [ "More Questions?", "faq.html#faq_more", null ]
    ] ],
    [ "FEATURE_MATRIX", "md_docs_2FEATURE__MATRIX.html", [
      [ "Logger System Production Feature Matrix", "md_docs_2FEATURE__MATRIX.html#autotoc_md1652", [
        [ "autotoc_md1650", "md_docs_2FEATURE__MATRIX.html#autotoc_md1650", null ],
        [ "category: \"FEAT\"", "md_docs_2FEATURE__MATRIX.html#autotoc_md1651", null ],
        [ "How to Read This Matrix", "md_docs_2FEATURE__MATRIX.html#autotoc_md1654", null ],
        [ "Default-Value Authority", "md_docs_2FEATURE__MATRIX.html#autotoc_md1656", null ],
        [ "Feature Matrix", "md_docs_2FEATURE__MATRIX.html#autotoc_md1658", null ],
        [ "", "md_docs_2FEATURE__MATRIX.html#autotoc_md1659", null ],
        [ "Default Build Profile", "md_docs_2FEATURE__MATRIX.html#autotoc_md1660", null ],
        [ "OpenSSL / Encryption Default Behavior", "md_docs_2FEATURE__MATRIX.html#autotoc_md1662", null ],
        [ "Optional Integration Details", "md_docs_2FEATURE__MATRIX.html#autotoc_md1664", [
          [ "thread_system integration (LOGGER_USE_THREAD_SYSTEM, default OFF)", "md_docs_2FEATURE__MATRIX.html#autotoc_md1665", null ],
          [ "OTLP export (LOGGER_ENABLE_OTLP, default OFF)", "md_docs_2FEATURE__MATRIX.html#autotoc_md1666", null ],
          [ "Network writer (LOGGER_ENABLE_NETWORK_WRITER, default OFF)", "md_docs_2FEATURE__MATRIX.html#autotoc_md1667", null ],
          [ "Log compression (LOGGER_USE_COMPRESSION, default OFF)", "md_docs_2FEATURE__MATRIX.html#autotoc_md1668", null ]
        ] ],
        [ "Verification Summary", "md_docs_2FEATURE__MATRIX.html#autotoc_md1670", null ],
        [ "Related Documentation", "md_docs_2FEATURE__MATRIX.html#autotoc_md1672", null ]
      ] ]
    ] ],
    [ "FEATURES.kr", "md_docs_2FEATURES_8kr.html", [
      [ "Logger System - 상세 기능", "md_docs_2FEATURES_8kr.html#autotoc_md1675", [
        [ "autotoc_md1673", "md_docs_2FEATURES_8kr.html#autotoc_md1673", null ],
        [ "category: \"FEAT\"", "md_docs_2FEATURES_8kr.html#autotoc_md1674", null ],
        [ "목차", "md_docs_2FEATURES_8kr.html#autotoc_md1677", null ],
        [ "핵심 기능", "md_docs_2FEATURES_8kr.html#autotoc_md1679", [
          [ "설계 원칙", "md_docs_2FEATURES_8kr.html#autotoc_md1680", null ],
          [ "주요 기능", "md_docs_2FEATURES_8kr.html#autotoc_md1681", null ]
        ] ],
        [ "로그 레벨", "md_docs_2FEATURES_8kr.html#autotoc_md1683", [
          [ "레벨 정의", "md_docs_2FEATURES_8kr.html#autotoc_md1684", null ],
          [ "레벨별 사용 지침", "md_docs_2FEATURES_8kr.html#autotoc_md1685", null ],
          [ "사용 예시", "md_docs_2FEATURES_8kr.html#autotoc_md1686", null ]
        ] ],
        [ "로그 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1688", [
          [ "콘솔 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1689", null ],
          [ "파일 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1690", null ],
          [ "회전 파일 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1691", null ],
          [ "원격 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1692", null ],
          [ "커스텀 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1693", null ]
        ] ],
        [ "포맷팅", "md_docs_2FEATURES_8kr.html#autotoc_md1695", [
          [ "기본 포맷", "md_docs_2FEATURES_8kr.html#autotoc_md1696", null ],
          [ "JSON 포맷", "md_docs_2FEATURES_8kr.html#autotoc_md1697", null ],
          [ "커스텀 포맷터", "md_docs_2FEATURES_8kr.html#autotoc_md1698", null ]
        ] ],
        [ "필터링", "md_docs_2FEATURES_8kr.html#autotoc_md1700", [
          [ "레벨 필터링", "md_docs_2FEATURES_8kr.html#autotoc_md1701", null ],
          [ "패턴 필터링", "md_docs_2FEATURES_8kr.html#autotoc_md1702", null ],
          [ "조건부 필터링", "md_docs_2FEATURES_8kr.html#autotoc_md1703", null ]
        ] ],
        [ "비동기 로깅", "md_docs_2FEATURES_8kr.html#autotoc_md1705", [
          [ "비동기 모드 활성화", "md_docs_2FEATURES_8kr.html#autotoc_md1706", null ],
          [ "성능 비교", "md_docs_2FEATURES_8kr.html#autotoc_md1707", null ]
        ] ],
        [ "성능 특성", "md_docs_2FEATURES_8kr.html#autotoc_md1709", [
          [ "벤치마크 결과", "md_docs_2FEATURES_8kr.html#autotoc_md1710", null ],
          [ "최적화 팁", "md_docs_2FEATURES_8kr.html#autotoc_md1711", null ]
        ] ],
        [ "통합 기능", "md_docs_2FEATURES_8kr.html#autotoc_md1713", [
          [ "thread_system 통합", "md_docs_2FEATURES_8kr.html#autotoc_md1714", null ],
          [ "monitoring_system 통합", "md_docs_2FEATURES_8kr.html#autotoc_md1715", null ],
          [ "common_system 통합", "md_docs_2FEATURES_8kr.html#autotoc_md1716", null ]
        ] ],
        [ "구성 예시", "md_docs_2FEATURES_8kr.html#autotoc_md1718", [
          [ "프로덕션 설정", "md_docs_2FEATURES_8kr.html#autotoc_md1719", null ],
          [ "개발 설정", "md_docs_2FEATURES_8kr.html#autotoc_md1720", null ]
        ] ],
        [ "OTLP 통합", "md_docs_2FEATURES_8kr.html#autotoc_md1722", [
          [ "개요", "md_docs_2FEATURES_8kr.html#autotoc_md1723", null ],
          [ "OTLP Writer 설정", "md_docs_2FEATURES_8kr.html#autotoc_md1724", null ],
          [ "Trace Context 전파", "md_docs_2FEATURES_8kr.html#autotoc_md1725", null ],
          [ "프로토콜 지원", "md_docs_2FEATURES_8kr.html#autotoc_md1726", null ],
          [ "기능", "md_docs_2FEATURES_8kr.html#autotoc_md1727", null ]
        ] ],
        [ "로그 샘플링", "md_docs_2FEATURES_8kr.html#autotoc_md1729", [
          [ "샘플링 전략", "md_docs_2FEATURES_8kr.html#autotoc_md1730", null ],
          [ "빠른 설정", "md_docs_2FEATURES_8kr.html#autotoc_md1731", null ],
          [ "설정", "md_docs_2FEATURES_8kr.html#autotoc_md1732", null ],
          [ "기능", "md_docs_2FEATURES_8kr.html#autotoc_md1733", null ]
        ] ],
        [ "실시간 로그 분석", "md_docs_2FEATURES_8kr.html#autotoc_md1735", [
          [ "이상 유형", "md_docs_2FEATURES_8kr.html#autotoc_md1736", null ],
          [ "사용법", "md_docs_2FEATURES_8kr.html#autotoc_md1737", null ]
        ] ],
        [ "참고사항", "md_docs_2FEATURES_8kr.html#autotoc_md1739", [
          [ "스레드 안전성", "md_docs_2FEATURES_8kr.html#autotoc_md1740", null ],
          [ "주의사항", "md_docs_2FEATURES_8kr.html#autotoc_md1741", null ]
        ] ]
      ] ]
    ] ],
    [ "FEATURES", "md_docs_2FEATURES.html", [
      [ "Logger System Features", "md_docs_2FEATURES.html#autotoc_md1746", [
        [ "autotoc_md1744", "md_docs_2FEATURES.html#autotoc_md1744", null ],
        [ "category: \"FEAT\"", "md_docs_2FEATURES.html#autotoc_md1745", null ],
        [ "Table of Contents", "md_docs_2FEATURES.html#autotoc_md1748", null ],
        [ "Core Features", "md_docs_2FEATURES.html#autotoc_md1750", [
          [ "🎯 High-Performance Logging", "md_docs_2FEATURES.html#autotoc_md1751", [
            [ "Asynchronous Processing", "md_docs_2FEATURES.html#autotoc_md1752", null ],
            [ "Thread-Safe Operations", "md_docs_2FEATURES.html#autotoc_md1753", null ],
            [ "Configuration Validation", "md_docs_2FEATURES.html#autotoc_md1754", null ]
          ] ],
          [ "🔧 Advanced Capabilities", "md_docs_2FEATURES.html#autotoc_md1755", [
            [ "Result-Based Error Handling", "md_docs_2FEATURES.html#autotoc_md1756", null ],
            [ "Builder Pattern API", "md_docs_2FEATURES.html#autotoc_md1757", null ],
            [ "Configuration Strategies", "md_docs_2FEATURES.html#autotoc_md1758", null ],
            [ "Monitoring Integration", "md_docs_2FEATURES.html#autotoc_md1759", null ]
          ] ]
        ] ],
        [ "Writer Types", "md_docs_2FEATURES.html#autotoc_md1761", [
          [ "Console Writer", "md_docs_2FEATURES.html#autotoc_md1762", null ],
          [ "File Writer", "md_docs_2FEATURES.html#autotoc_md1763", null ],
          [ "Rotating File Writer", "md_docs_2FEATURES.html#autotoc_md1764", null ],
          [ "Network Writer", "md_docs_2FEATURES.html#autotoc_md1765", null ],
          [ "Critical Writer", "md_docs_2FEATURES.html#autotoc_md1766", null ],
          [ "Hybrid Writer", "md_docs_2FEATURES.html#autotoc_md1767", null ]
        ] ],
        [ "Formatting Options", "md_docs_2FEATURES.html#autotoc_md1769", [
          [ "Plain Text Formatter", "md_docs_2FEATURES.html#autotoc_md1770", null ],
          [ "JSON Formatter", "md_docs_2FEATURES.html#autotoc_md1771", null ],
          [ "Logfmt Formatter", "md_docs_2FEATURES.html#autotoc_md1772", null ],
          [ "Custom Formatter", "md_docs_2FEATURES.html#autotoc_md1773", null ]
        ] ],
        [ "Filtering Capabilities", "md_docs_2FEATURES.html#autotoc_md1775", [
          [ "Level-Based Filtering", "md_docs_2FEATURES.html#autotoc_md1776", null ],
          [ "Regex-Based Filtering", "md_docs_2FEATURES.html#autotoc_md1777", null ],
          [ "Function-Based Filtering", "md_docs_2FEATURES.html#autotoc_md1778", null ],
          [ "Composite Filtering", "md_docs_2FEATURES.html#autotoc_md1779", null ]
        ] ],
        [ "Rotation Policies", "md_docs_2FEATURES.html#autotoc_md1781", [
          [ "Size-Based Rotation", "md_docs_2FEATURES.html#autotoc_md1782", null ],
          [ "Time-Based Rotation", "md_docs_2FEATURES.html#autotoc_md1783", null ],
          [ "Hybrid Rotation", "md_docs_2FEATURES.html#autotoc_md1784", null ],
          [ "Compression Support", "md_docs_2FEATURES.html#autotoc_md1785", null ]
        ] ],
        [ "Security Features", "md_docs_2FEATURES.html#autotoc_md1787", [
          [ "Secure Key Storage", "md_docs_2FEATURES.html#autotoc_md1788", null ],
          [ "Path Validation", "md_docs_2FEATURES.html#autotoc_md1789", null ],
          [ "Signal Handler Safety", "md_docs_2FEATURES.html#autotoc_md1790", null ],
          [ "Security Audit Logging", "md_docs_2FEATURES.html#autotoc_md1791", null ],
          [ "Sensitive Data Sanitization", "md_docs_2FEATURES.html#autotoc_md1792", null ],
          [ "Access Control", "md_docs_2FEATURES.html#autotoc_md1793", null ],
          [ "Compliance Support", "md_docs_2FEATURES.html#autotoc_md1794", null ]
        ] ],
        [ "Advanced Capabilities", "md_docs_2FEATURES.html#autotoc_md1796", [
          [ "Structured Logging", "md_docs_2FEATURES.html#autotoc_md1797", null ],
          [ "Log Routing", "md_docs_2FEATURES.html#autotoc_md1798", null ],
          [ "Performance Metrics Collection", "md_docs_2FEATURES.html#autotoc_md1799", null ],
          [ "Real-time Log Analysis", "md_docs_2FEATURES.html#autotoc_md1800", [
            [ "Post-hoc Analysis (log_analyzer)", "md_docs_2FEATURES.html#autotoc_md1801", null ],
            [ "Real-time Anomaly Detection (realtime_log_analyzer)", "md_docs_2FEATURES.html#autotoc_md1802", null ]
          ] ],
          [ "Distributed Logging", "md_docs_2FEATURES.html#autotoc_md1803", null ]
        ] ],
        [ "OTLP Integration", "md_docs_2FEATURES.html#autotoc_md1805", [
          [ "Overview", "md_docs_2FEATURES.html#autotoc_md1806", null ],
          [ "OTLP Writer Configuration", "md_docs_2FEATURES.html#autotoc_md1807", null ],
          [ "Trace Context Propagation", "md_docs_2FEATURES.html#autotoc_md1808", null ],
          [ "Protocol Support", "md_docs_2FEATURES.html#autotoc_md1809", null ],
          [ "Export Statistics", "md_docs_2FEATURES.html#autotoc_md1810", null ],
          [ "Features", "md_docs_2FEATURES.html#autotoc_md1811", null ]
        ] ],
        [ "Log Sampling", "md_docs_2FEATURES.html#autotoc_md1813", [
          [ "Overview", "md_docs_2FEATURES.html#autotoc_md1814", null ],
          [ "Sampling Strategies", "md_docs_2FEATURES.html#autotoc_md1815", null ],
          [ "Quick Setup", "md_docs_2FEATURES.html#autotoc_md1816", null ],
          [ "Configuration", "md_docs_2FEATURES.html#autotoc_md1817", null ],
          [ "Runtime Control", "md_docs_2FEATURES.html#autotoc_md1818", null ],
          [ "Features", "md_docs_2FEATURES.html#autotoc_md1819", null ]
        ] ],
        [ "See Also", "md_docs_2FEATURES.html#autotoc_md1821", null ]
      ] ]
    ] ],
    [ "GETTING_STARTED", "md_docs_2GETTING__STARTED.html", [
      [ "Getting Started with logger_system", "md_docs_2GETTING__STARTED.html#autotoc_md1824", [
        [ "autotoc_md1822", "md_docs_2GETTING__STARTED.html#autotoc_md1822", null ],
        [ "category: \"GUID\"", "md_docs_2GETTING__STARTED.html#autotoc_md1823", null ],
        [ "Prerequisites", "md_docs_2GETTING__STARTED.html#autotoc_md1825", null ],
        [ "Installation", "md_docs_2GETTING__STARTED.html#autotoc_md1826", [
          [ "Option A: CMake FetchContent", "md_docs_2GETTING__STARTED.html#autotoc_md1827", null ],
          [ "Option B: Build from source", "md_docs_2GETTING__STARTED.html#autotoc_md1828", null ],
          [ "Option C: vcpkg preset", "md_docs_2GETTING__STARTED.html#autotoc_md1829", null ]
        ] ],
        [ "First Logger", "md_docs_2GETTING__STARTED.html#autotoc_md1830", null ],
        [ "Writer Composition (Decorator Pattern)", "md_docs_2GETTING__STARTED.html#autotoc_md1831", [
          [ "Console with level filtering", "md_docs_2GETTING__STARTED.html#autotoc_md1832", null ],
          [ "Buffered + async file writer", "md_docs_2GETTING__STARTED.html#autotoc_md1833", null ],
          [ "Production multi-writer setup", "md_docs_2GETTING__STARTED.html#autotoc_md1834", null ]
        ] ],
        [ "Backends", "md_docs_2GETTING__STARTED.html#autotoc_md1835", [
          [ "Custom writers", "md_docs_2GETTING__STARTED.html#autotoc_md1836", null ]
        ] ],
        [ "Structured Logging", "md_docs_2GETTING__STARTED.html#autotoc_md1837", [
          [ "Context propagation", "md_docs_2GETTING__STARTED.html#autotoc_md1838", null ],
          [ "RAII context scopes", "md_docs_2GETTING__STARTED.html#autotoc_md1839", null ],
          [ "Formatters", "md_docs_2GETTING__STARTED.html#autotoc_md1840", null ]
        ] ],
        [ "OpenTelemetry Integration", "md_docs_2GETTING__STARTED.html#autotoc_md1841", null ],
        [ "Configuration Builder", "md_docs_2GETTING__STARTED.html#autotoc_md1842", null ],
        [ "Next Steps", "md_docs_2GETTING__STARTED.html#autotoc_md1843", null ]
      ] ]
    ] ],
    [ "BEST_PRACTICES.kr", "md_docs_2guides_2BEST__PRACTICES_8kr.html", [
      [ "Logger System 모범 사례 가이드", "md_docs_2guides_2BEST__PRACTICES_8kr.html#autotoc_md1846", null ]
    ] ],
    [ "BEST_PRACTICES", "md_docs_2guides_2BEST__PRACTICES.html", [
      [ "Logger System Best Practices Guide", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1851", [
        [ "autotoc_md1849", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1849", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1850", null ],
        [ "Table of Contents", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1852", null ],
        [ "Design Principles", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1853", [
          [ "Single Responsibility", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1854", null ],
          [ "Dependency Injection", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1855", null ],
          [ "Interface Segregation", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1856", null ]
        ] ],
        [ "Configuration Best Practices", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1857", [
          [ "Environment-Specific Configuration", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1858", null ],
          [ "Configuration Validation", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1859", null ],
          [ "Dynamic Reconfiguration", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1860", null ]
        ] ],
        [ "Performance Guidelines", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1861", [
          [ "Minimize Allocations", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1862", null ],
          [ "Lazy Evaluation", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1863", null ],
          [ "Batch Operations", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1864", null ],
          [ "Async Logging Pattern", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1865", null ]
        ] ],
        [ "Error Handling", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1866", [
          [ "Graceful Degradation", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1867", null ],
          [ "Error Recovery", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1868", null ],
          [ "Circuit Breaker Pattern", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1869", null ]
        ] ],
        [ "Security Considerations", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1870", [
          [ "Sanitize Sensitive Data", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1871", null ],
          [ "Access Control", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1872", null ],
          [ "Log Injection Prevention", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1873", null ]
        ] ],
        [ "Testing Strategies", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1874", [
          [ "Mock Logger for Testing", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1875", null ],
          [ "Performance Testing", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1876", null ],
          [ "Stress Testing", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1877", null ]
        ] ],
        [ "Production Deployment", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1878", [
          [ "Monitoring Integration", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1879", null ],
          [ "Log Rotation Strategy", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1880", null ],
          [ "Graceful Shutdown", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1881", null ]
        ] ],
        [ "Common Pitfalls", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1882", [
          [ "Memory Leaks", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1883", null ],
          [ "Deadlocks", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1884", null ],
          [ "Performance Bottlenecks", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1885", null ]
        ] ],
        [ "Summary", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1886", null ],
        [ "Remember: Good logging is invisible when everything works but invaluable when things go wrong.", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1887", null ]
      ] ]
    ] ],
    [ "BUILD", "md_docs_2guides_2BUILD.html", [
      [ "Logger System Build Guide", "md_docs_2guides_2BUILD.html#autotoc_md1890", [
        [ "autotoc_md1888", "md_docs_2guides_2BUILD.html#autotoc_md1888", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2BUILD.html#autotoc_md1889", null ],
        [ "Table of Contents", "md_docs_2guides_2BUILD.html#autotoc_md1892", null ],
        [ "Prerequisites", "md_docs_2guides_2BUILD.html#autotoc_md1894", null ],
        [ "Source Layout", "md_docs_2guides_2BUILD.html#autotoc_md1896", null ],
        [ "Standard Build", "md_docs_2guides_2BUILD.html#autotoc_md1898", null ],
        [ "CMake Presets", "md_docs_2guides_2BUILD.html#autotoc_md1900", null ],
        [ "CMake Options", "md_docs_2guides_2BUILD.html#autotoc_md1902", [
          [ "Core Features", "md_docs_2guides_2BUILD.html#autotoc_md1903", null ],
          [ "Advanced Features", "md_docs_2guides_2BUILD.html#autotoc_md1904", null ],
          [ "Performance Tuning", "md_docs_2guides_2BUILD.html#autotoc_md1905", null ],
          [ "Quality Assurance", "md_docs_2guides_2BUILD.html#autotoc_md1906", null ]
        ] ],
        [ "Optional Features", "md_docs_2guides_2BUILD.html#autotoc_md1908", [
          [ "Encryption (OpenSSL)", "md_docs_2guides_2BUILD.html#autotoc_md1909", null ],
          [ "thread_system Integration", "md_docs_2guides_2BUILD.html#autotoc_md1910", null ],
          [ "OpenTelemetry Export", "md_docs_2guides_2BUILD.html#autotoc_md1911", null ],
          [ "C++20 Modules (Experimental)", "md_docs_2guides_2BUILD.html#autotoc_md1912", null ]
        ] ],
        [ "Installing", "md_docs_2guides_2BUILD.html#autotoc_md1914", null ],
        [ "Consuming from Another Project", "md_docs_2guides_2BUILD.html#autotoc_md1916", [
          [ "With find_package", "md_docs_2guides_2BUILD.html#autotoc_md1917", null ],
          [ "With FetchContent", "md_docs_2guides_2BUILD.html#autotoc_md1918", null ]
        ] ],
        [ "Platform Notes", "md_docs_2guides_2BUILD.html#autotoc_md1920", null ],
        [ "Next Steps", "md_docs_2guides_2BUILD.html#autotoc_md1922", null ]
      ] ]
    ] ],
    [ "DECORATOR_MIGRATION", "md_docs_2guides_2DECORATOR__MIGRATION.html", [
      [ "Decorator Pattern Writer Migration Guide", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1925", [
        [ "autotoc_md1923", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1923", null ],
        [ "category: \"MIGR\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1924", null ],
        [ "Table of Contents", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1926", null ],
        [ "Overview", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1928", [
          [ "Why Decorator Pattern?", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1929", null ],
          [ "Key Components", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1930", null ]
        ] ],
        [ "New Architecture", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1932", [
          [ "Base Classes", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1933", [
            [ "log_writer_interface", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1934", null ],
            [ "decorator_writer_base", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1935", null ]
          ] ]
        ] ],
        [ "Built-in Decorators", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1937", [
          [ "filtered_writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1938", null ],
          [ "buffered_writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1939", null ],
          [ "formatted_writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1940", null ]
        ] ],
        [ "Migrating Custom Writers", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1942", [
          [ "Before (v3.x): Monolithic Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1943", null ],
          [ "After (v4.0): Focused Leaf Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1944", null ],
          [ "Creating a Custom Decorator", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1945", null ]
        ] ],
        [ "Composing Decorators", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1947", [
          [ "Order Matters", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1948", null ],
          [ "Recommended Order", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1949", null ],
          [ "Using writer_builder (Recommended)", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1950", null ],
          [ "Using logger_builder", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1951", null ]
        ] ],
        [ "Deprecation Timeline and Legacy Patterns", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1953", [
          [ "Deprecated Writer Patterns", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1954", null ],
          [ "Timeline", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1955", null ],
          [ "Migration Paths", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1956", [
            [ "Scenario 1: Simple File Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1957", null ],
            [ "Scenario 2: Rotating File Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1958", null ],
            [ "Scenario 3: Async + Encrypted Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1959", null ],
            [ "Scenario 4: Buffered + Filtered Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1960", null ],
            [ "Scenario 5: Multiple Decorators (Production Setup)", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1961", null ],
            [ "Scenario 6: Custom Writer Integration", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1962", null ]
          ] ],
          [ "Support Policy", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1963", null ]
        ] ],
        [ "Best Practices", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1965", [
          [ "Keep Leaf Writers Simple", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1966", null ],
          [ "Use Category Tags", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1967", null ],
          [ "Handle Errors Gracefully", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1968", null ],
          [ "Ensure Thread Safety", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1969", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1971", [
          [ "Common Issues", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1972", [
            [ "\"Writer not receiving entries\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1973", null ],
            [ "\"Entries not being formatted\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1974", null ],
            [ "\"Flush not working\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1975", null ],
            [ "\"Compilation errors with custom writers\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1976", null ],
            [ "\"Performance regression after migration\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1977", null ]
          ] ],
          [ "Migration Checklist", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1978", null ],
          [ "Getting Help", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1979", null ]
        ] ],
        [ "Related Documentation", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1981", null ]
      ] ]
    ] ],
    [ "FAQ", "md_docs_2guides_2FAQ.html", [
      [ "Logger System - Frequently Asked Questions", "md_docs_2guides_2FAQ.html#autotoc_md1985", [
        [ "autotoc_md1983", "md_docs_2guides_2FAQ.html#autotoc_md1983", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2FAQ.html#autotoc_md1984", null ],
        [ "Table of Contents", "md_docs_2guides_2FAQ.html#autotoc_md1987", null ],
        [ "General Questions", "md_docs_2guides_2FAQ.html#autotoc_md1989", [
          [ "What is the logger_system?", "md_docs_2guides_2FAQ.html#autotoc_md1990", null ],
          [ "What C++ standard is required?", "md_docs_2guides_2FAQ.html#autotoc_md1992", null ]
        ] ],
        [ "Logging Basics", "md_docs_2guides_2FAQ.html#autotoc_md1994", [
          [ "How do I create a logger?", "md_docs_2guides_2FAQ.html#autotoc_md1995", null ],
          [ "How do I log messages?", "md_docs_2guides_2FAQ.html#autotoc_md1997", null ]
        ] ],
        [ "Log Levels", "md_docs_2guides_2FAQ.html#autotoc_md1999", [
          [ "What log levels are available?", "md_docs_2guides_2FAQ.html#autotoc_md2000", null ],
          [ "How do I set the log level?", "md_docs_2guides_2FAQ.html#autotoc_md2002", null ],
          [ "Can I change log level at runtime?", "md_docs_2guides_2FAQ.html#autotoc_md2004", null ]
        ] ],
        [ "Writers", "md_docs_2guides_2FAQ.html#autotoc_md2006", [
          [ "What writers are available?", "md_docs_2guides_2FAQ.html#autotoc_md2007", null ],
          [ "How do I use multiple writers?", "md_docs_2guides_2FAQ.html#autotoc_md2009", null ],
          [ "How does log rotation work?", "md_docs_2guides_2FAQ.html#autotoc_md2011", null ]
        ] ],
        [ "Performance", "md_docs_2guides_2FAQ.html#autotoc_md2013", [
          [ "What is the performance?", "md_docs_2guides_2FAQ.html#autotoc_md2014", null ],
          [ "How do I enable asynchronous logging?", "md_docs_2guides_2FAQ.html#autotoc_md2016", null ],
          [ "How do I optimize performance?", "md_docs_2guides_2FAQ.html#autotoc_md2018", null ]
        ] ],
        [ "Integration", "md_docs_2guides_2FAQ.html#autotoc_md2020", [
          [ "How do I integrate with monitoring_system?", "md_docs_2guides_2FAQ.html#autotoc_md2021", null ],
          [ "How do I integrate with thread_system?", "md_docs_2guides_2FAQ.html#autotoc_md2023", null ]
        ] ],
        [ "Advanced Topics", "md_docs_2guides_2FAQ.html#autotoc_md2025", [
          [ "Is it thread-safe?", "md_docs_2guides_2FAQ.html#autotoc_md2026", null ],
          [ "How do I create custom log formats?", "md_docs_2guides_2FAQ.html#autotoc_md2028", null ],
          [ "How do I filter logs?", "md_docs_2guides_2FAQ.html#autotoc_md2030", null ],
          [ "How do I handle critical logs?", "md_docs_2guides_2FAQ.html#autotoc_md2032", null ],
          [ "Where can I find more information?", "md_docs_2guides_2FAQ.html#autotoc_md2034", null ]
        ] ]
      ] ]
    ] ],
    [ "GETTING_STARTED.kr", "md_docs_2guides_2GETTING__STARTED_8kr.html", [
      [ "Logger System 시작하기", "md_docs_2guides_2GETTING__STARTED_8kr.html#autotoc_md2038", null ]
    ] ],
    [ "GETTING_STARTED", "md_docs_2guides_2GETTING__STARTED.html", [
      [ "Getting Started with Logger System", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2043", [
        [ "autotoc_md2041", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2041", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2042", null ],
        [ "Table of Contents", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2044", null ],
        [ "Requirements", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2045", null ],
        [ "Installation", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2046", [
          [ "Using CMake FetchContent (Recommended)", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2047", null ],
          [ "Building from Source", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2048", null ],
          [ "Using as Installed Package", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2049", null ]
        ] ],
        [ "Basic Usage", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2050", [
          [ "Simple Console Logging", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2051", null ],
          [ "Logging with Source Location", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2052", null ],
          [ "Structured JSON Output", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2053", null ],
          [ "Log Levels", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2054", null ],
          [ "Filtering by Level", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2055", null ]
        ] ],
        [ "Configuration", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2056", [
          [ "Synchronous vs Asynchronous Logging", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2057", null ],
          [ "Multiple Writers", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2058", null ],
          [ "Windows Notes (Networking)", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2059", null ],
          [ "Console Writer Options", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2060", null ]
        ] ],
        [ "Integration with Thread System", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2061", [
          [ "Using Service Container", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2062", null ],
          [ "Direct Integration", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2063", null ]
        ] ],
        [ "Next Steps", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md2064", null ]
      ] ]
    ] ],
    [ "INTEGRATION", "md_docs_2guides_2INTEGRATION.html", [
      [ "Logger System Integration Guide", "md_docs_2guides_2INTEGRATION.html#autotoc_md2068", [
        [ "autotoc_md2066", "md_docs_2guides_2INTEGRATION.html#autotoc_md2066", null ],
        [ "category: \"INTR\"", "md_docs_2guides_2INTEGRATION.html#autotoc_md2067", null ],
        [ "Table of Contents", "md_docs_2guides_2INTEGRATION.html#autotoc_md2070", null ],
        [ "Overview", "md_docs_2guides_2INTEGRATION.html#autotoc_md2071", null ],
        [ "System Dependencies", "md_docs_2guides_2INTEGRATION.html#autotoc_md2072", [
          [ "Required Dependencies", "md_docs_2guides_2INTEGRATION.html#autotoc_md2073", [
            [ "common_system (Header-only)", "md_docs_2guides_2INTEGRATION.html#autotoc_md2074", null ]
          ] ],
          [ "Optional Dependencies", "md_docs_2guides_2INTEGRATION.html#autotoc_md2075", [
            [ "thread_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2076", null ],
            [ "monitoring_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2077", null ]
          ] ]
        ] ],
        [ "Integration with common_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2078", [
          [ "ILogger Interface Implementation", "md_docs_2guides_2INTEGRATION.html#autotoc_md2079", null ],
          [ "Result<T> Pattern Usage", "md_docs_2guides_2INTEGRATION.html#autotoc_md2080", null ],
          [ "Adapter Pattern", "md_docs_2guides_2INTEGRATION.html#autotoc_md2081", null ]
        ] ],
        [ "Integration with thread_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2082", [
          [ "Asynchronous Logging with IExecutor", "md_docs_2guides_2INTEGRATION.html#autotoc_md2083", null ],
          [ "Cross-System Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2084", null ]
        ] ],
        [ "Integration with monitoring_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2085", [
          [ "IMonitor Interface Injection", "md_docs_2guides_2INTEGRATION.html#autotoc_md2086", null ],
          [ "Performance Metrics Collection", "md_docs_2guides_2INTEGRATION.html#autotoc_md2087", null ],
          [ "Health Monitoring Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2088", null ]
        ] ],
        [ "Build Configuration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2089", [
          [ "CMake Integration Examples", "md_docs_2guides_2INTEGRATION.html#autotoc_md2090", [
            [ "Minimal Configuration (common_system only)", "md_docs_2guides_2INTEGRATION.html#autotoc_md2091", null ],
            [ "Full Ecosystem Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2092", null ]
          ] ],
          [ "Build Configuration Flags", "md_docs_2guides_2INTEGRATION.html#autotoc_md2093", null ],
          [ "vcpkg Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2094", null ]
        ] ],
        [ "Code Examples", "md_docs_2guides_2INTEGRATION.html#autotoc_md2095", [
          [ "Example 1: Basic Integration with common_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2096", null ],
          [ "Example 2: Full Stack Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2097", null ],
          [ "Example 3: Error Handling with Result<T>", "md_docs_2guides_2INTEGRATION.html#autotoc_md2098", null ]
        ] ],
        [ "Result Handling Cheatsheet", "md_docs_2guides_2INTEGRATION.html#autotoc_md2099", null ],
        [ "Error Code Registry", "md_docs_2guides_2INTEGRATION.html#autotoc_md2100", [
          [ "Allocated Error Code Range", "md_docs_2guides_2INTEGRATION.html#autotoc_md2101", null ],
          [ "Error Message Mapping", "md_docs_2guides_2INTEGRATION.html#autotoc_md2102", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2guides_2INTEGRATION.html#autotoc_md2103", [
          [ "Build Fails with \"common_system not found\"", "md_docs_2guides_2INTEGRATION.html#autotoc_md2104", null ],
          [ "Linker Errors: Undefined References", "md_docs_2guides_2INTEGRATION.html#autotoc_md2105", null ],
          [ "Runtime: Logger Not Outputting", "md_docs_2guides_2INTEGRATION.html#autotoc_md2106", null ],
          [ "Integration: IMonitor Not Injecting", "md_docs_2guides_2INTEGRATION.html#autotoc_md2107", null ],
          [ "Performance: High Latency", "md_docs_2guides_2INTEGRATION.html#autotoc_md2108", null ]
        ] ],
        [ "Best Practices", "md_docs_2guides_2INTEGRATION.html#autotoc_md2109", [
          [ "Use Result<T> for Error Handling", "md_docs_2guides_2INTEGRATION.html#autotoc_md2110", null ],
          [ "Prefer Interface-Based Design", "md_docs_2guides_2INTEGRATION.html#autotoc_md2111", null ],
          [ "Leverage DI for Testing", "md_docs_2guides_2INTEGRATION.html#autotoc_md2112", null ],
          [ "Monitor Performance in Production", "md_docs_2guides_2INTEGRATION.html#autotoc_md2113", null ]
        ] ],
        [ "References", "md_docs_2guides_2INTEGRATION.html#autotoc_md2114", null ]
      ] ]
    ] ],
    [ "MIGRATION_GUIDE.kr", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html", [
      [ "로거 시스템 마이그레이션 가이드", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2117", [
        [ "autotoc_md2115", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2115", null ],
        [ "category: \"MIGR\"", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2116", null ],
        [ "목차", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2118", null ],
        [ "개요", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2119", null ],
        [ "CMake 구성 변경", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2120", [
          [ "v2.x (이전)", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2121", null ],
          [ "v3.0 (현재)", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2122", null ],
          [ "CMake 플래그 변경 요약", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2123", null ],
          [ "타겟명 변경", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2124", null ],
          [ "의존성 변경", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2125", null ]
        ] ],
        [ "v2.x에서 v3.0으로 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2126", [
          [ "주요 변경사항 요약", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2127", null ],
          [ "네임스페이스 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2128", null ],
          [ "인터페이스 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2129", null ],
          [ "Result 타입 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2130", null ],
          [ "thread_system 의존성 변경", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2131", null ],
          [ "백엔드 선택 (v3.0 신규)", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2132", null ],
          [ "C++20 source_location (v3.0 신규)", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2133", null ]
        ] ],
        [ "마이그레이션 체크리스트", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2134", [
          [ "v2.x → v3.0 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2135", null ]
        ] ],
        [ "일반적인 문제와 해결책", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2136", [
          [ "문제 1: 네임스페이스를 찾을 수 없음", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2137", null ],
          [ "문제 2: 인터페이스 타입 불일치", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2138", null ],
          [ "문제 3: C++20 미활성화", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2139", null ]
        ] ],
        [ "지원 및 리소스", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2140", null ]
      ] ]
    ] ],
    [ "MIGRATION_GUIDE", "md_docs_2guides_2MIGRATION__GUIDE.html", [
      [ "Logger System Migration Guide", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2145", [
        [ "autotoc_md2143", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2143", null ],
        [ "category: \"MIGR\"", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2144", null ],
        [ "Table of Contents", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2146", null ],
        [ "Overview", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2147", [
          [ "Breaking Changes Summary", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2148", null ]
        ] ],
        [ "From v3.x to v4.0", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2150", [
          [ "What Changed", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2151", null ],
          [ "Migration for Standard Users", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2152", null ],
          [ "Migration for Custom Writer Authors", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2153", null ],
          [ "Context ID API Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2154", null ],
          [ "Unified Context API (New in v4.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2155", [
            [ "Why This Change?", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2156", null ],
            [ "Migration Table", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2157", null ],
            [ "Context Categories", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2158", null ],
            [ "Code Examples", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2159", null ],
            [ "Thread Safety", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2160", null ],
            [ "Migration Script", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2161", null ],
            [ "Deprecation Timeline", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2162", null ]
          ] ]
        ] ],
        [ "CMake Configuration Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2164", [
          [ "v2.x (Previous)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2165", null ],
          [ "v3.0 (Current)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2166", null ],
          [ "CMake Flag Changes Summary", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2167", null ],
          [ "Target Name Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2168", null ],
          [ "Dependency Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2169", null ]
        ] ],
        [ "Version Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2170", [
          [ "From v2.x to v3.0", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2171", [
            [ "Summary of Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2172", null ],
            [ "Namespace Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2173", null ],
            [ "Interface Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2174", null ],
            [ "Dual API Support", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2175", null ],
            [ "Result Type Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2176", null ],
            [ "thread_system Dependency Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2177", null ],
            [ "Backend Selection (New in v3.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2178", null ],
            [ "C++20 Source Location", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2179", null ],
            [ "Configuration Strategies (Enhanced in v3.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2180", null ],
            [ "Monitoring Integration (New in v3.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2181", null ],
            [ "Migration Script", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2182", null ]
          ] ],
          [ "From v1.x to v2.x", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2184", [
            [ "Error Handling Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2185", null ],
            [ "Memory Management Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2186", null ],
            [ "Configuration Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2187", null ],
            [ "Logging API Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2188", null ]
          ] ]
        ] ],
        [ "API Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2189", [
          [ "v3.0 API Changes Summary", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2190", null ],
          [ "Deprecated Native log_level API (Planned for Removal in v3.0.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2191", null ],
          [ "Core Logger API", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2192", null ]
        ] ],
        [ "Configuration Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2193", [
          [ "From INI/XML Configuration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2194", null ],
          [ "Environment-based Configuration (v3.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2195", null ]
        ] ],
        [ "Migration from Other Libraries", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2196", [
          [ "From spdlog", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2197", null ],
          [ "From Boost.Log", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2198", null ],
          [ "From Google glog", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2199", null ]
        ] ],
        [ "Compatibility Wrappers", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2200", [
          [ "v2.x to v3.0 Compatibility Header", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2201", null ],
          [ "Legacy API Wrapper (v1.x compatibility)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2202", null ]
        ] ],
        [ "Step-by-Step Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2203", [
          [ "Phase 1: Preparation", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2204", null ],
          [ "Phase 2: Namespace Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2205", null ],
          [ "Phase 3: Interface Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2206", null ],
          [ "Phase 4: Validation", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2207", null ]
        ] ],
        [ "Common Issues and Solutions", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2208", [
          [ "Issue 1: Namespace Not Found", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2209", null ],
          [ "Issue 2: Interface Type Mismatch", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2210", null ],
          [ "Issue 3: Result Type Mismatch", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2211", null ],
          [ "Issue 4: Missing thread_system", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2212", null ],
          [ "Issue 5: C++20 Not Enabled", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2213", null ]
        ] ],
        [ "Migration Checklist", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2214", [
          [ "v2.x to v3.0 Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2215", null ],
          [ "v1.x to v2.x Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2216", null ]
        ] ],
        [ "Support and Resources", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2217", null ]
      ] ]
    ] ],
    [ "OPENTELEMETRY", "md_docs_2guides_2OPENTELEMETRY.html", [
      [ "OpenTelemetry Integration Guide", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2221", [
        [ "autotoc_md2219", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2219", null ],
        [ "category: \"INTR\"", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2220", null ],
        [ "Table of Contents", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2222", null ],
        [ "Overview", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2223", null ],
        [ "Installation", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2224", [
          [ "With vcpkg", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2225", null ],
          [ "With CMake", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2226", null ],
          [ "Dependencies", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2227", null ]
        ] ],
        [ "Quick Start", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2228", [
          [ "Basic OTLP Export", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2229", null ],
          [ "With Trace Correlation", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2230", null ],
          [ "Using RAII Scope Guard", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2231", null ]
        ] ],
        [ "Configuration", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2232", [
          [ "OTLP Writer Configuration", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2233", null ],
          [ "Protocol Selection", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2234", null ]
        ] ],
        [ "Trace Context", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2235", [
          [ "otel_context Structure", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2236", null ],
          [ "Thread-Local Storage", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2237", null ],
          [ "Context in Log Entries", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2238", null ]
        ] ],
        [ "OTLP Writer", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2239", [
          [ "Statistics Monitoring", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2240", null ],
          [ "Health Checking", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2241", null ],
          [ "Force Flush", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2242", null ]
        ] ],
        [ "Integration Examples", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2243", [
          [ "With OpenTelemetry Collector", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2244", null ],
          [ "Docker Compose Setup", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2245", null ],
          [ "W3C Trace Context Parsing", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2246", null ]
        ] ],
        [ "Best Practices", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2247", [
          [ "Always Set Service Name", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2248", null ],
          [ "Use RAII Scope Guards", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2249", null ],
          [ "Configure Appropriate Batch Sizes", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2250", null ],
          [ "Handle Collector Unavailability", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2251", null ],
          [ "Include Meaningful Resource Attributes", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2252", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2253", [
          [ "Logs Not Appearing in Collector", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2254", null ],
          [ "High Log Drop Rate", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2255", null ],
          [ "Connection Timeouts", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2256", null ],
          [ "Missing Trace Context", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2257", null ]
        ] ],
        [ "Related Documentation", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2258", null ]
      ] ]
    ] ],
    [ "PERFORMANCE.kr", "md_docs_2guides_2PERFORMANCE_8kr.html", [
      [ "Logger System 성능 가이드", "md_docs_2guides_2PERFORMANCE_8kr.html#autotoc_md2261", null ]
    ] ],
    [ "PERFORMANCE", "md_docs_2guides_2PERFORMANCE.html", [
      [ "Logger System Performance Guide", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2266", [
        [ "autotoc_md2264", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2264", null ],
        [ "category: \"PERF\"", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2265", null ],
        [ "Overview", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2267", null ],
        [ "Performance Characteristics", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2268", [
          [ "Synchronous Mode", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2269", null ],
          [ "Asynchronous Mode", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2270", null ]
        ] ],
        [ "Benchmarks", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2271", [
          [ "Test Environment", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2272", null ],
          [ "Single Thread Performance", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2273", null ],
          [ "Multi-threaded Performance", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2274", null ],
          [ "Memory Usage", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2275", null ]
        ] ],
        [ "Optimization Strategies", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2276", [
          [ "Choose the Right Mode", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2277", null ],
          [ "Buffer Size Tuning", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2278", null ],
          [ "2.1 Batch Size Considerations", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2279", null ],
          [ "Level Filtering", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2280", null ],
          [ "Message Construction", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2281", null ],
          [ "Writer Optimization", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2282", [
            [ "Console Writer", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2283", null ],
            [ "Custom High-Performance Writer", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2284", null ]
          ] ],
          [ "Batch Processing", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2285", null ]
        ] ],
        [ "Performance Anti-patterns", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2286", [
          [ "Synchronous I/O in Hot Paths", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2287", null ],
          [ "Excessive String Formatting", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2288", null ],
          [ "Logging in Tight Loops", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2289", null ]
        ] ],
        [ "Profiling and Monitoring", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2290", [
          [ "Built-in Metrics", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2291", null ],
          [ "External Profiling", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2292", null ]
        ] ],
        [ "Best Practices Summary", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2293", null ],
        [ "Platform-Specific Optimizations", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2294", [
          [ "Linux", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2295", null ],
          [ "Windows", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2296", null ],
          [ "macOS", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2297", null ]
        ] ]
      ] ]
    ] ],
    [ "QUICK_START.kr", "md_docs_2guides_2QUICK__START_8kr.html", [
      [ "빠른 시작 가이드", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2301", [
        [ "autotoc_md2299", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2299", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2300", null ],
        [ "사전 요구사항", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2303", null ],
        [ "설치", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2304", [
          [ "저장소 클론", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2305", null ],
          [ "의존성 설치", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2306", null ],
          [ "빌드", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2307", null ],
          [ "설치 확인", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2308", null ]
        ] ],
        [ "첫 번째 로거 만들기", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2310", [
          [ "애플리케이션 빌드", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2311", null ]
        ] ],
        [ "핵심 개념", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2313", [
          [ "로거 빌더", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2314", null ],
          [ "라이터", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2315", null ],
          [ "로그 레벨", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2316", null ]
        ] ],
        [ "구성 템플릿", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2318", [
          [ "Production (프로덕션)", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2319", null ],
          [ "Debug (디버그)", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2320", null ],
          [ "High Performance (고성능)", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2321", null ]
        ] ],
        [ "CMake 구성", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2323", [
          [ "핵심 빌드 옵션", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2324", null ],
          [ "기능 플래그", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2325", null ]
        ] ],
        [ "thread_system 통합 (선택 사항)", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2327", [
          [ "빌드 시 활성화", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2328", null ],
          [ "런타임 활성화", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2329", null ]
        ] ],
        [ "일반적인 패턴", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2331", [
          [ "오류 처리", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2332", null ],
          [ "여러 라이터 사용", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2333", null ],
          [ "정상 종료", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2334", null ]
        ] ],
        [ "다음 단계", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2336", null ],
        [ "문제 해결", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2338", [
          [ "일반적인 문제", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2339", null ]
        ] ]
      ] ]
    ] ],
    [ "QUICK_START", "md_docs_2guides_2QUICK__START.html", [
      [ "Quick Start Guide", "md_docs_2guides_2QUICK__START.html#autotoc_md2343", [
        [ "autotoc_md2341", "md_docs_2guides_2QUICK__START.html#autotoc_md2341", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2QUICK__START.html#autotoc_md2342", null ],
        [ "Prerequisites", "md_docs_2guides_2QUICK__START.html#autotoc_md2345", null ],
        [ "Installation", "md_docs_2guides_2QUICK__START.html#autotoc_md2346", [
          [ "Clone the Repositories", "md_docs_2guides_2QUICK__START.html#autotoc_md2347", null ],
          [ "Install Dependencies", "md_docs_2guides_2QUICK__START.html#autotoc_md2348", null ],
          [ "Build", "md_docs_2guides_2QUICK__START.html#autotoc_md2349", null ],
          [ "Verify Installation", "md_docs_2guides_2QUICK__START.html#autotoc_md2350", null ]
        ] ],
        [ "Your First Logger", "md_docs_2guides_2QUICK__START.html#autotoc_md2352", [
          [ "Building Your Application", "md_docs_2guides_2QUICK__START.html#autotoc_md2353", null ]
        ] ],
        [ "Key Concepts", "md_docs_2guides_2QUICK__START.html#autotoc_md2355", [
          [ "Logger Builder", "md_docs_2guides_2QUICK__START.html#autotoc_md2356", null ],
          [ "Writers", "md_docs_2guides_2QUICK__START.html#autotoc_md2357", null ],
          [ "Log Levels", "md_docs_2guides_2QUICK__START.html#autotoc_md2358", null ]
        ] ],
        [ "Configuration Templates", "md_docs_2guides_2QUICK__START.html#autotoc_md2360", [
          [ "Production", "md_docs_2guides_2QUICK__START.html#autotoc_md2361", null ],
          [ "Debug", "md_docs_2guides_2QUICK__START.html#autotoc_md2362", null ],
          [ "High Performance", "md_docs_2guides_2QUICK__START.html#autotoc_md2363", null ]
        ] ],
        [ "CMake Configuration", "md_docs_2guides_2QUICK__START.html#autotoc_md2365", [
          [ "Core Build Options", "md_docs_2guides_2QUICK__START.html#autotoc_md2366", null ],
          [ "Feature Flags", "md_docs_2guides_2QUICK__START.html#autotoc_md2367", null ]
        ] ],
        [ "thread_system Integration (Optional)", "md_docs_2guides_2QUICK__START.html#autotoc_md2369", [
          [ "Enable at Build Time", "md_docs_2guides_2QUICK__START.html#autotoc_md2370", null ],
          [ "Enable at Runtime", "md_docs_2guides_2QUICK__START.html#autotoc_md2371", null ]
        ] ],
        [ "Common Patterns", "md_docs_2guides_2QUICK__START.html#autotoc_md2373", [
          [ "Error Handling", "md_docs_2guides_2QUICK__START.html#autotoc_md2374", null ],
          [ "Multiple Writers", "md_docs_2guides_2QUICK__START.html#autotoc_md2375", null ],
          [ "Graceful Shutdown", "md_docs_2guides_2QUICK__START.html#autotoc_md2376", null ]
        ] ],
        [ "Next Steps", "md_docs_2guides_2QUICK__START.html#autotoc_md2378", null ],
        [ "Troubleshooting", "md_docs_2guides_2QUICK__START.html#autotoc_md2380", [
          [ "Common Issues", "md_docs_2guides_2QUICK__START.html#autotoc_md2381", null ]
        ] ]
      ] ]
    ] ],
    [ "SECURITY.kr", "md_docs_2guides_2SECURITY_8kr.html", [
      [ "보안 가이드", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2385", [
        [ "autotoc_md2383", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2383", null ],
        [ "category: \"SECU\"", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2384", null ],
        [ "위협 모델 (범위)", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2386", null ],
        [ "현재 기능", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2387", null ],
        [ "중요한 제한사항", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2388", null ],
        [ "권장사항 (프로덕션)", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2389", null ],
        [ "안전한 구성 체크리스트", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2390", null ],
        [ "로드맵", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2391", null ]
      ] ]
    ] ],
    [ "SECURITY", "md_docs_2guides_2SECURITY.html", [
      [ "Security Guide", "md_docs_2guides_2SECURITY.html#autotoc_md2395", [
        [ "autotoc_md2393", "md_docs_2guides_2SECURITY.html#autotoc_md2393", null ],
        [ "category: \"SECU\"", "md_docs_2guides_2SECURITY.html#autotoc_md2394", null ],
        [ "Threat Model (Scope)", "md_docs_2guides_2SECURITY.html#autotoc_md2396", null ],
        [ "Current Capabilities", "md_docs_2guides_2SECURITY.html#autotoc_md2397", null ],
        [ "Important Limitations", "md_docs_2guides_2SECURITY.html#autotoc_md2398", null ],
        [ "Recommendations (Production)", "md_docs_2guides_2SECURITY.html#autotoc_md2399", null ],
        [ "Secure Configuration Checklist", "md_docs_2guides_2SECURITY.html#autotoc_md2400", null ],
        [ "Roadmap", "md_docs_2guides_2SECURITY.html#autotoc_md2401", null ]
      ] ]
    ] ],
    [ "TROUBLESHOOTING", "md_docs_2guides_2TROUBLESHOOTING.html", [
      [ "Logger System Troubleshooting Guide", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2405", [
        [ "autotoc_md2403", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2403", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2404", null ],
        [ "Table of Contents", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2407", null ],
        [ "Build Errors", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2409", [
          [ "common_system not found", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2410", null ],
          [ "C++20 feature not available", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2411", null ],
          [ "OpenSSL headers missing", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2412", null ],
          [ "opentelemetry-cpp not found", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2413", null ],
          [ "C++20 modules build fails", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2414", null ]
        ] ],
        [ "Linker Errors", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2416", [
          [ "Undefined references to kcenon::logger::logger_builder", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2417", null ],
          [ "Multiple definitions when mixing static and shared builds", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2418", null ],
          [ "Missing OpenSSL symbols at link time", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2419", null ]
        ] ],
        [ "Runtime Issues", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2421", [
          [ "logger_builder::build() returns an error", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2422", null ],
          [ "No output appears in the console", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2423", null ],
          [ "File writer produces an empty or truncated file", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2424", null ],
          [ "Messages are lost under high load", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2425", null ],
          [ "Crash or deadlock on program exit", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2426", null ]
        ] ],
        [ "Performance Issues", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2428", null ],
        [ "Integration Issues", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2430", [
          [ "thread_system pool not used", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2431", null ],
          [ "OTLP exporter fails with connection refused", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2432", null ]
        ] ],
        [ "Platform-Specific Issues", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2434", [
          [ "MinGW", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2435", null ],
          [ "macOS: std::format unavailable", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2436", null ],
          [ "Windows: dll not found at runtime", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2437", null ]
        ] ],
        [ "Getting More Help", "md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2439", null ]
      ] ]
    ] ],
    [ "README", "md_docs_2integration_2README.html", [
      [ "Logger System Integration Guide", "md_docs_2integration_2README.html#autotoc_md2442", [
        [ "autotoc_md2440", "md_docs_2integration_2README.html#autotoc_md2440", null ],
        [ "category: \"GUID\"", "md_docs_2integration_2README.html#autotoc_md2441", null ],
        [ "Overview", "md_docs_2integration_2README.html#autotoc_md2443", null ],
        [ "Integration Guides", "md_docs_2integration_2README.html#autotoc_md2444", null ],
        [ "Quick Start", "md_docs_2integration_2README.html#autotoc_md2445", [
          [ "Basic Logging", "md_docs_2integration_2README.html#autotoc_md2446", null ],
          [ "Structured Logging", "md_docs_2integration_2README.html#autotoc_md2447", null ],
          [ "Async Logging for High Performance", "md_docs_2integration_2README.html#autotoc_md2448", null ]
        ] ],
        [ "Integration Patterns", "md_docs_2integration_2README.html#autotoc_md2449", [
          [ "Dependency Injection", "md_docs_2integration_2README.html#autotoc_md2450", null ],
          [ "Multiple Loggers", "md_docs_2integration_2README.html#autotoc_md2451", null ]
        ] ],
        [ "Common Use Cases", "md_docs_2integration_2README.html#autotoc_md2452", [
          [ "Request/Response Logging", "md_docs_2integration_2README.html#autotoc_md2453", null ],
          [ "Log Rotation", "md_docs_2integration_2README.html#autotoc_md2454", null ],
          [ "Contextual Logging", "md_docs_2integration_2README.html#autotoc_md2455", null ]
        ] ],
        [ "Best Practices", "md_docs_2integration_2README.html#autotoc_md2456", null ],
        [ "Additional Resources", "md_docs_2integration_2README.html#autotoc_md2457", null ]
      ] ]
    ] ],
    [ "THREAD_SYSTEM.kr", "md_docs_2integration_2THREAD__SYSTEM_8kr.html", [
      [ "thread_system 통합 가이드", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2460", [
        [ "autotoc_md2458", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2458", null ],
        [ "category: \"GUID\"", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2459", null ],
        [ "개요", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2461", null ],
        [ "빠른 시작", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2462", [
          [ "빌드 시 활성화", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2463", null ],
          [ "런타임에 활성화", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2464", null ]
        ] ],
        [ "설정", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2465", [
          [ "CMake 옵션", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2466", null ],
          [ "컴파일 정의", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2467", null ]
        ] ],
        [ "의존성 구성", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2468", [
          [ "양방향 의존성 위험 (Issue #252)", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2469", null ],
          [ "권장 구성 매트릭스", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2470", null ],
          [ "안전한 구성 예제", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2471", null ],
          [ "CMake 충돌 감지", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2472", null ]
        ] ],
        [ "API 레퍼런스", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2473", [
          [ "thread_system_integration", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2474", [
            [ "메서드", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2475", null ]
          ] ],
          [ "async_backend_type", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2476", null ],
          [ "컴파일 타임 감지", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2477", null ]
        ] ],
        [ "사용 패턴", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2478", [
          [ "패턴 1: 애플리케이션 전체 공유 풀", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2479", null ],
          [ "패턴 2: 사용자 정의 풀 설정", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2480", null ],
          [ "패턴 3: 조건부 통합", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2481", null ]
        ] ],
        [ "스레드 안전성", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2482", null ],
        [ "성능 고려사항", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2483", [
          [ "thread_system 통합 사용 시기", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2484", null ],
          [ "오버헤드 비교", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2485", null ]
        ] ],
        [ "문제 해결", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2486", [
          [ "thread_system 미발견", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2487", null ],
          [ "런타임 감지 미작동", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2488", null ]
        ] ],
        [ "관련 문서", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2489", null ]
      ] ]
    ] ],
    [ "THREAD_SYSTEM", "md_docs_2integration_2THREAD__SYSTEM.html", [
      [ "Async Integration Guide", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2493", [
        [ "autotoc_md2491", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2491", null ],
        [ "category: \"GUID\"", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2492", null ],
        [ "Overview", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2494", null ],
        [ "Integration Approaches", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2495", [
          [ "Recommended: IExecutor Interface (v1.5.0+)", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2496", null ],
          [ "Legacy: Direct thread_system Integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2497", null ]
        ] ],
        [ "Quick Start", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2498", [
          [ "Enable at Build Time", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2499", null ],
          [ "Enable at Runtime", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2500", null ]
        ] ],
        [ "Configuration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2501", [
          [ "CMake Options", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2502", null ],
          [ "Compile Definitions", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2503", null ]
        ] ],
        [ "Dependency Configuration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2504", [
          [ "Bidirectional Dependency Risk (Issue #252 — Resolved)", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2505", null ],
          [ "Recommended Configuration Matrix", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2506", null ],
          [ "Safe Configuration Examples", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2507", null ],
          [ "CMake Conflict Detection", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2508", null ]
        ] ],
        [ "API Reference", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2509", [
          [ "thread_system_integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2510", [
            [ "Methods", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2511", null ]
          ] ],
          [ "async_backend_type", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2512", null ],
          [ "Compile-time Detection", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2513", null ]
        ] ],
        [ "Usage Patterns", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2514", [
          [ "Pattern 1: Application-wide Shared Pool", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2515", null ],
          [ "Pattern 2: Custom Pool Configuration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2516", null ],
          [ "Pattern 3: Conditional Integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2517", null ]
        ] ],
        [ "Thread Safety", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2518", null ],
        [ "Performance Considerations", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2519", [
          [ "When to Use thread_system Integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2520", null ],
          [ "Overhead Comparison", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2521", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2522", [
          [ "thread_system not found", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2523", null ],
          [ "Runtime detection not working", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2524", null ]
        ] ],
        [ "IExecutor Integration API (v1.5.0+)", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2525", [
          [ "executor_integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2526", null ],
          [ "executor_type", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2527", null ],
          [ "standalone_executor", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2528", null ],
          [ "Compile-time Detection", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2529", null ]
        ] ],
        [ "Migration from thread_system_integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2530", null ],
        [ "Related Documentation", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2531", null ]
      ] ]
    ] ],
    [ "LOG_SERVER_AND_CRASH_SAFETY", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html", [
      [ "Log Server and Crash-Safe Logger", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2535", [
        [ "autotoc_md2533", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2533", null ],
        [ "category: \"GUID\"", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2534", null ],
        [ "Sub-Documents", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2536", null ],
        [ "Overview", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2537", null ],
        [ "Quick Links", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2538", [
          [ "Log Server", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2539", null ],
          [ "Crash-Safe Logger", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2540", null ],
          [ "Combined Usage", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2541", null ]
        ] ],
        [ "Related Documentation", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2542", null ]
      ] ]
    ] ],
    [ "LOG_SERVER_GUIDE", "md_docs_2LOG__SERVER__GUIDE.html", [
      [ "Log Server Guide", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2546", [
        [ "autotoc_md2544", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2544", null ],
        [ "category: \"GUID\"", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2545", null ],
        [ "Table of Contents", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2547", null ],
        [ "Overview", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2549", null ],
        [ "Architecture", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2551", [
          [ "Components", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2552", null ],
          [ "Role in Distributed Logging", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2553", null ],
          [ "Protocol", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2554", null ]
        ] ],
        [ "Configuration", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2556", [
          [ "server_config Structure", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2557", null ],
          [ "Configuration Examples", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2558", null ]
        ] ],
        [ "API Reference", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2560", [
          [ "log_server Class", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2561", null ],
          [ "log_server_factory Class", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2562", null ],
          [ "Method Details", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2563", [
            [ "start()", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2564", null ],
            [ "stop()", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2565", null ],
            [ "is_running()", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2566", null ]
          ] ]
        ] ],
        [ "Deployment Patterns", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2568", [
          [ "Pattern 1: Single Server with Multiple Clients", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2569", null ],
          [ "Pattern 2: Log Aggregation for ELK Stack", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2570", null ],
          [ "Pattern 3: Multi-Region Deployment", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2571", null ]
        ] ],
        [ "Integration Examples", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2573", [
          [ "Example 1: Basic Server Setup", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2574", null ],
          [ "Example 2: Programmatic Control", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2575", null ],
          [ "Example 3: Docker Deployment", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2576", null ]
        ] ],
        [ "Best Practices", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2578", [
          [ "Capacity Planning", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2579", null ],
          [ "Network Security", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2580", null ],
          [ "Storage Management", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2581", null ],
          [ "Monitoring", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2582", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2584", [
          [ "Problem: Server fails to start", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2585", null ],
          [ "Problem: Clients cannot connect", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2586", null ]
        ] ],
        [ "Related Documentation", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2588", [
          [ "Header Files", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2589", null ],
          [ "External Resources", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2590", null ]
        ] ]
      ] ]
    ] ],
    [ "BASELINE.kr", "md_docs_2performance_2BASELINE_8kr.html", [
      [ "Logger System - 성능 기준 메트릭", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2594", [
        [ "autotoc_md2592", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2592", null ],
        [ "category: \"PERF\"", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2593", null ],
        [ "시스템 정보", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2597", [
          [ "하드웨어 구성", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2598", null ],
          [ "소프트웨어 구성", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2599", null ]
        ] ],
        [ "성능 메트릭", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2601", [
          [ "로깅 Throughput", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2602", null ],
          [ "Latency (Async Mode)", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2603", null ],
          [ "메모리 성능", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2604", null ]
        ] ],
        [ "벤치마크 결과", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2606", null ],
        [ "주요 특징", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2608", null ],
        [ "Baseline 검증", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2610", [
          [ "Phase 0 요구사항", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2611", null ],
          [ "수락 기준", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2612", null ]
        ] ]
      ] ]
    ] ],
    [ "BASELINE", "md_docs_2performance_2BASELINE.html", [
      [ "Logger System - Performance Baseline Metrics", "md_docs_2performance_2BASELINE.html#autotoc_md2616", [
        [ "autotoc_md2614", "md_docs_2performance_2BASELINE.html#autotoc_md2614", null ],
        [ "category: \"PERF\"", "md_docs_2performance_2BASELINE.html#autotoc_md2615", null ],
        [ "System Information", "md_docs_2performance_2BASELINE.html#autotoc_md2619", [
          [ "Hardware Configuration", "md_docs_2performance_2BASELINE.html#autotoc_md2620", null ],
          [ "Software Configuration", "md_docs_2performance_2BASELINE.html#autotoc_md2621", null ]
        ] ],
        [ "Performance Metrics", "md_docs_2performance_2BASELINE.html#autotoc_md2623", [
          [ "Logging Throughput", "md_docs_2performance_2BASELINE.html#autotoc_md2624", null ],
          [ "Latency (Async Mode)", "md_docs_2performance_2BASELINE.html#autotoc_md2625", null ],
          [ "Memory Performance", "md_docs_2performance_2BASELINE.html#autotoc_md2626", null ]
        ] ],
        [ "Benchmark Results", "md_docs_2performance_2BASELINE.html#autotoc_md2628", null ],
        [ "Key Features", "md_docs_2performance_2BASELINE.html#autotoc_md2630", null ],
        [ "Baseline Validation", "md_docs_2performance_2BASELINE.html#autotoc_md2632", [
          [ "Phase 0 Requirements", "md_docs_2performance_2BASELINE.html#autotoc_md2633", null ],
          [ "Acceptance Criteria", "md_docs_2performance_2BASELINE.html#autotoc_md2634", null ]
        ] ]
      ] ]
    ] ],
    [ "CI_CD_PERFORMANCE_PROPOSAL", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html", [
      [ "CI/CD 성능 지표 자동화 제안서", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2638", [
        [ "autotoc_md2636", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2636", null ],
        [ "category: \"PERF\"", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2637", null ],
        [ "📊 Executive Summary", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2640", null ],
        [ "🔍 현재 상태 분석", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2642", [
          [ "✅ 장점", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2643", null ],
          [ "❌ 개선 필요 사항", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2644", null ]
        ] ],
        [ "🎯 제안 솔루션 아키텍처", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2646", [
          [ "전체 워크플로우", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2647", null ]
        ] ],
        [ "📁 제안 디렉토리 구조", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2649", null ],
        [ "🛠️ 구현 상세 (4단계)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2651", [
          [ "Phase 1: 벤치마크 재활성화 및 JSON 출력 설정", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2652", [
            [ "1.1 GitHub Actions 워크플로우 수정", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2653", null ],
            [ "1.2 벤치마크 실행 및 JSON 저장", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2654", null ],
            [ "1.3 API 변경에 따른 벤치마크 코드 수정", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2655", null ]
          ] ],
          [ "Phase 2: 결과 파싱 및 기준선 비교", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2657", [
            [ "2.1 벤치마크 결과 파싱 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2658", null ],
            [ "2.2 기준선 비교 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2659", null ]
          ] ],
          [ "Phase 3: 배지 및 차트 생성", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2661", [
            [ "3.1 성능 배지 생성 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2662", null ],
            [ "3.2 트렌드 차트 생성 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2663", null ]
          ] ],
          [ "Phase 4: README.md 자동 업데이트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2665", [
            [ "4.1 README 업데이트 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2666", null ],
            [ "4.2 GitHub Actions에서 자동 커밋", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2667", null ]
          ] ]
        ] ],
        [ "🎨 README.md 성능 섹션 디자인", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2669", [
          [ "제안 1: 배지 중심 디자인", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2670", null ],
          [ "제안 2: 테이블 중심 디자인", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2671", null ],
          [ "제안 3: 카드 스타일 디자인", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2672", null ]
        ] ],
        [ "🔄 CI/CD 워크플로우 전체 예시", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2674", null ],
        [ "📈 예상 결과물", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2676", [
          [ "GitHub Actions 실행 결과", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2677", null ],
          [ "PR 자동 코멘트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2678", null ],
          [ "업데이트된 README.md", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2679", null ]
        ] ],
        [ "✅ 구현 체크리스트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2681", [
          [ "Phase 1: 인프라 구축 (예상 소요: 2시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2682", null ],
          [ "Phase 2: 배지 및 차트 (예상 소요: 1.5시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2683", null ],
          [ "Phase 3: README 자동 업데이트 (예상 소요: 1시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2684", null ],
          [ "Phase 4: CI/CD 통합 (예상 소요: 2시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2685", null ],
          [ "Phase 5: 테스트 및 검증 (예상 소요: 1.5시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2686", null ],
          [ "Phase 6: 문서화 (예상 소요: 1시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2687", null ]
        ] ],
        [ "🔧 트러블슈팅 가이드", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2689", [
          [ "문제 1: 벤치마크 빌드 실패", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2690", null ],
          [ "문제 2: Python 스크립트 실행 오류", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2691", null ],
          [ "문제 3: Git 자동 커밋 권한 오류", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2692", null ],
          [ "문제 4: 배지가 README에 표시되지 않음", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2693", null ]
        ] ],
        [ "📚 참고 자료", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2695", [
          [ "도구 및 라이브러리", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2696", null ],
          [ "관련 프로젝트 예시", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2697", null ],
          [ "내부 문서", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2698", null ]
        ] ],
        [ "🎯 성공 지표", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2700", null ],
        [ "📝 다음 단계", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2702", null ]
      ] ]
    ] ],
    [ "DECORATOR_PERFORMANCE", "md_docs_2performance_2DECORATOR__PERFORMANCE.html", [
      [ "Decorator Pattern Performance Characteristics", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2706", [
        [ "autotoc_md2704", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2704", null ],
        [ "category: \"PERF\"", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2705", null ],
        [ "Overview", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2707", null ],
        [ "Executive Summary", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2708", null ],
        [ "Benchmark Methodology", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2709", [
          [ "Test Environment", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2710", null ],
          [ "Measured Configurations", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2711", null ],
          [ "Metrics", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2712", null ]
        ] ],
        [ "Performance Results", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2713", [
          [ "Throughput Comparison", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2714", null ],
          [ "Latency Analysis", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2715", null ],
          [ "Memory Overhead", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2716", null ]
        ] ],
        [ "Detailed Benchmark Results", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2717", [
          [ "Baseline: Direct File Writer", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2718", null ],
          [ "Single Decorator: Async", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2719", null ],
          [ "Double Decorator: Buffered + Async", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2720", null ],
          [ "Message Size Impact", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2721", null ]
        ] ],
        [ "Performance Recommendations", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2722", [
          [ "Production Guidelines", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2723", null ],
          [ "Decorator Ordering", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2724", null ],
          [ "Configuration Tuning", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2725", null ]
        ] ],
        [ "Running Benchmarks", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2726", [
          [ "Build and Execute", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2727", null ],
          [ "Using CMake Targets", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2728", null ],
          [ "Comparing Results", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2729", null ]
        ] ],
        [ "Regression Testing", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2730", [
          [ "CI Integration (Optional)", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2731", null ],
          [ "Regression Criteria", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2732", null ]
        ] ],
        [ "Known Limitations", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2733", [
          [ "Benchmark Accuracy", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2734", null ],
          [ "Excluded Scenarios", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2735", null ]
        ] ],
        [ "Conclusions", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2736", null ],
        [ "Related Documentation", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2737", null ],
        [ "Version History", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2738", null ]
      ] ]
    ] ],
    [ "PRODUCTION_QUALITY.kr", "md_docs_2PRODUCTION__QUALITY_8kr.html", [
      [ "Logger System 프로덕션 품질", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2741", [
        [ "autotoc_md2739", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2739", null ],
        [ "category: \"QUAL\"", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2740", null ],
        [ "요약", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2743", [
          [ "개발 상태: ✅ 개발 중", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2744", null ]
        ] ],
        [ "빌드 & 테스팅 인프라", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2746", [
          [ "지속적 통합 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2747", [
            [ "메인 CI 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2748", null ],
            [ "새니타이저 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2749", null ],
            [ "성능 벤치마크 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2750", null ],
            [ "코드 커버리지 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2751", null ],
            [ "정적 분석 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2752", null ]
          ] ]
        ] ],
        [ "스레드 안전성 & 동시성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2754", [
          [ "스레드 안전성 등급: A+", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2755", null ],
          [ "Writer 스레드 안전성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2756", null ],
          [ "비동기 처리 아키텍처", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2757", null ]
        ] ],
        [ "리소스 관리 (RAII)", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2759", [
          [ "RAII 준수 등급: A", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2760", null ],
          [ "스마트 포인터 사용", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2761", null ],
          [ "메모리 누수 탐지", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2762", null ],
          [ "예외 안전성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2763", null ]
        ] ],
        [ "오류 처리 전략", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2765", [
          [ "오류 처리 등급: A (90% 완료)", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2766", null ],
          [ "Result<T> 패턴", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2767", null ],
          [ "오류 코드 레지스트리", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2768", null ]
        ] ],
        [ "성능 특성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2770", [
          [ "기준선 메트릭", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2771", null ]
        ] ],
        [ "보안 기능", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2773", [
          [ "보안 등급: A (v3.0.0)", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2774", null ],
          [ "보안 키 저장소", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2775", null ],
          [ "경로 검증", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2776", null ],
          [ "시그널 핸들러 안전성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2777", null ],
          [ "보안 감사 로깅", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2778", null ],
          [ "컴플라이언스 지원", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2779", null ]
        ] ],
        [ "플랫폼 지원", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2781", [
          [ "공식 지원 플랫폼", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2782", null ],
          [ "최소 요구사항", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2783", null ]
        ] ]
      ] ]
    ] ],
    [ "PRODUCTION_QUALITY", "md_docs_2PRODUCTION__QUALITY.html", [
      [ "Logger System Production Quality", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2789", [
        [ "autotoc_md2787", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2787", null ],
        [ "category: \"QUAL\"", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2788", null ],
        [ "Table of Contents", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2791", null ],
        [ "Executive Summary", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2793", [
          [ "Quality Status", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2794", null ]
        ] ],
        [ "Build & Testing Infrastructure", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2796", [
          [ "Continuous Integration Pipelines", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2797", [
            [ "Main CI Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2798", null ],
            [ "Sanitizer Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2800", null ],
            [ "Performance Benchmark Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2802", null ],
            [ "Code Coverage Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2804", null ],
            [ "Static Analysis Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2806", null ]
          ] ],
          [ "Test Organization", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2808", [
            [ "Unit Tests", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2809", null ],
            [ "Integration Tests", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2811", null ],
            [ "Benchmarks", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2813", null ]
          ] ]
        ] ],
        [ "Thread Safety & Concurrency", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2815", [
          [ "Thread Safety Grade: A+", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2816", null ],
          [ "Writer Thread Safety", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2817", null ],
          [ "Asynchronous Processing Architecture", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2819", null ],
          [ "Concurrency Patterns", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2821", null ]
        ] ],
        [ "Resource Management (RAII)", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2823", [
          [ "RAII Compliance Grade: A", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2824", null ],
          [ "Smart Pointer Usage", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2825", null ],
          [ "Memory Leak Detection", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2827", null ],
          [ "Exception Safety", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2829", null ]
        ] ],
        [ "Error Handling Strategy", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2831", [
          [ "Error Handling Grade: A (90% Complete)", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2832", null ],
          [ "Result<T> Pattern", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2833", null ],
          [ "Error Code Registry", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2835", null ],
          [ "Hot Path Optimization", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2837", null ]
        ] ],
        [ "Performance Characteristics", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2839", [
          [ "Baseline Metrics", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2840", null ],
          [ "Performance Baselines (CI/CD)", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2842", null ]
        ] ],
        [ "Security Features", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2844", [
          [ "Security Grade: A (v3.0.0)", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2845", null ],
          [ "Secure Key Storage", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2846", null ],
          [ "Path Validation", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2847", null ],
          [ "Signal Handler Safety", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2848", null ],
          [ "Security Audit Logging", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2849", null ],
          [ "Compliance Support", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2850", null ],
          [ "OWASP Top 10 Mitigations", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2851", null ]
        ] ],
        [ "Operational Metrics", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2853", [
          [ "Built-in Metrics Collection", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2854", null ],
          [ "Health Checks", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2856", null ]
        ] ],
        [ "Platform Support", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2858", [
          [ "Officially Supported Platforms", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2859", null ],
          [ "Minimum Requirements", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2860", null ],
          [ "Cross-Platform Compatibility", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2862", null ]
        ] ],
        [ "Remaining Work & Future Enhancements", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2864", [
          [ "Current Status: 90% Production Ready", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2865", null ]
        ] ],
        [ "See Also", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2867", null ]
      ] ]
    ] ],
    [ "PROJECT_STRUCTURE.kr", "md_docs_2PROJECT__STRUCTURE_8kr.html", [
      [ "Logger System 프로젝트 구조", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2870", [
        [ "autotoc_md2868", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2868", null ],
        [ "category: \"PROJ\"", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2869", null ],
        [ "목차", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2872", null ],
        [ "디렉토리 개요", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2874", null ],
        [ "코어 모듈 파일", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2876", [
          [ "logger.h / logger.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2877", null ],
          [ "logger_builder.h / logger_builder.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2879", null ],
          [ "log_entry.h", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2881", null ],
          [ "result_types.h", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2883", null ]
        ] ],
        [ "Writer 구현", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2885", [
          [ "console_writer.h / console_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2886", null ],
          [ "file_writer.h / file_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2888", null ],
          [ "rotating_file_writer.h / rotating_file_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2890", null ],
          [ "network_writer.h / network_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2892", null ],
          [ "critical_writer.h / critical_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2894", null ],
          [ "hybrid_writer.h / hybrid_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2896", null ]
        ] ],
        [ "Filter 구현", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2898", [
          [ "level_filter.h / level_filter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2899", null ],
          [ "regex_filter.h / regex_filter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2901", null ],
          [ "function_filter.h / function_filter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2903", null ]
        ] ],
        [ "Formatter 구현", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2905", [
          [ "plain_formatter.h / plain_formatter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2906", null ],
          [ "json_formatter.h / json_formatter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2908", null ],
          [ "custom_formatter.h / custom_formatter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2910", null ]
        ] ],
        [ "설정 시스템", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2912", [
          [ "config_templates.h / config_templates.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2913", null ],
          [ "config_validator.h / config_validator.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2915", null ]
        ] ],
        [ "보안 컴포넌트", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2917", [
          [ "secure_key_storage.h / secure_key_storage.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2918", null ],
          [ "path_validator.h / path_validator.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2920", null ],
          [ "log_sanitizer.h / log_sanitizer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2922", null ]
        ] ],
        [ "테스팅 구성", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2924", [
          [ "유닛 테스트", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2925", null ],
          [ "통합 테스트", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2927", null ],
          [ "벤치마크", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2929", null ]
        ] ],
        [ "빌드 시스템", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2931", [
          [ "CMakeLists.txt (루트)", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2932", null ]
        ] ],
        [ "모듈 의존성", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2934", [
          [ "의존성 그래프", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2935", null ],
          [ "외부 의존성", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2936", null ]
        ] ],
        [ "파일 명명 규칙", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2938", [
          [ "헤더 파일", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2939", null ],
          [ "소스 파일", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2940", null ],
          [ "문서 파일", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2941", null ]
        ] ],
        [ "참고 문서", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2943", null ]
      ] ]
    ] ],
    [ "PROJECT_STRUCTURE", "md_docs_2PROJECT__STRUCTURE.html", [
      [ "Logger System Project Structure", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2948", [
        [ "autotoc_md2946", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2946", null ],
        [ "category: \"PROJ\"", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2947", null ],
        [ "Table of Contents", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2950", null ],
        [ "Directory Overview", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2952", null ],
        [ "Core Module Files", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2954", [
          [ "Header Files (include/kcenon/logger/core/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2955", null ],
          [ "Core Subdirectories", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2956", null ],
          [ "Source Files (src/core/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2957", null ],
          [ "Interfaces (include/kcenon/logger/interfaces/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2958", null ],
          [ "Root-Level Headers (include/kcenon/logger/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2959", null ]
        ] ],
        [ "Builder Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2961", [
          [ "writer_builder.h / writer_builder.cpp", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2962", null ]
        ] ],
        [ "Writer Implementations", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2964", [
          [ "Header Files (include/kcenon/logger/writers/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2965", null ],
          [ "Source Files (src/impl/writers/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2966", null ],
          [ "Async Processing (src/impl/async/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2967", null ]
        ] ],
        [ "Filter Implementations", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2969", [
          [ "log_filter.h", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2970", null ]
        ] ],
        [ "Formatter Implementations", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2972", [
          [ "Header Files (include/kcenon/logger/formatters/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2973", null ]
        ] ],
        [ "Factory Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2975", [
          [ "Header Files (include/kcenon/logger/factories/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2976", null ]
        ] ],
        [ "Adapter Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2978", [
          [ "Header Files (include/kcenon/logger/adapters/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2979", null ]
        ] ],
        [ "Backend Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2981", [
          [ "Header Files (include/kcenon/logger/backends/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2982", null ]
        ] ],
        [ "Sink Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2984", [
          [ "Header Files (include/kcenon/logger/sinks/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2985", null ]
        ] ],
        [ "Integration Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2987", [
          [ "Header Files (include/kcenon/logger/integration/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2988", null ],
          [ "Source Files (src/integration/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2989", null ]
        ] ],
        [ "Sampling Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2991", [
          [ "Header Files (include/kcenon/logger/sampling/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2992", null ],
          [ "Source Files (src/sampling/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2993", null ]
        ] ],
        [ "Utility Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2995", [
          [ "Header Files (include/kcenon/logger/utils/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2996", null ]
        ] ],
        [ "Security Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2998", [
          [ "Header Files (include/kcenon/logger/security/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2999", null ],
          [ "Source Files (src/security/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3000", null ]
        ] ],
        [ "Additional Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3002", [
          [ "Dependency Injection (include/kcenon/logger/di/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3003", null ],
          [ "OpenTelemetry (include/kcenon/logger/otlp/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3004", null ],
          [ "Routing (include/kcenon/logger/routing/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3005", null ],
          [ "Safety (include/kcenon/logger/safety/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3006", null ],
          [ "Analysis (include/kcenon/logger/analysis/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3007", null ],
          [ "Structured Logging (include/kcenon/logger/structured/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3008", null ],
          [ "Server (include/kcenon/logger/server/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3009", null ],
          [ "Internal Implementation Details (src/impl/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3010", null ]
        ] ],
        [ "Testing Organization", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3012", [
          [ "Unit Tests", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3013", null ],
          [ "Integration Tests", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3015", null ],
          [ "Benchmarks", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3017", null ]
        ] ],
        [ "Build System", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3019", [
          [ "CMakeLists.txt (Root)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3020", null ],
          [ "CMake Modules", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3022", null ]
        ] ],
        [ "Module Dependencies", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3024", [
          [ "Dependency Graph", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3025", null ],
          [ "External Dependencies", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3026", null ]
        ] ],
        [ "File Naming Conventions", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3028", [
          [ "Header Files", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3029", null ],
          [ "Source Files", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3030", null ],
          [ "Documentation Files", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3031", null ]
        ] ],
        [ "See Also", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md3033", null ]
      ] ]
    ] ],
    [ "README.kr", "md_docs_2README_8kr.html", [
      [ "Logger System 문서", "md_docs_2README_8kr.html#autotoc_md3036", [
        [ "autotoc_md3034", "md_docs_2README_8kr.html#autotoc_md3034", null ],
        [ "category: \"GUID\"", "md_docs_2README_8kr.html#autotoc_md3035", null ],
        [ "📚 문서 구조", "md_docs_2README_8kr.html#autotoc_md3037", [
          [ "핵심 문서", "md_docs_2README_8kr.html#autotoc_md3038", null ],
          [ "가이드 (docs/guides/)", "md_docs_2README_8kr.html#autotoc_md3039", null ],
          [ "고급 주제 (docs/advanced/)", "md_docs_2README_8kr.html#autotoc_md3040", null ]
        ] ],
        [ "🚀 빠른 탐색", "md_docs_2README_8kr.html#autotoc_md3041", [
          [ "신규 사용자", "md_docs_2README_8kr.html#autotoc_md3042", null ],
          [ "기존 사용자", "md_docs_2README_8kr.html#autotoc_md3043", null ],
          [ "기여자", "md_docs_2README_8kr.html#autotoc_md3044", null ],
          [ "성능 엔지니어", "md_docs_2README_8kr.html#autotoc_md3045", null ]
        ] ],
        [ "🔧 문서 유지관리", "md_docs_2README_8kr.html#autotoc_md3046", [
          [ "최근 변경사항", "md_docs_2README_8kr.html#autotoc_md3047", null ]
        ] ],
        [ "📖 외부 리소스", "md_docs_2README_8kr.html#autotoc_md3048", null ],
        [ "🤝 도움 및 지원", "md_docs_2README_8kr.html#autotoc_md3049", null ]
      ] ]
    ] ],
    [ "README", "md_docs_2README.html", [
      [ "Logger System — Documentation Registry", "md_docs_2README.html#autotoc_md3054", [
        [ "autotoc_md3052", "md_docs_2README.html#autotoc_md3052", null ],
        [ "category: \"GUID\"", "md_docs_2README.html#autotoc_md3053", null ],
        [ "Document Index", "md_docs_2README.html#autotoc_md3055", null ],
        [ "Documents by Category", "md_docs_2README.html#autotoc_md3056", [
          [ "Architecture (9)", "md_docs_2README.html#autotoc_md3057", null ],
          [ "API Reference (2)", "md_docs_2README.html#autotoc_md3058", null ],
          [ "Features (2)", "md_docs_2README.html#autotoc_md3059", null ],
          [ "Guides (28)", "md_docs_2README.html#autotoc_md3060", null ],
          [ "Performance (8)", "md_docs_2README.html#autotoc_md3061", null ],
          [ "Migration (4)", "md_docs_2README.html#autotoc_md3062", null ],
          [ "Integration (2)", "md_docs_2README.html#autotoc_md3063", null ],
          [ "Quality (4)", "md_docs_2README.html#autotoc_md3064", null ],
          [ "Security (3)", "md_docs_2README.html#autotoc_md3065", null ],
          [ "Architecture Decision Records (2)", "md_docs_2README.html#autotoc_md3066", null ],
          [ "Project (10)", "md_docs_2README.html#autotoc_md3067", null ]
        ] ]
      ] ]
    ] ],
    [ "SECURITY_GUIDE", "md_docs_2SECURITY__GUIDE.html", [
      [ "Security Module Guide", "md_docs_2SECURITY__GUIDE.html#autotoc_md3071", [
        [ "autotoc_md3069", "md_docs_2SECURITY__GUIDE.html#autotoc_md3069", null ],
        [ "category: \"SECU\"", "md_docs_2SECURITY__GUIDE.html#autotoc_md3070", null ],
        [ "Table of Contents", "md_docs_2SECURITY__GUIDE.html#autotoc_md3073", null ],
        [ "Overview", "md_docs_2SECURITY__GUIDE.html#autotoc_md3075", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md3076", null ],
          [ "Components", "md_docs_2SECURITY__GUIDE.html#autotoc_md3077", null ]
        ] ],
        [ "Threat Model", "md_docs_2SECURITY__GUIDE.html#autotoc_md3079", [
          [ "Threats Defended Against", "md_docs_2SECURITY__GUIDE.html#autotoc_md3080", null ],
          [ "Security Assumptions", "md_docs_2SECURITY__GUIDE.html#autotoc_md3081", null ],
          [ "Out of Scope", "md_docs_2SECURITY__GUIDE.html#autotoc_md3082", null ]
        ] ],
        [ "Architecture", "md_docs_2SECURITY__GUIDE.html#autotoc_md3084", [
          [ "Component Integration", "md_docs_2SECURITY__GUIDE.html#autotoc_md3085", null ],
          [ "Dependency Graph", "md_docs_2SECURITY__GUIDE.html#autotoc_md3086", null ]
        ] ],
        [ "Audit Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md3088", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md3089", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md3090", null ],
          [ "Audit Event Types", "md_docs_2SECURITY__GUIDE.html#autotoc_md3091", null ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md3092", [
            [ "Initialize Audit Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md3093", null ],
            [ "Log an Audit Event", "md_docs_2SECURITY__GUIDE.html#autotoc_md3094", null ],
            [ "Verify Entry Integrity", "md_docs_2SECURITY__GUIDE.html#autotoc_md3095", null ]
          ] ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3096", null ],
          [ "Compliance Considerations", "md_docs_2SECURITY__GUIDE.html#autotoc_md3097", null ]
        ] ],
        [ "Log Sanitizer", "md_docs_2SECURITY__GUIDE.html#autotoc_md3099", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md3100", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md3101", null ],
          [ "Sensitive Data Types", "md_docs_2SECURITY__GUIDE.html#autotoc_md3102", null ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md3103", [
            [ "Add Built-in Pattern", "md_docs_2SECURITY__GUIDE.html#autotoc_md3104", null ],
            [ "Add Custom Pattern", "md_docs_2SECURITY__GUIDE.html#autotoc_md3105", null ],
            [ "Remove Pattern", "md_docs_2SECURITY__GUIDE.html#autotoc_md3106", null ],
            [ "Sanitize a String", "md_docs_2SECURITY__GUIDE.html#autotoc_md3107", null ],
            [ "Check for Sensitive Data", "md_docs_2SECURITY__GUIDE.html#autotoc_md3108", null ],
            [ "Get Active Patterns", "md_docs_2SECURITY__GUIDE.html#autotoc_md3109", null ],
            [ "Add All Common Patterns", "md_docs_2SECURITY__GUIDE.html#autotoc_md3110", null ]
          ] ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3111", null ],
          [ "Performance Impact", "md_docs_2SECURITY__GUIDE.html#autotoc_md3112", null ],
          [ "Integration with Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md3113", null ]
        ] ],
        [ "Path Validator", "md_docs_2SECURITY__GUIDE.html#autotoc_md3115", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md3116", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md3117", null ],
          [ "Security Checks", "md_docs_2SECURITY__GUIDE.html#autotoc_md3118", null ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md3119", [
            [ "Constructor", "md_docs_2SECURITY__GUIDE.html#autotoc_md3120", null ],
            [ "Validate Path", "md_docs_2SECURITY__GUIDE.html#autotoc_md3121", null ],
            [ "Static: Check Filename Safety", "md_docs_2SECURITY__GUIDE.html#autotoc_md3122", null ],
            [ "Static: Sanitize Filename", "md_docs_2SECURITY__GUIDE.html#autotoc_md3123", null ],
            [ "Static: Safe Path Join", "md_docs_2SECURITY__GUIDE.html#autotoc_md3124", null ]
          ] ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3125", null ],
          [ "Integration with Writers", "md_docs_2SECURITY__GUIDE.html#autotoc_md3126", null ]
        ] ],
        [ "Secure Key Storage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3128", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md3129", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md3130", null ],
          [ "Classes", "md_docs_2SECURITY__GUIDE.html#autotoc_md3131", [
            [ "secure_key", "md_docs_2SECURITY__GUIDE.html#autotoc_md3132", null ],
            [ "secure_key_storage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3133", null ]
          ] ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md3134", [
            [ "Generate Secure Random Key", "md_docs_2SECURITY__GUIDE.html#autotoc_md3135", null ],
            [ "Save Key to File", "md_docs_2SECURITY__GUIDE.html#autotoc_md3136", null ],
            [ "Load Key from File", "md_docs_2SECURITY__GUIDE.html#autotoc_md3137", null ]
          ] ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3138", null ],
          [ "Key Rotation", "md_docs_2SECURITY__GUIDE.html#autotoc_md3139", null ],
          [ "Platform-Specific Considerations", "md_docs_2SECURITY__GUIDE.html#autotoc_md3140", [
            [ "Linux/macOS", "md_docs_2SECURITY__GUIDE.html#autotoc_md3141", null ],
            [ "Windows", "md_docs_2SECURITY__GUIDE.html#autotoc_md3142", null ]
          ] ]
        ] ],
        [ "Signal Manager", "md_docs_2SECURITY__GUIDE.html#autotoc_md3144", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md3145", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md3146", null ],
          [ "Handled Signals", "md_docs_2SECURITY__GUIDE.html#autotoc_md3147", null ],
          [ "Signal-Safe Guarantees", "md_docs_2SECURITY__GUIDE.html#autotoc_md3148", null ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md3149", [
            [ "Register Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md3150", null ],
            [ "Unregister Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md3151", null ],
            [ "Check Handler Installation", "md_docs_2SECURITY__GUIDE.html#autotoc_md3152", null ],
            [ "Get Logger Count", "md_docs_2SECURITY__GUIDE.html#autotoc_md3153", null ]
          ] ],
          [ "Critical Logger Interface", "md_docs_2SECURITY__GUIDE.html#autotoc_md3154", null ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3155", null ],
          [ "Emergency Flush Implementation", "md_docs_2SECURITY__GUIDE.html#autotoc_md3156", null ],
          [ "Testing Considerations", "md_docs_2SECURITY__GUIDE.html#autotoc_md3157", null ]
        ] ],
        [ "Configuration Examples", "md_docs_2SECURITY__GUIDE.html#autotoc_md3159", [
          [ "Minimal Security Setup", "md_docs_2SECURITY__GUIDE.html#autotoc_md3160", null ],
          [ "High-Security Setup (Compliance)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3161", null ],
          [ "Custom Security Layer", "md_docs_2SECURITY__GUIDE.html#autotoc_md3162", null ]
        ] ],
        [ "Compliance Considerations", "md_docs_2SECURITY__GUIDE.html#autotoc_md3164", [
          [ "SOX (Sarbanes-Oxley)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3165", null ],
          [ "HIPAA (Health Insurance Portability and Accountability Act)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3166", null ],
          [ "GDPR (General Data Protection Regulation)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3167", null ],
          [ "PCI-DSS (Payment Card Industry Data Security Standard)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3168", null ],
          [ "ISO 27001 (Information Security Management)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3169", [
            [ "Writer Integrity Policy (Issue #612)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3170", null ]
          ] ]
        ] ],
        [ "Frequently Asked Questions", "md_docs_2SECURITY__GUIDE.html#autotoc_md3172", [
          [ "Q: Can I use the security module without OpenSSL?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3173", null ],
          [ "Q: How do I rotate encryption keys?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3174", null ],
          [ "Q: What's the performance overhead of log sanitization?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3175", null ],
          [ "Q: How do I verify audit log integrity?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3176", null ],
          [ "Q: Can I use multiple loggers with one signal manager?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3177", null ],
          [ "Q: How do I handle signal manager cleanup?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3178", null ]
        ] ],
        [ "Summary", "md_docs_2SECURITY__GUIDE.html#autotoc_md3180", null ]
      ] ]
    ] ],
    [ "SOUP", "md_docs_2SOUP.html", [
      [ "SOUP List — logger_system", "md_docs_2SOUP.html#autotoc_md3184", [
        [ "autotoc_md3182", "md_docs_2SOUP.html#autotoc_md3182", null ],
        [ "category: \"PROJ\"", "md_docs_2SOUP.html#autotoc_md3183", null ],
        [ "Production SOUP", "md_docs_2SOUP.html#autotoc_md3186", null ],
        [ "Optional SOUP", "md_docs_2SOUP.html#autotoc_md3188", null ],
        [ "Development/Test SOUP (Not Deployed)", "md_docs_2SOUP.html#autotoc_md3190", null ],
        [ "Safety Classification Key", "md_docs_2SOUP.html#autotoc_md3192", null ],
        [ "Version Pinning (IEC 62304 Compliance)", "md_docs_2SOUP.html#autotoc_md3194", null ],
        [ "Version Update Process", "md_docs_2SOUP.html#autotoc_md3196", null ],
        [ "License Compliance Summary", "md_docs_2SOUP.html#autotoc_md3198", null ]
      ] ]
    ] ],
    [ "Test & CI Inventory", "md_docs_2TEST__INVENTORY.html", [
      [ "Test build wiring", "md_docs_2TEST__INVENTORY.html#autotoc_md3200", null ],
      [ "Test sources (tracked counts)", "md_docs_2TEST__INVENTORY.html#autotoc_md3201", [
        [ "tests/ layout", "md_docs_2TEST__INVENTORY.html#autotoc_md3202", null ],
        [ "integration_tests/ layout", "md_docs_2TEST__INVENTORY.html#autotoc_md3203", null ]
      ] ],
      [ "CI workflows (.github/workflows/)", "md_docs_2TEST__INVENTORY.html#autotoc_md3204", null ],
      [ "Coverage observations (factual)", "md_docs_2TEST__INVENTORY.html#autotoc_md3205", null ],
      [ "Added by this change (issue #641)", "md_docs_2TEST__INVENTORY.html#autotoc_md3206", null ],
      [ "Scope note", "md_docs_2TEST__INVENTORY.html#autotoc_md3207", null ]
    ] ],
    [ "TRACEABILITY", "md_docs_2TRACEABILITY.html", [
      [ "Traceability Matrix", "md_docs_2TRACEABILITY.html#autotoc_md3210", [
        [ "autotoc_md3208", "md_docs_2TRACEABILITY.html#autotoc_md3208", null ],
        [ "category: \"QUAL\"", "md_docs_2TRACEABILITY.html#autotoc_md3209", null ],
        [ "Feature -> Test -> Module Mapping", "md_docs_2TRACEABILITY.html#autotoc_md3211", [
          [ "Core Logger", "md_docs_2TRACEABILITY.html#autotoc_md3212", null ],
          [ "Writer Types", "md_docs_2TRACEABILITY.html#autotoc_md3213", null ],
          [ "Formatting & Filtering", "md_docs_2TRACEABILITY.html#autotoc_md3214", null ],
          [ "Security Features", "md_docs_2TRACEABILITY.html#autotoc_md3215", null ],
          [ "Advanced Capabilities", "md_docs_2TRACEABILITY.html#autotoc_md3216", null ],
          [ "OTLP Integration", "md_docs_2TRACEABILITY.html#autotoc_md3217", null ],
          [ "Log Sampling", "md_docs_2TRACEABILITY.html#autotoc_md3218", null ],
          [ "Monitoring & Health", "md_docs_2TRACEABILITY.html#autotoc_md3219", null ],
          [ "Integration & DI", "md_docs_2TRACEABILITY.html#autotoc_md3220", null ],
          [ "Utilities", "md_docs_2TRACEABILITY.html#autotoc_md3221", null ],
          [ "Production Quality", "md_docs_2TRACEABILITY.html#autotoc_md3222", null ]
        ] ],
        [ "Coverage Summary", "md_docs_2TRACEABILITY.html#autotoc_md3223", null ],
        [ "See Also", "md_docs_2TRACEABILITY.html#autotoc_md3224", null ]
      ] ]
    ] ],
    [ "Troubleshooting Guide", "troubleshooting.html", [
      [ "Lost log messages", "troubleshooting.html#ts_lost_messages", [
        [ "Symptoms", "troubleshooting.html#ts_lost_symptoms", null ],
        [ "Causes", "troubleshooting.html#ts_lost_causes", null ],
        [ "Fix", "troubleshooting.html#ts_lost_fix", null ]
      ] ],
      [ "File permission errors", "troubleshooting.html#ts_file_perm", [
        [ "Symptoms", "troubleshooting.html#ts_perm_symptoms", null ],
        [ "Causes", "troubleshooting.html#ts_perm_causes", null ],
        [ "Fix", "troubleshooting.html#ts_perm_fix", null ]
      ] ],
      [ "Async queue overflow", "troubleshooting.html#ts_queue_overflow", [
        [ "Symptoms", "troubleshooting.html#ts_overflow_symptoms", null ],
        [ "Causes", "troubleshooting.html#ts_overflow_causes", null ],
        [ "Fix", "troubleshooting.html#ts_overflow_fix", null ]
      ] ],
      [ "Performance degradation under load", "troubleshooting.html#ts_perf", [
        [ "Symptoms", "troubleshooting.html#ts_perf_symptoms", null ],
        [ "Causes", "troubleshooting.html#ts_perf_causes", null ],
        [ "Fix", "troubleshooting.html#ts_perf_fix", null ]
      ] ],
      [ "Decorator composition issues", "troubleshooting.html#ts_decorator_compose", [
        [ "Symptoms", "troubleshooting.html#ts_decocomp_symptoms", null ],
        [ "Causes", "troubleshooting.html#ts_decocomp_causes", null ],
        [ "Fix", "troubleshooting.html#ts_decocomp_fix", null ]
      ] ],
      [ "More Help", "troubleshooting.html#ts_more", null ]
    ] ],
    [ "Tutorial: Basic Logging", "tutorial_basic_logging.html", [
      [ "Prerequisites", "tutorial_basic_logging.html#basic_prereq", null ],
      [ "Step 1: Create a Logger", "tutorial_basic_logging.html#basic_step1", null ],
      [ "Step 2: Combine Console and File Writers", "tutorial_basic_logging.html#basic_step2", null ],
      [ "Step 3: Choose a Log Level", "tutorial_basic_logging.html#basic_step3", null ],
      [ "Step 4: Emit Structured Logs", "tutorial_basic_logging.html#basic_step4", null ],
      [ "Three Complete Examples", "tutorial_basic_logging.html#basic_examples", [
        [ "Example 1: Hello, Logger", "tutorial_basic_logging.html#basic_example_minimal", null ],
        [ "Example 2: Console + Rotating File", "tutorial_basic_logging.html#basic_example_file", null ],
        [ "Example 3: Structured Login Audit", "tutorial_basic_logging.html#basic_example_struct", null ]
      ] ],
      [ "Next Steps", "tutorial_basic_logging.html#basic_next", null ]
    ] ],
    [ "Tutorial: Decorator Composition", "tutorial_decorators.html", [
      [ "Conceptual Model", "tutorial_decorators.html#deco_concept", null ],
      [ "Ordering Rules", "tutorial_decorators.html#deco_ordering", null ],
      [ "Using writer_builder", "tutorial_decorators.html#deco_builder", null ],
      [ "Three Composition Examples", "tutorial_decorators.html#deco_examples", [
        [ "Example 1: Encrypted Audit Trail", "tutorial_decorators.html#deco_example_audit", null ],
        [ "Example 2: Filtered Console + Persistent File", "tutorial_decorators.html#deco_example_console", null ],
        [ "Example 3: Custom Decorator", "tutorial_decorators.html#deco_example_custom", null ]
      ] ],
      [ "Common Pitfalls", "tutorial_decorators.html#deco_pitfalls", null ],
      [ "Next Steps", "tutorial_decorators.html#deco_next", null ]
    ] ],
    [ "Tutorial: Production Configuration", "tutorial_production.html", [
      [ "Production Goals", "tutorial_production.html#prod_goals", null ],
      [ "High-Throughput Configuration", "tutorial_production.html#prod_throughput", null ],
      [ "Async Logging", "tutorial_production.html#prod_async", null ],
      [ "Log Rotation", "tutorial_production.html#prod_rotation", null ],
      [ "OpenTelemetry Export", "tutorial_production.html#prod_otlp", null ],
      [ "Three Production Examples", "tutorial_production.html#prod_examples", [
        [ "Example 1: Stateless Web Service", "tutorial_production.html#prod_example_web", null ],
        [ "Example 2: Compliance / Audit Pipeline", "tutorial_production.html#prod_example_audit", null ],
        [ "Example 3: Batch Job with Bulk Throughput", "tutorial_production.html#prod_example_batch", null ]
      ] ],
      [ "Metrics and Self-Observability", "tutorial_production.html#prod_metrics", null ],
      [ "Next Steps", "tutorial_production.html#prod_next", null ]
    ] ],
    [ "WRITER_GUIDE", "md_docs_2WRITER__GUIDE.html", [
      [ "Writer Composition and Decorator Guide", "md_docs_2WRITER__GUIDE.html#autotoc_md3227", [
        [ "autotoc_md3225", "md_docs_2WRITER__GUIDE.html#autotoc_md3225", null ],
        [ "category: \"GUID\"", "md_docs_2WRITER__GUIDE.html#autotoc_md3226", null ],
        [ "Table of Contents", "md_docs_2WRITER__GUIDE.html#autotoc_md3229", null ],
        [ "Architecture Overview", "md_docs_2WRITER__GUIDE.html#autotoc_md3231", [
          [ "Decorator Pattern", "md_docs_2WRITER__GUIDE.html#autotoc_md3232", null ],
          [ "Inheritance Chain", "md_docs_2WRITER__GUIDE.html#autotoc_md3233", null ],
          [ "Stack Visualization", "md_docs_2WRITER__GUIDE.html#autotoc_md3234", null ]
        ] ],
        [ "Writer Catalog", "md_docs_2WRITER__GUIDE.html#autotoc_md3236", [
          [ "Base Writers", "md_docs_2WRITER__GUIDE.html#autotoc_md3237", [
            [ "console_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3238", null ],
            [ "file_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3240", null ],
            [ "rotating_file_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3242", null ],
            [ "network_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3244", null ],
            [ "otlp_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3246", null ]
          ] ],
          [ "Decorator Writers", "md_docs_2WRITER__GUIDE.html#autotoc_md3248", [
            [ "async_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3249", null ],
            [ "batch_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3251", null ],
            [ "buffered_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3253", null ],
            [ "filtered_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3255", null ],
            [ "formatted_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3257", null ],
            [ "thread_safe_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3259", null ],
            [ "encrypted_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3261", null ],
            [ "critical_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3263", null ],
            [ "composite_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3265", null ],
            [ "queued_writer_base", "md_docs_2WRITER__GUIDE.html#autotoc_md3267", null ],
            [ "legacy_writer_adapter", "md_docs_2WRITER__GUIDE.html#autotoc_md3269", null ]
          ] ]
        ] ],
        [ "Composition Patterns", "md_docs_2WRITER__GUIDE.html#autotoc_md3271", [
          [ "Pattern 1: Simple Console (Development)", "md_docs_2WRITER__GUIDE.html#autotoc_md3272", null ],
          [ "Pattern 2: Async Console (Development + Performance)", "md_docs_2WRITER__GUIDE.html#autotoc_md3274", null ],
          [ "Pattern 3: Filtered Formatted File (Production)", "md_docs_2WRITER__GUIDE.html#autotoc_md3276", null ],
          [ "Pattern 4: High-Throughput Production", "md_docs_2WRITER__GUIDE.html#autotoc_md3278", null ],
          [ "Pattern 5: Encrypted Compliance Logging", "md_docs_2WRITER__GUIDE.html#autotoc_md3280", null ],
          [ "Pattern 6: Multi-Destination (File + Network)", "md_docs_2WRITER__GUIDE.html#autotoc_md3282", null ]
        ] ],
        [ "Recommended Stacks", "md_docs_2WRITER__GUIDE.html#autotoc_md3284", [
          [ "Development", "md_docs_2WRITER__GUIDE.html#autotoc_md3285", null ],
          [ "Production File Logging", "md_docs_2WRITER__GUIDE.html#autotoc_md3286", null ],
          [ "Compliance & Security", "md_docs_2WRITER__GUIDE.html#autotoc_md3287", null ],
          [ "Distributed Systems", "md_docs_2WRITER__GUIDE.html#autotoc_md3288", null ],
          [ "Resource-Constrained", "md_docs_2WRITER__GUIDE.html#autotoc_md3289", null ]
        ] ],
        [ "Code Examples", "md_docs_2WRITER__GUIDE.html#autotoc_md3291", [
          [ "Example 1: Development Setup (3 lines)", "md_docs_2WRITER__GUIDE.html#autotoc_md3292", null ],
          [ "Example 2: Production File Logging", "md_docs_2WRITER__GUIDE.html#autotoc_md3294", null ],
          [ "Example 3: Encrypted Compliance Logging", "md_docs_2WRITER__GUIDE.html#autotoc_md3296", null ],
          [ "Example 4: Multi-Destination Setup", "md_docs_2WRITER__GUIDE.html#autotoc_md3298", null ],
          [ "Example 5: Custom Stack Order", "md_docs_2WRITER__GUIDE.html#autotoc_md3300", null ]
        ] ],
        [ "Performance Impact", "md_docs_2WRITER__GUIDE.html#autotoc_md3302", [
          [ "Throughput per Configuration", "md_docs_2WRITER__GUIDE.html#autotoc_md3303", null ],
          [ "Latency per Decorator", "md_docs_2WRITER__GUIDE.html#autotoc_md3304", null ],
          [ "Memory Usage per Decorator", "md_docs_2WRITER__GUIDE.html#autotoc_md3305", null ]
        ] ],
        [ "Best Practices", "md_docs_2WRITER__GUIDE.html#autotoc_md3307", [
          [ "Filter Early, Format Late", "md_docs_2WRITER__GUIDE.html#autotoc_md3308", null ],
          [ "Use Async for I/O-Heavy Writers", "md_docs_2WRITER__GUIDE.html#autotoc_md3310", null ],
          [ "Batch + Buffer for Maximum Throughput", "md_docs_2WRITER__GUIDE.html#autotoc_md3312", null ],
          [ "Encrypt Only What's Necessary", "md_docs_2WRITER__GUIDE.html#autotoc_md3314", null ],
          [ "Use composite_writer for Multi-Destination", "md_docs_2WRITER__GUIDE.html#autotoc_md3316", null ],
          [ "Flush on Critical Logs", "md_docs_2WRITER__GUIDE.html#autotoc_md3318", null ],
          [ "Thread-Safety Only When Needed", "md_docs_2WRITER__GUIDE.html#autotoc_md3320", null ],
          [ "Start Async Writers Explicitly", "md_docs_2WRITER__GUIDE.html#autotoc_md3322", null ]
        ] ],
        [ "Summary", "md_docs_2WRITER__GUIDE.html#autotoc_md3324", null ]
      ] ]
    ] ],
    [ "Modules", "modules.html", [
      [ "Modules List", "modules.html", "modules_dup" ]
    ] ],
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
    [ "Concepts", "concepts.html", "concepts" ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", "functions_vars" ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ],
    [ "Examples", "examples.html", "examples" ]
  ] ]
];

var NAVTREEINDEX =
[
"01-example.html",
"classkcenon_1_1logger_1_1adapters_1_1logger__adapter.html#a5a20e6b539a493efbc1c2f6ccedeea1a",
"classkcenon_1_1logger_1_1async_1_1async__worker.html#a345c26aa8fe977c79b6c6ef6926d2e05",
"classkcenon_1_1logger_1_1async_1_1lockfree__spsc__queue.html#afb5593aa1a281823523c5e6c93a42ebf",
"classkcenon_1_1logger_1_1batch__writer.html#ade402b458a1ddd63cbfb4d9fee97a49d",
"classkcenon_1_1logger_1_1core_1_1logger__registry.html#a59d0fdccc528658bc6db40e0253a63bf",
"classkcenon_1_1logger_1_1file__writer.html#ac339c57861822580dce95c6010599cc8",
"classkcenon_1_1logger_1_1formatter__factory.html#a8e4be22abcb1f64e5f7e6915616935d1",
"classkcenon_1_1logger_1_1log__decryptor.html#a0fd42e2935c78c0ca42710739cac6fff",
"classkcenon_1_1logger_1_1logger__builder.html#a32b27f0f8fb0109fd8fcfefe0884975f",
"classkcenon_1_1logger_1_1monitoring__interface.html#af1fd37f55e242866fbf4d3ba10640410",
"classkcenon_1_1logger_1_1queued__writer__base.html#a28370f7b44fffffa05bea41d566d86fe",
"classkcenon_1_1logger_1_1sampling_1_1log__sampler.html#a35ffd1cfaf355300be016cfe3f77cb41",
"classkcenon_1_1logger_1_1security_1_1log__sanitizer.html#a706520ac3b54f03a522f9d4fcb0722cc",
"classkcenon_1_1logger_1_1structured_1_1basic__structured__logger.html#abad096944f743416c5bb4e7117a7e2e0",
"classkcenon_1_1logger_1_1utils_1_1time__utils.html#a26321675ddbd5ee99407238042fdbdd3",
"distributed__logging__demo_8cpp.html#ae66f6b31b5ad750f1fe042a706a4e3d4",
"if-example.html",
"md_docs_2API__QUICK__REFERENCE.html#autotoc_md802",
"md_docs_2ARCHITECTURE.html#autotoc_md1091",
"md_docs_2CHANGELOG_8kr.html#autotoc_md1261",
"md_docs_2FEATURES_8kr.html#autotoc_md1688",
"md_docs_2PROJECT__STRUCTURE.html#autotoc_md2978",
"md_docs_2TRACEABILITY.html#autotoc_md3215",
"md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md304",
"md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md514",
"md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1866",
"md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2187",
"md_docs_2guides_2TROUBLESHOOTING.html#autotoc_md2437",
"module__kcenon_8logger.html#a77729c081f2b1c32cba29bbd81daa96b",
"namespacekcenon_1_1logger.html#ac1734ee737d5da06db8a1ad9236b0ff0ab7a582af4c2e181215b55161560713e5",
"structkcenon_1_1common_1_1error__info.html#ab4c30ba3bd7e78e2d7ae4007d02a304a",
"structkcenon_1_1logger_1_1async_1_1high__performance__async__writer_1_1queued__log__entry.html#ad43c053ec8080ddf69308116038d2bd3",
"structkcenon_1_1logger_1_1logger__config.html#a48af226491c0e7fdfe2aedd87a10cd01",
"structkcenon_1_1logger_1_1otlp__writer_1_1config.html#aaf20652d6b03fe8ee8fbb0bce7d8b0fd",
"thread__safe__writer_8h_source.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';