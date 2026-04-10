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
        [ "Quick Start", "md_README.html#autotoc_md4", [
          [ "Basic Example", "md_README.html#autotoc_md5", null ],
          [ "Composing Writers with Decorator Pattern", "md_README.html#autotoc_md6", [
            [ "Decorator Application Order", "md_README.html#autotoc_md7", null ],
            [ "Common Usage Patterns", "md_README.html#autotoc_md8", null ]
          ] ],
          [ "Installation", "md_README.html#autotoc_md9", null ],
          [ "Requirements", "md_README.html#autotoc_md10", [
            [ "Optional Feature Dependencies", "md_README.html#autotoc_md11", null ],
            [ "Development and Benchmark Dependencies", "md_README.html#autotoc_md12", null ],
            [ "Dependency Flow", "md_README.html#autotoc_md13", null ],
            [ "Building with Dependencies", "md_README.html#autotoc_md14", null ]
          ] ]
        ] ],
        [ "Core Features", "md_README.html#autotoc_md16", [
          [ "Asynchronous Logging", "md_README.html#autotoc_md17", null ],
          [ "Multiple Writer Types", "md_README.html#autotoc_md18", null ],
          [ "OpenTelemetry Integration (v3.0.0)", "md_README.html#autotoc_md19", null ],
          [ "Security Features (v3.0.0)", "md_README.html#autotoc_md20", null ],
          [ "Structured Logging (v3.1.0)", "md_README.html#autotoc_md21", null ]
        ] ],
        [ "Performance Highlights", "md_README.html#autotoc_md23", [
          [ "Throughput", "md_README.html#autotoc_md24", null ],
          [ "Latency", "md_README.html#autotoc_md25", null ],
          [ "Memory Efficiency", "md_README.html#autotoc_md26", null ]
        ] ],
        [ "Architecture Overview", "md_README.html#autotoc_md28", [
          [ "Modular Design", "md_README.html#autotoc_md29", null ],
          [ "Key Components", "md_README.html#autotoc_md30", null ]
        ] ],
        [ "Ecosystem Integration", "md_README.html#autotoc_md32", [
          [ "Ecosystem Dependency Map", "md_README.html#autotoc_md33", null ],
          [ "Dependencies", "md_README.html#autotoc_md34", null ],
          [ "Integration Pattern", "md_README.html#autotoc_md35", null ]
        ] ],
        [ "C++20 Module Support", "md_README.html#autotoc_md37", [
          [ "Requirements for Modules", "md_README.html#autotoc_md38", null ],
          [ "Building with Modules", "md_README.html#autotoc_md39", null ],
          [ "Using Modules", "md_README.html#autotoc_md40", null ],
          [ "Module Structure", "md_README.html#autotoc_md41", null ]
        ] ],
        [ "", "md_README.html#autotoc_md42", null ],
        [ "Documentation", "md_README.html#autotoc_md43", [
          [ "Getting Started", "md_README.html#autotoc_md44", null ],
          [ "Core Documentation", "md_README.html#autotoc_md45", null ],
          [ "Advanced Topics", "md_README.html#autotoc_md46", null ],
          [ "Development", "md_README.html#autotoc_md47", null ]
        ] ],
        [ "Configuration Templates", "md_README.html#autotoc_md49", [
          [ "Advanced Configuration", "md_README.html#autotoc_md50", null ]
        ] ],
        [ "Build Configuration", "md_README.html#autotoc_md52", [
          [ "CMake Feature Flags", "md_README.html#autotoc_md53", null ]
        ] ],
        [ "Platform Support", "md_README.html#autotoc_md55", [
          [ "Officially Supported", "md_README.html#autotoc_md56", null ]
        ] ],
        [ "Testing", "md_README.html#autotoc_md58", [
          [ "Test Coverage", "md_README.html#autotoc_md59", null ],
          [ "Running Tests", "md_README.html#autotoc_md60", null ],
          [ "CI/CD Status", "md_README.html#autotoc_md61", null ]
        ] ],
        [ "Contributing", "md_README.html#autotoc_md63", [
          [ "Development Workflow", "md_README.html#autotoc_md64", null ],
          [ "Code Standards", "md_README.html#autotoc_md65", null ]
        ] ],
        [ "Support", "md_README.html#autotoc_md67", null ],
        [ "License", "md_README.html#autotoc_md69", null ],
        [ "Acknowledgments", "md_README.html#autotoc_md71", null ]
      ] ]
    ] ],
    [ "Security Policy", "md_SECURITY.html", [
      [ "Supported Versions", "md_SECURITY.html#autotoc_md74", null ],
      [ "Reporting a Vulnerability", "md_SECURITY.html#autotoc_md75", [
        [ "What to Include", "md_SECURITY.html#autotoc_md76", null ],
        [ "Response Timeline", "md_SECURITY.html#autotoc_md77", null ],
        [ "After Reporting", "md_SECURITY.html#autotoc_md78", null ]
      ] ],
      [ "Security Best Practices", "md_SECURITY.html#autotoc_md79", null ]
    ] ],
    [ "ADR-001-decorator-pattern-adoption", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html", [
      [ "ADR-001: Decorator Pattern for Writer Composition", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md85", [
        [ "autotoc_md83", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md83", null ],
        [ "category: \"ADR\"", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md84", null ],
        [ "Context", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md86", null ],
        [ "Decision", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md87", null ],
        [ "Alternatives Considered", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md88", [
          [ "Monolithic Writer Classes", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md89", null ],
          [ "Template-Based Policy Composition (Mixin)", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md90", null ],
          [ "Strategy Pattern with Feature Flags", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md91", null ]
        ] ],
        [ "Consequences", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md92", [
          [ "Positive", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md93", null ],
          [ "Negative", "md_docs_2adr_2ADR-001-decorator-pattern-adoption.html#autotoc_md94", null ]
        ] ]
      ] ]
    ] ],
    [ "ADR-002-async-writer-architecture", "md_docs_2adr_2ADR-002-async-writer-architecture.html", [
      [ "ADR-002: Async Writer Architecture", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md97", [
        [ "autotoc_md95", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md95", null ],
        [ "category: \"ADR\"", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md96", null ],
        [ "Context", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md98", null ],
        [ "Decision", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md99", null ],
        [ "Alternatives Considered", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md100", [
          [ "Always Use std::jthread", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md101", null ],
          [ "Always Require thread_system", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md102", null ],
          [ "Lock-Free Ring Buffer", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md103", null ]
        ] ],
        [ "Consequences", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md104", [
          [ "Positive", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md105", null ],
          [ "Negative", "md_docs_2adr_2ADR-002-async-writer-architecture.html#autotoc_md106", null ]
        ] ]
      ] ]
    ] ],
    [ "ARCHITECTURE", "md_docs_2advanced_2ARCHITECTURE.html", [
      [ "Architecture - Logger System", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md109", [
        [ "autotoc_md107", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md107", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md108", null ],
        [ "Overview", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md110", null ],
        [ "Table of Contents", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md112", null ],
        [ "Design Philosophy", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md114", [
          [ "Core Principles", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md115", null ]
        ] ],
        [ "Layered Architecture", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md117", [
          [ "Layer Responsibilities", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md118", null ]
        ] ],
        [ "Core Components", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md120", [
          [ "Logger (core/logger.h, core/logger.cpp)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md121", null ],
          [ "Logger Builder (core/logger_builder.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md123", null ],
          [ "Log Collector (core/log_collector.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md125", null ],
          [ "Writers", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md127", [
            [ "Console Writer (writers/console_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md128", null ],
            [ "File Writer (writers/file_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md129", null ],
            [ "Rotating File Writer (writers/rotating_file_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md130", null ],
            [ "Network Writer (writers/network_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md131", null ],
            [ "Critical Writer (writers/critical_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md132", null ],
            [ "Async Writer (writers/async_writer.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md133", null ]
          ] ],
          [ "Formatters", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md135", [
            [ "Base Formatter (formatters/base_formatter.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md136", null ],
            [ "JSON Formatter", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md137", null ],
            [ "XML Formatter", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md138", null ]
          ] ],
          [ "Filters", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md140", [
            [ "Level Filter (filters/log_filter.h)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md141", null ],
            [ "Regex Filter", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md142", null ],
            [ "Function Filter", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md143", null ]
          ] ]
        ] ],
        [ "Integration Architecture", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md145", [
          [ "Common System Integration", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md146", null ],
          [ "Thread System Integration", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md147", null ],
          [ "Monitoring System Integration", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md148", null ]
        ] ],
        [ "Threading Model", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md150", [
          [ "Asynchronous Processing", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md151", null ],
          [ "Thread Safety Guarantees", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md152", null ]
        ] ],
        [ "Memory Management", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md154", [
          [ "Small String Optimization (SSO)", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md155", null ],
          [ "Log Entry Structure", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md156", null ]
        ] ],
        [ "Performance Characteristics", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md158", [
          [ "Benchmarks", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md159", null ],
          [ "Optimization Techniques", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md160", null ]
        ] ],
        [ "Design Patterns", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md162", [
          [ "Observer Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md163", null ],
          [ "Strategy Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md164", null ],
          [ "Builder Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md165", null ],
          [ "Template Method Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md166", null ],
          [ "Dependency Injection", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md167", null ],
          [ "Factory Pattern", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md168", null ]
        ] ],
        [ "Build Configuration", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md170", [
          [ "CMake Options", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md171", null ],
          [ "Dependency Resolution", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md172", null ],
          [ "Compiler Requirements", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md173", null ]
        ] ],
        [ "Future Enhancements", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md175", [
          [ "Planned Features", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md176", null ],
          [ "Research Areas", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md177", null ]
        ] ],
        [ "Error Code Allocation", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md179", null ],
        [ "References", "md_docs_2advanced_2ARCHITECTURE.html#autotoc_md181", null ]
      ] ]
    ] ],
    [ "ASYNC_WRITERS.kr", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html", [
      [ "비동기 Writer 구현체 가이드", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md185", [
        [ "autotoc_md183", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md183", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md184", null ],
        [ "개요", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md186", null ],
        [ "목차", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md188", null ],
        [ "빠른 선택 가이드", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md190", [
          [ "선택 플로우차트", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md191", null ]
        ] ],
        [ "구현체 비교", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md193", null ],
        [ "상세 설명", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md195", [
          [ "async_writer (공개 API)", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md196", [
            [ "주요 특징", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md197", null ],
            [ "아키텍처", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md198", null ],
            [ "API 레퍼런스", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md199", null ],
            [ "사용 시기", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md200", null ]
          ] ],
          [ "async_worker (내부)", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md202", [
            [ "주요 특징", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md203", null ],
            [ "아키텍처", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md204", null ],
            [ "사용 시기", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md205", null ]
          ] ]
        ] ],
        [ "", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md206", [
          [ "high_performance_async_writer (내부)", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md207", [
            [ "주요 특징", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md208", null ],
            [ "아키텍처", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md209", null ],
            [ "설정", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md210", null ],
            [ "사용 시기", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md211", null ]
          ] ]
        ] ],
        [ "", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md212", null ],
        [ "성능 특성", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md213", [
          [ "벤치마크 비교", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md214", null ],
          [ "지연 시간 분포", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md215", null ]
        ] ],
        [ "사용 예제", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md217", [
          [ "예제 1: 기본 async_writer 사용", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md218", null ],
          [ "예제 2: 콘솔 출력과 async_writer", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md219", null ],
          [ "예제 3: 큐 가득 참 처리", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md220", null ],
          [ "예제 4: 정상 종료 패턴", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md221", null ]
        ] ],
        [ "모범 사례", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md223", [
          [ "큐 크기 선택", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md224", null ],
          [ "플러시 타임아웃 설정", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md225", null ],
          [ "에러 처리", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md226", null ],
          [ "생명주기 관리", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md227", null ],
          [ "스레드 안전성 고려사항", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md228", null ]
        ] ],
        [ "마이그레이션 가이드", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md230", [
          [ "동기에서 async_writer로", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md231", null ],
          [ "큐 가득 참 마이그레이션", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md232", null ]
        ] ],
        [ "관련 문서", "md_docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md234", null ]
      ] ]
    ] ],
    [ "ASYNC_WRITERS", "md_docs_2advanced_2ASYNC__WRITERS.html", [
      [ "Asynchronous Writer Implementations Guide", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md238", [
        [ "autotoc_md236", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md236", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md237", null ],
        [ "Overview", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md239", null ],
        [ "Table of Contents", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md241", null ],
        [ "Quick Decision Guide", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md243", [
          [ "Decision Flowchart", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md244", null ]
        ] ],
        [ "Implementation Comparison", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md246", null ],
        [ "Detailed Descriptions", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md248", [
          [ "async_writer (Public API)", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md249", [
            [ "Key Features", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md250", null ],
            [ "Architecture", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md251", null ],
            [ "API Reference", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md252", null ],
            [ "When to Use", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md253", null ]
          ] ],
          [ "async_worker (Internal)", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md255", [
            [ "Key Features", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md256", null ],
            [ "Architecture", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md257", null ],
            [ "When to Use", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md258", null ]
          ] ]
        ] ],
        [ "", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md259", [
          [ "high_performance_async_writer (Internal)", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md260", [
            [ "Key Features", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md261", null ],
            [ "Architecture", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md262", null ],
            [ "Configuration", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md263", null ],
            [ "When to Use", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md264", null ]
          ] ]
        ] ],
        [ "", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md265", null ],
        [ "Performance Characteristics", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md266", [
          [ "Benchmark Comparison", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md267", null ],
          [ "Latency Distribution", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md268", null ]
        ] ],
        [ "Usage Examples", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md270", [
          [ "Example 1: Basic async_writer Usage", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md271", null ],
          [ "Example 2: async_writer with Console Output", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md272", null ],
          [ "Example 3: Queue Full Handling", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md273", null ],
          [ "Example 4: Graceful Shutdown Pattern", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md274", null ]
        ] ],
        [ "Best Practices", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md276", [
          [ "Queue Size Selection", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md277", null ],
          [ "Flush Timeout Configuration", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md278", null ],
          [ "Error Handling", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md279", null ],
          [ "Lifecycle Management", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md280", null ],
          [ "Thread Safety Considerations", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md281", null ]
        ] ],
        [ "Migration Guide", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md283", [
          [ "From Synchronous to async_writer", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md284", null ],
          [ "Queue Full Migration", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md285", null ]
        ] ],
        [ "Related Documentation", "md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md287", null ]
      ] ]
    ] ],
    [ "CI_CD_DASHBOARD.kr", "md_docs_2advanced_2CI__CD__DASHBOARD_8kr.html", [
      [ "Logger System CI/CD 대시보드", "md_docs_2advanced_2CI__CD__DASHBOARD_8kr.html#autotoc_md291", null ]
    ] ],
    [ "CI_CD_DASHBOARD", "md_docs_2advanced_2CI__CD__DASHBOARD.html", [
      [ "Logger System CI/CD Dashboard", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md296", [
        [ "autotoc_md294", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md294", null ],
        [ "category: \"PROJ\"", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md295", null ],
        [ "🚀 Build Status", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md297", [
          [ "Main Branch", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md298", null ]
        ] ],
        [ "📊 Performance Metrics", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md299", [
          [ "Latest Benchmark Results", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md300", null ],
          [ "Performance Trends", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md301", null ]
        ] ],
        [ "🔍 Code Quality", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md302", [
          [ "Static Analysis", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md303", null ],
          [ "Test Coverage", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md304", null ]
        ] ],
        [ "🧪 Test Results", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md305", [
          [ "Test Execution Summary", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md306", null ],
          [ "Recent Test Failures", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md307", null ]
        ] ],
        [ "🔥 Sanitizer Results", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md308", null ],
        [ "📈 Build Time Analysis", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md309", [
          [ "Average Build Times", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md310", null ]
        ] ],
        [ "🚨 Recent Issues", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md311", [
          [ "Build Failures (Last 7 Days)", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md312", null ],
          [ "Performance Regressions", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md313", null ]
        ] ],
        [ "📝 Dependency Status", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md314", [
          [ "External Dependencies", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md315", null ]
        ] ],
        [ "🔄 CI/CD Pipeline Health", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md316", [
          [ "Workflow Execution Stats (Last 30 Days)", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md317", null ],
          [ "Resource Usage", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md318", null ]
        ] ],
        [ "📅 Release History", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md319", [
          [ "Recent Releases", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md320", null ]
        ] ],
        [ "🔗 Quick Links", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md321", null ],
        [ "📊 Dashboard Update", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md322", null ],
        [ "This dashboard is automatically updated by CI/CD workflows. For real-time data, check the GitHub Acti...", "md_docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md324", null ]
      ] ]
    ] ],
    [ "CONDITIONAL_COMPILATION_REFACTORING", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html", [
      [ "Conditional Compilation Refactoring Strategy", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md327", [
        [ "autotoc_md325", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md325", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md326", null ],
        [ "Problem Statement", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md328", [
          [ "Current State", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md329", null ],
          [ "Issues", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md330", null ],
          [ "Example of Current Pattern", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md331", null ]
        ] ],
        [ "Proposed Solution: Adapter Pattern", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md332", [
          [ "Architecture", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md333", null ],
          [ "Benefits", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md334", null ],
          [ "Trade-offs", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md335", null ]
        ] ],
        [ "Implementation Plan", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md336", [
          [ "Phase 3.1: Analysis and Documentation ✅", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md337", null ],
          [ "Phase 3.2: Core Interface Design", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md338", null ],
          [ "Phase 3.3: Adapter Implementation", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md339", null ],
          [ "Phase 3.4: Migration", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md340", null ],
          [ "Phase 3.5: Validation", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md341", null ]
        ] ],
        [ "Detailed Design", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md342", [
          [ "Core Logger Interface", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md343", null ],
          [ "Adapter Interface", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md344", null ],
          [ "Thread System Adapter", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md345", null ],
          [ "Common System Adapter", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md346", null ],
          [ "Factory Pattern", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md347", null ]
        ] ],
        [ "Migration Strategy", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md348", [
          [ "Step 1: Extract Core (1 week)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md349", null ],
          [ "Step 2: Implement Adapters (1 week)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md350", null ],
          [ "Step 3: Update Public Interface (3 days)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md351", null ],
          [ "Step 4: Remove Conditional Compilation (2 days)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md352", null ],
          [ "Step 5: Testing and Validation (3 days)", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md353", null ]
        ] ],
        [ "Performance Considerations", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md354", [
          [ "Virtual Function Overhead", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md355", null ],
          [ "Binary Size Impact", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md356", null ]
        ] ],
        [ "Risk Mitigation", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md357", [
          [ "Risks", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md358", null ],
          [ "Rollback Plan", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md359", null ]
        ] ],
        [ "Success Criteria", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md360", null ],
        [ "References", "md_docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md361", null ]
      ] ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION.kr", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html", [
      [ "크리티컬 로그 손실 방지 가이드", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html#autotoc_md365", null ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html", [
      [ "Critical Log Loss Prevention Guide", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md371", [
        [ "autotoc_md368", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md368", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md369", null ],
        [ "Table of Contents", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md370", null ],
        [ "Overview", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md372", [
          [ "Problem", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md373", null ],
          [ "Solution", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md374", null ]
        ] ],
        [ "Architecture", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md376", null ],
        [ "Class Structure", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md378", [
          [ "critical_writer", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md379", [
            [ "Key Features:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md380", null ],
            [ "Configuration:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md381", null ],
            [ "Usage Example:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md382", null ]
          ] ],
          [ "hybrid_writer", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md383", [
            [ "Features:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md384", null ],
            [ "Usage Example:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md385", null ]
          ] ]
        ] ],
        [ "How It Works", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md387", [
          [ "Level-Based Routing", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md388", null ],
          [ "Critical Write Flow", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md389", [
            [ "Step-by-Step Explanation:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md390", null ]
          ] ],
          [ "Write-Ahead Logging (WAL)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md391", [
            [ "WAL Format:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md392", null ],
            [ "WAL Usage:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md393", null ]
          ] ],
          [ "Signal Handler", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md394", [
            [ "Handled Signals:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md395", null ],
            [ "Signal Handler Implementation:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md396", null ],
            [ "Important Notes:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md397", null ]
          ] ],
          [ "File Descriptor Sync", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md398", [
            [ "Buffer Layers:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md399", null ],
            [ "Implementation:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md400", null ]
          ] ]
        ] ],
        [ "Performance Impact", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md402", [
          [ "Critical Log Overhead", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md403", null ],
          [ "Normal Log Impact", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md404", null ],
          [ "Optimization Strategies", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md405", [
            [ "Strategy 1: Disable sync_on_critical", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md406", null ],
            [ "Strategy 2: Disable force_flush_on_error", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md407", null ],
            [ "Strategy 3: Use Hybrid Writer", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md408", null ]
          ] ],
          [ "Benchmark Results (Estimated)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md409", null ]
        ] ],
        [ "Production Recommendations", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md411", [
          [ "Default Configuration (General Services)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md412", null ],
          [ "High Reliability Configuration (Finance/Medical)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md413", null ],
          [ "High Performance Configuration (Games/Real-time Systems)", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md414", null ]
        ] ],
        [ "Testing & Verification", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md416", [
          [ "Critical Log Loss Test", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md417", [
            [ "Verification Method:", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md418", null ]
          ] ],
          [ "WAL Recovery Test", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md419", null ],
          [ "Performance Benchmark", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md420", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md422", [
          [ "Issue 1: WAL File Not Created", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md423", null ],
          [ "Issue 2: Signal Handler Not Working", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md424", null ],
          [ "Issue 3: Performance Degradation", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md425", null ]
        ] ],
        [ "FAQ", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md427", [
          [ "Q1: Should I wrap all logs with critical_writer?", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md428", null ],
          [ "Q2: When should I use WAL?", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md429", null ],
          [ "Q3: fsync() overhead is too high.", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md430", null ],
          [ "Q4: What can I do in a signal handler?", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md431", null ]
        ] ],
        [ "References", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md433", null ],
        [ "Version History", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md435", null ],
        [ "License", "md_docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md437", null ]
      ] ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START.kr", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html", [
      [ "크리티컬 로깅 빠른 시작", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html#autotoc_md441", null ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html", [
      [ "Critical Logging Quick Start", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md447", [
        [ "autotoc_md444", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md444", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md445", null ],
        [ "Table of Contents", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md446", null ],
        [ "🎯 Goal", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md449", null ],
        [ "🚀 Quick Start", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md451", [
          [ "Step 1: Include Headers", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md452", null ],
          [ "Step 2: Create Hybrid Writer", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md453", null ],
          [ "Step 3: Verify", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md454", null ]
        ] ],
        [ "📊 3 Usage Scenarios", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md456", [
          [ "Scenario 1: General Web Services (Recommended)", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md457", null ],
          [ "Scenario 2: Finance/Medical Systems (Maximum Reliability)", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md458", null ],
          [ "Scenario 3: Games/Real-time Systems (High Performance)", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md459", null ]
        ] ],
        [ "🔧 Configuration Options", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md461", null ],
        [ "✅ Verification Methods", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md463", [
          [ "Test 1: Check Critical Logs", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md464", null ],
          [ "Test 2: Crash Simulation", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md465", null ],
          [ "Test 3: Ctrl+C Test", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md466", null ]
        ] ],
        [ "📚 Learn More", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md468", null ],
        [ "🐛 Troubleshooting", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md470", [
          [ "\"WAL file not created\"", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md471", null ],
          [ "\"Performance too slow\"", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md472", null ],
          [ "\"Signal handler not working\"", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md473", null ]
        ] ],
        [ "💡 Key Points", "md_docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md475", null ]
      ] ]
    ] ],
    [ "CUSTOM_WRITERS.kr", "md_docs_2advanced_2CUSTOM__WRITERS_8kr.html", [
      [ "커스텀 작성기 생성", "md_docs_2advanced_2CUSTOM__WRITERS_8kr.html#autotoc_md480", null ]
    ] ],
    [ "CUSTOM_WRITERS", "md_docs_2advanced_2CUSTOM__WRITERS.html", [
      [ "Creating Custom Writers", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md485", [
        [ "autotoc_md483", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md483", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md484", null ],
        [ "Overview", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md486", null ],
        [ "Writer Hierarchy", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md487", null ],
        [ "Recommended: Using thread_safe_writer (Since v1.3.0)", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md488", [
          [ "Benefits of thread_safe_writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md489", null ]
        ] ],
        [ "Base Writer Interface", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md490", null ],
        [ "Simple Examples", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md491", [
          [ "Simple File Writer (Using thread_safe_writer)", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md492", null ],
          [ "Rotating File Writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md493", null ],
          [ "Network Writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md494", null ],
          [ "Database Writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md495", null ]
        ] ],
        [ "Advanced Patterns", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md496", [
          [ "Filtering Writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md497", null ],
          [ "Async Writer Wrapper", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md498", null ]
        ] ],
        [ "Choosing the Right Base Class", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md499", [
          [ "When to Use thread_safe_writer", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md500", null ],
          [ "When to Use base_writer Directly", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md501", null ]
        ] ],
        [ "Best Practices", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md502", null ],
        [ "Testing Custom Writers", "md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md503", null ]
      ] ]
    ] ],
    [ "LOG_LEVEL_SEMANTIC_STANDARD", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html", [
      [ "Log Level Semantic Standard", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md507", [
        [ "autotoc_md505", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md505", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md506", null ],
        [ "Problem Statement", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md509", [
          [ "logger_system (Ascending: Low → High)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md510", null ],
          [ "thread_system (Descending: High → Low)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md511", null ],
          [ "Impact", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md512", null ]
        ] ],
        [ "Decision", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md514", [
          [ "Rationale", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md515", null ]
        ] ],
        [ "Migration Plan", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md517", [
          [ "Phase 1: Documentation (Sprint 1 - Current)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md518", null ],
          [ "Phase 2: thread_system Unification (Sprint 2)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md519", null ],
          [ "Phase 3: Adapter Simplification (Sprint 3)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md520", null ]
        ] ],
        [ "Implementation Details", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md522", [
          [ "Standard Definition (logger_system - already compliant)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md523", null ],
          [ "thread_system Changes (Sprint 2)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md524", null ],
          [ "Adapter Simplification (Sprint 3)", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md525", null ]
        ] ],
        [ "Compatibility Considerations", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md527", [
          [ "Breaking Change Assessment", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md528", null ],
          [ "Mitigation Strategy", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md529", null ],
          [ "User Impact", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md530", null ]
        ] ],
        [ "Testing Requirements", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md532", [
          [ "Unit Tests", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md533", null ],
          [ "Integration Tests", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md534", null ],
          [ "Regression Tests", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md535", null ]
        ] ],
        [ "Documentation Updates Required", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md537", null ],
        [ "Success Criteria", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md539", null ],
        [ "Timeline", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md541", null ],
        [ "References", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md543", null ],
        [ "Approval", "md_docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md545", null ]
      ] ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE.kr", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html", [
      [ "로거 시스템 아키텍처", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md549", [
        [ "autotoc_md547", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md547", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md548", null ],
        [ "개요", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md550", [
          [ "주요 기능 (v3.0)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md551", null ]
        ] ],
        [ "아키텍처 다이어그램", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md552", null ],
        [ "핵심 컴포넌트", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md553", [
          [ "ILogger 인터페이스 구현", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md554", null ],
          [ "Dual API 설계", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md555", null ],
          [ "백엔드 추상화", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md556", null ]
        ] ],
        [ "성능 특성", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md557", [
          [ "벤치마크 (v3.0)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md558", null ],
          [ "멀티스레드 성능", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md559", null ]
        ] ]
      ] ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html", [
      [ "Logger System Architecture", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md564", [
        [ "autotoc_md562", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md562", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md563", null ],
        [ "Table of Contents", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md565", null ],
        [ "Overview", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md566", [
          [ "Key Features (v3.0)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md567", null ]
        ] ],
        [ "Architecture Diagram", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md568", null ],
        [ "Core Components", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md569", [
          [ "ILogger Interface Implementation", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md570", null ],
          [ "Dual API Design", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md571", null ],
          [ "Configuration Management", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md572", [
            [ "Logger Configuration with Validation", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md573", null ],
            [ "Configuration Templates", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md574", null ]
          ] ],
          [ "Builder Pattern with Validation", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md575", null ],
          [ "Backend Abstraction", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md576", null ],
          [ "Interface Segregation", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md577", [
            [ "Writer Interface", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md578", null ],
            [ "Filter Interface", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md579", null ],
            [ "Formatter Interface", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md580", null ]
          ] ],
          [ "Log Entry Structure", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md581", null ]
        ] ],
        [ "Advanced Features", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md582", [
          [ "Asynchronous Pipeline", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md583", null ],
          [ "Error Handling with Result Pattern", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md584", null ],
          [ "C++20 Source Location", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md585", null ],
          [ "Performance Monitoring", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md586", null ],
          [ "Configuration Strategies", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md587", null ]
        ] ],
        [ "Threading Model", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md588", [
          [ "Synchronous Mode", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md589", null ],
          [ "Asynchronous Mode (Default)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md590", null ],
          [ "Thread Safety Guarantees", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md591", null ]
        ] ],
        [ "Memory Management", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md592", [
          [ "Buffer Management", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md593", null ],
          [ "Object Lifetime", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md594", null ]
        ] ],
        [ "Performance Characteristics", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md595", [
          [ "Benchmarks (v3.0)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md596", null ],
          [ "Multi-threaded Performance", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md597", null ],
          [ "Optimization Strategies", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md598", null ]
        ] ],
        [ "Integration Patterns", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md599", [
          [ "ILogger Interface Integration", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md600", null ],
          [ "Dependency Injection", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md601", null ],
          [ "With Monitoring (Phase 2.2.4)", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md602", null ]
        ] ],
        [ "Extension Points", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md603", [
          [ "Custom Writers", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md604", null ],
          [ "Custom Filters", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md605", null ],
          [ "Custom Formatters", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md606", null ]
        ] ],
        [ "Future Enhancements", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md607", [
          [ "Performance Improvements", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md608", null ],
          [ "Feature Additions", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md609", null ],
          [ "Platform Extensions", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md610", null ]
        ] ],
        [ "Best Practices", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md611", [
          [ "For Library Users", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md612", null ],
          [ "For Contributors", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md613", null ]
        ] ],
        [ "Platform Notes", "md_docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md614", null ]
      ] ]
    ] ],
    [ "MIGRATION", "md_docs_2advanced_2MIGRATION.html", [
      [ "Migration Guide - Logger System", "md_docs_2advanced_2MIGRATION.html#autotoc_md618", [
        [ "autotoc_md616", "md_docs_2advanced_2MIGRATION.html#autotoc_md616", null ],
        [ "category: \"MIGR\"", "md_docs_2advanced_2MIGRATION.html#autotoc_md617", null ],
        [ "Overview", "md_docs_2advanced_2MIGRATION.html#autotoc_md619", null ],
        [ "Table of Contents", "md_docs_2advanced_2MIGRATION.html#autotoc_md621", null ],
        [ "Quick Migration Checklist", "md_docs_2advanced_2MIGRATION.html#autotoc_md623", null ],
        [ "Migration Path 1: From Integrated thread_system", "md_docs_2advanced_2MIGRATION.html#autotoc_md625", [
          [ "Background", "md_docs_2advanced_2MIGRATION.html#autotoc_md626", null ],
          [ "Step 1: Update CMakeLists.txt", "md_docs_2advanced_2MIGRATION.html#autotoc_md627", null ],
          [ "Step 2: Update Header Includes", "md_docs_2advanced_2MIGRATION.html#autotoc_md628", null ],
          [ "Step 3: Update Namespace", "md_docs_2advanced_2MIGRATION.html#autotoc_md629", null ],
          [ "Step 4: Migrate Logger Creation", "md_docs_2advanced_2MIGRATION.html#autotoc_md630", null ],
          [ "Step 5: Update Thread System Integration (Optional)", "md_docs_2advanced_2MIGRATION.html#autotoc_md631", null ]
        ] ],
        [ "Migration Path 2: From spdlog", "md_docs_2advanced_2MIGRATION.html#autotoc_md633", [
          [ "Overview", "md_docs_2advanced_2MIGRATION.html#autotoc_md634", null ],
          [ "API Mapping", "md_docs_2advanced_2MIGRATION.html#autotoc_md635", null ],
          [ "Example Migration", "md_docs_2advanced_2MIGRATION.html#autotoc_md636", null ],
          [ "Pattern Support", "md_docs_2advanced_2MIGRATION.html#autotoc_md637", null ]
        ] ],
        [ "Migration Path 3: From Custom Logger", "md_docs_2advanced_2MIGRATION.html#autotoc_md639", [
          [ "Common Custom Logger Patterns", "md_docs_2advanced_2MIGRATION.html#autotoc_md640", [
            [ "Pattern 1: Direct File Writing", "md_docs_2advanced_2MIGRATION.html#autotoc_md641", null ],
            [ "Pattern 2: Multiple Output Targets", "md_docs_2advanced_2MIGRATION.html#autotoc_md642", null ],
            [ "Pattern 3: Thread-Safe Singleton", "md_docs_2advanced_2MIGRATION.html#autotoc_md643", null ]
          ] ]
        ] ],
        [ "Common Migration Issues", "md_docs_2advanced_2MIGRATION.html#autotoc_md645", [
          [ "Issue 1: Namespace Conflicts", "md_docs_2advanced_2MIGRATION.html#autotoc_md646", null ],
          [ "Issue 2: CMake Cache Issues", "md_docs_2advanced_2MIGRATION.html#autotoc_md647", null ],
          [ "Issue 3: Header Not Found", "md_docs_2advanced_2MIGRATION.html#autotoc_md648", null ],
          [ "Issue 4: Result<T> Pattern Unfamiliar", "md_docs_2advanced_2MIGRATION.html#autotoc_md649", null ],
          [ "Issue 5: Performance Regression", "md_docs_2advanced_2MIGRATION.html#autotoc_md650", null ]
        ] ],
        [ "Testing Your Migration", "md_docs_2advanced_2MIGRATION.html#autotoc_md652", [
          [ "Compile Test", "md_docs_2advanced_2MIGRATION.html#autotoc_md653", null ],
          [ "Unit Tests", "md_docs_2advanced_2MIGRATION.html#autotoc_md654", null ],
          [ "Integration Tests", "md_docs_2advanced_2MIGRATION.html#autotoc_md655", null ],
          [ "Performance Benchmark", "md_docs_2advanced_2MIGRATION.html#autotoc_md656", null ]
        ] ],
        [ "Rollback Plan", "md_docs_2advanced_2MIGRATION.html#autotoc_md658", [
          [ "Option 1: Revert Changes", "md_docs_2advanced_2MIGRATION.html#autotoc_md659", null ],
          [ "Option 2: Gradual Migration", "md_docs_2advanced_2MIGRATION.html#autotoc_md660", null ]
        ] ],
        [ "Migration Assistance", "md_docs_2advanced_2MIGRATION.html#autotoc_md662", [
          [ "Automated Migration Script", "md_docs_2advanced_2MIGRATION.html#autotoc_md663", null ]
        ] ],
        [ "Support", "md_docs_2advanced_2MIGRATION.html#autotoc_md665", null ],
        [ "Next Steps After Migration", "md_docs_2advanced_2MIGRATION.html#autotoc_md667", null ]
      ] ]
    ] ],
    [ "STRUCTURE.kr", "md_docs_2advanced_2STRUCTURE_8kr.html", [
      [ "Logger System - 프로젝트 구조", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md671", [
        [ "autotoc_md669", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md669", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md670", null ],
        [ "📁 디렉토리 레이아웃", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md673", null ],
        [ "🏗️ Namespace 구조", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md674", [
          [ "Core Namespaces", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md675", null ],
          [ "Nested Namespaces", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md676", null ]
        ] ],
        [ "🔧 주요 컴포넌트 개요", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md677", [
          [ "🎯 Public API Layer (include/kcenon/logger/)", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md678", null ],
          [ "⚙️ Implementation Layer (src/)", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md679", null ]
        ] ],
        [ "📊 성능 특성", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md680", null ],
        [ "🔄 마이그레이션 가이드", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md681", [
          [ "Step 1: 현재 설정 백업", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md682", null ],
          [ "Step 2: Include 경로 업데이트", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md683", null ],
          [ "Step 3: Namespace 사용 업데이트", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md684", null ],
          [ "Step 4: 마이그레이션 스크립트 실행", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md685", null ]
        ] ],
        [ "🚀 새로운 구조로 빠른 시작", "md_docs_2advanced_2STRUCTURE_8kr.html#autotoc_md686", null ]
      ] ]
    ] ],
    [ "STRUCTURE", "md_docs_2advanced_2STRUCTURE.html", [
      [ "Logger System - Project Structure", "md_docs_2advanced_2STRUCTURE.html#autotoc_md689", [
        [ "autotoc_md687", "md_docs_2advanced_2STRUCTURE.html#autotoc_md687", null ],
        [ "category: \"ARCH\"", "md_docs_2advanced_2STRUCTURE.html#autotoc_md688", null ],
        [ "Table of Contents", "md_docs_2advanced_2STRUCTURE.html#autotoc_md691", null ],
        [ "Directory Layout", "md_docs_2advanced_2STRUCTURE.html#autotoc_md692", null ],
        [ "Namespace Organization", "md_docs_2advanced_2STRUCTURE.html#autotoc_md693", [
          [ "Primary Namespace Hierarchy", "md_docs_2advanced_2STRUCTURE.html#autotoc_md694", null ],
          [ "Namespace Usage Examples", "md_docs_2advanced_2STRUCTURE.html#autotoc_md695", null ]
        ] ],
        [ "Component Layout", "md_docs_2advanced_2STRUCTURE.html#autotoc_md696", [
          [ "Core Components", "md_docs_2advanced_2STRUCTURE.html#autotoc_md697", [
            [ "Logger Core (core/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md698", null ],
            [ "Interfaces (interfaces/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md699", null ],
            [ "Writers (writers/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md700", null ],
            [ "Formatters (formatters/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md701", null ],
            [ "Filters (filters/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md702", null ],
            [ "Routing (routing/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md703", null ],
            [ "Analysis (analysis/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md704", null ],
            [ "Structured Logging (structured/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md705", null ],
            [ "Safety (safety/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md706", null ],
            [ "Server (server/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md707", null ],
            [ "Adapters (adapters/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md708", null ]
          ] ],
          [ "Monitoring (core/monitoring/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md709", null ]
        ] ],
        [ "Module Dependencies", "md_docs_2advanced_2STRUCTURE.html#autotoc_md710", [
          [ "Internal Dependencies", "md_docs_2advanced_2STRUCTURE.html#autotoc_md711", null ],
          [ "External Dependencies", "md_docs_2advanced_2STRUCTURE.html#autotoc_md712", null ],
          [ "Compilation Units", "md_docs_2advanced_2STRUCTURE.html#autotoc_md713", null ]
        ] ],
        [ "Key Components Overview", "md_docs_2advanced_2STRUCTURE.html#autotoc_md714", [
          [ "Public API Layer (include/kcenon/logger/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md715", null ],
          [ "Implementation Layer (src/)", "md_docs_2advanced_2STRUCTURE.html#autotoc_md716", null ],
          [ "Test Organization", "md_docs_2advanced_2STRUCTURE.html#autotoc_md717", null ]
        ] ],
        [ "Performance Characteristics", "md_docs_2advanced_2STRUCTURE.html#autotoc_md718", [
          [ "Throughput", "md_docs_2advanced_2STRUCTURE.html#autotoc_md719", null ],
          [ "Memory", "md_docs_2advanced_2STRUCTURE.html#autotoc_md720", null ],
          [ "Threading", "md_docs_2advanced_2STRUCTURE.html#autotoc_md721", null ],
          [ "Resource Management", "md_docs_2advanced_2STRUCTURE.html#autotoc_md722", null ]
        ] ],
        [ "Build Configuration", "md_docs_2advanced_2STRUCTURE.html#autotoc_md723", [
          [ "CMake Targets", "md_docs_2advanced_2STRUCTURE.html#autotoc_md724", null ],
          [ "Feature Flags", "md_docs_2advanced_2STRUCTURE.html#autotoc_md725", null ]
        ] ],
        [ "References", "md_docs_2advanced_2STRUCTURE.html#autotoc_md726", null ]
      ] ]
    ] ],
    [ "WRITER_HIERARCHY", "md_docs_2advanced_2WRITER__HIERARCHY.html", [
      [ "Writer Hierarchy and Categories", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md729", [
        [ "autotoc_md727", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md727", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md728", null ],
        [ "Overview", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md730", null ],
        [ "Category System", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md731", [
          [ "Writer Categories", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md732", null ],
          [ "Category Tags", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md733", null ]
        ] ],
        [ "Complete Writer Hierarchy", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md734", null ],
        [ "Category Characteristics", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md735", [
          [ "Synchronous Writers (sync_writer_tag)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md736", null ],
          [ "Asynchronous Writers (async_writer_tag)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md737", null ],
          [ "Decorator Writers (decorator_writer_tag)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md738", null ],
          [ "Composite Writers (composite_writer_tag)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md739", null ]
        ] ],
        [ "Using Category Information", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md740", [
          [ "Compile-Time Type Traits", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md741", null ],
          [ "C++20 Concepts", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md742", null ],
          [ "Runtime Category Query", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md743", null ]
        ] ],
        [ "Design Patterns Used", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md744", [
          [ "Template Method Pattern (thread_safe_writer)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md745", null ],
          [ "Decorator Pattern", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md746", null ],
          [ "Pipeline Pattern (composite_writer)", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md747", null ]
        ] ],
        [ "Related Documentation", "md_docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md748", null ]
      ] ]
    ] ],
    [ "WRITER_SELECTION_GUIDE", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html", [
      [ "Writer Selection Guide", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md752", [
        [ "autotoc_md750", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md750", null ],
        [ "category: \"GUID\"", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md751", null ],
        [ "Quick Selection Flowchart", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md753", null ],
        [ "Writer Selection by Use Case", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md754", [
          [ "Simple Console Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md755", null ],
          [ "Basic File Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md757", null ],
          [ "Production File Logging with Rotation", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md759", null ],
          [ "High-Throughput Logging (>10K msg/sec)", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md761", null ],
          [ "Network/Remote Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md763", null ],
          [ "Encrypted/Secure Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md765", null ],
          [ "Critical/Safety-Critical Logging", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md767", null ],
          [ "Custom Pipeline (Formatter + Sink)", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md769", null ]
        ] ],
        [ "Decorator Stacking Patterns", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md771", [
          [ "Pattern 1: Async + Encrypted", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md772", null ],
          [ "Pattern 2: Critical + Async + Rotating", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md773", null ],
          [ "Pattern 3: Batch + Network", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md774", null ]
        ] ],
        [ "Performance Comparison", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md775", null ],
        [ "Decision Matrix", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md776", null ],
        [ "Related Documentation", "md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md777", null ]
      ] ]
    ] ],
    [ "API_QUICK_REFERENCE", "md_docs_2API__QUICK__REFERENCE.html", [
      [ "API Quick Reference", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md781", [
        [ "autotoc_md779", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md779", null ],
        [ "category: \"API\"", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md780", null ],
        [ "Logger Creation", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md782", [
          [ "Logger Builder", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md783", null ],
          [ "Config Builder", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md784", null ]
        ] ],
        [ "Logging Messages", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md785", [
          [ "Log Levels (low to high severity)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md786", null ]
        ] ],
        [ "Writer Builder (Decorator Chain)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md787", [
          [ "Adding Writers to Logger", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md788", null ],
          [ "Starting Async Writers", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md789", null ]
        ] ],
        [ "Core Writers", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md790", null ],
        [ "Structured Logging", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md791", [
          [ "Logger Context (applies to all subsequent logs)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md792", null ],
          [ "RAII Context Scopes (thread-local)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md793", null ],
          [ "Trace Context (distributed tracing)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md794", null ]
        ] ],
        [ "Filters", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md795", [
          [ "Per-Writer Filtering (via decorator)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md796", null ]
        ] ],
        [ "Formatters", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md797", null ],
        [ "Metrics", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md798", null ],
        [ "Critical Logging", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md799", [
          [ "Hybrid Writer (async normal + sync critical)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md800", null ]
        ] ],
        [ "Custom Writer", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md801", null ],
        [ "Composite Writer (SRP Architecture)", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md802", null ],
        [ "Header Quick Index", "md_docs_2API__QUICK__REFERENCE.html#autotoc_md803", null ]
      ] ]
    ] ],
    [ "API_REFERENCE.kr", "md_docs_2API__REFERENCE_8kr.html", [
      [ "API 레퍼런스", "md_docs_2API__REFERENCE_8kr.html#autotoc_md806", [
        [ "autotoc_md804", "md_docs_2API__REFERENCE_8kr.html#autotoc_md804", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE_8kr.html#autotoc_md805", null ],
        [ "v3.0 주요 변경사항", "md_docs_2API__REFERENCE_8kr.html#autotoc_md807", [
          [ "네임스페이스 변경", "md_docs_2API__REFERENCE_8kr.html#autotoc_md808", null ],
          [ "인터페이스 변경", "md_docs_2API__REFERENCE_8kr.html#autotoc_md809", null ],
          [ "헤더 경로 변경", "md_docs_2API__REFERENCE_8kr.html#autotoc_md810", null ],
          [ "주요 기능", "md_docs_2API__REFERENCE_8kr.html#autotoc_md811", null ]
        ] ],
        [ "빠른 시작", "md_docs_2API__REFERENCE_8kr.html#autotoc_md812", null ]
      ] ]
    ] ],
    [ "API_REFERENCE", "md_docs_2API__REFERENCE.html", [
      [ "API Reference", "md_docs_2API__REFERENCE.html#autotoc_md817", [
        [ "autotoc_md815", "md_docs_2API__REFERENCE.html#autotoc_md815", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE.html#autotoc_md816", null ],
        [ "Sub-Documents", "md_docs_2API__REFERENCE.html#autotoc_md818", null ],
        [ "Overview", "md_docs_2API__REFERENCE.html#autotoc_md819", null ],
        [ "Quick Links", "md_docs_2API__REFERENCE.html#autotoc_md820", [
          [ "Core", "md_docs_2API__REFERENCE.html#autotoc_md821", null ],
          [ "Writers and Filters", "md_docs_2API__REFERENCE.html#autotoc_md822", null ],
          [ "Security", "md_docs_2API__REFERENCE.html#autotoc_md823", null ]
        ] ]
      ] ]
    ] ],
    [ "API_REFERENCE_CORE", "md_docs_2API__REFERENCE__CORE.html", [
      [ "API Reference - Core", "md_docs_2API__REFERENCE__CORE.html#autotoc_md827", [
        [ "autotoc_md825", "md_docs_2API__REFERENCE__CORE.html#autotoc_md825", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE__CORE.html#autotoc_md826", null ],
        [ "Table of Contents", "md_docs_2API__REFERENCE__CORE.html#autotoc_md828", null ],
        [ "Overview", "md_docs_2API__REFERENCE__CORE.html#autotoc_md829", [
          [ "Namespace", "md_docs_2API__REFERENCE__CORE.html#autotoc_md830", null ]
        ] ],
        [ "Core Classes", "md_docs_2API__REFERENCE__CORE.html#autotoc_md831", [
          [ "kcenon::logger::logger", "md_docs_2API__REFERENCE__CORE.html#autotoc_md832", [
            [ "Constructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md833", null ],
            [ "Destructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md834", null ]
          ] ]
        ] ],
        [ "ILogger Interface (Phase 2.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md835", [
          [ "Log Methods (ILogger Interface)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md836", null ],
          [ "Level Management (ILogger Interface)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md837", null ],
          [ "Native API (Backward Compatible)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md838", null ],
          [ "Writer Management", "md_docs_2API__REFERENCE__CORE.html#autotoc_md839", null ],
          [ "Lifecycle Management", "md_docs_2API__REFERENCE__CORE.html#autotoc_md840", null ],
          [ "Metrics", "md_docs_2API__REFERENCE__CORE.html#autotoc_md841", null ],
          [ "Filtering", "md_docs_2API__REFERENCE__CORE.html#autotoc_md842", null ],
          [ "Emergency Flush (Signal Safety)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md843", null ]
        ] ],
        [ "Unified Log Context (v3.3.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md844", [
          [ "kcenon::logger::unified_log_context", "md_docs_2API__REFERENCE__CORE.html#autotoc_md845", [
            [ "Value Type", "md_docs_2API__REFERENCE__CORE.html#autotoc_md846", null ],
            [ "Context Categories", "md_docs_2API__REFERENCE__CORE.html#autotoc_md847", null ],
            [ "Constructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md848", null ],
            [ "Setters", "md_docs_2API__REFERENCE__CORE.html#autotoc_md849", null ],
            [ "Getters", "md_docs_2API__REFERENCE__CORE.html#autotoc_md850", null ],
            [ "Query Methods", "md_docs_2API__REFERENCE__CORE.html#autotoc_md851", null ],
            [ "Removal and Export", "md_docs_2API__REFERENCE__CORE.html#autotoc_md852", null ],
            [ "Usage Example", "md_docs_2API__REFERENCE__CORE.html#autotoc_md853", null ],
            [ "Thread Safety", "md_docs_2API__REFERENCE__CORE.html#autotoc_md854", null ]
          ] ]
        ] ],
        [ "Scoped Context Guard (v3.3.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md856", [
          [ "kcenon::logger::scoped_context_guard", "md_docs_2API__REFERENCE__CORE.html#autotoc_md857", [
            [ "Constructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md858", null ],
            [ "Setters (Chainable)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md859", null ],
            [ "Usage Example", "md_docs_2API__REFERENCE__CORE.html#autotoc_md860", null ]
          ] ]
        ] ],
        [ "Structured Log Builder (v3.1.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md862", [
          [ "kcenon::logger::structured_log_builder", "md_docs_2API__REFERENCE__CORE.html#autotoc_md863", [
            [ "Constructor", "md_docs_2API__REFERENCE__CORE.html#autotoc_md864", null ],
            [ "Builder Methods", "md_docs_2API__REFERENCE__CORE.html#autotoc_md865", null ],
            [ "Logger Integration (log_structured)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md866", null ],
            [ "Usage Example", "md_docs_2API__REFERENCE__CORE.html#autotoc_md867", null ],
            [ "Thread Safety", "md_docs_2API__REFERENCE__CORE.html#autotoc_md868", null ]
          ] ]
        ] ],
        [ "Writer Builder (v4.1.0)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md870", [
          [ "kcenon::logger::writer_builder", "md_docs_2API__REFERENCE__CORE.html#autotoc_md871", [
            [ "Core Writers (Terminal Nodes)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md872", null ],
            [ "Decorator Writers (Middleware)", "md_docs_2API__REFERENCE__CORE.html#autotoc_md873", null ],
            [ "Build and Utility", "md_docs_2API__REFERENCE__CORE.html#autotoc_md874", null ],
            [ "Usage Examples", "md_docs_2API__REFERENCE__CORE.html#autotoc_md875", null ]
          ] ]
        ] ],
        [ "Configuration", "md_docs_2API__REFERENCE__CORE.html#autotoc_md877", [
          [ "kcenon::logger::logger_config", "md_docs_2API__REFERENCE__CORE.html#autotoc_md878", null ],
          [ "Static Factory Methods", "md_docs_2API__REFERENCE__CORE.html#autotoc_md879", null ]
        ] ],
        [ "Builder Pattern", "md_docs_2API__REFERENCE__CORE.html#autotoc_md880", [
          [ "kcenon::logger::logger_builder", "md_docs_2API__REFERENCE__CORE.html#autotoc_md881", [
            [ "Basic Configuration", "md_docs_2API__REFERENCE__CORE.html#autotoc_md882", null ],
            [ "Feature Flags", "md_docs_2API__REFERENCE__CORE.html#autotoc_md883", null ],
            [ "Output Configuration", "md_docs_2API__REFERENCE__CORE.html#autotoc_md884", null ],
            [ "Writers and Filters", "md_docs_2API__REFERENCE__CORE.html#autotoc_md885", null ],
            [ "Backend Selection", "md_docs_2API__REFERENCE__CORE.html#autotoc_md886", null ],
            [ "Environment and Error Handling", "md_docs_2API__REFERENCE__CORE.html#autotoc_md887", null ],
            [ "Build", "md_docs_2API__REFERENCE__CORE.html#autotoc_md888", null ]
          ] ]
        ] ],
        [ "Configuration Strategies", "md_docs_2API__REFERENCE__CORE.html#autotoc_md889", [
          [ "Deployment Strategy", "md_docs_2API__REFERENCE__CORE.html#autotoc_md890", null ],
          [ "Performance Strategy", "md_docs_2API__REFERENCE__CORE.html#autotoc_md891", null ],
          [ "Environment Strategy", "md_docs_2API__REFERENCE__CORE.html#autotoc_md892", null ],
          [ "Composite Strategy", "md_docs_2API__REFERENCE__CORE.html#autotoc_md893", null ]
        ] ],
        [ "Interfaces", "md_docs_2API__REFERENCE__CORE.html#autotoc_md894", [
          [ "kcenon::logger::log_writer_interface", "md_docs_2API__REFERENCE__CORE.html#autotoc_md895", null ],
          [ "kcenon::logger::log_filter_interface", "md_docs_2API__REFERENCE__CORE.html#autotoc_md896", null ],
          [ "kcenon::logger::log_formatter_interface", "md_docs_2API__REFERENCE__CORE.html#autotoc_md897", null ],
          [ "kcenon::logger::log_entry", "md_docs_2API__REFERENCE__CORE.html#autotoc_md898", null ]
        ] ],
        [ "Error Handling", "md_docs_2API__REFERENCE__CORE.html#autotoc_md899", [
          [ "Error Codes", "md_docs_2API__REFERENCE__CORE.html#autotoc_md900", [
            [ "Error Code Categories Summary", "md_docs_2API__REFERENCE__CORE.html#autotoc_md901", null ]
          ] ],
          [ "Result Types", "md_docs_2API__REFERENCE__CORE.html#autotoc_md902", null ],
          [ "Helper Functions", "md_docs_2API__REFERENCE__CORE.html#autotoc_md903", [
            [ "kcenon::logger::result<T> Wrapper", "md_docs_2API__REFERENCE__CORE.html#autotoc_md904", null ]
          ] ]
        ] ]
      ] ]
    ] ],
    [ "API_REFERENCE_SECURITY", "md_docs_2API__REFERENCE__SECURITY.html", [
      [ "API Reference - Security", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md908", [
        [ "autotoc_md906", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md906", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md907", null ],
        [ "Table of Contents", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md909", null ],
        [ "Overview", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md910", null ],
        [ "Security Error Codes", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md912", null ],
        [ "Encrypted Writer", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md914", [
          [ "Writer Builder Integration", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md915", null ],
          [ "writer_builder::encrypted()", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md916", null ]
        ] ],
        [ "Secure Key Storage", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md918", [
          [ "Key Generation", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md919", null ],
          [ "Usage Example", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md920", null ]
        ] ],
        [ "Security-Critical Logger Interface", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md922", null ],
        [ "Log Sanitization", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md924", [
          [ "Common Sanitization Patterns", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md925", null ],
          [ "Integration with Filters", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md926", null ]
        ] ],
        [ "Path Validation", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md928", [
          [ "Validation Checks", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md929", null ],
          [ "Error Codes", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md930", null ],
          [ "Example", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md931", null ]
        ] ],
        [ "Security Filters", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md933", [
          [ "Level-Based Security Filtering", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md934", null ],
          [ "Sampling Bypass for Security Events", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md935", null ]
        ] ],
        [ "Audit Logging", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md937", [
          [ "Best Practices for Audit Logs", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md938", null ],
          [ "Example: Audit Trail", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md939", null ]
        ] ],
        [ "Related Documentation", "md_docs_2API__REFERENCE__SECURITY.html#autotoc_md941", null ]
      ] ]
    ] ],
    [ "API_REFERENCE_WRITERS", "md_docs_2API__REFERENCE__WRITERS.html", [
      [ "API Reference - Writers", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md945", [
        [ "autotoc_md943", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md943", null ],
        [ "category: \"API\"", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md944", null ],
        [ "Table of Contents", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md946", null ],
        [ "Writers", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md947", [
          [ "kcenon::logger::console_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md948", null ],
          [ "kcenon::logger::file_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md949", null ],
          [ "kcenon::logger::rotating_file_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md950", null ],
          [ "kcenon::logger::network_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md951", null ],
          [ "kcenon::logger::critical_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md952", null ],
          [ "kcenon::logger::batch_writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md953", null ]
        ] ],
        [ "Filters", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md954", [
          [ "kcenon::logger::filters::level_filter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md955", null ],
          [ "kcenon::logger::filters::regex_filter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md956", null ],
          [ "kcenon::logger::filters::function_filter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md957", null ],
          [ "kcenon::logger::filters::composite_filter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md958", null ]
        ] ],
        [ "Formatters", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md959", [
          [ "kcenon::logger::plain_formatter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md960", null ],
          [ "kcenon::logger::json_formatter", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md961", null ]
        ] ],
        [ "Log Sampling (v3.3.0)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md962", [
          [ "Sampling Strategies", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md963", null ],
          [ "kcenon::logger::sampling::sampling_config", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md964", [
            [ "Static Factory Methods", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md965", null ]
          ] ],
          [ "kcenon::logger::sampling::sampling_stats", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md966", null ],
          [ "kcenon::logger::sampling::log_sampler", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md967", [
            [ "Key Methods", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md968", null ]
          ] ],
          [ "kcenon::logger::sampling::sampler_factory", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md969", [
            [ "Usage Examples", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md970", null ]
          ] ]
        ] ],
        [ "Real-time Analysis (v3.2.0)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md972", [
          [ "kcenon::logger::analysis::anomaly_event", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md973", null ],
          [ "kcenon::logger::analysis::realtime_analysis_config", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md974", null ],
          [ "kcenon::logger::analysis::realtime_log_analyzer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md975", [
            [ "Callback Type", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md976", null ],
            [ "Constructor", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md977", null ],
            [ "Key Methods", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md978", null ],
            [ "Statistics", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md979", null ]
          ] ],
          [ "kcenon::logger::analysis::realtime_analyzer_factory", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md980", [
            [ "Usage Example", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md981", null ],
            [ "Thread Safety", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md982", null ]
          ] ]
        ] ],
        [ "Backend Integration", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md984", [
          [ "Integration Backend Interface", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md985", null ],
          [ "Standalone Backend (Default)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md986", null ]
        ] ],
        [ "Monitoring Integration", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md987", [
          [ "IMonitor Integration (Phase 2.2.4)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md988", null ]
        ] ],
        [ "Usage Examples", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md989", [
          [ "Basic Usage with ILogger Interface", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md990", null ],
          [ "Native API (Backward Compatible)", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md991", null ],
          [ "Configuration Strategies", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md992", null ],
          [ "Custom Writer", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md993", null ],
          [ "Filtering", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md994", null ]
        ] ],
        [ "Thread Safety", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md995", null ],
        [ "Performance Considerations", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md996", null ],
        [ "Migration from v2.x", "md_docs_2API__REFERENCE__WRITERS.html#autotoc_md997", null ]
      ] ]
    ] ],
    [ "ARCHITECTURE.kr", "md_docs_2ARCHITECTURE_8kr.html", [
      [ "Logger System 아키텍처", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1001", [
        [ "autotoc_md999", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md999", null ],
        [ "category: \"ARCH\"", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1000", null ],
        [ "Logger 파이프라인 아키텍처", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1002", [
          [ "파이프라인 흐름", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1003", null ],
          [ "핵심 아키텍처 결정", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1004", null ],
          [ "OTLP 관찰성 파이프라인", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1005", null ],
          [ "샘플링 및 분석 파이프라인", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1006", null ]
        ] ],
        [ "v3.0 주요 변경사항", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1008", [
          [ "의존성 구조 변경", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1009", null ],
          [ "핵심 변경사항", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1010", null ],
          [ "독립 실행 모드 vs 통합 모드", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1011", null ],
          [ "성능 비교", "md_docs_2ARCHITECTURE_8kr.html#autotoc_md1012", null ]
        ] ]
      ] ]
    ] ],
    [ "ARCHITECTURE", "md_docs_2ARCHITECTURE.html", [
      [ "Logger System Architecture", "md_docs_2ARCHITECTURE.html#autotoc_md1017", [
        [ "autotoc_md1015", "md_docs_2ARCHITECTURE.html#autotoc_md1015", null ],
        [ "category: \"ARCH\"", "md_docs_2ARCHITECTURE.html#autotoc_md1016", null ],
        [ "Table of Contents", "md_docs_2ARCHITECTURE.html#autotoc_md1018", null ],
        [ "Overview", "md_docs_2ARCHITECTURE.html#autotoc_md1019", null ],
        [ "", "md_docs_2ARCHITECTURE.html#autotoc_md1020", null ],
        [ "Logger Pipeline Architecture", "md_docs_2ARCHITECTURE.html#autotoc_md1021", [
          [ "Pipeline Flow", "md_docs_2ARCHITECTURE.html#autotoc_md1022", null ],
          [ "Backend Selection Strategy", "md_docs_2ARCHITECTURE.html#autotoc_md1023", null ],
          [ "Key Architectural Decisions", "md_docs_2ARCHITECTURE.html#autotoc_md1024", null ]
        ] ],
        [ "✍️ Writer Architecture", "md_docs_2ARCHITECTURE.html#autotoc_md1026", null ],
        [ "🔭 OTLP & Observability Pipeline", "md_docs_2ARCHITECTURE.html#autotoc_md1028", null ],
        [ "📊 Sampling & Analysis Pipeline", "md_docs_2ARCHITECTURE.html#autotoc_md1030", null ],
        [ "🏗️ Ecosystem Overview", "md_docs_2ARCHITECTURE.html#autotoc_md1032", null ],
        [ "📋 Project Roles & Responsibilities", "md_docs_2ARCHITECTURE.html#autotoc_md1033", [
          [ "common_system (Foundation) - Required", "md_docs_2ARCHITECTURE.html#autotoc_md1034", [
            [ "Responsibilities:", "md_docs_2ARCHITECTURE.html#autotoc_md1035", null ],
            [ "Key Components:", "md_docs_2ARCHITECTURE.html#autotoc_md1036", null ],
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1037", null ]
          ] ],
          [ "logger_system (Logging) - Standalone Capable", "md_docs_2ARCHITECTURE.html#autotoc_md1039", [
            [ "Responsibilities:", "md_docs_2ARCHITECTURE.html#autotoc_md1040", null ],
            [ "Key Components:", "md_docs_2ARCHITECTURE.html#autotoc_md1041", null ],
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1042", null ]
          ] ],
          [ "thread_system (Threading) - Optional", "md_docs_2ARCHITECTURE.html#autotoc_md1044", [
            [ "Responsibilities:", "md_docs_2ARCHITECTURE.html#autotoc_md1045", null ],
            [ "Key Components:", "md_docs_2ARCHITECTURE.html#autotoc_md1046", null ],
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1047", null ]
          ] ],
          [ "monitoring_system (Metrics)", "md_docs_2ARCHITECTURE.html#autotoc_md1049", [
            [ "Responsibilities:", "md_docs_2ARCHITECTURE.html#autotoc_md1050", null ],
            [ "Key Components:", "md_docs_2ARCHITECTURE.html#autotoc_md1051", null ],
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1052", null ]
          ] ],
          [ "integrated_thread_system (Integration Hub)", "md_docs_2ARCHITECTURE.html#autotoc_md1054", [
            [ "Dependencies:", "md_docs_2ARCHITECTURE.html#autotoc_md1055", null ]
          ] ]
        ] ],
        [ "🔄 Dependency Flow & Interface Contracts", "md_docs_2ARCHITECTURE.html#autotoc_md1056", [
          [ "Interface Hierarchy (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1057", null ],
          [ "Dependency Graph (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1058", null ],
          [ "Build Order Requirements", "md_docs_2ARCHITECTURE.html#autotoc_md1059", null ],
          [ "Standalone vs Integrated Mode", "md_docs_2ARCHITECTURE.html#autotoc_md1060", null ]
        ] ],
        [ "🔧 Integration Patterns", "md_docs_2ARCHITECTURE.html#autotoc_md1061", [
          [ "Interface-Based Integration (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1062", null ],
          [ "Dependency Injection Pattern", "md_docs_2ARCHITECTURE.html#autotoc_md1063", null ],
          [ "Configuration Management", "md_docs_2ARCHITECTURE.html#autotoc_md1064", null ],
          [ "Backend Selection Pattern", "md_docs_2ARCHITECTURE.html#autotoc_md1065", null ]
        ] ],
        [ "📊 Performance Characteristics", "md_docs_2ARCHITECTURE.html#autotoc_md1066", [
          [ "Design Principles", "md_docs_2ARCHITECTURE.html#autotoc_md1067", null ],
          [ "Performance Metrics (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1068", null ],
          [ "Standalone vs Thread System Performance", "md_docs_2ARCHITECTURE.html#autotoc_md1069", null ]
        ] ],
        [ "🔄 Evolution: Monolithic → Modular → Standalone", "md_docs_2ARCHITECTURE.html#autotoc_md1070", [
          [ "Phase 1: Monolithic (v1.x)", "md_docs_2ARCHITECTURE.html#autotoc_md1071", null ],
          [ "Phase 2: Modular (v2.x)", "md_docs_2ARCHITECTURE.html#autotoc_md1072", null ],
          [ "Phase 3: Standalone (v3.0) - Current", "md_docs_2ARCHITECTURE.html#autotoc_md1073", null ],
          [ "Migration Benefits (v3.0)", "md_docs_2ARCHITECTURE.html#autotoc_md1074", null ]
        ] ],
        [ "🚀 Getting Started", "md_docs_2ARCHITECTURE.html#autotoc_md1075", [
          [ "Minimal Setup (Standalone)", "md_docs_2ARCHITECTURE.html#autotoc_md1076", null ],
          [ "Full Ecosystem Setup", "md_docs_2ARCHITECTURE.html#autotoc_md1077", null ],
          [ "Build Order", "md_docs_2ARCHITECTURE.html#autotoc_md1078", null ],
          [ "CMake Integration", "md_docs_2ARCHITECTURE.html#autotoc_md1079", null ]
        ] ],
        [ "📚 Documentation Structure", "md_docs_2ARCHITECTURE.html#autotoc_md1080", [
          [ "common_system", "md_docs_2ARCHITECTURE.html#autotoc_md1081", null ],
          [ "logger_system", "md_docs_2ARCHITECTURE.html#autotoc_md1082", null ],
          [ "thread_system", "md_docs_2ARCHITECTURE.html#autotoc_md1083", null ],
          [ "monitoring_system", "md_docs_2ARCHITECTURE.html#autotoc_md1084", null ]
        ] ],
        [ "Ecosystem Dependencies", "md_docs_2ARCHITECTURE.html#autotoc_md1085", null ],
        [ "", "md_docs_2ARCHITECTURE.html#autotoc_md1086", null ],
        [ "Future Roadmap", "md_docs_2ARCHITECTURE.html#autotoc_md1087", [
          [ "Phase 3.1: Enhancement", "md_docs_2ARCHITECTURE.html#autotoc_md1088", null ],
          [ "Phase 3.2: Optimization (Current)", "md_docs_2ARCHITECTURE.html#autotoc_md1089", null ],
          [ "Phase 4: Ecosystem Expansion", "md_docs_2ARCHITECTURE.html#autotoc_md1090", null ]
        ] ]
      ] ]
    ] ],
    [ "BENCHMARKS.kr", "md_docs_2BENCHMARKS_8kr.html", [
      [ "Logger System 성능 벤치마크", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1095", [
        [ "autotoc_md1093", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1093", null ],
        [ "category: \"PERF\"", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1094", null ],
        [ "목차", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1097", null ],
        [ "요약", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1099", [
          [ "핵심 하이라이트", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1100", null ],
          [ "경쟁 우위", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1101", null ]
        ] ],
        [ "테스트 환경", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1103", [
          [ "하드웨어 구성", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1104", null ],
          [ "소프트웨어 구성", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1105", null ]
        ] ],
        [ "핵심 성능 메트릭", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1107", [
          [ "단일 스레드 성능", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1108", null ],
          [ "멀티스레드 성능", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1109", [
            [ "스레드 수별 처리량", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1110", null ]
          ] ]
        ] ],
        [ "업계 비교", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1112", [
          [ "vs spdlog (가장 인기 있는 C++ 로거)", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1113", [
            [ "단일 스레드 비교", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1114", null ],
            [ "멀티스레드 비교 (4 스레드)", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1115", null ]
          ] ],
          [ "vs Boost.Log", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1116", null ],
          [ "vs glog (Google 로깅 라이브러리)", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1117", null ]
        ] ],
        [ "지연시간 벤치마크", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1119", [
          [ "지연시간 분포 (단일 스레드, 비동기)", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1120", null ],
          [ "부하 시 지연시간", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1121", null ]
        ] ],
        [ "메모리 프로파일링", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1123", [
          [ "구성별 메모리 풋프린트", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1124", null ],
          [ "경쟁사와 메모리 비교", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1125", null ]
        ] ],
        [ "성능 회귀 기준", "md_docs_2BENCHMARKS_8kr.html#autotoc_md1127", null ]
      ] ]
    ] ],
    [ "BENCHMARKS", "md_docs_2BENCHMARKS.html", [
      [ "Logger System Performance Benchmarks", "md_docs_2BENCHMARKS.html#autotoc_md1132", [
        [ "autotoc_md1130", "md_docs_2BENCHMARKS.html#autotoc_md1130", null ],
        [ "category: \"PERF\"", "md_docs_2BENCHMARKS.html#autotoc_md1131", null ],
        [ "Table of Contents", "md_docs_2BENCHMARKS.html#autotoc_md1134", null ],
        [ "Executive Summary", "md_docs_2BENCHMARKS.html#autotoc_md1136", [
          [ "Key Highlights", "md_docs_2BENCHMARKS.html#autotoc_md1137", null ],
          [ "Competitive Advantages", "md_docs_2BENCHMARKS.html#autotoc_md1138", null ]
        ] ],
        [ "Test Environment", "md_docs_2BENCHMARKS.html#autotoc_md1140", [
          [ "Hardware Configuration", "md_docs_2BENCHMARKS.html#autotoc_md1141", null ],
          [ "Software Configuration", "md_docs_2BENCHMARKS.html#autotoc_md1142", null ]
        ] ],
        [ "Core Performance Metrics", "md_docs_2BENCHMARKS.html#autotoc_md1144", [
          [ "Single-Threaded Performance", "md_docs_2BENCHMARKS.html#autotoc_md1145", null ],
          [ "Multi-Threaded Performance", "md_docs_2BENCHMARKS.html#autotoc_md1146", [
            [ "Throughput by Thread Count", "md_docs_2BENCHMARKS.html#autotoc_md1147", null ],
            [ "Performance Scaling Graph", "md_docs_2BENCHMARKS.html#autotoc_md1148", null ]
          ] ]
        ] ],
        [ "Industry Comparisons", "md_docs_2BENCHMARKS.html#autotoc_md1150", [
          [ "vs spdlog (Most Popular C++ Logger)", "md_docs_2BENCHMARKS.html#autotoc_md1151", [
            [ "Single-Threaded Comparison", "md_docs_2BENCHMARKS.html#autotoc_md1152", null ],
            [ "Multi-Threaded Comparison (4 Threads)", "md_docs_2BENCHMARKS.html#autotoc_md1153", null ]
          ] ],
          [ "vs Boost.Log", "md_docs_2BENCHMARKS.html#autotoc_md1154", null ],
          [ "vs glog (Google's Logging Library)", "md_docs_2BENCHMARKS.html#autotoc_md1155", null ],
          [ "vs log4cxx", "md_docs_2BENCHMARKS.html#autotoc_md1156", null ]
        ] ],
        [ "Scalability Analysis", "md_docs_2BENCHMARKS.html#autotoc_md1158", [
          [ "Thread Scaling Efficiency", "md_docs_2BENCHMARKS.html#autotoc_md1159", null ],
          [ "Queue Utilization vs Thread Count", "md_docs_2BENCHMARKS.html#autotoc_md1160", null ],
          [ "CPU Utilization Efficiency", "md_docs_2BENCHMARKS.html#autotoc_md1161", null ]
        ] ],
        [ "Latency Benchmarks", "md_docs_2BENCHMARKS.html#autotoc_md1163", [
          [ "Latency Distribution (Single Thread, Async)", "md_docs_2BENCHMARKS.html#autotoc_md1164", null ],
          [ "Latency Comparison (p99)", "md_docs_2BENCHMARKS.html#autotoc_md1165", null ],
          [ "Latency Under Load", "md_docs_2BENCHMARKS.html#autotoc_md1166", null ]
        ] ],
        [ "Memory Profiling", "md_docs_2BENCHMARKS.html#autotoc_md1168", [
          [ "Memory Footprint by Configuration", "md_docs_2BENCHMARKS.html#autotoc_md1169", null ],
          [ "Memory Comparison with Competitors", "md_docs_2BENCHMARKS.html#autotoc_md1170", null ],
          [ "Memory Allocation Patterns", "md_docs_2BENCHMARKS.html#autotoc_md1171", null ]
        ] ],
        [ "Benchmark Methodology", "md_docs_2BENCHMARKS.html#autotoc_md1173", [
          [ "Test Scenarios", "md_docs_2BENCHMARKS.html#autotoc_md1174", [
            [ "Single-Threaded Throughput Test", "md_docs_2BENCHMARKS.html#autotoc_md1175", null ],
            [ "Multi-Threaded Contention Test", "md_docs_2BENCHMARKS.html#autotoc_md1176", null ],
            [ "Latency Measurement Test", "md_docs_2BENCHMARKS.html#autotoc_md1177", null ]
          ] ],
          [ "Measurement Tools", "md_docs_2BENCHMARKS.html#autotoc_md1178", null ],
          [ "Benchmark Caveats", "md_docs_2BENCHMARKS.html#autotoc_md1179", null ]
        ] ],
        [ "Performance Regression Baselines", "md_docs_2BENCHMARKS.html#autotoc_md1181", null ],
        [ "See Also", "md_docs_2BENCHMARKS.html#autotoc_md1183", null ]
      ] ]
    ] ],
    [ "CHANGELOG.kr", "md_docs_2CHANGELOG_8kr.html", [
      [ "변경 이력 - Logger System", "md_docs_2CHANGELOG_8kr.html#autotoc_md1186", [
        [ "autotoc_md1184", "md_docs_2CHANGELOG_8kr.html#autotoc_md1184", null ],
        [ "category: \"PROJ\"", "md_docs_2CHANGELOG_8kr.html#autotoc_md1185", null ],
        [ "[3.0.0] - 2025-12-31", "md_docs_2CHANGELOG_8kr.html#autotoc_md1188", [
          [ "제거됨 - Deprecated API 정리 (Issues #268, #324)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1189", [
            [ "Breaking Changes", "md_docs_2CHANGELOG_8kr.html#autotoc_md1190", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1191", null ]
          ] ]
        ] ],
        [ "[Unreleased]", "md_docs_2CHANGELOG_8kr.html#autotoc_md1193", [
          [ "실시간 로그 분석 통합 (Issue #281)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1194", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1195", null ],
            [ "예제", "md_docs_2CHANGELOG_8kr.html#autotoc_md1196", null ]
          ] ],
          [ "고용량 시나리오를 위한 로그 샘플링 (Issue #282)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1198", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1199", null ],
            [ "예제", "md_docs_2CHANGELOG_8kr.html#autotoc_md1200", null ]
          ] ],
          [ "Phase 3.4: 구조화된 로깅 통합 (Issue #311) - 2026-01-11", "md_docs_2CHANGELOG_8kr.html#autotoc_md1202", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1203", null ],
            [ "예제", "md_docs_2CHANGELOG_8kr.html#autotoc_md1204", null ]
          ] ],
          [ "Coverage 빌드 수정 (PR #291) - 2026-01-08", "md_docs_2CHANGELOG_8kr.html#autotoc_md1206", [
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1207", null ]
          ] ],
          [ "CI 워크플로우 수정 (PR #290) - 2026-01-08", "md_docs_2CHANGELOG_8kr.html#autotoc_md1209", [
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1210", null ]
          ] ],
          [ "OpenTelemetry 통합 수정 (Issue #283) - 2026-01-08", "md_docs_2CHANGELOG_8kr.html#autotoc_md1212", [
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1213", null ]
          ] ],
          [ "C++20 모듈 파일 (Issue #275) - 2026-01-03", "md_docs_2CHANGELOG_8kr.html#autotoc_md1215", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1216", null ],
            [ "모듈 구조", "md_docs_2CHANGELOG_8kr.html#autotoc_md1217", null ],
            [ "CMake 사용법", "md_docs_2CHANGELOG_8kr.html#autotoc_md1218", null ]
          ] ],
          [ "중복 Writer 추가 로직 수정 (Issue #270) - 2025-12-31", "md_docs_2CHANGELOG_8kr.html#autotoc_md1220", [
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1221", null ],
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1222", null ],
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1223", null ]
          ] ],
          [ "KCENON 기능 감지 사용 (Issue #250) - 2025-12-22", "md_docs_2CHANGELOG_8kr.html#autotoc_md1225", [
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1226", null ],
            [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1227", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1228", null ]
          ] ],
          [ "Deprecated common_system API 마이그레이션 (Issue #248) - 2025-12-22", "md_docs_2CHANGELOG_8kr.html#autotoc_md1230", [
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1231", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1232", null ]
          ] ],
          [ "thread_system v3.0 호환성 (Issue #244) - 2025-12-19", "md_docs_2CHANGELOG_8kr.html#autotoc_md1234", [
            [ "주요 변경 (Breaking Changes)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1235", null ],
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1236", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1237", null ]
          ] ],
          [ "#226 API 변경 후 손상된 예제 파일 수정 (Issue #228) - 2025-12-06", "md_docs_2CHANGELOG_8kr.html#autotoc_md1239", [
            [ "제거됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1240", null ],
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1241", null ],
            [ "비활성화됨 (기능 구현 대기)", "md_docs_2CHANGELOG_8kr.html#autotoc_md1242", null ],
            [ "관련 이슈", "md_docs_2CHANGELOG_8kr.html#autotoc_md1243", null ]
          ] ],
          [ "common::interfaces::ILogger 구현 - 2025-12-06", "md_docs_2CHANGELOG_8kr.html#autotoc_md1245", [
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1246", null ],
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1247", null ],
            [ "사용 중단됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1248", null ],
            [ "하위 호환성", "md_docs_2CHANGELOG_8kr.html#autotoc_md1249", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1250", null ],
            [ "관련 이슈", "md_docs_2CHANGELOG_8kr.html#autotoc_md1251", null ]
          ] ],
          [ "독립형 비동기 구현 (Issue #222) - 2025-12-06", "md_docs_2CHANGELOG_8kr.html#autotoc_md1253", [
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1254", null ],
            [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1255", null ],
            [ "이점", "md_docs_2CHANGELOG_8kr.html#autotoc_md1256", null ],
            [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1257", null ],
            [ "관련 이슈", "md_docs_2CHANGELOG_8kr.html#autotoc_md1258", null ]
          ] ],
          [ "CMake fmt Fallback 제거 - 2025-12-03", "md_docs_2CHANGELOG_8kr.html#autotoc_md1260", [
            [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1261", null ],
            [ "제거됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1262", null ],
            [ "이점", "md_docs_2CHANGELOG_8kr.html#autotoc_md1263", null ],
            [ "관련 이슈", "md_docs_2CHANGELOG_8kr.html#autotoc_md1264", null ]
          ] ]
        ] ],
        [ "[1.0.0] - 2025-10-22", "md_docs_2CHANGELOG_8kr.html#autotoc_md1266", [
          [ "추가됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1267", null ],
          [ "변경됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1268", null ],
          [ "사용 중단됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1269", null ],
          [ "제거됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1270", null ],
          [ "수정됨", "md_docs_2CHANGELOG_8kr.html#autotoc_md1271", null ],
          [ "보안", "md_docs_2CHANGELOG_8kr.html#autotoc_md1272", null ]
        ] ],
        [ "프로젝트 정보", "md_docs_2CHANGELOG_8kr.html#autotoc_md1274", null ],
        [ "버전 지원 매트릭스", "md_docs_2CHANGELOG_8kr.html#autotoc_md1276", null ],
        [ "마이그레이션 가이드", "md_docs_2CHANGELOG_8kr.html#autotoc_md1278", null ]
      ] ]
    ] ],
    [ "CHANGELOG", "md_docs_2CHANGELOG.html", [
      [ "Changelog - Logger System", "md_docs_2CHANGELOG.html#autotoc_md1282", [
        [ "autotoc_md1280", "md_docs_2CHANGELOG.html#autotoc_md1280", null ],
        [ "category: \"PROJ\"", "md_docs_2CHANGELOG.html#autotoc_md1281", null ],
        [ "[4.1.0] - Unreleased", "md_docs_2CHANGELOG.html#autotoc_md1284", [
          [ "Changed - Doxygen Modernization (Issue #581)", "md_docs_2CHANGELOG.html#autotoc_md1285", null ],
          [ "Added - Writer Builder for Fluent Decorator Composition (Issue #413, #415, #416)", "md_docs_2CHANGELOG.html#autotoc_md1286", [
            [ "New Components", "md_docs_2CHANGELOG.html#autotoc_md1287", null ],
            [ "Usage Examples", "md_docs_2CHANGELOG.html#autotoc_md1288", null ],
            [ "Benefits", "md_docs_2CHANGELOG.html#autotoc_md1289", null ],
            [ "Documentation", "md_docs_2CHANGELOG.html#autotoc_md1290", null ]
          ] ],
          [ "Deprecated - Legacy Writer Patterns (Issue #418)", "md_docs_2CHANGELOG.html#autotoc_md1291", [
            [ "Deprecated Classes and Patterns", "md_docs_2CHANGELOG.html#autotoc_md1292", null ],
            [ "Deprecation Timeline", "md_docs_2CHANGELOG.html#autotoc_md1293", null ],
            [ "Migration Support", "md_docs_2CHANGELOG.html#autotoc_md1294", null ]
          ] ]
        ] ],
        [ "[4.0.0] - Unreleased", "md_docs_2CHANGELOG.html#autotoc_md1296", [
          [ "Removed - Deprecated Context ID Convenience Methods (Issue #326)", "md_docs_2CHANGELOG.html#autotoc_md1297", [
            [ "Breaking Changes", "md_docs_2CHANGELOG.html#autotoc_md1298", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1299", null ],
            [ "Benefits", "md_docs_2CHANGELOG.html#autotoc_md1300", null ]
          ] ],
          [ "Added - Decorator Pattern Writer Architecture (Issue #356)", "md_docs_2CHANGELOG.html#autotoc_md1302", [
            [ "New Components", "md_docs_2CHANGELOG.html#autotoc_md1303", null ],
            [ "Usage Examples", "md_docs_2CHANGELOG.html#autotoc_md1304", null ],
            [ "Architecture Benefits", "md_docs_2CHANGELOG.html#autotoc_md1305", null ],
            [ "Custom Writer Migration", "md_docs_2CHANGELOG.html#autotoc_md1306", null ]
          ] ]
        ] ],
        [ "[3.0.0] - 2025-12-31", "md_docs_2CHANGELOG.html#autotoc_md1308", [
          [ "Removed - Deprecated log_level Types and Converters (Issue #339)", "md_docs_2CHANGELOG.html#autotoc_md1309", [
            [ "Breaking Changes", "md_docs_2CHANGELOG.html#autotoc_md1310", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1311", null ]
          ] ],
          [ "Removed - Deprecated API Cleanup (Issues #268, #324)", "md_docs_2CHANGELOG.html#autotoc_md1313", [
            [ "Breaking Changes", "md_docs_2CHANGELOG.html#autotoc_md1314", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1315", null ]
          ] ]
        ] ],
        [ "[Unreleased]", "md_docs_2CHANGELOG.html#autotoc_md1317", [
          [ "Real-time Log Analysis Integration (Issue #281)", "md_docs_2CHANGELOG.html#autotoc_md1318", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1319", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1320", null ]
          ] ],
          [ "Log Sampling for High-Volume Scenarios (Issue #282)", "md_docs_2CHANGELOG.html#autotoc_md1322", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1323", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1324", null ]
          ] ],
          [ "Phase 3.4: Structured Logging Integration (Issue #311) - 2026-01-11", "md_docs_2CHANGELOG.html#autotoc_md1326", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1327", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1328", null ]
          ] ],
          [ "Phase 3.3: Structured Logging Formatters (Issue #310) - 2026-01-11", "md_docs_2CHANGELOG.html#autotoc_md1330", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1331", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1332", null ]
          ] ],
          [ "Phase 3.1: Structured Logging API Design (Issue #308) - 2026-01-11", "md_docs_2CHANGELOG.html#autotoc_md1334", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1335", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1336", null ]
          ] ],
          [ "Phase 3.2: Structured Logging Core Implementation (Issue #309) - 2026-01-11", "md_docs_2CHANGELOG.html#autotoc_md1338", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1339", null ],
            [ "Example", "md_docs_2CHANGELOG.html#autotoc_md1340", null ]
          ] ],
          [ "Coverage Build Fix (PR #291) - 2026-01-08", "md_docs_2CHANGELOG.html#autotoc_md1342", [
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1343", null ]
          ] ],
          [ "CI Workflow Fix (PR #290) - 2026-01-08", "md_docs_2CHANGELOG.html#autotoc_md1345", [
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1346", null ]
          ] ],
          [ "OpenTelemetry Integration Fix (Issue #283) - 2026-01-08", "md_docs_2CHANGELOG.html#autotoc_md1348", [
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1349", null ]
          ] ],
          [ "C++20 Module Files (Issue #275) - 2026-01-03", "md_docs_2CHANGELOG.html#autotoc_md1351", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1352", null ],
            [ "Module Structure", "md_docs_2CHANGELOG.html#autotoc_md1353", null ],
            [ "CMake Usage", "md_docs_2CHANGELOG.html#autotoc_md1354", null ]
          ] ],
          [ "Fix Duplicate Writer Add Logic (Issue #270) - 2025-12-31", "md_docs_2CHANGELOG.html#autotoc_md1356", [
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1357", null ],
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1358", null ],
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1359", null ]
          ] ],
          [ "Use KCENON Feature Detection (Issue #250) - 2025-12-22", "md_docs_2CHANGELOG.html#autotoc_md1361", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1362", null ],
            [ "Fixed", "md_docs_2CHANGELOG.html#autotoc_md1363", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1364", null ]
          ] ],
          [ "Migrate from Deprecated common_system APIs (Issue #248) - 2025-12-22", "md_docs_2CHANGELOG.html#autotoc_md1366", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1367", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1368", null ]
          ] ],
          [ "thread_system v3.0 Compatibility (Issue #244) - 2025-12-19", "md_docs_2CHANGELOG.html#autotoc_md1370", [
            [ "Breaking Changes", "md_docs_2CHANGELOG.html#autotoc_md1371", null ],
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1372", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1373", null ]
          ] ],
          [ "Fix broken example files after #226 API changes (Issue #228) - 2025-12-06", "md_docs_2CHANGELOG.html#autotoc_md1375", [
            [ "Removed", "md_docs_2CHANGELOG.html#autotoc_md1376", null ],
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1377", null ],
            [ "Disabled (awaiting feature implementation)", "md_docs_2CHANGELOG.html#autotoc_md1378", null ],
            [ "Related Issues", "md_docs_2CHANGELOG.html#autotoc_md1379", null ]
          ] ],
          [ "common::interfaces::ILogger Implementation - 2025-12-06", "md_docs_2CHANGELOG.html#autotoc_md1381", [
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1382", null ],
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1383", null ],
            [ "Deprecated", "md_docs_2CHANGELOG.html#autotoc_md1384", null ],
            [ "Backward Compatibility", "md_docs_2CHANGELOG.html#autotoc_md1385", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1386", null ],
            [ "Related Issues", "md_docs_2CHANGELOG.html#autotoc_md1387", null ]
          ] ],
          [ "Standalone async implementation (Issue #222) - 2025-12-06", "md_docs_2CHANGELOG.html#autotoc_md1389", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1390", null ],
            [ "Added", "md_docs_2CHANGELOG.html#autotoc_md1391", null ],
            [ "Benefits", "md_docs_2CHANGELOG.html#autotoc_md1392", null ],
            [ "Migration Guide", "md_docs_2CHANGELOG.html#autotoc_md1393", null ],
            [ "Related Issues", "md_docs_2CHANGELOG.html#autotoc_md1394", null ]
          ] ],
          [ "CMake fmt Fallback Removal - 2025-12-03", "md_docs_2CHANGELOG.html#autotoc_md1396", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1397", null ],
            [ "Removed", "md_docs_2CHANGELOG.html#autotoc_md1398", null ],
            [ "Benefits", "md_docs_2CHANGELOG.html#autotoc_md1399", null ],
            [ "Related Issues", "md_docs_2CHANGELOG.html#autotoc_md1400", null ]
          ] ],
          [ "thread_system Required Dependency - 2025-12-03", "md_docs_2CHANGELOG.html#autotoc_md1402", [
            [ "Changed", "md_docs_2CHANGELOG.html#autotoc_md1403", null ]
          ] ]
        ] ]
      ] ]
    ] ],
    [ "CONFIG_STRATEGIES_ADVANCED", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html", [
      [ "Configuration Strategies - Advanced", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1406", [
        [ "autotoc_md1404", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1404", null ],
        [ "category: \"GUID\"", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1405", null ],
        [ "Table of Contents", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1408", null ],
        [ "Composite Strategy", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1410", [
          [ "How It Works", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1411", null ],
          [ "API", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1412", null ],
          [ "Method Details", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1413", [
            [ "add(std::unique_ptr<config_strategy_interface> strategy)", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1414", null ],
            [ "add<Strategy, Args...>(Args&&... args)", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1415", null ]
          ] ],
          [ "Priority-Based Application", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1416", null ],
          [ "Explicit Composite Strategy Example", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1417", null ]
        ] ],
        [ "Custom Strategy Implementation", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1419", [
          [ "Basic Custom Strategy", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1420", null ],
          [ "Conditional Strategy (is_applicable)", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1421", null ]
        ] ],
        [ "Strategy Priority and Conflict Resolution", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1423", [
          [ "Priority Values", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1424", null ],
          [ "Conflict Resolution Rules", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1425", null ],
          [ "Best Practice: Environment Variables as Final Override", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1426", null ]
        ] ],
        [ "Best Practices", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1428", [
          [ "Start with Defaults, Override as Needed", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1429", null ],
          [ "Use Environment Variables for Runtime Configuration", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1430", null ],
          [ "Combine Strategies for Complex Requirements", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1431", null ],
          [ "Test Strategies in Isolation", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1432", null ],
          [ "Document Custom Strategies", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1433", null ]
        ] ],
        [ "Summary", "md_docs_2CONFIG__STRATEGIES__ADVANCED.html#autotoc_md1435", null ]
      ] ]
    ] ],
    [ "CONFIG_STRATEGIES_BASIC", "md_docs_2CONFIG__STRATEGIES__BASIC.html", [
      [ "Configuration Strategies - Getting Started and Common Patterns", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1439", [
        [ "autotoc_md1437", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1437", null ],
        [ "category: \"GUID\"", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1438", null ],
        [ "Table of Contents", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1441", null ],
        [ "Overview", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1443", [
          [ "What are Configuration Strategies?", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1444", null ],
          [ "Architecture", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1445", null ]
        ] ],
        [ "Strategy Pattern Benefits", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1447", [
          [ "Why Use Strategies?", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1448", null ],
          [ "Example: Multi-Environment Deployment", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1449", null ]
        ] ],
        [ "Strategy Interface", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1451", [
          [ "Base Interface", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1452", null ],
          [ "Strategy Lifecycle", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1453", null ],
          [ "Method Details", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1454", [
            [ "get_name()", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1455", null ],
            [ "apply(logger_config& config)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1456", null ],
            [ "is_applicable()", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1457", null ],
            [ "priority()", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1458", null ]
          ] ]
        ] ],
        [ "Built-in Strategies", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1460", [
          [ "Deployment Strategy", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1461", [
            [ "Supported Environments", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1462", null ],
            [ "Environment Profiles", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1463", [
              [ "Development (deployment_env::development)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1464", null ],
              [ "Staging (deployment_env::staging)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1465", null ],
              [ "Production (deployment_env::production)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1466", null ],
              [ "Testing (deployment_env::testing)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1467", null ]
            ] ],
            [ "API", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1468", null ]
          ] ],
          [ "Environment Strategy", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1470", [
            [ "Supported Environment Variables", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1471", null ],
            [ "API", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1472", null ],
            [ "Docker/Kubernetes Example", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1473", null ]
          ] ],
          [ "Performance Strategy", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1475", [
            [ "Performance Levels", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1476", null ],
            [ "Performance Profiles", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1477", [
              [ "Low Latency (performance_level::low_latency)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1478", null ],
              [ "Balanced (performance_level::balanced)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1479", null ],
              [ "High Throughput (performance_level::high_throughput)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1480", null ],
              [ "Minimal Overhead (performance_level::minimal_overhead)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1481", null ]
            ] ],
            [ "API", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1482", null ]
          ] ]
        ] ],
        [ "Usage Examples", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1484", [
          [ "Example 1: Simple Deployment Strategy", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1485", null ],
          [ "Example 2: Combined Deployment + Performance", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1486", null ],
          [ "Example 3: Environment Variables + Deployment", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1487", null ],
          [ "Example 4: Conditional Strategy (Docker)", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1488", null ],
          [ "Example 5: Multi-Logger Configuration", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1489", null ]
        ] ],
        [ "Related Documentation", "md_docs_2CONFIG__STRATEGIES__BASIC.html#autotoc_md1491", null ]
      ] ]
    ] ],
    [ "CONFIGURATION_STRATEGIES", "md_docs_2CONFIGURATION__STRATEGIES.html", [
      [ "Configuration Strategies Guide", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1495", [
        [ "autotoc_md1493", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1493", null ],
        [ "category: \"GUID\"", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1494", null ],
        [ "Sub-Documents", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1496", null ],
        [ "Quick Links", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1497", [
          [ "Getting Started", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1498", null ],
          [ "Built-in Strategies", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1499", null ],
          [ "Advanced Topics", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1500", null ]
        ] ],
        [ "Related Documentation", "md_docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1501", null ]
      ] ]
    ] ],
    [ "CONTRIBUTING.kr", "md_docs_2contributing_2CONTRIBUTING_8kr.html", [
      [ "Logger System 기여하기", "md_docs_2contributing_2CONTRIBUTING_8kr.html#autotoc_md1505", null ]
    ] ],
    [ "CONTRIBUTING", "md_docs_2contributing_2CONTRIBUTING.html", [
      [ "Contributing to Logger System", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1510", [
        [ "autotoc_md1508", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1508", null ],
        [ "category: \"PROJ\"", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1509", null ],
        [ "Table of Contents", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1511", null ],
        [ "Code of Conduct", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1512", null ],
        [ "Getting Started", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1513", null ],
        [ "Development Process", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1514", [
          [ "Before You Start", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1515", null ],
          [ "Making Changes", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1516", null ],
          [ "Commit Message Format", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1517", null ]
        ] ],
        [ "Code Style", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1518", [
          [ "C++ Guidelines", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1519", null ],
          [ "Code Formatting", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1520", null ],
          [ "Naming Conventions", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1521", null ]
        ] ],
        [ "Testing", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1522", [
          [ "Unit Tests", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1523", null ],
          [ "Running Tests", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1524", null ],
          [ "Benchmarks", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1525", null ]
        ] ],
        [ "Documentation", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1526", [
          [ "Code Documentation", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1527", null ],
          [ "Documentation Updates", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1528", null ]
        ] ],
        [ "Submitting Changes", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1529", [
          [ "Pull Request Process", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1530", null ],
          [ "Review Process", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1531", null ]
        ] ],
        [ "Questions?", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1532", null ],
        [ "Thank you for contributing!", "md_docs_2contributing_2CONTRIBUTING.html#autotoc_md1533", null ]
      ] ]
    ] ],
    [ "TESTING_GUIDE", "md_docs_2contributing_2TESTING__GUIDE.html", null ],
    [ "TRANSLATION_SUMMARY", "md_docs_2contributing_2TRANSLATION__SUMMARY.html", [
      [ "Korean Translation Summary", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1539", [
        [ "autotoc_md1537", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1537", null ],
        [ "category: \"PROJ\"", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1538", null ],
        [ "Completion Status", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1540", null ],
        [ "Statistics", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1541", null ],
        [ "Files Created", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1542", [
          [ "Root Directory (19 files)", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1543", null ],
          [ "advanced/ Directory (2 files)", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1544", null ],
          [ "guides/ Directory (4 files)", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1545", null ]
        ] ],
        [ "Language Switcher Format", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1546", [
          [ "English Files", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1547", null ],
          [ "Korean Files", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1548", null ]
        ] ],
        [ "Translation Approach", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1549", null ],
        [ "File Structure", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1550", null ],
        [ "Verification", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1551", null ],
        [ "Notes", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1552", null ],
        [ "Next Steps (Optional)", "md_docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1553", null ]
      ] ]
    ] ],
    [ "CRASH_SAFETY_GUIDE", "md_docs_2CRASH__SAFETY__GUIDE.html", [
      [ "Crash-Safe Logger Guide", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1557", [
        [ "autotoc_md1555", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1555", null ],
        [ "category: \"GUID\"", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1556", null ],
        [ "Table of Contents", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1558", null ],
        [ "Overview", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1560", null ],
        [ "Crash Safety Mechanism", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1562", [
          [ "How It Works", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1563", null ],
          [ "Signal Handlers", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1564", null ],
          [ "Async-Signal-Safe Constraints", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1565", null ],
          [ "Recovery Procedure", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1566", null ]
        ] ],
        [ "Configuration", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1568", [
          [ "Basic Setup", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1569", null ],
          [ "Advanced Configuration", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1570", null ]
        ] ],
        [ "API Reference", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1572", [
          [ "crash_safe_logger Class", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1573", null ],
          [ "Method Details", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1574", [
            [ "create()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1575", null ],
            [ "install_crash_handlers()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1576", null ],
            [ "uninstall_crash_handlers()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1577", null ],
            [ "flush_with_timeout()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1578", null ],
            [ "emergency_flush()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1579", null ],
            [ "set_auto_flush_interval()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1580", null ],
            [ "set_min_level() / get_min_level()", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1581", null ]
          ] ]
        ] ],
        [ "Performance Overhead", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1583", [
          [ "Benchmarks", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1584", null ],
          [ "Overhead Sources", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1585", null ],
          [ "Optimization Tips", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1586", null ]
        ] ],
        [ "When to Use", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1588", [
          [ "Use Crash-Safe Logger When:", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1589", null ],
          [ "Do NOT Use When:", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1590", null ]
        ] ],
        [ "Combined Usage", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1592", [
          [ "Local Crash Safety + Network Forwarding", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1593", null ],
          [ "Server-Side Crash Safety", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1594", null ],
          [ "Complete Production Topology", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1595", null ]
        ] ],
        [ "Best Practices", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1597", [
          [ "Choose Appropriate Auto-Flush Interval", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1598", null ],
          [ "Test Signal Handlers", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1599", null ],
          [ "Avoid Signal Handler Conflicts", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1600", null ],
          [ "Graceful Shutdown", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1601", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1603", [
          [ "Problem: Emergency flush not working", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1604", null ],
          [ "Problem: High CPU usage from auto-flush", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1605", null ],
          [ "Problem: Signal handler conflicts", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1606", null ]
        ] ],
        [ "Related Documentation", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1608", [
          [ "Header Files", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1609", null ],
          [ "External Resources", "md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1610", null ]
        ] ]
      ] ]
    ] ],
    [ "DEPENDENCY_ARCHITECTURE", "md_docs_2DEPENDENCY__ARCHITECTURE.html", [
      [ "Dependency Architecture", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1614", [
        [ "autotoc_md1612", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1612", null ],
        [ "category: \"ARCH\"", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1613", null ],
        [ "Overview", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1615", null ],
        [ "Dependency Tiers", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1616", null ],
        [ "Abstraction Interfaces", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1617", null ],
        [ "Resolved: Bidirectional Dependency (Issue #252)", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1618", [
          [ "Previous State", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1619", null ],
          [ "Current State (Resolved)", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1620", null ],
          [ "CMake Guard", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1621", null ]
        ] ],
        [ "Configuration", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1622", [
          [ "Recommended (Default)", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1623", null ],
          [ "With thread_pool Backend", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1624", null ],
          [ "Deprecated (Avoid)", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1625", null ]
        ] ],
        [ "Migration Guide", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1626", null ],
        [ "Timeline", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1627", null ],
        [ "Related", "md_docs_2DEPENDENCY__ARCHITECTURE.html#autotoc_md1628", null ]
      ] ]
    ] ],
    [ "Ecosystem Integration", "md_docs_2ECOSYSTEM.html", [
      [ "Dependencies", "md_docs_2ECOSYSTEM.html#autotoc_md1630", null ],
      [ "Dependent Systems", "md_docs_2ECOSYSTEM.html#autotoc_md1631", null ],
      [ "All Systems", "md_docs_2ECOSYSTEM.html#autotoc_md1632", null ]
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
    [ "FEATURES.kr", "md_docs_2FEATURES_8kr.html", [
      [ "Logger System - 상세 기능", "md_docs_2FEATURES_8kr.html#autotoc_md1635", [
        [ "autotoc_md1633", "md_docs_2FEATURES_8kr.html#autotoc_md1633", null ],
        [ "category: \"FEAT\"", "md_docs_2FEATURES_8kr.html#autotoc_md1634", null ],
        [ "목차", "md_docs_2FEATURES_8kr.html#autotoc_md1637", null ],
        [ "핵심 기능", "md_docs_2FEATURES_8kr.html#autotoc_md1639", [
          [ "설계 원칙", "md_docs_2FEATURES_8kr.html#autotoc_md1640", null ],
          [ "주요 기능", "md_docs_2FEATURES_8kr.html#autotoc_md1641", null ]
        ] ],
        [ "로그 레벨", "md_docs_2FEATURES_8kr.html#autotoc_md1643", [
          [ "레벨 정의", "md_docs_2FEATURES_8kr.html#autotoc_md1644", null ],
          [ "레벨별 사용 지침", "md_docs_2FEATURES_8kr.html#autotoc_md1645", null ],
          [ "사용 예시", "md_docs_2FEATURES_8kr.html#autotoc_md1646", null ]
        ] ],
        [ "로그 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1648", [
          [ "콘솔 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1649", null ],
          [ "파일 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1650", null ],
          [ "회전 파일 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1651", null ],
          [ "원격 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1652", null ],
          [ "커스텀 작성자", "md_docs_2FEATURES_8kr.html#autotoc_md1653", null ]
        ] ],
        [ "포맷팅", "md_docs_2FEATURES_8kr.html#autotoc_md1655", [
          [ "기본 포맷", "md_docs_2FEATURES_8kr.html#autotoc_md1656", null ],
          [ "JSON 포맷", "md_docs_2FEATURES_8kr.html#autotoc_md1657", null ],
          [ "커스텀 포맷터", "md_docs_2FEATURES_8kr.html#autotoc_md1658", null ]
        ] ],
        [ "필터링", "md_docs_2FEATURES_8kr.html#autotoc_md1660", [
          [ "레벨 필터링", "md_docs_2FEATURES_8kr.html#autotoc_md1661", null ],
          [ "패턴 필터링", "md_docs_2FEATURES_8kr.html#autotoc_md1662", null ],
          [ "조건부 필터링", "md_docs_2FEATURES_8kr.html#autotoc_md1663", null ]
        ] ],
        [ "비동기 로깅", "md_docs_2FEATURES_8kr.html#autotoc_md1665", [
          [ "비동기 모드 활성화", "md_docs_2FEATURES_8kr.html#autotoc_md1666", null ],
          [ "성능 비교", "md_docs_2FEATURES_8kr.html#autotoc_md1667", null ]
        ] ],
        [ "성능 특성", "md_docs_2FEATURES_8kr.html#autotoc_md1669", [
          [ "벤치마크 결과", "md_docs_2FEATURES_8kr.html#autotoc_md1670", null ],
          [ "최적화 팁", "md_docs_2FEATURES_8kr.html#autotoc_md1671", null ]
        ] ],
        [ "통합 기능", "md_docs_2FEATURES_8kr.html#autotoc_md1673", [
          [ "thread_system 통합", "md_docs_2FEATURES_8kr.html#autotoc_md1674", null ],
          [ "monitoring_system 통합", "md_docs_2FEATURES_8kr.html#autotoc_md1675", null ],
          [ "common_system 통합", "md_docs_2FEATURES_8kr.html#autotoc_md1676", null ]
        ] ],
        [ "구성 예시", "md_docs_2FEATURES_8kr.html#autotoc_md1678", [
          [ "프로덕션 설정", "md_docs_2FEATURES_8kr.html#autotoc_md1679", null ],
          [ "개발 설정", "md_docs_2FEATURES_8kr.html#autotoc_md1680", null ]
        ] ],
        [ "OTLP 통합", "md_docs_2FEATURES_8kr.html#autotoc_md1682", [
          [ "개요", "md_docs_2FEATURES_8kr.html#autotoc_md1683", null ],
          [ "OTLP Writer 설정", "md_docs_2FEATURES_8kr.html#autotoc_md1684", null ],
          [ "Trace Context 전파", "md_docs_2FEATURES_8kr.html#autotoc_md1685", null ],
          [ "프로토콜 지원", "md_docs_2FEATURES_8kr.html#autotoc_md1686", null ],
          [ "기능", "md_docs_2FEATURES_8kr.html#autotoc_md1687", null ]
        ] ],
        [ "로그 샘플링", "md_docs_2FEATURES_8kr.html#autotoc_md1689", [
          [ "샘플링 전략", "md_docs_2FEATURES_8kr.html#autotoc_md1690", null ],
          [ "빠른 설정", "md_docs_2FEATURES_8kr.html#autotoc_md1691", null ],
          [ "설정", "md_docs_2FEATURES_8kr.html#autotoc_md1692", null ],
          [ "기능", "md_docs_2FEATURES_8kr.html#autotoc_md1693", null ]
        ] ],
        [ "실시간 로그 분석", "md_docs_2FEATURES_8kr.html#autotoc_md1695", [
          [ "이상 유형", "md_docs_2FEATURES_8kr.html#autotoc_md1696", null ],
          [ "사용법", "md_docs_2FEATURES_8kr.html#autotoc_md1697", null ]
        ] ],
        [ "참고사항", "md_docs_2FEATURES_8kr.html#autotoc_md1699", [
          [ "스레드 안전성", "md_docs_2FEATURES_8kr.html#autotoc_md1700", null ],
          [ "주의사항", "md_docs_2FEATURES_8kr.html#autotoc_md1701", null ]
        ] ]
      ] ]
    ] ],
    [ "FEATURES", "md_docs_2FEATURES.html", [
      [ "Logger System Features", "md_docs_2FEATURES.html#autotoc_md1706", [
        [ "autotoc_md1704", "md_docs_2FEATURES.html#autotoc_md1704", null ],
        [ "category: \"FEAT\"", "md_docs_2FEATURES.html#autotoc_md1705", null ],
        [ "Table of Contents", "md_docs_2FEATURES.html#autotoc_md1708", null ],
        [ "Core Features", "md_docs_2FEATURES.html#autotoc_md1710", [
          [ "🎯 High-Performance Logging", "md_docs_2FEATURES.html#autotoc_md1711", [
            [ "Asynchronous Processing", "md_docs_2FEATURES.html#autotoc_md1712", null ],
            [ "Thread-Safe Operations", "md_docs_2FEATURES.html#autotoc_md1713", null ],
            [ "Configuration Validation", "md_docs_2FEATURES.html#autotoc_md1714", null ]
          ] ],
          [ "🔧 Advanced Capabilities", "md_docs_2FEATURES.html#autotoc_md1715", [
            [ "Result-Based Error Handling", "md_docs_2FEATURES.html#autotoc_md1716", null ],
            [ "Builder Pattern API", "md_docs_2FEATURES.html#autotoc_md1717", null ],
            [ "Configuration Strategies", "md_docs_2FEATURES.html#autotoc_md1718", null ],
            [ "Monitoring Integration", "md_docs_2FEATURES.html#autotoc_md1719", null ]
          ] ]
        ] ],
        [ "Writer Types", "md_docs_2FEATURES.html#autotoc_md1721", [
          [ "Console Writer", "md_docs_2FEATURES.html#autotoc_md1722", null ],
          [ "File Writer", "md_docs_2FEATURES.html#autotoc_md1723", null ],
          [ "Rotating File Writer", "md_docs_2FEATURES.html#autotoc_md1724", null ],
          [ "Network Writer", "md_docs_2FEATURES.html#autotoc_md1725", null ],
          [ "Critical Writer", "md_docs_2FEATURES.html#autotoc_md1726", null ],
          [ "Hybrid Writer", "md_docs_2FEATURES.html#autotoc_md1727", null ]
        ] ],
        [ "Formatting Options", "md_docs_2FEATURES.html#autotoc_md1729", [
          [ "Plain Text Formatter", "md_docs_2FEATURES.html#autotoc_md1730", null ],
          [ "JSON Formatter", "md_docs_2FEATURES.html#autotoc_md1731", null ],
          [ "Logfmt Formatter", "md_docs_2FEATURES.html#autotoc_md1732", null ],
          [ "Custom Formatter", "md_docs_2FEATURES.html#autotoc_md1733", null ]
        ] ],
        [ "Filtering Capabilities", "md_docs_2FEATURES.html#autotoc_md1735", [
          [ "Level-Based Filtering", "md_docs_2FEATURES.html#autotoc_md1736", null ],
          [ "Regex-Based Filtering", "md_docs_2FEATURES.html#autotoc_md1737", null ],
          [ "Function-Based Filtering", "md_docs_2FEATURES.html#autotoc_md1738", null ],
          [ "Composite Filtering", "md_docs_2FEATURES.html#autotoc_md1739", null ]
        ] ],
        [ "Rotation Policies", "md_docs_2FEATURES.html#autotoc_md1741", [
          [ "Size-Based Rotation", "md_docs_2FEATURES.html#autotoc_md1742", null ],
          [ "Time-Based Rotation", "md_docs_2FEATURES.html#autotoc_md1743", null ],
          [ "Hybrid Rotation", "md_docs_2FEATURES.html#autotoc_md1744", null ],
          [ "Compression Support", "md_docs_2FEATURES.html#autotoc_md1745", null ]
        ] ],
        [ "Security Features", "md_docs_2FEATURES.html#autotoc_md1747", [
          [ "Secure Key Storage", "md_docs_2FEATURES.html#autotoc_md1748", null ],
          [ "Path Validation", "md_docs_2FEATURES.html#autotoc_md1749", null ],
          [ "Signal Handler Safety", "md_docs_2FEATURES.html#autotoc_md1750", null ],
          [ "Security Audit Logging", "md_docs_2FEATURES.html#autotoc_md1751", null ],
          [ "Sensitive Data Sanitization", "md_docs_2FEATURES.html#autotoc_md1752", null ],
          [ "Access Control", "md_docs_2FEATURES.html#autotoc_md1753", null ],
          [ "Compliance Support", "md_docs_2FEATURES.html#autotoc_md1754", null ]
        ] ],
        [ "Advanced Capabilities", "md_docs_2FEATURES.html#autotoc_md1756", [
          [ "Structured Logging", "md_docs_2FEATURES.html#autotoc_md1757", null ],
          [ "Log Routing", "md_docs_2FEATURES.html#autotoc_md1758", null ],
          [ "Performance Metrics Collection", "md_docs_2FEATURES.html#autotoc_md1759", null ],
          [ "Real-time Log Analysis", "md_docs_2FEATURES.html#autotoc_md1760", [
            [ "Post-hoc Analysis (log_analyzer)", "md_docs_2FEATURES.html#autotoc_md1761", null ],
            [ "Real-time Anomaly Detection (realtime_log_analyzer)", "md_docs_2FEATURES.html#autotoc_md1762", null ]
          ] ],
          [ "Distributed Logging", "md_docs_2FEATURES.html#autotoc_md1763", null ]
        ] ],
        [ "OTLP Integration", "md_docs_2FEATURES.html#autotoc_md1765", [
          [ "Overview", "md_docs_2FEATURES.html#autotoc_md1766", null ],
          [ "OTLP Writer Configuration", "md_docs_2FEATURES.html#autotoc_md1767", null ],
          [ "Trace Context Propagation", "md_docs_2FEATURES.html#autotoc_md1768", null ],
          [ "Protocol Support", "md_docs_2FEATURES.html#autotoc_md1769", null ],
          [ "Export Statistics", "md_docs_2FEATURES.html#autotoc_md1770", null ],
          [ "Features", "md_docs_2FEATURES.html#autotoc_md1771", null ]
        ] ],
        [ "Log Sampling", "md_docs_2FEATURES.html#autotoc_md1773", [
          [ "Overview", "md_docs_2FEATURES.html#autotoc_md1774", null ],
          [ "Sampling Strategies", "md_docs_2FEATURES.html#autotoc_md1775", null ],
          [ "Quick Setup", "md_docs_2FEATURES.html#autotoc_md1776", null ],
          [ "Configuration", "md_docs_2FEATURES.html#autotoc_md1777", null ],
          [ "Runtime Control", "md_docs_2FEATURES.html#autotoc_md1778", null ],
          [ "Features", "md_docs_2FEATURES.html#autotoc_md1779", null ]
        ] ],
        [ "See Also", "md_docs_2FEATURES.html#autotoc_md1781", null ]
      ] ]
    ] ],
    [ "GETTING_STARTED", "md_docs_2GETTING__STARTED.html", [
      [ "Getting Started with logger_system", "md_docs_2GETTING__STARTED.html#autotoc_md1784", [
        [ "autotoc_md1782", "md_docs_2GETTING__STARTED.html#autotoc_md1782", null ],
        [ "category: \"GUID\"", "md_docs_2GETTING__STARTED.html#autotoc_md1783", null ],
        [ "Prerequisites", "md_docs_2GETTING__STARTED.html#autotoc_md1785", null ],
        [ "Installation", "md_docs_2GETTING__STARTED.html#autotoc_md1786", [
          [ "Option A: CMake FetchContent", "md_docs_2GETTING__STARTED.html#autotoc_md1787", null ],
          [ "Option B: Build from source", "md_docs_2GETTING__STARTED.html#autotoc_md1788", null ],
          [ "Option C: vcpkg preset", "md_docs_2GETTING__STARTED.html#autotoc_md1789", null ]
        ] ],
        [ "First Logger", "md_docs_2GETTING__STARTED.html#autotoc_md1790", null ],
        [ "Writer Composition (Decorator Pattern)", "md_docs_2GETTING__STARTED.html#autotoc_md1791", [
          [ "Console with level filtering", "md_docs_2GETTING__STARTED.html#autotoc_md1792", null ],
          [ "Buffered + async file writer", "md_docs_2GETTING__STARTED.html#autotoc_md1793", null ],
          [ "Production multi-writer setup", "md_docs_2GETTING__STARTED.html#autotoc_md1794", null ]
        ] ],
        [ "Backends", "md_docs_2GETTING__STARTED.html#autotoc_md1795", [
          [ "Custom writers", "md_docs_2GETTING__STARTED.html#autotoc_md1796", null ]
        ] ],
        [ "Structured Logging", "md_docs_2GETTING__STARTED.html#autotoc_md1797", [
          [ "Context propagation", "md_docs_2GETTING__STARTED.html#autotoc_md1798", null ],
          [ "RAII context scopes", "md_docs_2GETTING__STARTED.html#autotoc_md1799", null ],
          [ "Formatters", "md_docs_2GETTING__STARTED.html#autotoc_md1800", null ]
        ] ],
        [ "OpenTelemetry Integration", "md_docs_2GETTING__STARTED.html#autotoc_md1801", null ],
        [ "Configuration Builder", "md_docs_2GETTING__STARTED.html#autotoc_md1802", null ],
        [ "Next Steps", "md_docs_2GETTING__STARTED.html#autotoc_md1803", null ]
      ] ]
    ] ],
    [ "BEST_PRACTICES.kr", "md_docs_2guides_2BEST__PRACTICES_8kr.html", [
      [ "Logger System 모범 사례 가이드", "md_docs_2guides_2BEST__PRACTICES_8kr.html#autotoc_md1806", null ]
    ] ],
    [ "BEST_PRACTICES", "md_docs_2guides_2BEST__PRACTICES.html", [
      [ "Logger System Best Practices Guide", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1811", [
        [ "autotoc_md1809", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1809", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1810", null ],
        [ "Table of Contents", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1812", null ],
        [ "Design Principles", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1813", [
          [ "Single Responsibility", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1814", null ],
          [ "Dependency Injection", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1815", null ],
          [ "Interface Segregation", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1816", null ]
        ] ],
        [ "Configuration Best Practices", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1817", [
          [ "Environment-Specific Configuration", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1818", null ],
          [ "Configuration Validation", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1819", null ],
          [ "Dynamic Reconfiguration", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1820", null ]
        ] ],
        [ "Performance Guidelines", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1821", [
          [ "Minimize Allocations", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1822", null ],
          [ "Lazy Evaluation", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1823", null ],
          [ "Batch Operations", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1824", null ],
          [ "Async Logging Pattern", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1825", null ]
        ] ],
        [ "Error Handling", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1826", [
          [ "Graceful Degradation", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1827", null ],
          [ "Error Recovery", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1828", null ],
          [ "Circuit Breaker Pattern", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1829", null ]
        ] ],
        [ "Security Considerations", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1830", [
          [ "Sanitize Sensitive Data", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1831", null ],
          [ "Access Control", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1832", null ],
          [ "Log Injection Prevention", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1833", null ]
        ] ],
        [ "Testing Strategies", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1834", [
          [ "Mock Logger for Testing", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1835", null ],
          [ "Performance Testing", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1836", null ],
          [ "Stress Testing", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1837", null ]
        ] ],
        [ "Production Deployment", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1838", [
          [ "Monitoring Integration", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1839", null ],
          [ "Log Rotation Strategy", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1840", null ],
          [ "Graceful Shutdown", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1841", null ]
        ] ],
        [ "Common Pitfalls", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1842", [
          [ "Memory Leaks", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1843", null ],
          [ "Deadlocks", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1844", null ],
          [ "Performance Bottlenecks", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1845", null ]
        ] ],
        [ "Summary", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1846", null ],
        [ "Remember: Good logging is invisible when everything works but invaluable when things go wrong.", "md_docs_2guides_2BEST__PRACTICES.html#autotoc_md1847", null ]
      ] ]
    ] ],
    [ "DECORATOR_MIGRATION", "md_docs_2guides_2DECORATOR__MIGRATION.html", [
      [ "Decorator Pattern Writer Migration Guide", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1850", [
        [ "autotoc_md1848", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1848", null ],
        [ "category: \"MIGR\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1849", null ],
        [ "Table of Contents", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1851", null ],
        [ "Overview", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1853", [
          [ "Why Decorator Pattern?", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1854", null ],
          [ "Key Components", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1855", null ]
        ] ],
        [ "New Architecture", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1857", [
          [ "Base Classes", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1858", [
            [ "log_writer_interface", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1859", null ],
            [ "decorator_writer_base", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1860", null ]
          ] ]
        ] ],
        [ "Built-in Decorators", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1862", [
          [ "filtered_writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1863", null ],
          [ "buffered_writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1864", null ],
          [ "formatted_writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1865", null ]
        ] ],
        [ "Migrating Custom Writers", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1867", [
          [ "Before (v3.x): Monolithic Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1868", null ],
          [ "After (v4.0): Focused Leaf Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1869", null ],
          [ "Creating a Custom Decorator", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1870", null ]
        ] ],
        [ "Composing Decorators", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1872", [
          [ "Order Matters", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1873", null ],
          [ "Recommended Order", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1874", null ],
          [ "Using writer_builder (Recommended)", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1875", null ],
          [ "Using logger_builder", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1876", null ]
        ] ],
        [ "Deprecation Timeline and Legacy Patterns", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1878", [
          [ "Deprecated Writer Patterns", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1879", null ],
          [ "Timeline", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1880", null ],
          [ "Migration Paths", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1881", [
            [ "Scenario 1: Simple File Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1882", null ],
            [ "Scenario 2: Rotating File Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1883", null ],
            [ "Scenario 3: Async + Encrypted Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1884", null ],
            [ "Scenario 4: Buffered + Filtered Writer", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1885", null ],
            [ "Scenario 5: Multiple Decorators (Production Setup)", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1886", null ],
            [ "Scenario 6: Custom Writer Integration", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1887", null ]
          ] ],
          [ "Support Policy", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1888", null ]
        ] ],
        [ "Best Practices", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1890", [
          [ "Keep Leaf Writers Simple", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1891", null ],
          [ "Use Category Tags", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1892", null ],
          [ "Handle Errors Gracefully", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1893", null ],
          [ "Ensure Thread Safety", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1894", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1896", [
          [ "Common Issues", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1897", [
            [ "\"Writer not receiving entries\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1898", null ],
            [ "\"Entries not being formatted\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1899", null ],
            [ "\"Flush not working\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1900", null ],
            [ "\"Compilation errors with custom writers\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1901", null ],
            [ "\"Performance regression after migration\"", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1902", null ]
          ] ],
          [ "Migration Checklist", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1903", null ],
          [ "Getting Help", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1904", null ]
        ] ],
        [ "Related Documentation", "md_docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1906", null ]
      ] ]
    ] ],
    [ "FAQ", "md_docs_2guides_2FAQ.html", [
      [ "Logger System - Frequently Asked Questions", "md_docs_2guides_2FAQ.html#autotoc_md1910", [
        [ "autotoc_md1908", "md_docs_2guides_2FAQ.html#autotoc_md1908", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2FAQ.html#autotoc_md1909", null ],
        [ "Table of Contents", "md_docs_2guides_2FAQ.html#autotoc_md1912", null ],
        [ "General Questions", "md_docs_2guides_2FAQ.html#autotoc_md1914", [
          [ "What is the logger_system?", "md_docs_2guides_2FAQ.html#autotoc_md1915", null ],
          [ "What C++ standard is required?", "md_docs_2guides_2FAQ.html#autotoc_md1917", null ]
        ] ],
        [ "Logging Basics", "md_docs_2guides_2FAQ.html#autotoc_md1919", [
          [ "How do I create a logger?", "md_docs_2guides_2FAQ.html#autotoc_md1920", null ],
          [ "How do I log messages?", "md_docs_2guides_2FAQ.html#autotoc_md1922", null ]
        ] ],
        [ "Log Levels", "md_docs_2guides_2FAQ.html#autotoc_md1924", [
          [ "What log levels are available?", "md_docs_2guides_2FAQ.html#autotoc_md1925", null ],
          [ "How do I set the log level?", "md_docs_2guides_2FAQ.html#autotoc_md1927", null ],
          [ "Can I change log level at runtime?", "md_docs_2guides_2FAQ.html#autotoc_md1929", null ]
        ] ],
        [ "Writers", "md_docs_2guides_2FAQ.html#autotoc_md1931", [
          [ "What writers are available?", "md_docs_2guides_2FAQ.html#autotoc_md1932", null ],
          [ "How do I use multiple writers?", "md_docs_2guides_2FAQ.html#autotoc_md1934", null ],
          [ "How does log rotation work?", "md_docs_2guides_2FAQ.html#autotoc_md1936", null ]
        ] ],
        [ "Performance", "md_docs_2guides_2FAQ.html#autotoc_md1938", [
          [ "What is the performance?", "md_docs_2guides_2FAQ.html#autotoc_md1939", null ],
          [ "How do I enable asynchronous logging?", "md_docs_2guides_2FAQ.html#autotoc_md1941", null ],
          [ "How do I optimize performance?", "md_docs_2guides_2FAQ.html#autotoc_md1943", null ]
        ] ],
        [ "Integration", "md_docs_2guides_2FAQ.html#autotoc_md1945", [
          [ "How do I integrate with monitoring_system?", "md_docs_2guides_2FAQ.html#autotoc_md1946", null ],
          [ "How do I integrate with thread_system?", "md_docs_2guides_2FAQ.html#autotoc_md1948", null ]
        ] ],
        [ "Advanced Topics", "md_docs_2guides_2FAQ.html#autotoc_md1950", [
          [ "Is it thread-safe?", "md_docs_2guides_2FAQ.html#autotoc_md1951", null ],
          [ "How do I create custom log formats?", "md_docs_2guides_2FAQ.html#autotoc_md1953", null ],
          [ "How do I filter logs?", "md_docs_2guides_2FAQ.html#autotoc_md1955", null ],
          [ "How do I handle critical logs?", "md_docs_2guides_2FAQ.html#autotoc_md1957", null ],
          [ "Where can I find more information?", "md_docs_2guides_2FAQ.html#autotoc_md1959", null ]
        ] ]
      ] ]
    ] ],
    [ "GETTING_STARTED.kr", "md_docs_2guides_2GETTING__STARTED_8kr.html", [
      [ "Logger System 시작하기", "md_docs_2guides_2GETTING__STARTED_8kr.html#autotoc_md1963", null ]
    ] ],
    [ "GETTING_STARTED", "md_docs_2guides_2GETTING__STARTED.html", [
      [ "Getting Started with Logger System", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1968", [
        [ "autotoc_md1966", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1966", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1967", null ],
        [ "Table of Contents", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1969", null ],
        [ "Requirements", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1970", null ],
        [ "Installation", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1971", [
          [ "Using CMake FetchContent (Recommended)", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1972", null ],
          [ "Building from Source", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1973", null ],
          [ "Using as Installed Package", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1974", null ]
        ] ],
        [ "Basic Usage", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1975", [
          [ "Simple Console Logging", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1976", null ],
          [ "Logging with Source Location", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1977", null ],
          [ "Structured JSON Output", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1978", null ],
          [ "Log Levels", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1979", null ],
          [ "Filtering by Level", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1980", null ]
        ] ],
        [ "Configuration", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1981", [
          [ "Synchronous vs Asynchronous Logging", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1982", null ],
          [ "Multiple Writers", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1983", null ],
          [ "Windows Notes (Networking)", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1984", null ],
          [ "Console Writer Options", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1985", null ]
        ] ],
        [ "Integration with Thread System", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1986", [
          [ "Using Service Container", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1987", null ],
          [ "Direct Integration", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1988", null ]
        ] ],
        [ "Next Steps", "md_docs_2guides_2GETTING__STARTED.html#autotoc_md1989", null ]
      ] ]
    ] ],
    [ "INTEGRATION", "md_docs_2guides_2INTEGRATION.html", [
      [ "Logger System Integration Guide", "md_docs_2guides_2INTEGRATION.html#autotoc_md1993", [
        [ "autotoc_md1991", "md_docs_2guides_2INTEGRATION.html#autotoc_md1991", null ],
        [ "category: \"INTR\"", "md_docs_2guides_2INTEGRATION.html#autotoc_md1992", null ],
        [ "Table of Contents", "md_docs_2guides_2INTEGRATION.html#autotoc_md1995", null ],
        [ "Overview", "md_docs_2guides_2INTEGRATION.html#autotoc_md1996", null ],
        [ "System Dependencies", "md_docs_2guides_2INTEGRATION.html#autotoc_md1997", [
          [ "Required Dependencies", "md_docs_2guides_2INTEGRATION.html#autotoc_md1998", [
            [ "common_system (Header-only)", "md_docs_2guides_2INTEGRATION.html#autotoc_md1999", null ]
          ] ],
          [ "Optional Dependencies", "md_docs_2guides_2INTEGRATION.html#autotoc_md2000", [
            [ "thread_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2001", null ],
            [ "monitoring_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2002", null ]
          ] ]
        ] ],
        [ "Integration with common_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2003", [
          [ "ILogger Interface Implementation", "md_docs_2guides_2INTEGRATION.html#autotoc_md2004", null ],
          [ "Result<T> Pattern Usage", "md_docs_2guides_2INTEGRATION.html#autotoc_md2005", null ],
          [ "Adapter Pattern", "md_docs_2guides_2INTEGRATION.html#autotoc_md2006", null ]
        ] ],
        [ "Integration with thread_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2007", [
          [ "Asynchronous Logging with IExecutor", "md_docs_2guides_2INTEGRATION.html#autotoc_md2008", null ],
          [ "Cross-System Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2009", null ]
        ] ],
        [ "Integration with monitoring_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2010", [
          [ "IMonitor Interface Injection", "md_docs_2guides_2INTEGRATION.html#autotoc_md2011", null ],
          [ "Performance Metrics Collection", "md_docs_2guides_2INTEGRATION.html#autotoc_md2012", null ],
          [ "Health Monitoring Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2013", null ]
        ] ],
        [ "Build Configuration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2014", [
          [ "CMake Integration Examples", "md_docs_2guides_2INTEGRATION.html#autotoc_md2015", [
            [ "Minimal Configuration (common_system only)", "md_docs_2guides_2INTEGRATION.html#autotoc_md2016", null ],
            [ "Full Ecosystem Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2017", null ]
          ] ],
          [ "Build Configuration Flags", "md_docs_2guides_2INTEGRATION.html#autotoc_md2018", null ],
          [ "vcpkg Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2019", null ]
        ] ],
        [ "Code Examples", "md_docs_2guides_2INTEGRATION.html#autotoc_md2020", [
          [ "Example 1: Basic Integration with common_system", "md_docs_2guides_2INTEGRATION.html#autotoc_md2021", null ],
          [ "Example 2: Full Stack Integration", "md_docs_2guides_2INTEGRATION.html#autotoc_md2022", null ],
          [ "Example 3: Error Handling with Result<T>", "md_docs_2guides_2INTEGRATION.html#autotoc_md2023", null ]
        ] ],
        [ "Result Handling Cheatsheet", "md_docs_2guides_2INTEGRATION.html#autotoc_md2024", null ],
        [ "Error Code Registry", "md_docs_2guides_2INTEGRATION.html#autotoc_md2025", [
          [ "Allocated Error Code Range", "md_docs_2guides_2INTEGRATION.html#autotoc_md2026", null ],
          [ "Error Message Mapping", "md_docs_2guides_2INTEGRATION.html#autotoc_md2027", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2guides_2INTEGRATION.html#autotoc_md2028", [
          [ "Build Fails with \"common_system not found\"", "md_docs_2guides_2INTEGRATION.html#autotoc_md2029", null ],
          [ "Linker Errors: Undefined References", "md_docs_2guides_2INTEGRATION.html#autotoc_md2030", null ],
          [ "Runtime: Logger Not Outputting", "md_docs_2guides_2INTEGRATION.html#autotoc_md2031", null ],
          [ "Integration: IMonitor Not Injecting", "md_docs_2guides_2INTEGRATION.html#autotoc_md2032", null ],
          [ "Performance: High Latency", "md_docs_2guides_2INTEGRATION.html#autotoc_md2033", null ]
        ] ],
        [ "Best Practices", "md_docs_2guides_2INTEGRATION.html#autotoc_md2034", [
          [ "Use Result<T> for Error Handling", "md_docs_2guides_2INTEGRATION.html#autotoc_md2035", null ],
          [ "Prefer Interface-Based Design", "md_docs_2guides_2INTEGRATION.html#autotoc_md2036", null ],
          [ "Leverage DI for Testing", "md_docs_2guides_2INTEGRATION.html#autotoc_md2037", null ],
          [ "Monitor Performance in Production", "md_docs_2guides_2INTEGRATION.html#autotoc_md2038", null ]
        ] ],
        [ "References", "md_docs_2guides_2INTEGRATION.html#autotoc_md2039", null ]
      ] ]
    ] ],
    [ "MIGRATION_GUIDE.kr", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html", [
      [ "로거 시스템 마이그레이션 가이드", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2042", [
        [ "autotoc_md2040", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2040", null ],
        [ "category: \"MIGR\"", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2041", null ],
        [ "목차", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2043", null ],
        [ "개요", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2044", null ],
        [ "CMake 구성 변경", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2045", [
          [ "v2.x (이전)", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2046", null ],
          [ "v3.0 (현재)", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2047", null ],
          [ "CMake 플래그 변경 요약", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2048", null ],
          [ "타겟명 변경", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2049", null ],
          [ "의존성 변경", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2050", null ]
        ] ],
        [ "v2.x에서 v3.0으로 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2051", [
          [ "주요 변경사항 요약", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2052", null ],
          [ "네임스페이스 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2053", null ],
          [ "인터페이스 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2054", null ],
          [ "Result 타입 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2055", null ],
          [ "thread_system 의존성 변경", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2056", null ],
          [ "백엔드 선택 (v3.0 신규)", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2057", null ],
          [ "C++20 source_location (v3.0 신규)", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2058", null ]
        ] ],
        [ "마이그레이션 체크리스트", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2059", [
          [ "v2.x → v3.0 마이그레이션", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2060", null ]
        ] ],
        [ "일반적인 문제와 해결책", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2061", [
          [ "문제 1: 네임스페이스를 찾을 수 없음", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2062", null ],
          [ "문제 2: 인터페이스 타입 불일치", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2063", null ],
          [ "문제 3: C++20 미활성화", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2064", null ]
        ] ],
        [ "지원 및 리소스", "md_docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md2065", null ]
      ] ]
    ] ],
    [ "MIGRATION_GUIDE", "md_docs_2guides_2MIGRATION__GUIDE.html", [
      [ "Logger System Migration Guide", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2070", [
        [ "autotoc_md2068", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2068", null ],
        [ "category: \"MIGR\"", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2069", null ],
        [ "Table of Contents", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2071", null ],
        [ "Overview", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2072", [
          [ "Breaking Changes Summary", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2073", null ]
        ] ],
        [ "From v3.x to v4.0", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2075", [
          [ "What Changed", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2076", null ],
          [ "Migration for Standard Users", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2077", null ],
          [ "Migration for Custom Writer Authors", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2078", null ],
          [ "Context ID API Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2079", null ],
          [ "Unified Context API (New in v4.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2080", [
            [ "Why This Change?", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2081", null ],
            [ "Migration Table", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2082", null ],
            [ "Context Categories", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2083", null ],
            [ "Code Examples", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2084", null ],
            [ "Thread Safety", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2085", null ],
            [ "Migration Script", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2086", null ],
            [ "Deprecation Timeline", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2087", null ]
          ] ]
        ] ],
        [ "CMake Configuration Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2089", [
          [ "v2.x (Previous)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2090", null ],
          [ "v3.0 (Current)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2091", null ],
          [ "CMake Flag Changes Summary", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2092", null ],
          [ "Target Name Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2093", null ],
          [ "Dependency Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2094", null ]
        ] ],
        [ "Version Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2095", [
          [ "From v2.x to v3.0", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2096", [
            [ "Summary of Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2097", null ],
            [ "Namespace Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2098", null ],
            [ "Interface Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2099", null ],
            [ "Dual API Support", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2100", null ],
            [ "Result Type Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2101", null ],
            [ "thread_system Dependency Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2102", null ],
            [ "Backend Selection (New in v3.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2103", null ],
            [ "C++20 Source Location", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2104", null ],
            [ "Configuration Strategies (Enhanced in v3.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2105", null ],
            [ "Monitoring Integration (New in v3.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2106", null ],
            [ "Migration Script", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2107", null ]
          ] ],
          [ "From v1.x to v2.x", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2109", [
            [ "Error Handling Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2110", null ],
            [ "Memory Management Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2111", null ],
            [ "Configuration Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2112", null ],
            [ "Logging API Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2113", null ]
          ] ]
        ] ],
        [ "API Changes", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2114", [
          [ "v3.0 API Changes Summary", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2115", null ],
          [ "Deprecated Native log_level API (Planned for Removal in v3.0.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2116", null ],
          [ "Core Logger API", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2117", null ]
        ] ],
        [ "Configuration Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2118", [
          [ "From INI/XML Configuration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2119", null ],
          [ "Environment-based Configuration (v3.0)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2120", null ]
        ] ],
        [ "Migration from Other Libraries", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2121", [
          [ "From spdlog", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2122", null ],
          [ "From Boost.Log", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2123", null ],
          [ "From Google glog", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2124", null ]
        ] ],
        [ "Compatibility Wrappers", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2125", [
          [ "v2.x to v3.0 Compatibility Header", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2126", null ],
          [ "Legacy API Wrapper (v1.x compatibility)", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2127", null ]
        ] ],
        [ "Step-by-Step Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2128", [
          [ "Phase 1: Preparation", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2129", null ],
          [ "Phase 2: Namespace Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2130", null ],
          [ "Phase 3: Interface Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2131", null ],
          [ "Phase 4: Validation", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2132", null ]
        ] ],
        [ "Common Issues and Solutions", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2133", [
          [ "Issue 1: Namespace Not Found", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2134", null ],
          [ "Issue 2: Interface Type Mismatch", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2135", null ],
          [ "Issue 3: Result Type Mismatch", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2136", null ],
          [ "Issue 4: Missing thread_system", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2137", null ],
          [ "Issue 5: C++20 Not Enabled", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2138", null ]
        ] ],
        [ "Migration Checklist", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2139", [
          [ "v2.x to v3.0 Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2140", null ],
          [ "v1.x to v2.x Migration", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2141", null ]
        ] ],
        [ "Support and Resources", "md_docs_2guides_2MIGRATION__GUIDE.html#autotoc_md2142", null ]
      ] ]
    ] ],
    [ "OPENTELEMETRY", "md_docs_2guides_2OPENTELEMETRY.html", [
      [ "OpenTelemetry Integration Guide", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2146", [
        [ "autotoc_md2144", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2144", null ],
        [ "category: \"INTR\"", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2145", null ],
        [ "Table of Contents", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2147", null ],
        [ "Overview", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2148", null ],
        [ "Installation", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2149", [
          [ "With vcpkg", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2150", null ],
          [ "With CMake", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2151", null ],
          [ "Dependencies", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2152", null ]
        ] ],
        [ "Quick Start", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2153", [
          [ "Basic OTLP Export", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2154", null ],
          [ "With Trace Correlation", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2155", null ],
          [ "Using RAII Scope Guard", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2156", null ]
        ] ],
        [ "Configuration", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2157", [
          [ "OTLP Writer Configuration", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2158", null ],
          [ "Protocol Selection", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2159", null ]
        ] ],
        [ "Trace Context", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2160", [
          [ "otel_context Structure", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2161", null ],
          [ "Thread-Local Storage", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2162", null ],
          [ "Context in Log Entries", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2163", null ]
        ] ],
        [ "OTLP Writer", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2164", [
          [ "Statistics Monitoring", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2165", null ],
          [ "Health Checking", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2166", null ],
          [ "Force Flush", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2167", null ]
        ] ],
        [ "Integration Examples", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2168", [
          [ "With OpenTelemetry Collector", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2169", null ],
          [ "Docker Compose Setup", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2170", null ],
          [ "W3C Trace Context Parsing", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2171", null ]
        ] ],
        [ "Best Practices", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2172", [
          [ "Always Set Service Name", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2173", null ],
          [ "Use RAII Scope Guards", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2174", null ],
          [ "Configure Appropriate Batch Sizes", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2175", null ],
          [ "Handle Collector Unavailability", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2176", null ],
          [ "Include Meaningful Resource Attributes", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2177", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2178", [
          [ "Logs Not Appearing in Collector", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2179", null ],
          [ "High Log Drop Rate", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2180", null ],
          [ "Connection Timeouts", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2181", null ],
          [ "Missing Trace Context", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2182", null ]
        ] ],
        [ "Related Documentation", "md_docs_2guides_2OPENTELEMETRY.html#autotoc_md2183", null ]
      ] ]
    ] ],
    [ "PERFORMANCE.kr", "md_docs_2guides_2PERFORMANCE_8kr.html", [
      [ "Logger System 성능 가이드", "md_docs_2guides_2PERFORMANCE_8kr.html#autotoc_md2186", null ]
    ] ],
    [ "PERFORMANCE", "md_docs_2guides_2PERFORMANCE.html", [
      [ "Logger System Performance Guide", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2191", [
        [ "autotoc_md2189", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2189", null ],
        [ "category: \"PERF\"", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2190", null ],
        [ "Overview", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2192", null ],
        [ "Performance Characteristics", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2193", [
          [ "Synchronous Mode", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2194", null ],
          [ "Asynchronous Mode", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2195", null ]
        ] ],
        [ "Benchmarks", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2196", [
          [ "Test Environment", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2197", null ],
          [ "Single Thread Performance", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2198", null ],
          [ "Multi-threaded Performance", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2199", null ],
          [ "Memory Usage", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2200", null ]
        ] ],
        [ "Optimization Strategies", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2201", [
          [ "Choose the Right Mode", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2202", null ],
          [ "Buffer Size Tuning", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2203", null ],
          [ "2.1 Batch Size Considerations", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2204", null ],
          [ "Level Filtering", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2205", null ],
          [ "Message Construction", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2206", null ],
          [ "Writer Optimization", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2207", [
            [ "Console Writer", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2208", null ],
            [ "Custom High-Performance Writer", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2209", null ]
          ] ],
          [ "Batch Processing", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2210", null ]
        ] ],
        [ "Performance Anti-patterns", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2211", [
          [ "Synchronous I/O in Hot Paths", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2212", null ],
          [ "Excessive String Formatting", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2213", null ],
          [ "Logging in Tight Loops", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2214", null ]
        ] ],
        [ "Profiling and Monitoring", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2215", [
          [ "Built-in Metrics", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2216", null ],
          [ "External Profiling", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2217", null ]
        ] ],
        [ "Best Practices Summary", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2218", null ],
        [ "Platform-Specific Optimizations", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2219", [
          [ "Linux", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2220", null ],
          [ "Windows", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2221", null ],
          [ "macOS", "md_docs_2guides_2PERFORMANCE.html#autotoc_md2222", null ]
        ] ]
      ] ]
    ] ],
    [ "QUICK_START.kr", "md_docs_2guides_2QUICK__START_8kr.html", [
      [ "빠른 시작 가이드", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2226", [
        [ "autotoc_md2224", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2224", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2225", null ],
        [ "사전 요구사항", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2228", null ],
        [ "설치", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2229", [
          [ "저장소 클론", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2230", null ],
          [ "의존성 설치", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2231", null ],
          [ "빌드", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2232", null ],
          [ "설치 확인", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2233", null ]
        ] ],
        [ "첫 번째 로거 만들기", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2235", [
          [ "애플리케이션 빌드", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2236", null ]
        ] ],
        [ "핵심 개념", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2238", [
          [ "로거 빌더", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2239", null ],
          [ "라이터", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2240", null ],
          [ "로그 레벨", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2241", null ]
        ] ],
        [ "구성 템플릿", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2243", [
          [ "Production (프로덕션)", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2244", null ],
          [ "Debug (디버그)", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2245", null ],
          [ "High Performance (고성능)", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2246", null ]
        ] ],
        [ "CMake 구성", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2248", [
          [ "핵심 빌드 옵션", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2249", null ],
          [ "기능 플래그", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2250", null ]
        ] ],
        [ "thread_system 통합 (선택 사항)", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2252", [
          [ "빌드 시 활성화", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2253", null ],
          [ "런타임 활성화", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2254", null ]
        ] ],
        [ "일반적인 패턴", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2256", [
          [ "오류 처리", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2257", null ],
          [ "여러 라이터 사용", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2258", null ],
          [ "정상 종료", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2259", null ]
        ] ],
        [ "다음 단계", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2261", null ],
        [ "문제 해결", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2263", [
          [ "일반적인 문제", "md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2264", null ]
        ] ]
      ] ]
    ] ],
    [ "QUICK_START", "md_docs_2guides_2QUICK__START.html", [
      [ "Quick Start Guide", "md_docs_2guides_2QUICK__START.html#autotoc_md2268", [
        [ "autotoc_md2266", "md_docs_2guides_2QUICK__START.html#autotoc_md2266", null ],
        [ "category: \"GUID\"", "md_docs_2guides_2QUICK__START.html#autotoc_md2267", null ],
        [ "Prerequisites", "md_docs_2guides_2QUICK__START.html#autotoc_md2270", null ],
        [ "Installation", "md_docs_2guides_2QUICK__START.html#autotoc_md2271", [
          [ "Clone the Repositories", "md_docs_2guides_2QUICK__START.html#autotoc_md2272", null ],
          [ "Install Dependencies", "md_docs_2guides_2QUICK__START.html#autotoc_md2273", null ],
          [ "Build", "md_docs_2guides_2QUICK__START.html#autotoc_md2274", null ],
          [ "Verify Installation", "md_docs_2guides_2QUICK__START.html#autotoc_md2275", null ]
        ] ],
        [ "Your First Logger", "md_docs_2guides_2QUICK__START.html#autotoc_md2277", [
          [ "Building Your Application", "md_docs_2guides_2QUICK__START.html#autotoc_md2278", null ]
        ] ],
        [ "Key Concepts", "md_docs_2guides_2QUICK__START.html#autotoc_md2280", [
          [ "Logger Builder", "md_docs_2guides_2QUICK__START.html#autotoc_md2281", null ],
          [ "Writers", "md_docs_2guides_2QUICK__START.html#autotoc_md2282", null ],
          [ "Log Levels", "md_docs_2guides_2QUICK__START.html#autotoc_md2283", null ]
        ] ],
        [ "Configuration Templates", "md_docs_2guides_2QUICK__START.html#autotoc_md2285", [
          [ "Production", "md_docs_2guides_2QUICK__START.html#autotoc_md2286", null ],
          [ "Debug", "md_docs_2guides_2QUICK__START.html#autotoc_md2287", null ],
          [ "High Performance", "md_docs_2guides_2QUICK__START.html#autotoc_md2288", null ]
        ] ],
        [ "CMake Configuration", "md_docs_2guides_2QUICK__START.html#autotoc_md2290", [
          [ "Core Build Options", "md_docs_2guides_2QUICK__START.html#autotoc_md2291", null ],
          [ "Feature Flags", "md_docs_2guides_2QUICK__START.html#autotoc_md2292", null ]
        ] ],
        [ "thread_system Integration (Optional)", "md_docs_2guides_2QUICK__START.html#autotoc_md2294", [
          [ "Enable at Build Time", "md_docs_2guides_2QUICK__START.html#autotoc_md2295", null ],
          [ "Enable at Runtime", "md_docs_2guides_2QUICK__START.html#autotoc_md2296", null ]
        ] ],
        [ "Common Patterns", "md_docs_2guides_2QUICK__START.html#autotoc_md2298", [
          [ "Error Handling", "md_docs_2guides_2QUICK__START.html#autotoc_md2299", null ],
          [ "Multiple Writers", "md_docs_2guides_2QUICK__START.html#autotoc_md2300", null ],
          [ "Graceful Shutdown", "md_docs_2guides_2QUICK__START.html#autotoc_md2301", null ]
        ] ],
        [ "Next Steps", "md_docs_2guides_2QUICK__START.html#autotoc_md2303", null ],
        [ "Troubleshooting", "md_docs_2guides_2QUICK__START.html#autotoc_md2305", [
          [ "Common Issues", "md_docs_2guides_2QUICK__START.html#autotoc_md2306", null ]
        ] ]
      ] ]
    ] ],
    [ "SECURITY.kr", "md_docs_2guides_2SECURITY_8kr.html", [
      [ "보안 가이드", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2310", [
        [ "autotoc_md2308", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2308", null ],
        [ "category: \"SECU\"", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2309", null ],
        [ "위협 모델 (범위)", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2311", null ],
        [ "현재 기능", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2312", null ],
        [ "중요한 제한사항", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2313", null ],
        [ "권장사항 (프로덕션)", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2314", null ],
        [ "안전한 구성 체크리스트", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2315", null ],
        [ "로드맵", "md_docs_2guides_2SECURITY_8kr.html#autotoc_md2316", null ]
      ] ]
    ] ],
    [ "SECURITY", "md_docs_2guides_2SECURITY.html", [
      [ "Security Guide", "md_docs_2guides_2SECURITY.html#autotoc_md2320", [
        [ "autotoc_md2318", "md_docs_2guides_2SECURITY.html#autotoc_md2318", null ],
        [ "category: \"SECU\"", "md_docs_2guides_2SECURITY.html#autotoc_md2319", null ],
        [ "Threat Model (Scope)", "md_docs_2guides_2SECURITY.html#autotoc_md2321", null ],
        [ "Current Capabilities", "md_docs_2guides_2SECURITY.html#autotoc_md2322", null ],
        [ "Important Limitations", "md_docs_2guides_2SECURITY.html#autotoc_md2323", null ],
        [ "Recommendations (Production)", "md_docs_2guides_2SECURITY.html#autotoc_md2324", null ],
        [ "Secure Configuration Checklist", "md_docs_2guides_2SECURITY.html#autotoc_md2325", null ],
        [ "Roadmap", "md_docs_2guides_2SECURITY.html#autotoc_md2326", null ]
      ] ]
    ] ],
    [ "README", "md_docs_2integration_2README.html", [
      [ "Logger System Integration Guide", "md_docs_2integration_2README.html#autotoc_md2330", [
        [ "autotoc_md2328", "md_docs_2integration_2README.html#autotoc_md2328", null ],
        [ "category: \"GUID\"", "md_docs_2integration_2README.html#autotoc_md2329", null ],
        [ "Overview", "md_docs_2integration_2README.html#autotoc_md2331", null ],
        [ "Integration Guides", "md_docs_2integration_2README.html#autotoc_md2332", null ],
        [ "Quick Start", "md_docs_2integration_2README.html#autotoc_md2333", [
          [ "Basic Logging", "md_docs_2integration_2README.html#autotoc_md2334", null ],
          [ "Structured Logging", "md_docs_2integration_2README.html#autotoc_md2335", null ],
          [ "Async Logging for High Performance", "md_docs_2integration_2README.html#autotoc_md2336", null ]
        ] ],
        [ "Integration Patterns", "md_docs_2integration_2README.html#autotoc_md2337", [
          [ "Dependency Injection", "md_docs_2integration_2README.html#autotoc_md2338", null ],
          [ "Multiple Loggers", "md_docs_2integration_2README.html#autotoc_md2339", null ]
        ] ],
        [ "Common Use Cases", "md_docs_2integration_2README.html#autotoc_md2340", [
          [ "Request/Response Logging", "md_docs_2integration_2README.html#autotoc_md2341", null ],
          [ "Log Rotation", "md_docs_2integration_2README.html#autotoc_md2342", null ],
          [ "Contextual Logging", "md_docs_2integration_2README.html#autotoc_md2343", null ]
        ] ],
        [ "Best Practices", "md_docs_2integration_2README.html#autotoc_md2344", null ],
        [ "Additional Resources", "md_docs_2integration_2README.html#autotoc_md2345", null ]
      ] ]
    ] ],
    [ "THREAD_SYSTEM.kr", "md_docs_2integration_2THREAD__SYSTEM_8kr.html", [
      [ "thread_system 통합 가이드", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2348", [
        [ "autotoc_md2346", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2346", null ],
        [ "category: \"GUID\"", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2347", null ],
        [ "개요", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2349", null ],
        [ "빠른 시작", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2350", [
          [ "빌드 시 활성화", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2351", null ],
          [ "런타임에 활성화", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2352", null ]
        ] ],
        [ "설정", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2353", [
          [ "CMake 옵션", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2354", null ],
          [ "컴파일 정의", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2355", null ]
        ] ],
        [ "의존성 구성", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2356", [
          [ "양방향 의존성 위험 (Issue #252)", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2357", null ],
          [ "권장 구성 매트릭스", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2358", null ],
          [ "안전한 구성 예제", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2359", null ],
          [ "CMake 충돌 감지", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2360", null ]
        ] ],
        [ "API 레퍼런스", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2361", [
          [ "thread_system_integration", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2362", [
            [ "메서드", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2363", null ]
          ] ],
          [ "async_backend_type", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2364", null ],
          [ "컴파일 타임 감지", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2365", null ]
        ] ],
        [ "사용 패턴", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2366", [
          [ "패턴 1: 애플리케이션 전체 공유 풀", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2367", null ],
          [ "패턴 2: 사용자 정의 풀 설정", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2368", null ],
          [ "패턴 3: 조건부 통합", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2369", null ]
        ] ],
        [ "스레드 안전성", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2370", null ],
        [ "성능 고려사항", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2371", [
          [ "thread_system 통합 사용 시기", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2372", null ],
          [ "오버헤드 비교", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2373", null ]
        ] ],
        [ "문제 해결", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2374", [
          [ "thread_system 미발견", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2375", null ],
          [ "런타임 감지 미작동", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2376", null ]
        ] ],
        [ "관련 문서", "md_docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2377", null ]
      ] ]
    ] ],
    [ "THREAD_SYSTEM", "md_docs_2integration_2THREAD__SYSTEM.html", [
      [ "Async Integration Guide", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2381", [
        [ "autotoc_md2379", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2379", null ],
        [ "category: \"GUID\"", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2380", null ],
        [ "Overview", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2382", null ],
        [ "Integration Approaches", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2383", [
          [ "Recommended: IExecutor Interface (v1.5.0+)", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2384", null ],
          [ "Legacy: Direct thread_system Integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2385", null ]
        ] ],
        [ "Quick Start", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2386", [
          [ "Enable at Build Time", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2387", null ],
          [ "Enable at Runtime", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2388", null ]
        ] ],
        [ "Configuration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2389", [
          [ "CMake Options", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2390", null ],
          [ "Compile Definitions", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2391", null ]
        ] ],
        [ "Dependency Configuration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2392", [
          [ "Bidirectional Dependency Risk (Issue #252 — Resolved)", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2393", null ],
          [ "Recommended Configuration Matrix", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2394", null ],
          [ "Safe Configuration Examples", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2395", null ],
          [ "CMake Conflict Detection", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2396", null ]
        ] ],
        [ "API Reference", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2397", [
          [ "thread_system_integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2398", [
            [ "Methods", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2399", null ]
          ] ],
          [ "async_backend_type", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2400", null ],
          [ "Compile-time Detection", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2401", null ]
        ] ],
        [ "Usage Patterns", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2402", [
          [ "Pattern 1: Application-wide Shared Pool", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2403", null ],
          [ "Pattern 2: Custom Pool Configuration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2404", null ],
          [ "Pattern 3: Conditional Integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2405", null ]
        ] ],
        [ "Thread Safety", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2406", null ],
        [ "Performance Considerations", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2407", [
          [ "When to Use thread_system Integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2408", null ],
          [ "Overhead Comparison", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2409", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2410", [
          [ "thread_system not found", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2411", null ],
          [ "Runtime detection not working", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2412", null ]
        ] ],
        [ "IExecutor Integration API (v1.5.0+)", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2413", [
          [ "executor_integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2414", null ],
          [ "executor_type", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2415", null ],
          [ "standalone_executor", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2416", null ],
          [ "Compile-time Detection", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2417", null ]
        ] ],
        [ "Migration from thread_system_integration", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2418", null ],
        [ "Related Documentation", "md_docs_2integration_2THREAD__SYSTEM.html#autotoc_md2419", null ]
      ] ]
    ] ],
    [ "LOG_SERVER_AND_CRASH_SAFETY", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html", [
      [ "Log Server and Crash-Safe Logger", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2423", [
        [ "autotoc_md2421", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2421", null ],
        [ "category: \"GUID\"", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2422", null ],
        [ "Sub-Documents", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2424", null ],
        [ "Overview", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2425", null ],
        [ "Quick Links", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2426", [
          [ "Log Server", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2427", null ],
          [ "Crash-Safe Logger", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2428", null ],
          [ "Combined Usage", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2429", null ]
        ] ],
        [ "Related Documentation", "md_docs_2LOG__SERVER__AND__CRASH__SAFETY.html#autotoc_md2430", null ]
      ] ]
    ] ],
    [ "LOG_SERVER_GUIDE", "md_docs_2LOG__SERVER__GUIDE.html", [
      [ "Log Server Guide", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2434", [
        [ "autotoc_md2432", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2432", null ],
        [ "category: \"GUID\"", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2433", null ],
        [ "Table of Contents", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2435", null ],
        [ "Overview", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2437", null ],
        [ "Architecture", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2439", [
          [ "Components", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2440", null ],
          [ "Role in Distributed Logging", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2441", null ],
          [ "Protocol", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2442", null ]
        ] ],
        [ "Configuration", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2444", [
          [ "server_config Structure", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2445", null ],
          [ "Configuration Examples", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2446", null ]
        ] ],
        [ "API Reference", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2448", [
          [ "log_server Class", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2449", null ],
          [ "log_server_factory Class", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2450", null ],
          [ "Method Details", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2451", [
            [ "start()", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2452", null ],
            [ "stop()", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2453", null ],
            [ "is_running()", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2454", null ]
          ] ]
        ] ],
        [ "Deployment Patterns", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2456", [
          [ "Pattern 1: Single Server with Multiple Clients", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2457", null ],
          [ "Pattern 2: Log Aggregation for ELK Stack", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2458", null ],
          [ "Pattern 3: Multi-Region Deployment", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2459", null ]
        ] ],
        [ "Integration Examples", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2461", [
          [ "Example 1: Basic Server Setup", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2462", null ],
          [ "Example 2: Programmatic Control", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2463", null ],
          [ "Example 3: Docker Deployment", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2464", null ]
        ] ],
        [ "Best Practices", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2466", [
          [ "Capacity Planning", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2467", null ],
          [ "Network Security", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2468", null ],
          [ "Storage Management", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2469", null ],
          [ "Monitoring", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2470", null ]
        ] ],
        [ "Troubleshooting", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2472", [
          [ "Problem: Server fails to start", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2473", null ],
          [ "Problem: Clients cannot connect", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2474", null ]
        ] ],
        [ "Related Documentation", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2476", [
          [ "Header Files", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2477", null ],
          [ "External Resources", "md_docs_2LOG__SERVER__GUIDE.html#autotoc_md2478", null ]
        ] ]
      ] ]
    ] ],
    [ "BASELINE.kr", "md_docs_2performance_2BASELINE_8kr.html", [
      [ "Logger System - 성능 기준 메트릭", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2482", [
        [ "autotoc_md2480", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2480", null ],
        [ "category: \"PERF\"", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2481", null ],
        [ "시스템 정보", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2485", [
          [ "하드웨어 구성", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2486", null ],
          [ "소프트웨어 구성", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2487", null ]
        ] ],
        [ "성능 메트릭", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2489", [
          [ "로깅 Throughput", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2490", null ],
          [ "Latency (Async Mode)", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2491", null ],
          [ "메모리 성능", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2492", null ]
        ] ],
        [ "벤치마크 결과", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2494", null ],
        [ "주요 특징", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2496", null ],
        [ "Baseline 검증", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2498", [
          [ "Phase 0 요구사항", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2499", null ],
          [ "수락 기준", "md_docs_2performance_2BASELINE_8kr.html#autotoc_md2500", null ]
        ] ]
      ] ]
    ] ],
    [ "BASELINE", "md_docs_2performance_2BASELINE.html", [
      [ "Logger System - Performance Baseline Metrics", "md_docs_2performance_2BASELINE.html#autotoc_md2504", [
        [ "autotoc_md2502", "md_docs_2performance_2BASELINE.html#autotoc_md2502", null ],
        [ "category: \"PERF\"", "md_docs_2performance_2BASELINE.html#autotoc_md2503", null ],
        [ "System Information", "md_docs_2performance_2BASELINE.html#autotoc_md2507", [
          [ "Hardware Configuration", "md_docs_2performance_2BASELINE.html#autotoc_md2508", null ],
          [ "Software Configuration", "md_docs_2performance_2BASELINE.html#autotoc_md2509", null ]
        ] ],
        [ "Performance Metrics", "md_docs_2performance_2BASELINE.html#autotoc_md2511", [
          [ "Logging Throughput", "md_docs_2performance_2BASELINE.html#autotoc_md2512", null ],
          [ "Latency (Async Mode)", "md_docs_2performance_2BASELINE.html#autotoc_md2513", null ],
          [ "Memory Performance", "md_docs_2performance_2BASELINE.html#autotoc_md2514", null ]
        ] ],
        [ "Benchmark Results", "md_docs_2performance_2BASELINE.html#autotoc_md2516", null ],
        [ "Key Features", "md_docs_2performance_2BASELINE.html#autotoc_md2518", null ],
        [ "Baseline Validation", "md_docs_2performance_2BASELINE.html#autotoc_md2520", [
          [ "Phase 0 Requirements", "md_docs_2performance_2BASELINE.html#autotoc_md2521", null ],
          [ "Acceptance Criteria", "md_docs_2performance_2BASELINE.html#autotoc_md2522", null ]
        ] ]
      ] ]
    ] ],
    [ "CI_CD_PERFORMANCE_PROPOSAL", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html", [
      [ "CI/CD 성능 지표 자동화 제안서", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2526", [
        [ "autotoc_md2524", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2524", null ],
        [ "category: \"PERF\"", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2525", null ],
        [ "📊 Executive Summary", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2528", null ],
        [ "🔍 현재 상태 분석", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2530", [
          [ "✅ 장점", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2531", null ],
          [ "❌ 개선 필요 사항", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2532", null ]
        ] ],
        [ "🎯 제안 솔루션 아키텍처", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2534", [
          [ "전체 워크플로우", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2535", null ]
        ] ],
        [ "📁 제안 디렉토리 구조", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2537", null ],
        [ "🛠️ 구현 상세 (4단계)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2539", [
          [ "Phase 1: 벤치마크 재활성화 및 JSON 출력 설정", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2540", [
            [ "1.1 GitHub Actions 워크플로우 수정", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2541", null ],
            [ "1.2 벤치마크 실행 및 JSON 저장", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2542", null ],
            [ "1.3 API 변경에 따른 벤치마크 코드 수정", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2543", null ]
          ] ],
          [ "Phase 2: 결과 파싱 및 기준선 비교", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2545", [
            [ "2.1 벤치마크 결과 파싱 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2546", null ],
            [ "2.2 기준선 비교 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2547", null ]
          ] ],
          [ "Phase 3: 배지 및 차트 생성", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2549", [
            [ "3.1 성능 배지 생성 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2550", null ],
            [ "3.2 트렌드 차트 생성 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2551", null ]
          ] ],
          [ "Phase 4: README.md 자동 업데이트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2553", [
            [ "4.1 README 업데이트 스크립트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2554", null ],
            [ "4.2 GitHub Actions에서 자동 커밋", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2555", null ]
          ] ]
        ] ],
        [ "🎨 README.md 성능 섹션 디자인", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2557", [
          [ "제안 1: 배지 중심 디자인", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2558", null ],
          [ "제안 2: 테이블 중심 디자인", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2559", null ],
          [ "제안 3: 카드 스타일 디자인", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2560", null ]
        ] ],
        [ "🔄 CI/CD 워크플로우 전체 예시", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2562", null ],
        [ "📈 예상 결과물", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2564", [
          [ "GitHub Actions 실행 결과", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2565", null ],
          [ "PR 자동 코멘트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2566", null ],
          [ "업데이트된 README.md", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2567", null ]
        ] ],
        [ "✅ 구현 체크리스트", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2569", [
          [ "Phase 1: 인프라 구축 (예상 소요: 2시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2570", null ],
          [ "Phase 2: 배지 및 차트 (예상 소요: 1.5시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2571", null ],
          [ "Phase 3: README 자동 업데이트 (예상 소요: 1시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2572", null ],
          [ "Phase 4: CI/CD 통합 (예상 소요: 2시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2573", null ],
          [ "Phase 5: 테스트 및 검증 (예상 소요: 1.5시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2574", null ],
          [ "Phase 6: 문서화 (예상 소요: 1시간)", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2575", null ]
        ] ],
        [ "🔧 트러블슈팅 가이드", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2577", [
          [ "문제 1: 벤치마크 빌드 실패", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2578", null ],
          [ "문제 2: Python 스크립트 실행 오류", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2579", null ],
          [ "문제 3: Git 자동 커밋 권한 오류", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2580", null ],
          [ "문제 4: 배지가 README에 표시되지 않음", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2581", null ]
        ] ],
        [ "📚 참고 자료", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2583", [
          [ "도구 및 라이브러리", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2584", null ],
          [ "관련 프로젝트 예시", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2585", null ],
          [ "내부 문서", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2586", null ]
        ] ],
        [ "🎯 성공 지표", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2588", null ],
        [ "📝 다음 단계", "md_docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2590", null ]
      ] ]
    ] ],
    [ "DECORATOR_PERFORMANCE", "md_docs_2performance_2DECORATOR__PERFORMANCE.html", [
      [ "Decorator Pattern Performance Characteristics", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2594", [
        [ "autotoc_md2592", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2592", null ],
        [ "category: \"PERF\"", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2593", null ],
        [ "Overview", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2595", null ],
        [ "Executive Summary", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2596", null ],
        [ "Benchmark Methodology", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2597", [
          [ "Test Environment", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2598", null ],
          [ "Measured Configurations", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2599", null ],
          [ "Metrics", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2600", null ]
        ] ],
        [ "Performance Results", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2601", [
          [ "Throughput Comparison", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2602", null ],
          [ "Latency Analysis", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2603", null ],
          [ "Memory Overhead", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2604", null ]
        ] ],
        [ "Detailed Benchmark Results", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2605", [
          [ "Baseline: Direct File Writer", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2606", null ],
          [ "Single Decorator: Async", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2607", null ],
          [ "Double Decorator: Buffered + Async", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2608", null ],
          [ "Message Size Impact", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2609", null ]
        ] ],
        [ "Performance Recommendations", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2610", [
          [ "Production Guidelines", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2611", null ],
          [ "Decorator Ordering", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2612", null ],
          [ "Configuration Tuning", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2613", null ]
        ] ],
        [ "Running Benchmarks", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2614", [
          [ "Build and Execute", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2615", null ],
          [ "Using CMake Targets", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2616", null ],
          [ "Comparing Results", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2617", null ]
        ] ],
        [ "Regression Testing", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2618", [
          [ "CI Integration (Optional)", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2619", null ],
          [ "Regression Criteria", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2620", null ]
        ] ],
        [ "Known Limitations", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2621", [
          [ "Benchmark Accuracy", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2622", null ],
          [ "Excluded Scenarios", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2623", null ]
        ] ],
        [ "Conclusions", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2624", null ],
        [ "Related Documentation", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2625", null ],
        [ "Version History", "md_docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2626", null ]
      ] ]
    ] ],
    [ "PRODUCTION_QUALITY.kr", "md_docs_2PRODUCTION__QUALITY_8kr.html", [
      [ "Logger System 프로덕션 품질", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2629", [
        [ "autotoc_md2627", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2627", null ],
        [ "category: \"QUAL\"", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2628", null ],
        [ "요약", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2631", [
          [ "개발 상태: ✅ 개발 중", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2632", null ]
        ] ],
        [ "빌드 & 테스팅 인프라", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2634", [
          [ "지속적 통합 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2635", [
            [ "메인 CI 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2636", null ],
            [ "새니타이저 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2637", null ],
            [ "성능 벤치마크 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2638", null ],
            [ "코드 커버리지 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2639", null ],
            [ "정적 분석 파이프라인", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2640", null ]
          ] ]
        ] ],
        [ "스레드 안전성 & 동시성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2642", [
          [ "스레드 안전성 등급: A+", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2643", null ],
          [ "Writer 스레드 안전성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2644", null ],
          [ "비동기 처리 아키텍처", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2645", null ]
        ] ],
        [ "리소스 관리 (RAII)", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2647", [
          [ "RAII 준수 등급: A", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2648", null ],
          [ "스마트 포인터 사용", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2649", null ],
          [ "메모리 누수 탐지", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2650", null ],
          [ "예외 안전성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2651", null ]
        ] ],
        [ "오류 처리 전략", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2653", [
          [ "오류 처리 등급: A (90% 완료)", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2654", null ],
          [ "Result<T> 패턴", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2655", null ],
          [ "오류 코드 레지스트리", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2656", null ]
        ] ],
        [ "성능 특성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2658", [
          [ "기준선 메트릭", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2659", null ]
        ] ],
        [ "보안 기능", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2661", [
          [ "보안 등급: A (v3.0.0)", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2662", null ],
          [ "보안 키 저장소", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2663", null ],
          [ "경로 검증", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2664", null ],
          [ "시그널 핸들러 안전성", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2665", null ],
          [ "보안 감사 로깅", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2666", null ],
          [ "컴플라이언스 지원", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2667", null ]
        ] ],
        [ "플랫폼 지원", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2669", [
          [ "공식 지원 플랫폼", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2670", null ],
          [ "최소 요구사항", "md_docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2671", null ]
        ] ]
      ] ]
    ] ],
    [ "PRODUCTION_QUALITY", "md_docs_2PRODUCTION__QUALITY.html", [
      [ "Logger System Production Quality", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2677", [
        [ "autotoc_md2675", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2675", null ],
        [ "category: \"QUAL\"", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2676", null ],
        [ "Table of Contents", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2679", null ],
        [ "Executive Summary", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2681", [
          [ "Quality Status", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2682", null ]
        ] ],
        [ "Build & Testing Infrastructure", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2684", [
          [ "Continuous Integration Pipelines", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2685", [
            [ "Main CI Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2686", null ],
            [ "Sanitizer Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2688", null ],
            [ "Performance Benchmark Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2690", null ],
            [ "Code Coverage Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2692", null ],
            [ "Static Analysis Pipeline", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2694", null ]
          ] ],
          [ "Test Organization", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2696", [
            [ "Unit Tests", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2697", null ],
            [ "Integration Tests", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2699", null ],
            [ "Benchmarks", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2701", null ]
          ] ]
        ] ],
        [ "Thread Safety & Concurrency", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2703", [
          [ "Thread Safety Grade: A+", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2704", null ],
          [ "Writer Thread Safety", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2705", null ],
          [ "Asynchronous Processing Architecture", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2707", null ],
          [ "Concurrency Patterns", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2709", null ]
        ] ],
        [ "Resource Management (RAII)", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2711", [
          [ "RAII Compliance Grade: A", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2712", null ],
          [ "Smart Pointer Usage", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2713", null ],
          [ "Memory Leak Detection", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2715", null ],
          [ "Exception Safety", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2717", null ]
        ] ],
        [ "Error Handling Strategy", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2719", [
          [ "Error Handling Grade: A (90% Complete)", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2720", null ],
          [ "Result<T> Pattern", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2721", null ],
          [ "Error Code Registry", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2723", null ],
          [ "Hot Path Optimization", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2725", null ]
        ] ],
        [ "Performance Characteristics", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2727", [
          [ "Baseline Metrics", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2728", null ],
          [ "Performance Baselines (CI/CD)", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2730", null ]
        ] ],
        [ "Security Features", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2732", [
          [ "Security Grade: A (v3.0.0)", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2733", null ],
          [ "Secure Key Storage", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2734", null ],
          [ "Path Validation", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2735", null ],
          [ "Signal Handler Safety", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2736", null ],
          [ "Security Audit Logging", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2737", null ],
          [ "Compliance Support", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2738", null ],
          [ "OWASP Top 10 Mitigations", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2739", null ]
        ] ],
        [ "Operational Metrics", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2741", [
          [ "Built-in Metrics Collection", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2742", null ],
          [ "Health Checks", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2744", null ]
        ] ],
        [ "Platform Support", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2746", [
          [ "Officially Supported Platforms", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2747", null ],
          [ "Minimum Requirements", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2748", null ],
          [ "Cross-Platform Compatibility", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2750", null ]
        ] ],
        [ "Remaining Work & Future Enhancements", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2752", [
          [ "Current Status: 90% Production Ready", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2753", null ]
        ] ],
        [ "See Also", "md_docs_2PRODUCTION__QUALITY.html#autotoc_md2755", null ]
      ] ]
    ] ],
    [ "PROJECT_STRUCTURE.kr", "md_docs_2PROJECT__STRUCTURE_8kr.html", [
      [ "Logger System 프로젝트 구조", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2758", [
        [ "autotoc_md2756", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2756", null ],
        [ "category: \"PROJ\"", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2757", null ],
        [ "목차", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2760", null ],
        [ "디렉토리 개요", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2762", null ],
        [ "코어 모듈 파일", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2764", [
          [ "logger.h / logger.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2765", null ],
          [ "logger_builder.h / logger_builder.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2767", null ],
          [ "log_entry.h", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2769", null ],
          [ "result_types.h", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2771", null ]
        ] ],
        [ "Writer 구현", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2773", [
          [ "console_writer.h / console_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2774", null ],
          [ "file_writer.h / file_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2776", null ],
          [ "rotating_file_writer.h / rotating_file_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2778", null ],
          [ "network_writer.h / network_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2780", null ],
          [ "critical_writer.h / critical_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2782", null ],
          [ "hybrid_writer.h / hybrid_writer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2784", null ]
        ] ],
        [ "Filter 구현", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2786", [
          [ "level_filter.h / level_filter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2787", null ],
          [ "regex_filter.h / regex_filter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2789", null ],
          [ "function_filter.h / function_filter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2791", null ]
        ] ],
        [ "Formatter 구현", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2793", [
          [ "plain_formatter.h / plain_formatter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2794", null ],
          [ "json_formatter.h / json_formatter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2796", null ],
          [ "custom_formatter.h / custom_formatter.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2798", null ]
        ] ],
        [ "설정 시스템", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2800", [
          [ "config_templates.h / config_templates.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2801", null ],
          [ "config_validator.h / config_validator.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2803", null ]
        ] ],
        [ "보안 컴포넌트", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2805", [
          [ "secure_key_storage.h / secure_key_storage.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2806", null ],
          [ "path_validator.h / path_validator.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2808", null ],
          [ "log_sanitizer.h / log_sanitizer.cpp", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2810", null ]
        ] ],
        [ "테스팅 구성", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2812", [
          [ "유닛 테스트", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2813", null ],
          [ "통합 테스트", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2815", null ],
          [ "벤치마크", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2817", null ]
        ] ],
        [ "빌드 시스템", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2819", [
          [ "CMakeLists.txt (루트)", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2820", null ]
        ] ],
        [ "모듈 의존성", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2822", [
          [ "의존성 그래프", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2823", null ],
          [ "외부 의존성", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2824", null ]
        ] ],
        [ "파일 명명 규칙", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2826", [
          [ "헤더 파일", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2827", null ],
          [ "소스 파일", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2828", null ],
          [ "문서 파일", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2829", null ]
        ] ],
        [ "참고 문서", "md_docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2831", null ]
      ] ]
    ] ],
    [ "PROJECT_STRUCTURE", "md_docs_2PROJECT__STRUCTURE.html", [
      [ "Logger System Project Structure", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2836", [
        [ "autotoc_md2834", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2834", null ],
        [ "category: \"PROJ\"", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2835", null ],
        [ "Table of Contents", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2838", null ],
        [ "Directory Overview", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2840", null ],
        [ "Core Module Files", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2842", [
          [ "Header Files (include/kcenon/logger/core/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2843", null ],
          [ "Core Subdirectories", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2844", null ],
          [ "Source Files (src/core/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2845", null ],
          [ "Interfaces (include/kcenon/logger/interfaces/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2846", null ],
          [ "Root-Level Headers (include/kcenon/logger/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2847", null ]
        ] ],
        [ "Builder Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2849", [
          [ "writer_builder.h / writer_builder.cpp", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2850", null ]
        ] ],
        [ "Writer Implementations", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2852", [
          [ "Header Files (include/kcenon/logger/writers/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2853", null ],
          [ "Source Files (src/impl/writers/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2854", null ],
          [ "Async Processing (src/impl/async/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2855", null ]
        ] ],
        [ "Filter Implementations", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2857", [
          [ "log_filter.h", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2858", null ]
        ] ],
        [ "Formatter Implementations", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2860", [
          [ "Header Files (include/kcenon/logger/formatters/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2861", null ]
        ] ],
        [ "Factory Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2863", [
          [ "Header Files (include/kcenon/logger/factories/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2864", null ]
        ] ],
        [ "Adapter Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2866", [
          [ "Header Files (include/kcenon/logger/adapters/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2867", null ]
        ] ],
        [ "Backend Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2869", [
          [ "Header Files (include/kcenon/logger/backends/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2870", null ]
        ] ],
        [ "Sink Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2872", [
          [ "Header Files (include/kcenon/logger/sinks/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2873", null ]
        ] ],
        [ "Integration Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2875", [
          [ "Header Files (include/kcenon/logger/integration/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2876", null ],
          [ "Source Files (src/integration/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2877", null ]
        ] ],
        [ "Sampling Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2879", [
          [ "Header Files (include/kcenon/logger/sampling/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2880", null ],
          [ "Source Files (src/sampling/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2881", null ]
        ] ],
        [ "Utility Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2883", [
          [ "Header Files (include/kcenon/logger/utils/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2884", null ]
        ] ],
        [ "Security Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2886", [
          [ "Header Files (include/kcenon/logger/security/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2887", null ],
          [ "Source Files (src/security/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2888", null ]
        ] ],
        [ "Additional Components", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2890", [
          [ "Dependency Injection (include/kcenon/logger/di/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2891", null ],
          [ "OpenTelemetry (include/kcenon/logger/otlp/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2892", null ],
          [ "Routing (include/kcenon/logger/routing/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2893", null ],
          [ "Safety (include/kcenon/logger/safety/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2894", null ],
          [ "Analysis (include/kcenon/logger/analysis/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2895", null ],
          [ "Structured Logging (include/kcenon/logger/structured/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2896", null ],
          [ "Server (include/kcenon/logger/server/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2897", null ],
          [ "Internal Implementation Details (src/impl/)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2898", null ]
        ] ],
        [ "Testing Organization", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2900", [
          [ "Unit Tests", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2901", null ],
          [ "Integration Tests", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2903", null ],
          [ "Benchmarks", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2905", null ]
        ] ],
        [ "Build System", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2907", [
          [ "CMakeLists.txt (Root)", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2908", null ],
          [ "CMake Modules", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2910", null ]
        ] ],
        [ "Module Dependencies", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2912", [
          [ "Dependency Graph", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2913", null ],
          [ "External Dependencies", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2914", null ]
        ] ],
        [ "File Naming Conventions", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2916", [
          [ "Header Files", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2917", null ],
          [ "Source Files", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2918", null ],
          [ "Documentation Files", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2919", null ]
        ] ],
        [ "See Also", "md_docs_2PROJECT__STRUCTURE.html#autotoc_md2921", null ]
      ] ]
    ] ],
    [ "README.kr", "md_docs_2README_8kr.html", [
      [ "Logger System 문서", "md_docs_2README_8kr.html#autotoc_md2924", [
        [ "autotoc_md2922", "md_docs_2README_8kr.html#autotoc_md2922", null ],
        [ "category: \"GUID\"", "md_docs_2README_8kr.html#autotoc_md2923", null ],
        [ "📚 문서 구조", "md_docs_2README_8kr.html#autotoc_md2925", [
          [ "핵심 문서", "md_docs_2README_8kr.html#autotoc_md2926", null ],
          [ "가이드 (docs/guides/)", "md_docs_2README_8kr.html#autotoc_md2927", null ],
          [ "고급 주제 (docs/advanced/)", "md_docs_2README_8kr.html#autotoc_md2928", null ]
        ] ],
        [ "🚀 빠른 탐색", "md_docs_2README_8kr.html#autotoc_md2929", [
          [ "신규 사용자", "md_docs_2README_8kr.html#autotoc_md2930", null ],
          [ "기존 사용자", "md_docs_2README_8kr.html#autotoc_md2931", null ],
          [ "기여자", "md_docs_2README_8kr.html#autotoc_md2932", null ],
          [ "성능 엔지니어", "md_docs_2README_8kr.html#autotoc_md2933", null ]
        ] ],
        [ "🔧 문서 유지관리", "md_docs_2README_8kr.html#autotoc_md2934", [
          [ "최근 변경사항", "md_docs_2README_8kr.html#autotoc_md2935", null ]
        ] ],
        [ "📖 외부 리소스", "md_docs_2README_8kr.html#autotoc_md2936", null ],
        [ "🤝 도움 및 지원", "md_docs_2README_8kr.html#autotoc_md2937", null ]
      ] ]
    ] ],
    [ "README", "md_docs_2README.html", [
      [ "Logger System — Documentation Registry", "md_docs_2README.html#autotoc_md2942", [
        [ "autotoc_md2940", "md_docs_2README.html#autotoc_md2940", null ],
        [ "category: \"GUID\"", "md_docs_2README.html#autotoc_md2941", null ],
        [ "Document Index", "md_docs_2README.html#autotoc_md2943", null ],
        [ "Documents by Category", "md_docs_2README.html#autotoc_md2944", [
          [ "Architecture (9)", "md_docs_2README.html#autotoc_md2945", null ],
          [ "API Reference (2)", "md_docs_2README.html#autotoc_md2946", null ],
          [ "Features (2)", "md_docs_2README.html#autotoc_md2947", null ],
          [ "Guides (25)", "md_docs_2README.html#autotoc_md2948", null ],
          [ "Performance (8)", "md_docs_2README.html#autotoc_md2949", null ],
          [ "Migration (4)", "md_docs_2README.html#autotoc_md2950", null ],
          [ "Integration (2)", "md_docs_2README.html#autotoc_md2951", null ],
          [ "Quality (4)", "md_docs_2README.html#autotoc_md2952", null ],
          [ "Security (3)", "md_docs_2README.html#autotoc_md2953", null ],
          [ "Architecture Decision Records (2)", "md_docs_2README.html#autotoc_md2954", null ],
          [ "Project (10)", "md_docs_2README.html#autotoc_md2955", null ]
        ] ]
      ] ]
    ] ],
    [ "SECURITY_GUIDE", "md_docs_2SECURITY__GUIDE.html", [
      [ "Security Module Guide", "md_docs_2SECURITY__GUIDE.html#autotoc_md2959", [
        [ "autotoc_md2957", "md_docs_2SECURITY__GUIDE.html#autotoc_md2957", null ],
        [ "category: \"SECU\"", "md_docs_2SECURITY__GUIDE.html#autotoc_md2958", null ],
        [ "Table of Contents", "md_docs_2SECURITY__GUIDE.html#autotoc_md2961", null ],
        [ "Overview", "md_docs_2SECURITY__GUIDE.html#autotoc_md2963", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md2964", null ],
          [ "Components", "md_docs_2SECURITY__GUIDE.html#autotoc_md2965", null ]
        ] ],
        [ "Threat Model", "md_docs_2SECURITY__GUIDE.html#autotoc_md2967", [
          [ "Threats Defended Against", "md_docs_2SECURITY__GUIDE.html#autotoc_md2968", null ],
          [ "Security Assumptions", "md_docs_2SECURITY__GUIDE.html#autotoc_md2969", null ],
          [ "Out of Scope", "md_docs_2SECURITY__GUIDE.html#autotoc_md2970", null ]
        ] ],
        [ "Architecture", "md_docs_2SECURITY__GUIDE.html#autotoc_md2972", [
          [ "Component Integration", "md_docs_2SECURITY__GUIDE.html#autotoc_md2973", null ],
          [ "Dependency Graph", "md_docs_2SECURITY__GUIDE.html#autotoc_md2974", null ]
        ] ],
        [ "Audit Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md2976", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md2977", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md2978", null ],
          [ "Audit Event Types", "md_docs_2SECURITY__GUIDE.html#autotoc_md2979", null ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md2980", [
            [ "Initialize Audit Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md2981", null ],
            [ "Log an Audit Event", "md_docs_2SECURITY__GUIDE.html#autotoc_md2982", null ],
            [ "Verify Entry Integrity", "md_docs_2SECURITY__GUIDE.html#autotoc_md2983", null ]
          ] ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md2984", null ],
          [ "Compliance Considerations", "md_docs_2SECURITY__GUIDE.html#autotoc_md2985", null ]
        ] ],
        [ "Log Sanitizer", "md_docs_2SECURITY__GUIDE.html#autotoc_md2987", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md2988", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md2989", null ],
          [ "Sensitive Data Types", "md_docs_2SECURITY__GUIDE.html#autotoc_md2990", null ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md2991", [
            [ "Add Built-in Pattern", "md_docs_2SECURITY__GUIDE.html#autotoc_md2992", null ],
            [ "Add Custom Pattern", "md_docs_2SECURITY__GUIDE.html#autotoc_md2993", null ],
            [ "Remove Pattern", "md_docs_2SECURITY__GUIDE.html#autotoc_md2994", null ],
            [ "Sanitize a String", "md_docs_2SECURITY__GUIDE.html#autotoc_md2995", null ],
            [ "Check for Sensitive Data", "md_docs_2SECURITY__GUIDE.html#autotoc_md2996", null ],
            [ "Get Active Patterns", "md_docs_2SECURITY__GUIDE.html#autotoc_md2997", null ],
            [ "Add All Common Patterns", "md_docs_2SECURITY__GUIDE.html#autotoc_md2998", null ]
          ] ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md2999", null ],
          [ "Performance Impact", "md_docs_2SECURITY__GUIDE.html#autotoc_md3000", null ],
          [ "Integration with Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md3001", null ]
        ] ],
        [ "Path Validator", "md_docs_2SECURITY__GUIDE.html#autotoc_md3003", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md3004", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md3005", null ],
          [ "Security Checks", "md_docs_2SECURITY__GUIDE.html#autotoc_md3006", null ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md3007", [
            [ "Constructor", "md_docs_2SECURITY__GUIDE.html#autotoc_md3008", null ],
            [ "Validate Path", "md_docs_2SECURITY__GUIDE.html#autotoc_md3009", null ],
            [ "Static: Check Filename Safety", "md_docs_2SECURITY__GUIDE.html#autotoc_md3010", null ],
            [ "Static: Sanitize Filename", "md_docs_2SECURITY__GUIDE.html#autotoc_md3011", null ],
            [ "Static: Safe Path Join", "md_docs_2SECURITY__GUIDE.html#autotoc_md3012", null ]
          ] ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3013", null ],
          [ "Integration with Writers", "md_docs_2SECURITY__GUIDE.html#autotoc_md3014", null ]
        ] ],
        [ "Secure Key Storage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3016", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md3017", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md3018", null ],
          [ "Classes", "md_docs_2SECURITY__GUIDE.html#autotoc_md3019", [
            [ "secure_key", "md_docs_2SECURITY__GUIDE.html#autotoc_md3020", null ],
            [ "secure_key_storage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3021", null ]
          ] ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md3022", [
            [ "Generate Secure Random Key", "md_docs_2SECURITY__GUIDE.html#autotoc_md3023", null ],
            [ "Save Key to File", "md_docs_2SECURITY__GUIDE.html#autotoc_md3024", null ],
            [ "Load Key from File", "md_docs_2SECURITY__GUIDE.html#autotoc_md3025", null ]
          ] ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3026", null ],
          [ "Key Rotation", "md_docs_2SECURITY__GUIDE.html#autotoc_md3027", null ],
          [ "Platform-Specific Considerations", "md_docs_2SECURITY__GUIDE.html#autotoc_md3028", [
            [ "Linux/macOS", "md_docs_2SECURITY__GUIDE.html#autotoc_md3029", null ],
            [ "Windows", "md_docs_2SECURITY__GUIDE.html#autotoc_md3030", null ]
          ] ]
        ] ],
        [ "Signal Manager", "md_docs_2SECURITY__GUIDE.html#autotoc_md3032", [
          [ "Purpose", "md_docs_2SECURITY__GUIDE.html#autotoc_md3033", null ],
          [ "Key Features", "md_docs_2SECURITY__GUIDE.html#autotoc_md3034", null ],
          [ "Handled Signals", "md_docs_2SECURITY__GUIDE.html#autotoc_md3035", null ],
          [ "Signal-Safe Guarantees", "md_docs_2SECURITY__GUIDE.html#autotoc_md3036", null ],
          [ "API", "md_docs_2SECURITY__GUIDE.html#autotoc_md3037", [
            [ "Register Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md3038", null ],
            [ "Unregister Logger", "md_docs_2SECURITY__GUIDE.html#autotoc_md3039", null ],
            [ "Check Handler Installation", "md_docs_2SECURITY__GUIDE.html#autotoc_md3040", null ],
            [ "Get Logger Count", "md_docs_2SECURITY__GUIDE.html#autotoc_md3041", null ]
          ] ],
          [ "Critical Logger Interface", "md_docs_2SECURITY__GUIDE.html#autotoc_md3042", null ],
          [ "Example Usage", "md_docs_2SECURITY__GUIDE.html#autotoc_md3043", null ],
          [ "Emergency Flush Implementation", "md_docs_2SECURITY__GUIDE.html#autotoc_md3044", null ],
          [ "Testing Considerations", "md_docs_2SECURITY__GUIDE.html#autotoc_md3045", null ]
        ] ],
        [ "Configuration Examples", "md_docs_2SECURITY__GUIDE.html#autotoc_md3047", [
          [ "Minimal Security Setup", "md_docs_2SECURITY__GUIDE.html#autotoc_md3048", null ],
          [ "High-Security Setup (Compliance)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3049", null ],
          [ "Custom Security Layer", "md_docs_2SECURITY__GUIDE.html#autotoc_md3050", null ]
        ] ],
        [ "Compliance Considerations", "md_docs_2SECURITY__GUIDE.html#autotoc_md3052", [
          [ "SOX (Sarbanes-Oxley)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3053", null ],
          [ "HIPAA (Health Insurance Portability and Accountability Act)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3054", null ],
          [ "GDPR (General Data Protection Regulation)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3055", null ],
          [ "PCI-DSS (Payment Card Industry Data Security Standard)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3056", null ],
          [ "ISO 27001 (Information Security Management)", "md_docs_2SECURITY__GUIDE.html#autotoc_md3057", null ]
        ] ],
        [ "Frequently Asked Questions", "md_docs_2SECURITY__GUIDE.html#autotoc_md3059", [
          [ "Q: Can I use the security module without OpenSSL?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3060", null ],
          [ "Q: How do I rotate encryption keys?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3061", null ],
          [ "Q: What's the performance overhead of log sanitization?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3062", null ],
          [ "Q: How do I verify audit log integrity?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3063", null ],
          [ "Q: Can I use multiple loggers with one signal manager?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3064", null ],
          [ "Q: How do I handle signal manager cleanup?", "md_docs_2SECURITY__GUIDE.html#autotoc_md3065", null ]
        ] ],
        [ "Summary", "md_docs_2SECURITY__GUIDE.html#autotoc_md3067", null ]
      ] ]
    ] ],
    [ "SOUP", "md_docs_2SOUP.html", [
      [ "SOUP List — logger_system", "md_docs_2SOUP.html#autotoc_md3071", [
        [ "autotoc_md3069", "md_docs_2SOUP.html#autotoc_md3069", null ],
        [ "category: \"PROJ\"", "md_docs_2SOUP.html#autotoc_md3070", null ],
        [ "Production SOUP", "md_docs_2SOUP.html#autotoc_md3073", null ],
        [ "Optional SOUP", "md_docs_2SOUP.html#autotoc_md3075", null ],
        [ "Development/Test SOUP (Not Deployed)", "md_docs_2SOUP.html#autotoc_md3077", null ],
        [ "Safety Classification Key", "md_docs_2SOUP.html#autotoc_md3079", null ],
        [ "Version Pinning (IEC 62304 Compliance)", "md_docs_2SOUP.html#autotoc_md3081", null ],
        [ "Version Update Process", "md_docs_2SOUP.html#autotoc_md3083", null ],
        [ "License Compliance Summary", "md_docs_2SOUP.html#autotoc_md3085", null ]
      ] ]
    ] ],
    [ "TRACEABILITY", "md_docs_2TRACEABILITY.html", [
      [ "Traceability Matrix", "md_docs_2TRACEABILITY.html#autotoc_md3088", [
        [ "autotoc_md3086", "md_docs_2TRACEABILITY.html#autotoc_md3086", null ],
        [ "category: \"QUAL\"", "md_docs_2TRACEABILITY.html#autotoc_md3087", null ],
        [ "Feature -> Test -> Module Mapping", "md_docs_2TRACEABILITY.html#autotoc_md3089", [
          [ "Core Logger", "md_docs_2TRACEABILITY.html#autotoc_md3090", null ],
          [ "Writer Types", "md_docs_2TRACEABILITY.html#autotoc_md3091", null ],
          [ "Formatting & Filtering", "md_docs_2TRACEABILITY.html#autotoc_md3092", null ],
          [ "Security Features", "md_docs_2TRACEABILITY.html#autotoc_md3093", null ],
          [ "Advanced Capabilities", "md_docs_2TRACEABILITY.html#autotoc_md3094", null ],
          [ "OTLP Integration", "md_docs_2TRACEABILITY.html#autotoc_md3095", null ],
          [ "Log Sampling", "md_docs_2TRACEABILITY.html#autotoc_md3096", null ],
          [ "Monitoring & Health", "md_docs_2TRACEABILITY.html#autotoc_md3097", null ],
          [ "Integration & DI", "md_docs_2TRACEABILITY.html#autotoc_md3098", null ],
          [ "Utilities", "md_docs_2TRACEABILITY.html#autotoc_md3099", null ],
          [ "Production Quality", "md_docs_2TRACEABILITY.html#autotoc_md3100", null ]
        ] ],
        [ "Coverage Summary", "md_docs_2TRACEABILITY.html#autotoc_md3101", null ],
        [ "See Also", "md_docs_2TRACEABILITY.html#autotoc_md3102", null ]
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
      [ "Writer Composition and Decorator Guide", "md_docs_2WRITER__GUIDE.html#autotoc_md3105", [
        [ "autotoc_md3103", "md_docs_2WRITER__GUIDE.html#autotoc_md3103", null ],
        [ "category: \"GUID\"", "md_docs_2WRITER__GUIDE.html#autotoc_md3104", null ],
        [ "Table of Contents", "md_docs_2WRITER__GUIDE.html#autotoc_md3107", null ],
        [ "Architecture Overview", "md_docs_2WRITER__GUIDE.html#autotoc_md3109", [
          [ "Decorator Pattern", "md_docs_2WRITER__GUIDE.html#autotoc_md3110", null ],
          [ "Inheritance Chain", "md_docs_2WRITER__GUIDE.html#autotoc_md3111", null ],
          [ "Stack Visualization", "md_docs_2WRITER__GUIDE.html#autotoc_md3112", null ]
        ] ],
        [ "Writer Catalog", "md_docs_2WRITER__GUIDE.html#autotoc_md3114", [
          [ "Base Writers", "md_docs_2WRITER__GUIDE.html#autotoc_md3115", [
            [ "console_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3116", null ],
            [ "file_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3118", null ],
            [ "rotating_file_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3120", null ],
            [ "network_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3122", null ],
            [ "otlp_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3124", null ]
          ] ],
          [ "Decorator Writers", "md_docs_2WRITER__GUIDE.html#autotoc_md3126", [
            [ "async_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3127", null ],
            [ "batch_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3129", null ],
            [ "buffered_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3131", null ],
            [ "filtered_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3133", null ],
            [ "formatted_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3135", null ],
            [ "thread_safe_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3137", null ],
            [ "encrypted_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3139", null ],
            [ "critical_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3141", null ],
            [ "composite_writer", "md_docs_2WRITER__GUIDE.html#autotoc_md3143", null ],
            [ "queued_writer_base", "md_docs_2WRITER__GUIDE.html#autotoc_md3145", null ],
            [ "legacy_writer_adapter", "md_docs_2WRITER__GUIDE.html#autotoc_md3147", null ]
          ] ]
        ] ],
        [ "Composition Patterns", "md_docs_2WRITER__GUIDE.html#autotoc_md3149", [
          [ "Pattern 1: Simple Console (Development)", "md_docs_2WRITER__GUIDE.html#autotoc_md3150", null ],
          [ "Pattern 2: Async Console (Development + Performance)", "md_docs_2WRITER__GUIDE.html#autotoc_md3152", null ],
          [ "Pattern 3: Filtered Formatted File (Production)", "md_docs_2WRITER__GUIDE.html#autotoc_md3154", null ],
          [ "Pattern 4: High-Throughput Production", "md_docs_2WRITER__GUIDE.html#autotoc_md3156", null ],
          [ "Pattern 5: Encrypted Compliance Logging", "md_docs_2WRITER__GUIDE.html#autotoc_md3158", null ],
          [ "Pattern 6: Multi-Destination (File + Network)", "md_docs_2WRITER__GUIDE.html#autotoc_md3160", null ]
        ] ],
        [ "Recommended Stacks", "md_docs_2WRITER__GUIDE.html#autotoc_md3162", [
          [ "Development", "md_docs_2WRITER__GUIDE.html#autotoc_md3163", null ],
          [ "Production File Logging", "md_docs_2WRITER__GUIDE.html#autotoc_md3164", null ],
          [ "Compliance & Security", "md_docs_2WRITER__GUIDE.html#autotoc_md3165", null ],
          [ "Distributed Systems", "md_docs_2WRITER__GUIDE.html#autotoc_md3166", null ],
          [ "Resource-Constrained", "md_docs_2WRITER__GUIDE.html#autotoc_md3167", null ]
        ] ],
        [ "Code Examples", "md_docs_2WRITER__GUIDE.html#autotoc_md3169", [
          [ "Example 1: Development Setup (3 lines)", "md_docs_2WRITER__GUIDE.html#autotoc_md3170", null ],
          [ "Example 2: Production File Logging", "md_docs_2WRITER__GUIDE.html#autotoc_md3172", null ],
          [ "Example 3: Encrypted Compliance Logging", "md_docs_2WRITER__GUIDE.html#autotoc_md3174", null ],
          [ "Example 4: Multi-Destination Setup", "md_docs_2WRITER__GUIDE.html#autotoc_md3176", null ],
          [ "Example 5: Custom Stack Order", "md_docs_2WRITER__GUIDE.html#autotoc_md3178", null ]
        ] ],
        [ "Performance Impact", "md_docs_2WRITER__GUIDE.html#autotoc_md3180", [
          [ "Throughput per Configuration", "md_docs_2WRITER__GUIDE.html#autotoc_md3181", null ],
          [ "Latency per Decorator", "md_docs_2WRITER__GUIDE.html#autotoc_md3182", null ],
          [ "Memory Usage per Decorator", "md_docs_2WRITER__GUIDE.html#autotoc_md3183", null ]
        ] ],
        [ "Best Practices", "md_docs_2WRITER__GUIDE.html#autotoc_md3185", [
          [ "Filter Early, Format Late", "md_docs_2WRITER__GUIDE.html#autotoc_md3186", null ],
          [ "Use Async for I/O-Heavy Writers", "md_docs_2WRITER__GUIDE.html#autotoc_md3188", null ],
          [ "Batch + Buffer for Maximum Throughput", "md_docs_2WRITER__GUIDE.html#autotoc_md3190", null ],
          [ "Encrypt Only What's Necessary", "md_docs_2WRITER__GUIDE.html#autotoc_md3192", null ],
          [ "Use composite_writer for Multi-Destination", "md_docs_2WRITER__GUIDE.html#autotoc_md3194", null ],
          [ "Flush on Critical Logs", "md_docs_2WRITER__GUIDE.html#autotoc_md3196", null ],
          [ "Thread-Safety Only When Needed", "md_docs_2WRITER__GUIDE.html#autotoc_md3198", null ],
          [ "Start Async Writers Explicitly", "md_docs_2WRITER__GUIDE.html#autotoc_md3200", null ]
        ] ],
        [ "Summary", "md_docs_2WRITER__GUIDE.html#autotoc_md3202", null ]
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
"classkcenon_1_1logger_1_1adapters_1_1logger__adapter.html#a801c1a98d0fc8a02140b4a0b3b3defce",
"classkcenon_1_1logger_1_1async_1_1async__worker.html#a38fe968fe718ae235d3690a3e3c5ae99",
"classkcenon_1_1logger_1_1async_1_1simple__stop__source.html",
"classkcenon_1_1logger_1_1batch__writer.html#ade51c2c67ccacce411542df4ca0fb1aa",
"classkcenon_1_1logger_1_1core_1_1logger__context.html#aef4a1b9f6cabd8a81aae39ef9dd6a0eb",
"classkcenon_1_1logger_1_1file__writer.html#a7b5e41ceb5a57de69a1ee9e5011ca2eb",
"classkcenon_1_1logger_1_1formatter__factory.html#a6640392c833a4800b2df8ffb6e2566dd",
"classkcenon_1_1logger_1_1log__context__storage.html",
"classkcenon_1_1logger_1_1logger_1_1impl.html#ac0e6b0d01eaad6f879097a4f8dbcdf67",
"classkcenon_1_1logger_1_1memory_1_1thread__local__object__pool.html#af2d8969cbc3aef734b504ba5d2f24031",
"classkcenon_1_1logger_1_1output__sink__interface.html#abcb3c90352cb5b8156e55b5c6008e5e0",
"classkcenon_1_1logger_1_1safety_1_1crash__safe__logger.html#a990e584a3da1e6a3ef793b873852aeb1",
"classkcenon_1_1logger_1_1security_1_1log__sanitizer.html",
"classkcenon_1_1logger_1_1structured_1_1basic__structured__logger.html#a40617f187beba07728b5ccd0bae253b7",
"classkcenon_1_1logger_1_1unified__log__context.html#ae23905a5eac8262209ae6eb4c6f325ae",
"decorator__usage_8cpp.html#a9aa00470ae59104f2d2bf1ef03226dff",
"functions_func_d.html",
"logger__types_8h.html#ae3766003c8285b1cba6e1d4ee4475a19a9b18c2f10c10654136badda227767e2a",
"md_docs_2API__REFERENCE__WRITERS.html#autotoc_md943",
"md_docs_2CHANGELOG.html#autotoc_md1330",
"md_docs_2CRASH__SAFETY__GUIDE.html#autotoc_md1592",
"md_docs_2PRODUCTION__QUALITY.html#autotoc_md2697",
"md_docs_2SECURITY__GUIDE.html#autotoc_md2989",
"md_docs_2advanced_2ASYNC__WRITERS.html#autotoc_md239",
"md_docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md488",
"md_docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md763",
"md_docs_2guides_2INTEGRATION.html#autotoc_md2030",
"md_docs_2guides_2QUICK__START_8kr.html#autotoc_md2259",
"module__kcenon_8logger.html#a0ab93e4ca16dbe68651d775eb2bd5470",
"namespacekcenon_1_1logger.html#a3d04613d2854b686866a52f7d8407423abdfb01f3ea1f6351d0d53d79625c347f",
"namespacemembers_enum.html",
"structkcenon_1_1logger_1_1async_1_1batch__processor_1_1batch__entry.html#a161f0a9d44282a245cbd295e42f0c389",
"structkcenon_1_1logger_1_1encryption__config.html#a2ab2b676d5efc9a38498ca6a52c0b83a",
"structkcenon_1_1logger_1_1metrics_1_1logger__performance__stats.html#a24c9e86eebbea36aefa45b00663c56d3",
"structkcenon_1_1logger_1_1server_1_1server__config.html#a5eee6f00b7ee1aee3eb3c87d5b36e6e5"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';