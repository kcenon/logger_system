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
    [ "Logger System", "index.html#autotoc_md0", [
      [ "Overview", "index.html#autotoc_md1", null ],
      [ "Quick Start", "index.html#autotoc_md3", [
        [ "Basic Example", "index.html#autotoc_md4", null ],
        [ "Composing Writers with Decorator Pattern", "index.html#autotoc_md5", [
          [ "Decorator Application Order", "index.html#autotoc_md6", null ],
          [ "Common Usage Patterns", "index.html#autotoc_md7", null ]
        ] ],
        [ "Installation", "index.html#autotoc_md8", null ],
        [ "Requirements", "index.html#autotoc_md9", [
          [ "Dependency Flow", "index.html#autotoc_md10", null ],
          [ "Building with Dependencies", "index.html#autotoc_md11", null ]
        ] ]
      ] ],
      [ "Core Features", "index.html#autotoc_md13", [
        [ "Asynchronous Logging", "index.html#autotoc_md14", null ],
        [ "Multiple Writer Types", "index.html#autotoc_md15", null ],
        [ "OpenTelemetry Integration (v3.0.0)", "index.html#autotoc_md16", null ],
        [ "Security Features (v3.0.0)", "index.html#autotoc_md17", null ],
        [ "Structured Logging (v3.1.0)", "index.html#autotoc_md18", null ]
      ] ],
      [ "Performance Highlights", "index.html#autotoc_md20", [
        [ "Throughput", "index.html#autotoc_md21", null ],
        [ "Latency", "index.html#autotoc_md22", null ],
        [ "Memory Efficiency", "index.html#autotoc_md23", null ]
      ] ],
      [ "Architecture Overview", "index.html#autotoc_md25", [
        [ "Modular Design", "index.html#autotoc_md26", null ],
        [ "Key Components", "index.html#autotoc_md27", null ]
      ] ],
      [ "Ecosystem Integration", "index.html#autotoc_md29", [
        [ "Dependencies", "index.html#autotoc_md30", null ],
        [ "Integration Pattern", "index.html#autotoc_md31", null ]
      ] ],
      [ "Documentation", "index.html#autotoc_md33", [
        [ "Getting Started", "index.html#autotoc_md34", null ],
        [ "Core Documentation", "index.html#autotoc_md35", null ],
        [ "Advanced Topics", "index.html#autotoc_md36", null ],
        [ "Development", "index.html#autotoc_md37", null ]
      ] ],
      [ "Configuration Templates", "index.html#autotoc_md39", [
        [ "Advanced Configuration", "index.html#autotoc_md40", null ]
      ] ],
      [ "Build Configuration", "index.html#autotoc_md42", [
        [ "CMake Feature Flags", "index.html#autotoc_md43", null ]
      ] ],
      [ "Platform Support", "index.html#autotoc_md45", [
        [ "Officially Supported", "index.html#autotoc_md46", null ]
      ] ],
      [ "Testing", "index.html#autotoc_md48", [
        [ "Test Coverage", "index.html#autotoc_md49", null ],
        [ "Running Tests", "index.html#autotoc_md50", null ],
        [ "CI/CD Status", "index.html#autotoc_md51", null ]
      ] ],
      [ "Contributing", "index.html#autotoc_md53", [
        [ "Development Workflow", "index.html#autotoc_md54", null ],
        [ "Code Standards", "index.html#autotoc_md55", null ]
      ] ],
      [ "Support", "index.html#autotoc_md57", null ],
      [ "License", "index.html#autotoc_md59", null ],
      [ "Acknowledgments", "index.html#autotoc_md61", null ]
    ] ],
    [ "Architecture - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md64", null ],
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md66", null ],
      [ "Design Philosophy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md68", [
        [ "Core Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md69", null ]
      ] ],
      [ "Layered Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md71", [
        [ "Layer Responsibilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md72", null ]
      ] ],
      [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md74", [
        [ "1. Logger (<tt>core/logger.h</tt>, <tt>core/logger.cpp</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md75", null ],
        [ "2. Logger Builder (<tt>core/logger_builder.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md77", null ],
        [ "3. Log Collector (<tt>core/log_collector.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md79", null ],
        [ "4. Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md81", [
          [ "Console Writer (<tt>writers/console_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md82", null ],
          [ "File Writer (<tt>writers/file_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md83", null ],
          [ "Rotating File Writer (<tt>writers/rotating_file_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md84", null ],
          [ "Network Writer (<tt>writers/network_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md85", null ],
          [ "Critical Writer (<tt>writers/critical_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md86", null ],
          [ "Async Writer (<tt>writers/async_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md87", null ]
        ] ],
        [ "5. Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md89", [
          [ "Base Formatter (<tt>formatters/base_formatter.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md90", null ],
          [ "JSON Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md91", null ],
          [ "XML Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md92", null ]
        ] ],
        [ "6. Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md94", [
          [ "Level Filter (<tt>filters/log_filter.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md95", null ],
          [ "Regex Filter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md96", null ],
          [ "Function Filter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md97", null ]
        ] ]
      ] ],
      [ "Integration Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md99", [
        [ "Common System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md100", null ],
        [ "Thread System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md101", null ],
        [ "Monitoring System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md102", null ]
      ] ],
      [ "Threading Model", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md104", [
        [ "Asynchronous Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md105", null ],
        [ "Thread Safety Guarantees", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md106", null ]
      ] ],
      [ "Memory Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md108", [
        [ "Small String Optimization (SSO)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md109", null ],
        [ "Log Entry Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md110", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md112", [
        [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md113", null ],
        [ "Optimization Techniques", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md114", null ]
      ] ],
      [ "Design Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md116", [
        [ "1. Observer Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md117", null ],
        [ "2. Strategy Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md118", null ],
        [ "3. Builder Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md119", null ],
        [ "4. Template Method Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md120", null ],
        [ "5. Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md121", null ],
        [ "6. Factory Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md122", null ]
      ] ],
      [ "Build Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md124", [
        [ "CMake Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md125", null ],
        [ "Dependency Resolution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md126", null ],
        [ "Compiler Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md127", null ]
      ] ],
      [ "Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md129", [
        [ "Planned Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md130", null ],
        [ "Research Areas", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md131", null ]
      ] ],
      [ "Error Code Allocation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md133", null ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md135", null ]
    ] ],
    [ "비동기 Writer 구현체 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html", [
      [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md138", null ],
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md140", null ],
      [ "빠른 선택 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md142", [
        [ "선택 플로우차트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md143", null ]
      ] ],
      [ "구현체 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md145", null ],
      [ "상세 설명", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md147", [
        [ "1. async_writer (공개 API)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md148", [
          [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md149", null ],
          [ "아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md150", null ],
          [ "API 레퍼런스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md151", null ],
          [ "사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md152", null ]
        ] ],
        [ "2. async_worker (내부)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md154", [
          [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md155", null ],
          [ "아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md156", null ],
          [ "사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md157", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md158", [
        [ "3. high_performance_async_writer (내부)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md159", [
          [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md160", null ],
          [ "아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md161", null ],
          [ "설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md162", null ],
          [ "사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md163", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md164", null ],
      [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md165", [
        [ "벤치마크 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md166", null ],
        [ "지연 시간 분포", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md167", null ]
      ] ],
      [ "사용 예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md169", [
        [ "예제 1: 기본 async_writer 사용", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md170", null ],
        [ "예제 2: 콘솔 출력과 async_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md171", null ],
        [ "예제 3: 큐 가득 참 처리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md172", null ],
        [ "예제 4: 정상 종료 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md173", null ]
      ] ],
      [ "모범 사례", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md175", [
        [ "1. 큐 크기 선택", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md176", null ],
        [ "2. 플러시 타임아웃 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md177", null ],
        [ "3. 에러 처리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md178", null ],
        [ "4. 생명주기 관리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md179", null ],
        [ "5. 스레드 안전성 고려사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md180", null ]
      ] ],
      [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md182", [
        [ "동기에서 async_writer로", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md183", null ],
        [ "큐 가득 참 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md184", null ]
      ] ],
      [ "관련 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md186", null ]
    ] ],
    [ "Asynchronous Writer Implementations Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md189", null ],
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md191", null ],
      [ "Quick Decision Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md193", [
        [ "Decision Flowchart", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md194", null ]
      ] ],
      [ "Implementation Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md196", null ],
      [ "Detailed Descriptions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md198", [
        [ "1. async_writer (Public API)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md199", [
          [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md200", null ],
          [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md201", null ],
          [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md202", null ],
          [ "When to Use", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md203", null ]
        ] ],
        [ "2. async_worker (Internal)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md205", [
          [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md206", null ],
          [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md207", null ],
          [ "When to Use", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md208", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md209", [
        [ "3. high_performance_async_writer (Internal)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md210", [
          [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md211", null ],
          [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md212", null ],
          [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md213", null ],
          [ "When to Use", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md214", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md215", null ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md216", [
        [ "Benchmark Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md217", null ],
        [ "Latency Distribution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md218", null ]
      ] ],
      [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md220", [
        [ "Example 1: Basic async_writer Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md221", null ],
        [ "Example 2: async_writer with Console Output", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md222", null ],
        [ "Example 3: Queue Full Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md223", null ],
        [ "Example 4: Graceful Shutdown Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md224", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md226", [
        [ "1. Queue Size Selection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md227", null ],
        [ "2. Flush Timeout Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md228", null ],
        [ "3. Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md229", null ],
        [ "4. Lifecycle Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md230", null ],
        [ "5. Thread Safety Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md231", null ]
      ] ],
      [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md233", [
        [ "From Synchronous to async_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md234", null ],
        [ "Queue Full Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md235", null ]
      ] ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md237", null ]
    ] ],
    [ "CI_CD_DASHBOARD.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD_8kr.html", [
      [ "Logger System CI/CD 대시보드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD_8kr.html#autotoc_md239", null ]
    ] ],
    [ "CI_CD_DASHBOARD", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html", [
      [ "Logger System CI/CD Dashboard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md242", [
        [ "🚀 Build Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md243", [
          [ "Main Branch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md244", null ]
        ] ],
        [ "📊 Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md245", [
          [ "Latest Benchmark Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md246", null ],
          [ "Performance Trends", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md247", null ]
        ] ],
        [ "🔍 Code Quality", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md248", [
          [ "Static Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md249", null ],
          [ "Test Coverage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md250", null ]
        ] ],
        [ "🧪 Test Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md251", [
          [ "Test Execution Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md252", null ],
          [ "Recent Test Failures", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md253", null ]
        ] ],
        [ "🔥 Sanitizer Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md254", null ],
        [ "📈 Build Time Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md255", [
          [ "Average Build Times", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md256", null ]
        ] ],
        [ "🚨 Recent Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md257", [
          [ "Build Failures (Last 7 Days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md258", null ],
          [ "Performance Regressions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md259", null ]
        ] ],
        [ "📝 Dependency Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md260", [
          [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md261", null ]
        ] ],
        [ "🔄 CI/CD Pipeline Health", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md262", [
          [ "Workflow Execution Stats (Last 30 Days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md263", null ],
          [ "Resource Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md264", null ]
        ] ],
        [ "📅 Release History", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md265", [
          [ "Recent Releases", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md266", null ]
        ] ],
        [ "🔗 Quick Links", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md267", null ],
        [ "📊 Dashboard Update", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md268", null ],
        [ "<em>This dashboard is automatically updated by CI/CD workflows. For real-time data, check the <a href=\"https://github.com/kcenon/logger_system/actions\" >GitHub Actions page</a>.</em>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md270", null ]
      ] ]
    ] ],
    [ "Conditional Compilation Refactoring Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html", [
      [ "Problem Statement", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md272", [
        [ "Current State", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md273", null ],
        [ "Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md274", null ],
        [ "Example of Current Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md275", null ]
      ] ],
      [ "Proposed Solution: Adapter Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md276", [
        [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md277", null ],
        [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md278", null ],
        [ "Trade-offs", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md279", null ]
      ] ],
      [ "Implementation Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md280", [
        [ "Phase 3.1: Analysis and Documentation ✅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md281", null ],
        [ "Phase 3.2: Core Interface Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md282", null ],
        [ "Phase 3.3: Adapter Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md283", null ],
        [ "Phase 3.4: Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md284", null ],
        [ "Phase 3.5: Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md285", null ]
      ] ],
      [ "Detailed Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md286", [
        [ "Core Logger Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md287", null ],
        [ "Adapter Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md288", null ],
        [ "Thread System Adapter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md289", null ],
        [ "Common System Adapter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md290", null ],
        [ "Factory Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md291", null ]
      ] ],
      [ "Migration Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md292", [
        [ "Step 1: Extract Core (1 week)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md293", null ],
        [ "Step 2: Implement Adapters (1 week)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md294", null ],
        [ "Step 3: Update Public Interface (3 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md295", null ],
        [ "Step 4: Remove Conditional Compilation (2 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md296", null ],
        [ "Step 5: Testing and Validation (3 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md297", null ]
      ] ],
      [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md298", [
        [ "Virtual Function Overhead", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md299", null ],
        [ "Binary Size Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md300", null ]
      ] ],
      [ "Risk Mitigation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md301", [
        [ "Risks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md302", null ],
        [ "Rollback Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md303", null ]
      ] ],
      [ "Success Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md304", null ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md305", null ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html", [
      [ "크리티컬 로그 손실 방지 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html#autotoc_md307", null ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html", [
      [ "Critical Log Loss Prevention Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md311", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md310", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md312", [
          [ "Problem", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md313", null ],
          [ "Solution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md314", null ]
        ] ],
        [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md316", null ],
        [ "Class Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md318", [
          [ "1. <tt>critical_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md319", [
            [ "Key Features:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md320", null ],
            [ "Configuration:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md321", null ],
            [ "Usage Example:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md322", null ]
          ] ],
          [ "2. <tt>hybrid_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md323", [
            [ "Features:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md324", null ],
            [ "Usage Example:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md325", null ]
          ] ]
        ] ],
        [ "How It Works", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md327", [
          [ "1. Level-Based Routing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md328", null ],
          [ "2. Critical Write Flow", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md329", [
            [ "Step-by-Step Explanation:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md330", null ]
          ] ],
          [ "3. Write-Ahead Logging (WAL)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md331", [
            [ "WAL Format:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md332", null ],
            [ "WAL Usage:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md333", null ]
          ] ],
          [ "4. Signal Handler", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md334", [
            [ "Handled Signals:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md335", null ],
            [ "Signal Handler Implementation:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md336", null ],
            [ "Important Notes:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md337", null ]
          ] ],
          [ "5. File Descriptor Sync", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md338", [
            [ "Buffer Layers:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md339", null ],
            [ "Implementation:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md340", null ]
          ] ]
        ] ],
        [ "Performance Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md342", [
          [ "1. Critical Log Overhead", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md343", null ],
          [ "2. Normal Log Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md344", null ],
          [ "3. Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md345", [
            [ "Strategy 1: Disable <tt>sync_on_critical</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md346", null ],
            [ "Strategy 2: Disable <tt>force_flush_on_error</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md347", null ],
            [ "Strategy 3: Use Hybrid Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md348", null ]
          ] ],
          [ "4. Benchmark Results (Estimated)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md349", null ]
        ] ],
        [ "Production Recommendations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md351", [
          [ "1. Default Configuration (General Services)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md352", null ],
          [ "2. High Reliability Configuration (Finance/Medical)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md353", null ],
          [ "3. High Performance Configuration (Games/Real-time Systems)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md354", null ]
        ] ],
        [ "Testing & Verification", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md356", [
          [ "1. Critical Log Loss Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md357", [
            [ "Verification Method:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md358", null ]
          ] ],
          [ "2. WAL Recovery Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md359", null ],
          [ "3. Performance Benchmark", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md360", null ]
        ] ],
        [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md362", [
          [ "Issue 1: WAL File Not Created", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md363", null ],
          [ "Issue 2: Signal Handler Not Working", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md364", null ],
          [ "Issue 3: Performance Degradation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md365", null ]
        ] ],
        [ "FAQ", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md367", [
          [ "Q1: Should I wrap all logs with critical_writer?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md368", null ],
          [ "Q2: When should I use WAL?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md369", null ],
          [ "Q3: fsync() overhead is too high.", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md370", null ],
          [ "Q4: What can I do in a signal handler?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md371", null ]
        ] ],
        [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md373", null ],
        [ "Version History", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md375", null ],
        [ "License", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md377", null ]
      ] ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html", [
      [ "크리티컬 로깅 빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html#autotoc_md379", null ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html", [
      [ "Critical Logging Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md383", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md382", null ],
        [ "🎯 Goal", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md385", null ],
        [ "🚀 Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md387", [
          [ "Step 1: Include Headers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md388", null ],
          [ "Step 2: Create Hybrid Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md389", null ],
          [ "Step 3: Verify", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md390", null ]
        ] ],
        [ "📊 3 Usage Scenarios", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md392", [
          [ "Scenario 1: General Web Services (Recommended)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md393", null ],
          [ "Scenario 2: Finance/Medical Systems (Maximum Reliability)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md394", null ],
          [ "Scenario 3: Games/Real-time Systems (High Performance)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md395", null ]
        ] ],
        [ "🔧 Configuration Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md397", null ],
        [ "✅ Verification Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md399", [
          [ "Test 1: Check Critical Logs", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md400", null ],
          [ "Test 2: Crash Simulation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md401", null ],
          [ "Test 3: Ctrl+C Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md402", null ]
        ] ],
        [ "📚 Learn More", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md404", null ],
        [ "🐛 Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md406", [
          [ "\"WAL file not created\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md407", null ],
          [ "\"Performance too slow\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md408", null ],
          [ "\"Signal handler not working\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md409", null ]
        ] ],
        [ "💡 Key Points", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md411", null ]
      ] ]
    ] ],
    [ "CUSTOM_WRITERS.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS_8kr.html", [
      [ "커스텀 작성기 생성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS_8kr.html#autotoc_md414", null ]
    ] ],
    [ "CUSTOM_WRITERS", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html", [
      [ "Creating Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md417", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md418", null ],
        [ "Writer Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md419", null ],
        [ "Recommended: Using thread_safe_writer (Since v1.3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md420", [
          [ "Benefits of thread_safe_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md421", null ]
        ] ],
        [ "Base Writer Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md422", null ],
        [ "Simple Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md423", [
          [ "1. Simple File Writer (Using thread_safe_writer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md424", null ],
          [ "2. Rotating File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md425", null ],
          [ "3. Network Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md426", null ],
          [ "4. Database Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md427", null ]
        ] ],
        [ "Advanced Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md428", [
          [ "1. Filtering Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md429", null ],
          [ "2. Async Writer Wrapper", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md430", null ]
        ] ],
        [ "Choosing the Right Base Class", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md431", [
          [ "When to Use thread_safe_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md432", null ],
          [ "When to Use base_writer Directly", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md433", null ]
        ] ],
        [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md434", null ],
        [ "Testing Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md435", null ]
      ] ]
    ] ],
    [ "Log Level Semantic Standard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html", [
      [ "Problem Statement", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md439", [
        [ "logger_system (Ascending: Low → High)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md440", null ],
        [ "thread_system (Descending: High → Low)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md441", null ],
        [ "Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md442", null ]
      ] ],
      [ "Decision", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md444", [
        [ "Rationale", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md445", null ]
      ] ],
      [ "Migration Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md447", [
        [ "Phase 1: Documentation (Sprint 1 - Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md448", null ],
        [ "Phase 2: thread_system Unification (Sprint 2)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md449", null ],
        [ "Phase 3: Adapter Simplification (Sprint 3)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md450", null ]
      ] ],
      [ "Implementation Details", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md452", [
        [ "Standard Definition (logger_system - already compliant)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md453", null ],
        [ "thread_system Changes (Sprint 2)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md454", null ],
        [ "Adapter Simplification (Sprint 3)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md455", null ]
      ] ],
      [ "Compatibility Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md457", [
        [ "Breaking Change Assessment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md458", null ],
        [ "Mitigation Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md459", null ],
        [ "User Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md460", null ]
      ] ],
      [ "Testing Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md462", [
        [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md463", null ],
        [ "Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md464", null ],
        [ "Regression Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md465", null ]
      ] ],
      [ "Documentation Updates Required", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md467", null ],
      [ "Success Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md469", null ],
      [ "Timeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md471", null ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md473", null ],
      [ "Approval", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md475", null ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html", [
      [ "로거 시스템 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md477", [
        [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md478", [
          [ "주요 기능 (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md479", null ]
        ] ],
        [ "아키텍처 다이어그램", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md480", null ],
        [ "핵심 컴포넌트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md481", [
          [ "1. ILogger 인터페이스 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md482", null ],
          [ "2. Dual API 설계", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md483", null ],
          [ "3. 백엔드 추상화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md484", null ]
        ] ],
        [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md485", [
          [ "벤치마크 (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md486", null ],
          [ "멀티스레드 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md487", null ]
        ] ]
      ] ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html", [
      [ "Logger System Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md490", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md491", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md492", [
          [ "Key Features (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md493", null ]
        ] ],
        [ "Architecture Diagram", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md494", null ],
        [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md495", [
          [ "1. ILogger Interface Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md496", null ],
          [ "2. Dual API Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md497", null ],
          [ "3. Configuration Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md498", [
            [ "Logger Configuration with Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md499", null ],
            [ "Configuration Templates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md500", null ]
          ] ],
          [ "4. Builder Pattern with Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md501", null ],
          [ "5. Backend Abstraction", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md502", null ],
          [ "6. Interface Segregation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md503", [
            [ "Writer Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md504", null ],
            [ "Filter Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md505", null ],
            [ "Formatter Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md506", null ]
          ] ],
          [ "7. Log Entry Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md507", null ]
        ] ],
        [ "Advanced Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md508", [
          [ "1. Asynchronous Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md509", null ],
          [ "2. Error Handling with Result Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md510", null ],
          [ "3. C++20 Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md511", null ],
          [ "4. Performance Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md512", null ],
          [ "5. Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md513", null ]
        ] ],
        [ "Threading Model", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md514", [
          [ "Synchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md515", null ],
          [ "Asynchronous Mode (Default)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md516", null ],
          [ "Thread Safety Guarantees", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md517", null ]
        ] ],
        [ "Memory Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md518", [
          [ "Buffer Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md519", null ],
          [ "Object Lifetime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md520", null ]
        ] ],
        [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md521", [
          [ "Benchmarks (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md522", null ],
          [ "Multi-threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md523", null ],
          [ "Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md524", null ]
        ] ],
        [ "Integration Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md525", [
          [ "ILogger Interface Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md526", null ],
          [ "Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md527", null ],
          [ "With Monitoring (Phase 2.2.4)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md528", null ]
        ] ],
        [ "Extension Points", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md529", [
          [ "Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md530", null ],
          [ "Custom Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md531", null ],
          [ "Custom Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md532", null ]
        ] ],
        [ "Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md533", [
          [ "Performance Improvements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md534", null ],
          [ "Feature Additions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md535", null ],
          [ "Platform Extensions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md536", null ]
        ] ],
        [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md537", [
          [ "For Library Users", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md538", null ],
          [ "For Contributors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md539", null ]
        ] ],
        [ "Platform Notes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md540", null ]
      ] ]
    ] ],
    [ "Migration Guide - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md543", null ],
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md545", null ],
      [ "Quick Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md547", null ],
      [ "Migration Path 1: From Integrated thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md549", [
        [ "Background", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md550", null ],
        [ "Step 1: Update CMakeLists.txt", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md551", null ],
        [ "Step 2: Update Header Includes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md552", null ],
        [ "Step 3: Update Namespace", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md553", null ],
        [ "Step 4: Migrate Logger Creation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md554", null ],
        [ "Step 5: Update Thread System Integration (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md555", null ]
      ] ],
      [ "Migration Path 2: From spdlog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md557", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md558", null ],
        [ "API Mapping", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md559", null ],
        [ "Example Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md560", null ],
        [ "Pattern Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md561", null ]
      ] ],
      [ "Migration Path 3: From Custom Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md563", [
        [ "Common Custom Logger Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md564", [
          [ "Pattern 1: Direct File Writing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md565", null ],
          [ "Pattern 2: Multiple Output Targets", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md566", null ],
          [ "Pattern 3: Thread-Safe Singleton", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md567", null ]
        ] ]
      ] ],
      [ "Common Migration Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md569", [
        [ "Issue 1: Namespace Conflicts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md570", null ],
        [ "Issue 2: CMake Cache Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md571", null ],
        [ "Issue 3: Header Not Found", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md572", null ],
        [ "Issue 4: Result<T> Pattern Unfamiliar", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md573", null ],
        [ "Issue 5: Performance Regression", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md574", null ]
      ] ],
      [ "Testing Your Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md576", [
        [ "1. Compile Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md577", null ],
        [ "2. Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md578", null ],
        [ "3. Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md579", null ],
        [ "4. Performance Benchmark", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md580", null ]
      ] ],
      [ "Rollback Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md582", [
        [ "Option 1: Revert Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md583", null ],
        [ "Option 2: Gradual Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md584", null ]
      ] ],
      [ "Migration Assistance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md586", [
        [ "Automated Migration Script", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md587", null ]
      ] ],
      [ "Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md589", null ],
      [ "Next Steps After Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md591", null ]
    ] ],
    [ "Logger System - 프로젝트 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html", [
      [ "📁 디렉토리 레이아웃", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md595", null ],
      [ "🏗️ Namespace 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md596", [
        [ "Core Namespaces", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md597", null ],
        [ "Nested Namespaces", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md598", null ]
      ] ],
      [ "🔧 주요 컴포넌트 개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md599", [
        [ "🎯 Public API Layer (<tt>include/kcenon/logger/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md600", null ],
        [ "⚙️ Implementation Layer (<tt>src/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md601", null ]
      ] ],
      [ "📊 성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md602", null ],
      [ "🔄 마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md603", [
        [ "Step 1: 현재 설정 백업", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md604", null ],
        [ "Step 2: Include 경로 업데이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md605", null ],
        [ "Step 3: Namespace 사용 업데이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md606", null ],
        [ "Step 4: 마이그레이션 스크립트 실행", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md607", null ]
      ] ],
      [ "🚀 새로운 구조로 빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md608", null ]
    ] ],
    [ "Logger System - Project Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md611", null ],
      [ "Directory Layout", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md612", null ],
      [ "Namespace Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md613", [
        [ "Primary Namespace Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md614", null ],
        [ "Namespace Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md615", null ]
      ] ],
      [ "Component Layout", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md616", [
        [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md617", [
          [ "Logger Core (<tt>core/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md618", null ],
          [ "Interfaces (<tt>interfaces/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md619", null ],
          [ "Writers (<tt>writers/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md620", null ],
          [ "Formatters (<tt>formatters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md621", null ],
          [ "Filters (<tt>filters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md622", null ],
          [ "Routing (<tt>routing/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md623", null ],
          [ "Analysis (<tt>analysis/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md624", null ],
          [ "Structured Logging (<tt>structured/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md625", null ],
          [ "Safety (<tt>safety/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md626", null ],
          [ "Server (<tt>server/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md627", null ],
          [ "Adapters (<tt>adapters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md628", null ]
        ] ],
        [ "Monitoring (<tt>core/monitoring/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md629", null ]
      ] ],
      [ "Module Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md630", [
        [ "Internal Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md631", null ],
        [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md632", null ],
        [ "Compilation Units", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md633", null ]
      ] ],
      [ "Key Components Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md634", [
        [ "Public API Layer (<tt>include/kcenon/logger/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md635", null ],
        [ "Implementation Layer (<tt>src/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md636", null ],
        [ "Test Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md637", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md638", [
        [ "Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md639", null ],
        [ "Memory", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md640", null ],
        [ "Threading", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md641", null ],
        [ "Resource Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md642", null ]
      ] ],
      [ "Build Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md643", [
        [ "CMake Targets", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md644", null ],
        [ "Feature Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md645", null ]
      ] ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md646", null ]
    ] ],
    [ "WRITER_HIERARCHY", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html", [
      [ "Writer Hierarchy and Categories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md647", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md648", null ],
        [ "Category System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md649", [
          [ "Writer Categories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md650", null ],
          [ "Category Tags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md651", null ]
        ] ],
        [ "Complete Writer Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md652", null ],
        [ "Category Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md653", [
          [ "Synchronous Writers (sync_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md654", null ],
          [ "Asynchronous Writers (async_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md655", null ],
          [ "Decorator Writers (decorator_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md656", null ],
          [ "Composite Writers (composite_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md657", null ]
        ] ],
        [ "Using Category Information", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md658", [
          [ "Compile-Time Type Traits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md659", null ],
          [ "C++20 Concepts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md660", null ],
          [ "Runtime Category Query", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md661", null ]
        ] ],
        [ "Design Patterns Used", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md662", [
          [ "Template Method Pattern (thread_safe_writer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md663", null ],
          [ "Decorator Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md664", null ],
          [ "Pipeline Pattern (composite_writer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md665", null ]
        ] ],
        [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md666", null ]
      ] ]
    ] ],
    [ "WRITER_SELECTION_GUIDE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html", [
      [ "Writer Selection Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md668", [
        [ "Quick Selection Flowchart", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md669", null ],
        [ "Writer Selection by Use Case", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md670", [
          [ "1. Simple Console Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md671", null ],
          [ "2. Basic File Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md673", null ],
          [ "3. Production File Logging with Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md675", null ],
          [ "4. High-Throughput Logging (>10K msg/sec)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md677", null ],
          [ "5. Network/Remote Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md679", null ],
          [ "6. Encrypted/Secure Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md681", null ],
          [ "7. Critical/Safety-Critical Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md683", null ],
          [ "8. Custom Pipeline (Formatter + Sink)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md685", null ]
        ] ],
        [ "Decorator Stacking Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md687", [
          [ "Pattern 1: Async + Encrypted", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md688", null ],
          [ "Pattern 2: Critical + Async + Rotating", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md689", null ],
          [ "Pattern 3: Batch + Network", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md690", null ]
        ] ],
        [ "Performance Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md691", null ],
        [ "Decision Matrix", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md692", null ],
        [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md693", null ]
      ] ]
    ] ],
    [ "API_REFERENCE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html", [
      [ "API 레퍼런스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md695", [
        [ "v3.0 주요 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md696", [
          [ "네임스페이스 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md697", null ],
          [ "인터페이스 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md698", null ],
          [ "헤더 경로 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md699", null ],
          [ "주요 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md700", null ]
        ] ],
        [ "빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md701", null ]
      ] ]
    ] ],
    [ "API_REFERENCE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html", [
      [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md704", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md705", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md706", [
          [ "Namespace", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md707", null ]
        ] ],
        [ "Core Classes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md708", [
          [ "<tt>kcenon::logger::logger</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md709", [
            [ "Constructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md710", null ],
            [ "Destructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md711", null ]
          ] ]
        ] ],
        [ "ILogger Interface (Phase 2.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md712", [
          [ "Log Methods (ILogger Interface)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md713", null ],
          [ "Level Management (ILogger Interface)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md714", null ],
          [ "Native API (Backward Compatible)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md715", null ],
          [ "Writer Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md716", null ],
          [ "Lifecycle Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md717", null ],
          [ "Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md718", null ],
          [ "Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md719", null ],
          [ "Emergency Flush (Signal Safety)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md720", null ]
        ] ],
        [ "Unified Log Context (v3.3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md721", [
          [ "<tt>kcenon::logger::unified_log_context</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md722", [
            [ "Value Type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md723", null ],
            [ "Context Categories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md724", null ],
            [ "Constructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md725", null ],
            [ "Setters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md726", null ],
            [ "Getters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md727", null ],
            [ "Query Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md728", null ],
            [ "Removal and Export", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md729", null ],
            [ "Usage Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md730", null ],
            [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md731", null ]
          ] ]
        ] ],
        [ "Scoped Context Guard (v3.3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md733", [
          [ "<tt>kcenon::logger::scoped_context_guard</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md734", [
            [ "Constructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md735", null ],
            [ "Setters (Chainable)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md736", null ],
            [ "Usage Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md737", null ]
          ] ]
        ] ],
        [ "Structured Log Builder (v3.1.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md739", [
          [ "<tt>kcenon::logger::structured_log_builder</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md740", [
            [ "Constructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md741", null ],
            [ "Builder Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md742", null ],
            [ "Logger Integration (<tt>log_structured</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md743", null ],
            [ "Usage Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md744", null ],
            [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md745", null ]
          ] ]
        ] ],
        [ "Writer Builder (v4.1.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md747", [
          [ "<tt>kcenon::logger::writer_builder</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md748", [
            [ "Core Writers (Terminal Nodes)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md749", null ],
            [ "Decorator Writers (Middleware)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md750", null ],
            [ "Build and Utility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md751", null ],
            [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md752", null ]
          ] ]
        ] ],
        [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md754", [
          [ "<tt>kcenon::logger::logger_config</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md755", null ],
          [ "Static Factory Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md756", null ]
        ] ],
        [ "Builder Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md757", [
          [ "<tt>kcenon::logger::logger_builder</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md758", [
            [ "Basic Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md759", null ],
            [ "Feature Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md760", null ],
            [ "Output Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md761", null ],
            [ "Writers and Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md762", null ],
            [ "Backend Selection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md763", null ],
            [ "Environment and Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md764", null ],
            [ "Build", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md765", null ]
          ] ]
        ] ],
        [ "Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md766", [
          [ "Deployment Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md767", null ],
          [ "Performance Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md768", null ],
          [ "Environment Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md769", null ],
          [ "Composite Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md770", null ]
        ] ],
        [ "Interfaces", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md771", [
          [ "<tt>kcenon::logger::log_writer_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md772", null ],
          [ "<tt>kcenon::logger::log_filter_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md773", null ],
          [ "<tt>kcenon::logger::log_formatter_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md774", null ],
          [ "<tt>kcenon::logger::log_entry</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md775", null ]
        ] ],
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md776", [
          [ "Error Codes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md777", [
            [ "Error Code Categories Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md778", null ]
          ] ],
          [ "Result Types", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md779", null ],
          [ "Helper Functions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md780", [
            [ "<tt>kcenon::logger::result<T></tt> Wrapper", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md781", null ]
          ] ]
        ] ],
        [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md782", [
          [ "<tt>kcenon::logger::console_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md783", null ],
          [ "<tt>kcenon::logger::file_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md784", null ],
          [ "<tt>kcenon::logger::rotating_file_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md785", null ],
          [ "<tt>kcenon::logger::network_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md786", null ],
          [ "<tt>kcenon::logger::critical_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md787", null ],
          [ "<tt>kcenon::logger::batch_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md788", null ]
        ] ],
        [ "Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md789", [
          [ "<tt>kcenon::logger::filters::level_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md790", null ],
          [ "<tt>kcenon::logger::filters::regex_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md791", null ],
          [ "<tt>kcenon::logger::filters::function_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md792", null ],
          [ "<tt>kcenon::logger::filters::composite_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md793", null ]
        ] ],
        [ "Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md794", [
          [ "<tt>kcenon::logger::plain_formatter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md795", null ],
          [ "<tt>kcenon::logger::json_formatter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md796", null ]
        ] ],
        [ "Log Sampling (v3.3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md797", [
          [ "Sampling Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md798", null ],
          [ "<tt>kcenon::logger::sampling::sampling_config</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md799", [
            [ "Static Factory Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md800", null ]
          ] ],
          [ "<tt>kcenon::logger::sampling::sampling_stats</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md801", null ],
          [ "<tt>kcenon::logger::sampling::log_sampler</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md802", [
            [ "Key Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md803", null ]
          ] ],
          [ "<tt>kcenon::logger::sampling::sampler_factory</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md804", [
            [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md805", null ]
          ] ]
        ] ],
        [ "Real-time Analysis (v3.2.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md807", [
          [ "<tt>kcenon::logger::analysis::anomaly_event</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md808", null ],
          [ "<tt>kcenon::logger::analysis::realtime_analysis_config</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md809", null ],
          [ "<tt>kcenon::logger::analysis::realtime_log_analyzer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md810", [
            [ "Callback Type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md811", null ],
            [ "Constructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md812", null ],
            [ "Key Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md813", null ],
            [ "Statistics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md814", null ]
          ] ],
          [ "<tt>kcenon::logger::analysis::realtime_analyzer_factory</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md815", [
            [ "Usage Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md816", null ],
            [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md817", null ]
          ] ]
        ] ],
        [ "Backend Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md819", [
          [ "Integration Backend Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md820", null ],
          [ "Standalone Backend (Default)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md821", null ]
        ] ],
        [ "Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md822", [
          [ "IMonitor Integration (Phase 2.2.4)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md823", null ]
        ] ],
        [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md824", [
          [ "Basic Usage with ILogger Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md825", null ],
          [ "Native API (Backward Compatible)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md826", null ],
          [ "Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md827", null ],
          [ "Custom Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md828", null ],
          [ "Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md829", null ]
        ] ],
        [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md830", null ],
        [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md831", null ],
        [ "Migration from v2.x", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md832", null ]
      ] ]
    ] ],
    [ "ARCHITECTURE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html", [
      [ "Logger System 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md834", [
        [ "Logger 파이프라인 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md835", [
          [ "파이프라인 흐름", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md836", null ],
          [ "핵심 아키텍처 결정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md837", null ],
          [ "OTLP 관찰성 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md838", null ],
          [ "샘플링 및 분석 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md839", null ]
        ] ],
        [ "v3.0 주요 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md841", [
          [ "의존성 구조 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md842", null ],
          [ "핵심 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md843", null ],
          [ "독립 실행 모드 vs 통합 모드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md844", null ],
          [ "성능 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md845", null ]
        ] ]
      ] ]
    ] ],
    [ "ARCHITECTURE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html", [
      [ "Logger System Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md848", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md849", null ],
        [ "🔧 Logger Pipeline Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md850", [
          [ "Pipeline Flow", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md851", null ],
          [ "Backend Selection Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md852", null ],
          [ "Key Architectural Decisions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md853", null ]
        ] ],
        [ "✍️ Writer Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md855", null ],
        [ "🔭 OTLP & Observability Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md857", null ],
        [ "📊 Sampling & Analysis Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md859", null ],
        [ "🏗️ Ecosystem Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md861", null ],
        [ "📋 Project Roles & Responsibilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md862", [
          [ "1. common_system (Foundation) - <strong>Required</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md863", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md864", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md865", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md866", null ]
          ] ],
          [ "2. logger_system (Logging) - <strong>Standalone Capable</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md868", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md869", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md870", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md871", null ]
          ] ],
          [ "3. thread_system (Threading) - <strong>Optional</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md873", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md874", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md875", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md876", null ]
          ] ],
          [ "4. monitoring_system (Metrics)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md878", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md879", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md880", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md881", null ]
          ] ],
          [ "5. integrated_thread_system (Integration Hub)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md883", [
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md884", null ]
          ] ]
        ] ],
        [ "🔄 Dependency Flow & Interface Contracts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md885", [
          [ "Interface Hierarchy (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md886", null ],
          [ "Dependency Graph (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md887", null ],
          [ "Build Order Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md888", null ],
          [ "Standalone vs Integrated Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md889", null ]
        ] ],
        [ "🔧 Integration Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md890", [
          [ "1. Interface-Based Integration (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md891", null ],
          [ "2. Dependency Injection Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md892", null ],
          [ "3. Configuration Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md893", null ],
          [ "4. Backend Selection Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md894", null ]
        ] ],
        [ "📊 Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md895", [
          [ "Design Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md896", null ],
          [ "Performance Metrics (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md897", null ],
          [ "Standalone vs Thread System Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md898", null ]
        ] ],
        [ "🔄 Evolution: Monolithic → Modular → Standalone", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md899", [
          [ "Phase 1: Monolithic (v1.x)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md900", null ],
          [ "Phase 2: Modular (v2.x)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md901", null ],
          [ "Phase 3: Standalone (v3.0) - Current", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md902", null ],
          [ "Migration Benefits (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md903", null ]
        ] ],
        [ "🚀 Getting Started", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md904", [
          [ "1. Minimal Setup (Standalone)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md905", null ],
          [ "2. Full Ecosystem Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md906", null ],
          [ "3. Build Order", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md907", null ],
          [ "4. CMake Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md908", null ]
        ] ],
        [ "📚 Documentation Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md909", [
          [ "common_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md910", null ],
          [ "logger_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md911", null ],
          [ "thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md912", null ],
          [ "monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md913", null ]
        ] ],
        [ "🔮 Future Roadmap", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md914", [
          [ "Phase 3.1: Enhancement", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md915", null ],
          [ "Phase 3.2: Optimization (Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md916", null ],
          [ "Phase 4: Ecosystem Expansion", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md917", null ]
        ] ]
      ] ]
    ] ],
    [ "Logger System 성능 벤치마크", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html", [
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md922", null ],
      [ "요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md924", [
        [ "핵심 하이라이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md925", null ],
        [ "경쟁 우위", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md926", null ]
      ] ],
      [ "테스트 환경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md928", [
        [ "하드웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md929", null ],
        [ "소프트웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md930", null ]
      ] ],
      [ "핵심 성능 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md932", [
        [ "단일 스레드 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md933", null ],
        [ "멀티스레드 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md934", [
          [ "스레드 수별 처리량", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md935", null ]
        ] ]
      ] ],
      [ "업계 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md937", [
        [ "vs spdlog (가장 인기 있는 C++ 로거)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md938", [
          [ "단일 스레드 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md939", null ],
          [ "멀티스레드 비교 (4 스레드)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md940", null ]
        ] ],
        [ "vs Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md941", null ],
        [ "vs glog (Google 로깅 라이브러리)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md942", null ]
      ] ],
      [ "지연시간 벤치마크", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md944", [
        [ "지연시간 분포 (단일 스레드, 비동기)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md945", null ],
        [ "부하 시 지연시간", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md946", null ]
      ] ],
      [ "메모리 프로파일링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md948", [
        [ "구성별 메모리 풋프린트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md949", null ],
        [ "경쟁사와 메모리 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md950", null ]
      ] ],
      [ "성능 회귀 기준", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md952", null ]
    ] ],
    [ "Logger System Performance Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md957", null ],
      [ "Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md959", [
        [ "Key Highlights", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md960", null ],
        [ "Competitive Advantages", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md961", null ]
      ] ],
      [ "Test Environment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md963", [
        [ "Hardware Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md964", null ],
        [ "Software Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md965", null ]
      ] ],
      [ "Core Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md967", [
        [ "Single-Threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md968", null ],
        [ "Multi-Threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md969", [
          [ "Throughput by Thread Count", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md970", null ],
          [ "Performance Scaling Graph", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md971", null ]
        ] ]
      ] ],
      [ "Industry Comparisons", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md973", [
        [ "vs spdlog (Most Popular C++ Logger)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md974", [
          [ "Single-Threaded Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md975", null ],
          [ "Multi-Threaded Comparison (4 Threads)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md976", null ]
        ] ],
        [ "vs Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md977", null ],
        [ "vs glog (Google's Logging Library)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md978", null ],
        [ "vs log4cxx", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md979", null ]
      ] ],
      [ "Scalability Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md981", [
        [ "Thread Scaling Efficiency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md982", null ],
        [ "Queue Utilization vs Thread Count", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md983", null ],
        [ "CPU Utilization Efficiency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md984", null ]
      ] ],
      [ "Latency Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md986", [
        [ "Latency Distribution (Single Thread, Async)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md987", null ],
        [ "Latency Comparison (p99)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md988", null ],
        [ "Latency Under Load", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md989", null ]
      ] ],
      [ "Memory Profiling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md991", [
        [ "Memory Footprint by Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md992", null ],
        [ "Memory Comparison with Competitors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md993", null ],
        [ "Memory Allocation Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md994", null ]
      ] ],
      [ "Benchmark Methodology", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md996", [
        [ "Test Scenarios", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md997", [
          [ "1. Single-Threaded Throughput Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md998", null ],
          [ "2. Multi-Threaded Contention Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md999", null ],
          [ "3. Latency Measurement Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md1000", null ]
        ] ],
        [ "Measurement Tools", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md1001", null ],
        [ "Benchmark Caveats", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md1002", null ]
      ] ],
      [ "Performance Regression Baselines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md1004", null ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md1006", null ]
    ] ],
    [ "변경 이력 - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html", [
      [ "[3.0.0] - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1009", [
        [ "제거됨 - Deprecated API 정리 (Issues #268, #324)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1010", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1011", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1012", null ]
        ] ]
      ] ],
      [ "[Unreleased]", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1014", [
        [ "실시간 로그 분석 통합 (Issue #281)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1015", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1016", null ],
          [ "예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1017", null ]
        ] ],
        [ "고용량 시나리오를 위한 로그 샘플링 (Issue #282)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1019", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1020", null ],
          [ "예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1021", null ]
        ] ],
        [ "Phase 3.4: 구조화된 로깅 통합 (Issue #311) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1023", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1024", null ],
          [ "예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1025", null ]
        ] ],
        [ "Coverage 빌드 수정 (PR #291) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1027", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1028", null ]
        ] ],
        [ "CI 워크플로우 수정 (PR #290) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1030", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1031", null ]
        ] ],
        [ "OpenTelemetry 통합 수정 (Issue #283) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1033", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1034", null ]
        ] ],
        [ "C++20 모듈 파일 (Issue #275) - 2026-01-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1036", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1037", null ],
          [ "모듈 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1038", null ],
          [ "CMake 사용법", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1039", null ]
        ] ],
        [ "중복 Writer 추가 로직 수정 (Issue #270) - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1041", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1042", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1043", null ],
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1044", null ]
        ] ],
        [ "KCENON 기능 감지 사용 (Issue #250) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1046", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1047", null ],
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1048", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1049", null ]
        ] ],
        [ "Deprecated common_system API 마이그레이션 (Issue #248) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1051", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1052", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1053", null ]
        ] ],
        [ "thread_system v3.0 호환성 (Issue #244) - 2025-12-19", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1055", [
          [ "주요 변경 (Breaking Changes)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1056", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1057", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1058", null ]
        ] ],
        [ "#226 API 변경 후 손상된 예제 파일 수정 (Issue #228) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1060", [
          [ "제거됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1061", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1062", null ],
          [ "비활성화됨 (기능 구현 대기)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1063", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1064", null ]
        ] ],
        [ "common::interfaces::ILogger 구현 - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1066", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1067", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1068", null ],
          [ "사용 중단됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1069", null ],
          [ "하위 호환성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1070", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1071", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1072", null ]
        ] ],
        [ "독립형 비동기 구현 (Issue #222) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1074", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1075", null ],
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1076", null ],
          [ "이점", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1077", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1078", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1079", null ]
        ] ],
        [ "CMake fmt Fallback 제거 - 2025-12-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1081", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1082", null ],
          [ "제거됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1083", null ],
          [ "이점", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1084", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1085", null ]
        ] ]
      ] ],
      [ "[1.0.0] - 2025-10-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1087", [
        [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1088", null ],
        [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1089", null ],
        [ "사용 중단됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1090", null ],
        [ "제거됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1091", null ],
        [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1092", null ],
        [ "보안", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1093", null ]
      ] ],
      [ "프로젝트 정보", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1095", null ],
      [ "버전 지원 매트릭스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1097", null ],
      [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1099", null ]
    ] ],
    [ "Changelog - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html", [
      [ "[4.1.0] - Unreleased", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1103", [
        [ "Added - Writer Builder for Fluent Decorator Composition (Issue #413, #415, #416)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1104", [
          [ "New Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1105", null ],
          [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1106", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1107", null ],
          [ "Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1108", null ]
        ] ],
        [ "Deprecated - Legacy Writer Patterns (Issue #418)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1109", [
          [ "Deprecated Classes and Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1110", null ],
          [ "Deprecation Timeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1111", null ],
          [ "Migration Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1112", null ]
        ] ]
      ] ],
      [ "[4.0.0] - Unreleased", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1114", [
        [ "Removed - Deprecated Context ID Convenience Methods (Issue #326)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1115", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1116", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1117", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1118", null ]
        ] ],
        [ "Added - Decorator Pattern Writer Architecture (Issue #356)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1120", [
          [ "New Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1121", null ],
          [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1122", null ],
          [ "Architecture Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1123", null ],
          [ "Custom Writer Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1124", null ]
        ] ]
      ] ],
      [ "[3.0.0] - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1126", [
        [ "Removed - Deprecated log_level Types and Converters (Issue #339)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1127", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1128", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1129", null ]
        ] ],
        [ "Removed - Deprecated API Cleanup (Issues #268, #324)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1131", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1132", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1133", null ]
        ] ]
      ] ],
      [ "[Unreleased]", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1135", [
        [ "Real-time Log Analysis Integration (Issue #281)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1136", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1137", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1138", null ]
        ] ],
        [ "Log Sampling for High-Volume Scenarios (Issue #282)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1140", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1141", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1142", null ]
        ] ],
        [ "Phase 3.4: Structured Logging Integration (Issue #311) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1144", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1145", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1146", null ]
        ] ],
        [ "Phase 3.3: Structured Logging Formatters (Issue #310) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1148", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1149", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1150", null ]
        ] ],
        [ "Phase 3.1: Structured Logging API Design (Issue #308) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1152", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1153", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1154", null ]
        ] ],
        [ "Phase 3.2: Structured Logging Core Implementation (Issue #309) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1156", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1157", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1158", null ]
        ] ],
        [ "Coverage Build Fix (PR #291) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1160", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1161", null ]
        ] ],
        [ "CI Workflow Fix (PR #290) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1163", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1164", null ]
        ] ],
        [ "OpenTelemetry Integration Fix (Issue #283) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1166", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1167", null ]
        ] ],
        [ "C++20 Module Files (Issue #275) - 2026-01-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1169", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1170", null ],
          [ "Module Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1171", null ],
          [ "CMake Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1172", null ]
        ] ],
        [ "Fix Duplicate Writer Add Logic (Issue #270) - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1174", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1175", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1176", null ],
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1177", null ]
        ] ],
        [ "Use KCENON Feature Detection (Issue #250) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1179", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1180", null ],
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1181", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1182", null ]
        ] ],
        [ "Migrate from Deprecated common_system APIs (Issue #248) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1184", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1185", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1186", null ]
        ] ],
        [ "thread_system v3.0 Compatibility (Issue #244) - 2025-12-19", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1188", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1189", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1190", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1191", null ]
        ] ],
        [ "Fix broken example files after #226 API changes (Issue #228) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1193", [
          [ "Removed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1194", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1195", null ],
          [ "Disabled (awaiting feature implementation)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1196", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1197", null ]
        ] ],
        [ "common::interfaces::ILogger Implementation - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1199", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1200", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1201", null ],
          [ "Deprecated", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1202", null ],
          [ "Backward Compatibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1203", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1204", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1205", null ]
        ] ],
        [ "Standalone async implementation (Issue #222) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1207", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1208", null ],
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1209", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1210", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1211", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1212", null ]
        ] ],
        [ "CMake fmt Fallback Removal - 2025-12-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1214", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1215", null ],
          [ "Removed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1216", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1217", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1218", null ]
        ] ],
        [ "thread_system Required Dependency - 2025-12-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1220", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1221", null ]
        ] ]
      ] ]
    ] ],
    [ "Configuration Strategies Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1224", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1226", [
        [ "What are Configuration Strategies?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1227", null ],
        [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1228", null ]
      ] ],
      [ "Strategy Pattern Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1230", [
        [ "Why Use Strategies?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1231", null ],
        [ "Example: Multi-Environment Deployment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1232", null ]
      ] ],
      [ "Strategy Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1234", [
        [ "Base Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1235", null ],
        [ "Strategy Lifecycle", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1236", null ],
        [ "Method Details", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1237", [
          [ "<tt>get_name()</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1238", null ],
          [ "<tt>apply(logger_config& config)</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1239", null ],
          [ "<tt>is_applicable()</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1240", null ],
          [ "<tt>priority()</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1241", null ]
        ] ]
      ] ],
      [ "Built-in Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1243", [
        [ "Deployment Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1244", [
          [ "Supported Environments", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1245", null ],
          [ "Environment Profiles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1246", [
            [ "Development (<tt>deployment_env::development</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1247", null ],
            [ "Staging (<tt>deployment_env::staging</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1248", null ],
            [ "Production (<tt>deployment_env::production</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1249", null ],
            [ "Testing (<tt>deployment_env::testing</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1250", null ]
          ] ],
          [ "API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1251", null ]
        ] ],
        [ "Environment Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1253", [
          [ "Supported Environment Variables", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1254", null ],
          [ "API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1255", null ],
          [ "Docker/Kubernetes Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1256", null ]
        ] ],
        [ "Performance Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1258", [
          [ "Performance Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1259", null ],
          [ "Performance Profiles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1260", [
            [ "Low Latency (<tt>performance_level::low_latency</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1261", null ],
            [ "Balanced (<tt>performance_level::balanced</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1262", null ],
            [ "High Throughput (<tt>performance_level::high_throughput</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1263", null ],
            [ "Minimal Overhead (<tt>performance_level::minimal_overhead</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1264", null ]
          ] ],
          [ "API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1265", null ]
        ] ]
      ] ],
      [ "Composite Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1267", [
        [ "How It Works", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1268", null ],
        [ "API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1269", null ],
        [ "Method Details", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1270", [
          [ "<tt>add(std::unique_ptr<config_strategy_interface> strategy)</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1271", null ],
          [ "<tt>add<Strategy, Args...>(Args&&... args)</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1272", null ]
        ] ],
        [ "Priority-Based Application", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1273", null ]
      ] ],
      [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1275", [
        [ "Example 1: Simple Deployment Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1276", null ],
        [ "Example 2: Combined Deployment + Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1277", null ],
        [ "Example 3: Environment Variables + Deployment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1278", null ],
        [ "Example 4: Explicit Composite Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1279", null ],
        [ "Example 5: Conditional Strategy (Docker)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1280", null ],
        [ "Example 6: Multi-Logger Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1281", null ]
      ] ],
      [ "Custom Strategy Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1283", [
        [ "Basic Custom Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1284", null ],
        [ "Conditional Strategy (is_applicable)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1285", null ]
      ] ],
      [ "Strategy Priority and Conflict Resolution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1287", [
        [ "Priority Values", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1288", null ],
        [ "Conflict Resolution Rules", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1289", null ],
        [ "Best Practice: Environment Variables as Final Override", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1290", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1292", [
        [ "1. Start with Defaults, Override as Needed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1293", null ],
        [ "2. Use Environment Variables for Runtime Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1294", null ],
        [ "3. Combine Strategies for Complex Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1295", null ],
        [ "4. Test Strategies in Isolation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1296", null ],
        [ "5. Document Custom Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1297", null ]
      ] ],
      [ "Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1299", null ]
    ] ],
    [ "CONTRIBUTING.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING_8kr.html", [
      [ "Logger System 기여하기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING_8kr.html#autotoc_md1301", null ]
    ] ],
    [ "CONTRIBUTING", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html", [
      [ "Contributing to Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1304", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1305", null ],
        [ "Code of Conduct", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1306", null ],
        [ "Getting Started", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1307", null ],
        [ "Development Process", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1308", [
          [ "1. Before You Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1309", null ],
          [ "2. Making Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1310", null ],
          [ "3. Commit Message Format", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1311", null ]
        ] ],
        [ "Code Style", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1312", [
          [ "C++ Guidelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1313", null ],
          [ "Code Formatting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1314", null ],
          [ "Naming Conventions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1315", null ]
        ] ],
        [ "Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1316", [
          [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1317", null ],
          [ "Running Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1318", null ],
          [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1319", null ]
        ] ],
        [ "Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1320", [
          [ "Code Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1321", null ],
          [ "Documentation Updates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1322", null ]
        ] ],
        [ "Submitting Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1323", [
          [ "Pull Request Process", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1324", null ],
          [ "Review Process", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1325", null ]
        ] ],
        [ "Questions?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1326", null ],
        [ "Thank you for contributing!", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1327", null ]
      ] ]
    ] ],
    [ "Test Coverage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TESTING__GUIDE.html", null ],
    [ "Korean Translation Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html", [
      [ "Completion Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1330", null ],
      [ "Statistics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1331", null ],
      [ "Files Created", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1332", [
        [ "Root Directory (19 files)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1333", null ],
        [ "advanced/ Directory (2 files)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1334", null ],
        [ "guides/ Directory (4 files)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1335", null ]
      ] ],
      [ "Language Switcher Format", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1336", [
        [ "English Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1337", null ],
        [ "Korean Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1338", null ]
      ] ],
      [ "Translation Approach", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1339", null ],
      [ "File Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1340", null ],
      [ "Verification", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1341", null ],
      [ "Notes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1342", null ],
      [ "Next Steps (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1343", null ]
    ] ],
    [ "Logger System - 상세 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html", [
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1347", null ],
      [ "핵심 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1349", [
        [ "설계 원칙", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1350", null ],
        [ "주요 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1351", null ]
      ] ],
      [ "로그 레벨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1353", [
        [ "레벨 정의", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1354", null ],
        [ "레벨별 사용 지침", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1355", null ],
        [ "사용 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1356", null ]
      ] ],
      [ "로그 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1358", [
        [ "콘솔 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1359", null ],
        [ "파일 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1360", null ],
        [ "회전 파일 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1361", null ],
        [ "원격 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1362", null ],
        [ "커스텀 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1363", null ]
      ] ],
      [ "포맷팅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1365", [
        [ "기본 포맷", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1366", null ],
        [ "JSON 포맷", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1367", null ],
        [ "커스텀 포맷터", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1368", null ]
      ] ],
      [ "필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1370", [
        [ "레벨 필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1371", null ],
        [ "패턴 필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1372", null ],
        [ "조건부 필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1373", null ]
      ] ],
      [ "비동기 로깅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1375", [
        [ "비동기 모드 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1376", null ],
        [ "성능 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1377", null ]
      ] ],
      [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1379", [
        [ "벤치마크 결과", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1380", null ],
        [ "최적화 팁", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1381", null ]
      ] ],
      [ "통합 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1383", [
        [ "thread_system 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1384", null ],
        [ "monitoring_system 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1385", null ],
        [ "common_system 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1386", null ]
      ] ],
      [ "구성 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1388", [
        [ "프로덕션 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1389", null ],
        [ "개발 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1390", null ]
      ] ],
      [ "OTLP 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1392", [
        [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1393", null ],
        [ "OTLP Writer 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1394", null ],
        [ "Trace Context 전파", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1395", null ],
        [ "프로토콜 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1396", null ],
        [ "기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1397", null ]
      ] ],
      [ "로그 샘플링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1399", [
        [ "샘플링 전략", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1400", null ],
        [ "빠른 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1401", null ],
        [ "설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1402", null ],
        [ "기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1403", null ]
      ] ],
      [ "실시간 로그 분석", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1405", [
        [ "이상 유형", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1406", null ],
        [ "사용법", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1407", null ]
      ] ],
      [ "참고사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1409", [
        [ "스레드 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1410", null ],
        [ "주의사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1411", null ]
      ] ]
    ] ],
    [ "Logger System Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1416", null ],
      [ "Core Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1418", [
        [ "🎯 High-Performance Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1419", [
          [ "Asynchronous Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1420", null ],
          [ "Thread-Safe Operations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1421", null ],
          [ "Configuration Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1422", null ]
        ] ],
        [ "🔧 Advanced Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1423", [
          [ "Result-Based Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1424", null ],
          [ "Builder Pattern API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1425", null ],
          [ "Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1426", null ],
          [ "Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1427", null ]
        ] ]
      ] ],
      [ "Writer Types", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1429", [
        [ "Console Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1430", null ],
        [ "File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1431", null ],
        [ "Rotating File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1432", null ],
        [ "Network Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1433", null ],
        [ "Critical Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1434", null ],
        [ "Hybrid Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1435", null ]
      ] ],
      [ "Formatting Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1437", [
        [ "Plain Text Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1438", null ],
        [ "JSON Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1439", null ],
        [ "Logfmt Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1440", null ],
        [ "Custom Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1441", null ]
      ] ],
      [ "Filtering Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1443", [
        [ "Level-Based Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1444", null ],
        [ "Regex-Based Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1445", null ],
        [ "Function-Based Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1446", null ],
        [ "Composite Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1447", null ]
      ] ],
      [ "Rotation Policies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1449", [
        [ "Size-Based Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1450", null ],
        [ "Time-Based Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1451", null ],
        [ "Hybrid Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1452", null ],
        [ "Compression Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1453", null ]
      ] ],
      [ "Security Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1455", [
        [ "Secure Key Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1456", null ],
        [ "Path Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1457", null ],
        [ "Signal Handler Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1458", null ],
        [ "Security Audit Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1459", null ],
        [ "Sensitive Data Sanitization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1460", null ],
        [ "Access Control", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1461", null ],
        [ "Compliance Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1462", null ]
      ] ],
      [ "Advanced Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1464", [
        [ "Structured Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1465", null ],
        [ "Log Routing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1466", null ],
        [ "Performance Metrics Collection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1467", null ],
        [ "Real-time Log Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1468", [
          [ "Post-hoc Analysis (log_analyzer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1469", null ],
          [ "Real-time Anomaly Detection (realtime_log_analyzer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1470", null ]
        ] ],
        [ "Distributed Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1471", null ]
      ] ],
      [ "OTLP Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1473", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1474", null ],
        [ "OTLP Writer Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1475", null ],
        [ "Trace Context Propagation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1476", null ],
        [ "Protocol Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1477", null ],
        [ "Export Statistics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1478", null ],
        [ "Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1479", null ]
      ] ],
      [ "Log Sampling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1481", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1482", null ],
        [ "Sampling Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1483", null ],
        [ "Quick Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1484", null ],
        [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1485", null ],
        [ "Runtime Control", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1486", null ],
        [ "Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1487", null ]
      ] ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1489", null ]
    ] ],
    [ "BEST_PRACTICES.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES_8kr.html", [
      [ "Logger System 모범 사례 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES_8kr.html#autotoc_md1490", null ]
    ] ],
    [ "BEST_PRACTICES", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html", [
      [ "Logger System Best Practices Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1493", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1494", null ],
        [ "Design Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1495", [
          [ "1. Single Responsibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1496", null ],
          [ "2. Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1497", null ],
          [ "3. Interface Segregation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1498", null ]
        ] ],
        [ "Configuration Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1499", [
          [ "Environment-Specific Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1500", null ],
          [ "Configuration Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1501", null ],
          [ "Dynamic Reconfiguration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1502", null ]
        ] ],
        [ "Performance Guidelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1503", [
          [ "1. Minimize Allocations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1504", null ],
          [ "2. Lazy Evaluation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1505", null ],
          [ "3. Batch Operations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1506", null ],
          [ "4. Async Logging Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1507", null ]
        ] ],
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1508", [
          [ "Graceful Degradation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1509", null ],
          [ "Error Recovery", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1510", null ],
          [ "Circuit Breaker Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1511", null ]
        ] ],
        [ "Security Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1512", [
          [ "1. Sanitize Sensitive Data", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1513", null ],
          [ "2. Access Control", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1514", null ],
          [ "3. Log Injection Prevention", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1515", null ]
        ] ],
        [ "Testing Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1516", [
          [ "1. Mock Logger for Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1517", null ],
          [ "2. Performance Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1518", null ],
          [ "3. Stress Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1519", null ]
        ] ],
        [ "Production Deployment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1520", [
          [ "1. Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1521", null ],
          [ "2. Log Rotation Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1522", null ],
          [ "3. Graceful Shutdown", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1523", null ]
        ] ],
        [ "Common Pitfalls", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1524", [
          [ "1. Memory Leaks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1525", null ],
          [ "2. Deadlocks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1526", null ],
          [ "3. Performance Bottlenecks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1527", null ]
        ] ],
        [ "Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1528", null ],
        [ "Remember: Good logging is invisible when everything works but invaluable when things go wrong.", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1529", null ]
      ] ]
    ] ],
    [ "Decorator Pattern Writer Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1531", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1533", [
        [ "Why Decorator Pattern?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1534", null ],
        [ "Key Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1535", null ]
      ] ],
      [ "New Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1537", [
        [ "Base Classes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1538", [
          [ "<tt>log_writer_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1539", null ],
          [ "<tt>decorator_writer_base</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1540", null ]
        ] ]
      ] ],
      [ "Built-in Decorators", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1542", [
        [ "<tt>filtered_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1543", null ],
        [ "<tt>buffered_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1544", null ],
        [ "<tt>formatted_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1545", null ]
      ] ],
      [ "Migrating Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1547", [
        [ "Before (v3.x): Monolithic Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1548", null ],
        [ "After (v4.0): Focused Leaf Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1549", null ],
        [ "Creating a Custom Decorator", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1550", null ]
      ] ],
      [ "Composing Decorators", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1552", [
        [ "Order Matters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1553", null ],
        [ "Recommended Order", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1554", null ],
        [ "Using <tt>writer_builder</tt> (Recommended)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1555", null ],
        [ "Using <tt>logger_builder</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1556", null ]
      ] ],
      [ "Deprecation Timeline and Legacy Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1558", [
        [ "Deprecated Writer Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1559", null ],
        [ "Timeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1560", null ],
        [ "Migration Paths", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1561", [
          [ "Scenario 1: Simple File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1562", null ],
          [ "Scenario 2: Rotating File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1563", null ],
          [ "Scenario 3: Async + Encrypted Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1564", null ],
          [ "Scenario 4: Buffered + Filtered Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1565", null ],
          [ "Scenario 5: Multiple Decorators (Production Setup)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1566", null ],
          [ "Scenario 6: Custom Writer Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1567", null ]
        ] ],
        [ "Support Policy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1568", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1570", [
        [ "1. Keep Leaf Writers Simple", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1571", null ],
        [ "2. Use Category Tags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1572", null ],
        [ "3. Handle Errors Gracefully", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1573", null ],
        [ "4. Ensure Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1574", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1576", [
        [ "Common Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1577", [
          [ "\"Writer not receiving entries\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1578", null ],
          [ "\"Entries not being formatted\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1579", null ],
          [ "\"Flush not working\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1580", null ],
          [ "\"Compilation errors with custom writers\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1581", null ],
          [ "\"Performance regression after migration\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1582", null ]
        ] ],
        [ "Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1583", null ],
        [ "Getting Help", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1584", null ]
      ] ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1586", null ]
    ] ],
    [ "Logger System - Frequently Asked Questions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1590", null ],
      [ "General Questions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1592", [
        [ "1. What is the logger_system?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1593", null ],
        [ "2. What C++ standard is required?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1595", null ]
      ] ],
      [ "Logging Basics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1597", [
        [ "3. How do I create a logger?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1598", null ],
        [ "4. How do I log messages?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1600", null ]
      ] ],
      [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1602", [
        [ "5. What log levels are available?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1603", null ],
        [ "6. How do I set the log level?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1605", null ],
        [ "7. Can I change log level at runtime?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1607", null ]
      ] ],
      [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1609", [
        [ "8. What writers are available?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1610", null ],
        [ "9. How do I use multiple writers?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1612", null ],
        [ "10. How does log rotation work?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1614", null ]
      ] ],
      [ "Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1616", [
        [ "11. What is the performance?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1617", null ],
        [ "12. How do I enable asynchronous logging?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1619", null ],
        [ "13. How do I optimize performance?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1621", null ]
      ] ],
      [ "Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1623", [
        [ "14. How do I integrate with monitoring_system?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1624", null ],
        [ "15. How do I integrate with thread_system?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1626", null ]
      ] ],
      [ "Advanced Topics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1628", [
        [ "16. Is it thread-safe?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1629", null ],
        [ "17. How do I create custom log formats?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1631", null ],
        [ "18. How do I filter logs?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1633", null ],
        [ "19. How do I handle critical logs?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1635", null ],
        [ "20. Where can I find more information?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1637", null ]
      ] ]
    ] ],
    [ "GETTING_STARTED.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED_8kr.html", [
      [ "Logger System 시작하기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED_8kr.html#autotoc_md1639", null ]
    ] ],
    [ "GETTING_STARTED", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html", [
      [ "Getting Started with Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1642", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1643", null ],
        [ "Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1644", null ],
        [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1645", [
          [ "Using CMake FetchContent (Recommended)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1646", null ],
          [ "Building from Source", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1647", null ],
          [ "Using as Installed Package", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1648", null ]
        ] ],
        [ "Basic Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1649", [
          [ "Simple Console Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1650", null ],
          [ "Logging with Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1651", null ],
          [ "Structured JSON Output", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1652", null ],
          [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1653", null ],
          [ "Filtering by Level", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1654", null ]
        ] ],
        [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1655", [
          [ "Synchronous vs Asynchronous Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1656", null ],
          [ "Multiple Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1657", null ],
          [ "Windows Notes (Networking)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1658", null ],
          [ "Console Writer Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1659", null ]
        ] ],
        [ "Integration with Thread System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1660", [
          [ "Using Service Container", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1661", null ],
          [ "Direct Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1662", null ]
        ] ],
        [ "Next Steps", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1663", null ]
      ] ]
    ] ],
    [ "Logger System Integration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1667", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1668", null ],
      [ "System Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1669", [
        [ "Required Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1670", [
          [ "common_system (Header-only)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1671", null ]
        ] ],
        [ "Optional Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1672", [
          [ "thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1673", null ],
          [ "monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1674", null ]
        ] ]
      ] ],
      [ "Integration with common_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1675", [
        [ "ILogger Interface Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1676", null ],
        [ "Result<T> Pattern Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1677", null ],
        [ "Adapter Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1678", null ]
      ] ],
      [ "Integration with thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1679", [
        [ "Asynchronous Logging with IExecutor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1680", null ],
        [ "Cross-System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1681", null ]
      ] ],
      [ "Integration with monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1682", [
        [ "IMonitor Interface Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1683", null ],
        [ "Performance Metrics Collection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1684", null ],
        [ "Health Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1685", null ]
      ] ],
      [ "Build Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1686", [
        [ "CMake Integration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1687", [
          [ "Minimal Configuration (common_system only)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1688", null ],
          [ "Full Ecosystem Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1689", null ]
        ] ],
        [ "Build Configuration Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1690", null ],
        [ "vcpkg Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1691", null ]
      ] ],
      [ "Code Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1692", [
        [ "Example 1: Basic Integration with common_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1693", null ],
        [ "Example 2: Full Stack Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1694", null ],
        [ "Example 3: Error Handling with Result<T>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1695", null ]
      ] ],
      [ "Result Handling Cheatsheet", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1696", null ],
      [ "Error Code Registry", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1697", [
        [ "Allocated Error Code Range", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1698", null ],
        [ "Error Message Mapping", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1699", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1700", [
        [ "Build Fails with \"common_system not found\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1701", null ],
        [ "Linker Errors: Undefined References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1702", null ],
        [ "Runtime: Logger Not Outputting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1703", null ],
        [ "Integration: IMonitor Not Injecting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1704", null ],
        [ "Performance: High Latency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1705", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1706", [
        [ "1. Use Result<T> for Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1707", null ],
        [ "2. Prefer Interface-Based Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1708", null ],
        [ "3. Leverage DI for Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1709", null ],
        [ "4. Monitor Performance in Production", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1710", null ]
      ] ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1711", null ]
    ] ],
    [ "MIGRATION_GUIDE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html", [
      [ "로거 시스템 마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1712", [
        [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1713", null ],
        [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1714", null ],
        [ "CMake 구성 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1715", [
          [ "v2.x (이전)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1716", null ],
          [ "v3.0 (현재)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1717", null ],
          [ "CMake 플래그 변경 요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1718", null ],
          [ "타겟명 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1719", null ],
          [ "의존성 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1720", null ]
        ] ],
        [ "v2.x에서 v3.0으로 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1721", [
          [ "주요 변경사항 요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1722", null ],
          [ "1. 네임스페이스 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1723", null ],
          [ "2. 인터페이스 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1724", null ],
          [ "3. Result 타입 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1725", null ],
          [ "4. thread_system 의존성 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1726", null ],
          [ "5. 백엔드 선택 (v3.0 신규)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1727", null ],
          [ "6. C++20 source_location (v3.0 신규)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1728", null ]
        ] ],
        [ "마이그레이션 체크리스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1729", [
          [ "v2.x → v3.0 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1730", null ]
        ] ],
        [ "일반적인 문제와 해결책", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1731", [
          [ "문제 1: 네임스페이스를 찾을 수 없음", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1732", null ],
          [ "문제 2: 인터페이스 타입 불일치", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1733", null ],
          [ "문제 3: C++20 미활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1734", null ]
        ] ],
        [ "지원 및 리소스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1735", null ]
      ] ]
    ] ],
    [ "MIGRATION_GUIDE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html", [
      [ "Logger System Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1738", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1739", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1740", [
          [ "Breaking Changes Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1741", null ]
        ] ],
        [ "From v3.x to v4.0", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1743", [
          [ "What Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1744", null ],
          [ "Migration for Standard Users", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1745", null ],
          [ "Migration for Custom Writer Authors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1746", null ],
          [ "Context ID API Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1747", null ],
          [ "Unified Context API (New in v4.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1748", [
            [ "Why This Change?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1749", null ],
            [ "Migration Table", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1750", null ],
            [ "Context Categories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1751", null ],
            [ "Code Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1752", null ],
            [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1753", null ],
            [ "Migration Script", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1754", null ],
            [ "Deprecation Timeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1755", null ]
          ] ]
        ] ],
        [ "CMake Configuration Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1757", [
          [ "v2.x (Previous)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1758", null ],
          [ "v3.0 (Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1759", null ],
          [ "CMake Flag Changes Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1760", null ],
          [ "Target Name Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1761", null ],
          [ "Dependency Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1762", null ]
        ] ],
        [ "Version Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1763", [
          [ "From v2.x to v3.0", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1764", [
            [ "Summary of Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1765", null ],
            [ "1. Namespace Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1766", null ],
            [ "2. Interface Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1767", null ],
            [ "3. Dual API Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1768", null ],
            [ "4. Result Type Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1769", null ],
            [ "5. thread_system Dependency Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1770", null ],
            [ "6. Backend Selection (New in v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1771", null ],
            [ "7. C++20 Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1772", null ],
            [ "8. Configuration Strategies (Enhanced in v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1773", null ],
            [ "9. Monitoring Integration (New in v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1774", null ],
            [ "Migration Script", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1775", null ]
          ] ],
          [ "From v1.x to v2.x", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1777", [
            [ "1. Error Handling Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1778", null ],
            [ "2. Memory Management Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1779", null ],
            [ "3. Configuration Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1780", null ],
            [ "4. Logging API Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1781", null ]
          ] ]
        ] ],
        [ "API Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1782", [
          [ "v3.0 API Changes Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1783", null ],
          [ "Deprecated Native log_level API (Planned for Removal in v3.0.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1784", null ],
          [ "Core Logger API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1785", null ]
        ] ],
        [ "Configuration Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1786", [
          [ "From INI/XML Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1787", null ],
          [ "Environment-based Configuration (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1788", null ]
        ] ],
        [ "Migration from Other Libraries", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1789", [
          [ "From spdlog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1790", null ],
          [ "From Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1791", null ],
          [ "From Google glog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1792", null ]
        ] ],
        [ "Compatibility Wrappers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1793", [
          [ "v2.x to v3.0 Compatibility Header", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1794", null ],
          [ "Legacy API Wrapper (v1.x compatibility)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1795", null ]
        ] ],
        [ "Step-by-Step Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1796", [
          [ "Phase 1: Preparation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1797", null ],
          [ "Phase 2: Namespace Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1798", null ],
          [ "Phase 3: Interface Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1799", null ],
          [ "Phase 4: Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1800", null ]
        ] ],
        [ "Common Issues and Solutions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1801", [
          [ "Issue 1: Namespace Not Found", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1802", null ],
          [ "Issue 2: Interface Type Mismatch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1803", null ],
          [ "Issue 3: Result Type Mismatch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1804", null ],
          [ "Issue 4: Missing thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1805", null ],
          [ "Issue 5: C++20 Not Enabled", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1806", null ]
        ] ],
        [ "Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1807", [
          [ "v2.x to v3.0 Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1808", null ],
          [ "v1.x to v2.x Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1809", null ]
        ] ],
        [ "Support and Resources", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1810", null ]
      ] ]
    ] ],
    [ "OpenTelemetry Integration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1813", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1814", null ],
      [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1815", [
        [ "With vcpkg", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1816", null ],
        [ "With CMake", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1817", null ],
        [ "Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1818", null ]
      ] ],
      [ "Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1819", [
        [ "Basic OTLP Export", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1820", null ],
        [ "With Trace Correlation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1821", null ],
        [ "Using RAII Scope Guard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1822", null ]
      ] ],
      [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1823", [
        [ "OTLP Writer Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1824", null ],
        [ "Protocol Selection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1825", null ]
      ] ],
      [ "Trace Context", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1826", [
        [ "otel_context Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1827", null ],
        [ "Thread-Local Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1828", null ],
        [ "Context in Log Entries", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1829", null ]
      ] ],
      [ "OTLP Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1830", [
        [ "Statistics Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1831", null ],
        [ "Health Checking", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1832", null ],
        [ "Force Flush", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1833", null ]
      ] ],
      [ "Integration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1834", [
        [ "With OpenTelemetry Collector", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1835", null ],
        [ "Docker Compose Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1836", null ],
        [ "W3C Trace Context Parsing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1837", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1838", [
        [ "1. Always Set Service Name", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1839", null ],
        [ "2. Use RAII Scope Guards", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1840", null ],
        [ "3. Configure Appropriate Batch Sizes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1841", null ],
        [ "4. Handle Collector Unavailability", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1842", null ],
        [ "5. Include Meaningful Resource Attributes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1843", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1844", [
        [ "Logs Not Appearing in Collector", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1845", null ],
        [ "High Log Drop Rate", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1846", null ],
        [ "Connection Timeouts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1847", null ],
        [ "Missing Trace Context", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1848", null ]
      ] ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1849", null ]
    ] ],
    [ "PERFORMANCE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE_8kr.html", [
      [ "Logger System 성능 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE_8kr.html#autotoc_md1850", null ]
    ] ],
    [ "PERFORMANCE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html", [
      [ "Logger System Performance Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1853", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1854", null ],
        [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1855", [
          [ "Synchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1856", null ],
          [ "Asynchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1857", null ]
        ] ],
        [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1858", [
          [ "Test Environment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1859", null ],
          [ "Single Thread Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1860", null ],
          [ "Multi-threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1861", null ],
          [ "Memory Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1862", null ]
        ] ],
        [ "Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1863", [
          [ "1. Choose the Right Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1864", null ],
          [ "2. Buffer Size Tuning", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1865", null ],
          [ "2.1 Batch Size Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1866", null ],
          [ "3. Level Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1867", null ],
          [ "4. Message Construction", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1868", null ],
          [ "5. Writer Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1869", [
            [ "Console Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1870", null ],
            [ "Custom High-Performance Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1871", null ]
          ] ],
          [ "6. Batch Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1872", null ]
        ] ],
        [ "Performance Anti-patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1873", [
          [ "1. Synchronous I/O in Hot Paths", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1874", null ],
          [ "2. Excessive String Formatting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1875", null ],
          [ "3. Logging in Tight Loops", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1876", null ]
        ] ],
        [ "Profiling and Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1877", [
          [ "Built-in Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1878", null ],
          [ "External Profiling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1879", null ]
        ] ],
        [ "Best Practices Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1880", null ],
        [ "Platform-Specific Optimizations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1881", [
          [ "Linux", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1882", null ],
          [ "Windows", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1883", null ],
          [ "macOS", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1884", null ]
        ] ]
      ] ]
    ] ],
    [ "빠른 시작 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html", [
      [ "사전 요구사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1888", null ],
      [ "설치", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1889", [
        [ "1. 저장소 클론", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1890", null ],
        [ "2. 의존성 설치", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1891", null ],
        [ "3. 빌드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1892", null ],
        [ "4. 설치 확인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1893", null ]
      ] ],
      [ "첫 번째 로거 만들기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1895", [
        [ "애플리케이션 빌드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1896", null ]
      ] ],
      [ "핵심 개념", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1898", [
        [ "로거 빌더", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1899", null ],
        [ "라이터", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1900", null ],
        [ "로그 레벨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1901", null ]
      ] ],
      [ "구성 템플릿", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1903", [
        [ "Production (프로덕션)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1904", null ],
        [ "Debug (디버그)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1905", null ],
        [ "High Performance (고성능)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1906", null ]
      ] ],
      [ "CMake 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1908", [
        [ "핵심 빌드 옵션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1909", null ],
        [ "기능 플래그", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1910", null ]
      ] ],
      [ "thread_system 통합 (선택 사항)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1912", [
        [ "빌드 시 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1913", null ],
        [ "런타임 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1914", null ]
      ] ],
      [ "일반적인 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1916", [
        [ "오류 처리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1917", null ],
        [ "여러 라이터 사용", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1918", null ],
        [ "정상 종료", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1919", null ]
      ] ],
      [ "다음 단계", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1921", null ],
      [ "문제 해결", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1923", [
        [ "일반적인 문제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1924", null ]
      ] ]
    ] ],
    [ "Quick Start Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html", [
      [ "Prerequisites", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1928", null ],
      [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1929", [
        [ "1. Clone the Repositories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1930", null ],
        [ "2. Install Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1931", null ],
        [ "3. Build", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1932", null ],
        [ "4. Verify Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1933", null ]
      ] ],
      [ "Your First Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1935", [
        [ "Building Your Application", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1936", null ]
      ] ],
      [ "Key Concepts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1938", [
        [ "Logger Builder", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1939", null ],
        [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1940", null ],
        [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1941", null ]
      ] ],
      [ "Configuration Templates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1943", [
        [ "Production", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1944", null ],
        [ "Debug", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1945", null ],
        [ "High Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1946", null ]
      ] ],
      [ "CMake Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1948", [
        [ "Core Build Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1949", null ],
        [ "Feature Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1950", null ]
      ] ],
      [ "thread_system Integration (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1952", [
        [ "Enable at Build Time", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1953", null ],
        [ "Enable at Runtime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1954", null ]
      ] ],
      [ "Common Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1956", [
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1957", null ],
        [ "Multiple Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1958", null ],
        [ "Graceful Shutdown", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1959", null ]
      ] ],
      [ "Next Steps", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1961", null ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1963", [
        [ "Common Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1964", null ]
      ] ]
    ] ],
    [ "SECURITY.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html", [
      [ "보안 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1966", [
        [ "위협 모델 (범위)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1967", null ],
        [ "현재 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1968", null ],
        [ "중요한 제한사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1969", null ],
        [ "권장사항 (프로덕션)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1970", null ],
        [ "안전한 구성 체크리스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1971", null ],
        [ "로드맵", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1972", null ]
      ] ]
    ] ],
    [ "SECURITY", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html", [
      [ "Security Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1974", [
        [ "Threat Model (Scope)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1975", null ],
        [ "Current Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1976", null ],
        [ "Important Limitations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1977", null ],
        [ "Recommendations (Production)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1978", null ],
        [ "Secure Configuration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1979", null ],
        [ "Roadmap", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1980", null ]
      ] ]
    ] ],
    [ "thread_system 통합 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html", [
      [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1999", null ],
      [ "빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2000", [
        [ "빌드 시 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2001", null ],
        [ "런타임에 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2002", null ]
      ] ],
      [ "설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2003", [
        [ "CMake 옵션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2004", null ],
        [ "컴파일 정의", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2005", null ]
      ] ],
      [ "의존성 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2006", [
        [ "양방향 의존성 위험 (Issue #252)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2007", null ],
        [ "권장 구성 매트릭스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2008", null ],
        [ "안전한 구성 예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2009", null ],
        [ "CMake 충돌 감지", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2010", null ]
      ] ],
      [ "API 레퍼런스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2011", [
        [ "thread_system_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2012", [
          [ "메서드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2013", null ]
        ] ],
        [ "async_backend_type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2014", null ],
        [ "컴파일 타임 감지", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2015", null ]
      ] ],
      [ "사용 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2016", [
        [ "패턴 1: 애플리케이션 전체 공유 풀", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2017", null ],
        [ "패턴 2: 사용자 정의 풀 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2018", null ],
        [ "패턴 3: 조건부 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2019", null ]
      ] ],
      [ "스레드 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2020", null ],
      [ "성능 고려사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2021", [
        [ "thread_system 통합 사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2022", null ],
        [ "오버헤드 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2023", null ]
      ] ],
      [ "문제 해결", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2024", [
        [ "thread_system 미발견", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2025", null ],
        [ "런타임 감지 미작동", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2026", null ]
      ] ],
      [ "관련 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md2027", null ]
    ] ],
    [ "Async Integration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2030", null ],
      [ "Integration Approaches", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2031", [
        [ "Recommended: IExecutor Interface (v1.5.0+)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2032", null ],
        [ "Legacy: Direct thread_system Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2033", null ]
      ] ],
      [ "Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2034", [
        [ "Enable at Build Time", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2035", null ],
        [ "Enable at Runtime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2036", null ]
      ] ],
      [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2037", [
        [ "CMake Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2038", null ],
        [ "Compile Definitions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2039", null ]
      ] ],
      [ "Dependency Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2040", [
        [ "Bidirectional Dependency Risk (Issue #252)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2041", null ],
        [ "Recommended Configuration Matrix", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2042", null ],
        [ "Safe Configuration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2043", null ],
        [ "CMake Conflict Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2044", null ]
      ] ],
      [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2045", [
        [ "thread_system_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2046", [
          [ "Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2047", null ]
        ] ],
        [ "async_backend_type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2048", null ],
        [ "Compile-time Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2049", null ]
      ] ],
      [ "Usage Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2050", [
        [ "Pattern 1: Application-wide Shared Pool", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2051", null ],
        [ "Pattern 2: Custom Pool Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2052", null ],
        [ "Pattern 3: Conditional Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2053", null ]
      ] ],
      [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2054", null ],
      [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2055", [
        [ "When to Use thread_system Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2056", null ],
        [ "Overhead Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2057", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2058", [
        [ "thread_system not found", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2059", null ],
        [ "Runtime detection not working", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2060", null ]
      ] ],
      [ "IExecutor Integration API (v1.5.0+)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2061", [
        [ "executor_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2062", null ],
        [ "executor_type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2063", null ],
        [ "standalone_executor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2064", null ],
        [ "Compile-time Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2065", null ]
      ] ],
      [ "Migration from thread_system_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2066", null ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md2067", null ]
    ] ],
    [ "Logger System - 성능 기준 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html", [
      [ "시스템 정보", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2072", [
        [ "하드웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2073", null ],
        [ "소프트웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2074", null ]
      ] ],
      [ "성능 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2076", [
        [ "로깅 Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2077", null ],
        [ "Latency (Async Mode)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2078", null ],
        [ "메모리 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2079", null ]
      ] ],
      [ "벤치마크 결과", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2081", null ],
      [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2083", null ],
      [ "Baseline 검증", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2085", [
        [ "Phase 0 요구사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2086", null ],
        [ "수락 기준", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md2087", null ]
      ] ]
    ] ],
    [ "Logger System - Performance Baseline Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html", [
      [ "System Information", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2092", [
        [ "Hardware Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2093", null ],
        [ "Software Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2094", null ]
      ] ],
      [ "Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2096", [
        [ "Logging Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2097", null ],
        [ "Latency (Async Mode)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2098", null ],
        [ "Memory Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2099", null ]
      ] ],
      [ "Benchmark Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2101", null ],
      [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2103", null ],
      [ "Baseline Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2105", [
        [ "Phase 0 Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2106", null ],
        [ "Acceptance Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md2107", null ]
      ] ]
    ] ],
    [ "CI/CD 성능 지표 자동화 제안서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html", [
      [ "📊 Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2111", null ],
      [ "🔍 현재 상태 분석", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2113", [
        [ "✅ 장점", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2114", null ],
        [ "❌ 개선 필요 사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2115", null ]
      ] ],
      [ "🎯 제안 솔루션 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2117", [
        [ "전체 워크플로우", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2118", null ]
      ] ],
      [ "📁 제안 디렉토리 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2120", null ],
      [ "🛠️ 구현 상세 (4단계)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2122", [
        [ "Phase 1: 벤치마크 재활성화 및 JSON 출력 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2123", [
          [ "1.1 GitHub Actions 워크플로우 수정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2124", null ],
          [ "1.2 벤치마크 실행 및 JSON 저장", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2125", null ],
          [ "1.3 API 변경에 따른 벤치마크 코드 수정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2126", null ]
        ] ],
        [ "Phase 2: 결과 파싱 및 기준선 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2128", [
          [ "2.1 벤치마크 결과 파싱 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2129", null ],
          [ "2.2 기준선 비교 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2130", null ]
        ] ],
        [ "Phase 3: 배지 및 차트 생성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2132", [
          [ "3.1 성능 배지 생성 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2133", null ],
          [ "3.2 트렌드 차트 생성 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2134", null ]
        ] ],
        [ "Phase 4: README.md 자동 업데이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2136", [
          [ "4.1 README 업데이트 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2137", null ],
          [ "4.2 GitHub Actions에서 자동 커밋", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2138", null ]
        ] ]
      ] ],
      [ "🎨 README.md 성능 섹션 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2140", [
        [ "제안 1: 배지 중심 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2141", null ],
        [ "제안 2: 테이블 중심 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2142", null ],
        [ "제안 3: 카드 스타일 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2143", null ]
      ] ],
      [ "🔄 CI/CD 워크플로우 전체 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2145", null ],
      [ "📈 예상 결과물", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2147", [
        [ "1. GitHub Actions 실행 결과", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2148", null ],
        [ "2. PR 자동 코멘트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2149", null ],
        [ "3. 업데이트된 README.md", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2150", null ]
      ] ],
      [ "✅ 구현 체크리스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2152", [
        [ "Phase 1: 인프라 구축 (예상 소요: 2시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2153", null ],
        [ "Phase 2: 배지 및 차트 (예상 소요: 1.5시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2154", null ],
        [ "Phase 3: README 자동 업데이트 (예상 소요: 1시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2155", null ],
        [ "Phase 4: CI/CD 통합 (예상 소요: 2시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2156", null ],
        [ "Phase 5: 테스트 및 검증 (예상 소요: 1.5시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2157", null ],
        [ "Phase 6: 문서화 (예상 소요: 1시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2158", null ]
      ] ],
      [ "🔧 트러블슈팅 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2160", [
        [ "문제 1: 벤치마크 빌드 실패", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2161", null ],
        [ "문제 2: Python 스크립트 실행 오류", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2162", null ],
        [ "문제 3: Git 자동 커밋 권한 오류", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2163", null ],
        [ "문제 4: 배지가 README에 표시되지 않음", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2164", null ]
      ] ],
      [ "📚 참고 자료", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2166", [
        [ "도구 및 라이브러리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2167", null ],
        [ "관련 프로젝트 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2168", null ],
        [ "내부 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2169", null ]
      ] ],
      [ "🎯 성공 지표", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2171", null ],
      [ "📝 다음 단계", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md2173", null ]
    ] ],
    [ "Decorator Pattern Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2176", null ],
      [ "Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2177", null ],
      [ "Benchmark Methodology", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2178", [
        [ "Test Environment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2179", null ],
        [ "Measured Configurations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2180", null ],
        [ "Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2181", null ]
      ] ],
      [ "Performance Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2182", [
        [ "Throughput Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2183", null ],
        [ "Latency Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2184", null ],
        [ "Memory Overhead", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2185", null ]
      ] ],
      [ "Detailed Benchmark Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2186", [
        [ "Baseline: Direct File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2187", null ],
        [ "Single Decorator: Async", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2188", null ],
        [ "Double Decorator: Buffered + Async", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2189", null ],
        [ "Message Size Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2190", null ]
      ] ],
      [ "Performance Recommendations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2191", [
        [ "Production Guidelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2192", null ],
        [ "Decorator Ordering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2193", null ],
        [ "Configuration Tuning", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2194", null ]
      ] ],
      [ "Running Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2195", [
        [ "Build and Execute", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2196", null ],
        [ "Using CMake Targets", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2197", null ],
        [ "Comparing Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2198", null ]
      ] ],
      [ "Regression Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2199", [
        [ "CI Integration (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2200", null ],
        [ "Regression Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2201", null ]
      ] ],
      [ "Known Limitations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2202", [
        [ "Benchmark Accuracy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2203", null ],
        [ "Excluded Scenarios", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2204", null ]
      ] ],
      [ "Conclusions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2205", null ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2206", null ],
      [ "Version History", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2DECORATOR__PERFORMANCE.html#autotoc_md2207", null ]
    ] ],
    [ "Logger System 프로덕션 품질", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html", [
      [ "요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2210", [
        [ "개발 상태: ✅ <strong>개발 중</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2211", null ]
      ] ],
      [ "빌드 & 테스팅 인프라", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2213", [
        [ "지속적 통합 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2214", [
          [ "1. 메인 CI 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2215", null ],
          [ "2. 새니타이저 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2216", null ],
          [ "3. 성능 벤치마크 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2217", null ],
          [ "4. 코드 커버리지 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2218", null ],
          [ "5. 정적 분석 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2219", null ]
        ] ]
      ] ],
      [ "스레드 안전성 & 동시성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2221", [
        [ "스레드 안전성 등급: <strong>A+</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2222", null ],
        [ "Writer 스레드 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2223", null ],
        [ "비동기 처리 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2224", null ]
      ] ],
      [ "리소스 관리 (RAII)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2226", [
        [ "RAII 준수 등급: <strong>A</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2227", null ],
        [ "스마트 포인터 사용", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2228", null ],
        [ "메모리 누수 탐지", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2229", null ],
        [ "예외 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2230", null ]
      ] ],
      [ "오류 처리 전략", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2232", [
        [ "오류 처리 등급: <strong>A</strong> (90% 완료)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2233", null ],
        [ "Result<T> 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2234", null ],
        [ "오류 코드 레지스트리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2235", null ]
      ] ],
      [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2237", [
        [ "기준선 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2238", null ]
      ] ],
      [ "보안 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2240", [
        [ "보안 등급: <strong>A</strong> (v3.0.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2241", null ],
        [ "1. 보안 키 저장소", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2242", null ],
        [ "2. 경로 검증", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2243", null ],
        [ "3. 시그널 핸들러 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2244", null ],
        [ "4. 보안 감사 로깅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2245", null ],
        [ "5. 컴플라이언스 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2246", null ]
      ] ],
      [ "플랫폼 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2248", [
        [ "공식 지원 플랫폼", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2249", null ],
        [ "최소 요구사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2250", null ]
      ] ]
    ] ],
    [ "Logger System Production Quality", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2256", null ],
      [ "Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2258", [
        [ "Quality Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2259", null ]
      ] ],
      [ "Build & Testing Infrastructure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2261", [
        [ "Continuous Integration Pipelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2262", [
          [ "1. Main CI Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2263", null ],
          [ "2. Sanitizer Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2265", null ],
          [ "3. Performance Benchmark Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2267", null ],
          [ "4. Code Coverage Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2269", null ],
          [ "5. Static Analysis Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2271", null ]
        ] ],
        [ "Test Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2273", [
          [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2274", null ],
          [ "Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2276", null ],
          [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2278", null ]
        ] ]
      ] ],
      [ "Thread Safety & Concurrency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2280", [
        [ "Thread Safety Grade: <strong>A+</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2281", null ],
        [ "Writer Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2282", null ],
        [ "Asynchronous Processing Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2284", null ],
        [ "Concurrency Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2286", null ]
      ] ],
      [ "Resource Management (RAII)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2288", [
        [ "RAII Compliance Grade: <strong>A</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2289", null ],
        [ "Smart Pointer Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2290", null ],
        [ "Memory Leak Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2292", null ],
        [ "Exception Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2294", null ]
      ] ],
      [ "Error Handling Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2296", [
        [ "Error Handling Grade: <strong>A</strong> (90% Complete)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2297", null ],
        [ "Result<T> Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2298", null ],
        [ "Error Code Registry", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2300", null ],
        [ "Hot Path Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2302", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2304", [
        [ "Baseline Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2305", null ],
        [ "Performance Baselines (CI/CD)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2307", null ]
      ] ],
      [ "Security Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2309", [
        [ "Security Grade: <strong>A</strong> (v3.0.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2310", null ],
        [ "1. Secure Key Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2311", null ],
        [ "2. Path Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2312", null ],
        [ "3. Signal Handler Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2313", null ],
        [ "4. Security Audit Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2314", null ],
        [ "5. Compliance Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2315", null ],
        [ "6. OWASP Top 10 Mitigations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2316", null ]
      ] ],
      [ "Operational Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2318", [
        [ "Built-in Metrics Collection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2319", null ],
        [ "Health Checks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2321", null ]
      ] ],
      [ "Platform Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2323", [
        [ "Officially Supported Platforms", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2324", null ],
        [ "Minimum Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2325", null ],
        [ "Cross-Platform Compatibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2327", null ]
      ] ],
      [ "Remaining Work & Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2329", [
        [ "Current Status: <strong>90% Production Ready</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2330", null ]
      ] ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2332", null ]
    ] ],
    [ "Logger System 프로젝트 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html", [
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2335", null ],
      [ "디렉토리 개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2337", null ],
      [ "코어 모듈 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2339", [
        [ "logger.h / logger.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2340", null ],
        [ "logger_builder.h / logger_builder.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2342", null ],
        [ "log_entry.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2344", null ],
        [ "result_types.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2346", null ]
      ] ],
      [ "Writer 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2348", [
        [ "console_writer.h / console_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2349", null ],
        [ "file_writer.h / file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2351", null ],
        [ "rotating_file_writer.h / rotating_file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2353", null ],
        [ "network_writer.h / network_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2355", null ],
        [ "critical_writer.h / critical_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2357", null ],
        [ "hybrid_writer.h / hybrid_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2359", null ]
      ] ],
      [ "Filter 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2361", [
        [ "level_filter.h / level_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2362", null ],
        [ "regex_filter.h / regex_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2364", null ],
        [ "function_filter.h / function_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2366", null ]
      ] ],
      [ "Formatter 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2368", [
        [ "plain_formatter.h / plain_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2369", null ],
        [ "json_formatter.h / json_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2371", null ],
        [ "custom_formatter.h / custom_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2373", null ]
      ] ],
      [ "설정 시스템", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2375", [
        [ "config_templates.h / config_templates.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2376", null ],
        [ "config_validator.h / config_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2378", null ]
      ] ],
      [ "보안 컴포넌트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2380", [
        [ "secure_key_storage.h / secure_key_storage.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2381", null ],
        [ "path_validator.h / path_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2383", null ],
        [ "log_sanitizer.h / log_sanitizer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2385", null ]
      ] ],
      [ "테스팅 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2387", [
        [ "유닛 테스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2388", null ],
        [ "통합 테스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2390", null ],
        [ "벤치마크", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2392", null ]
      ] ],
      [ "빌드 시스템", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2394", [
        [ "CMakeLists.txt (루트)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2395", null ]
      ] ],
      [ "모듈 의존성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2397", [
        [ "의존성 그래프", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2398", null ],
        [ "외부 의존성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2399", null ]
      ] ],
      [ "파일 명명 규칙", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2401", [
        [ "헤더 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2402", null ],
        [ "소스 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2403", null ],
        [ "문서 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2404", null ]
      ] ],
      [ "참고 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2406", null ]
    ] ],
    [ "Logger System Project Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2411", null ],
      [ "Directory Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2413", null ],
      [ "Core Module Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2415", [
        [ "Header Files (<tt>include/kcenon/logger/core/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2416", null ],
        [ "Core Subdirectories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2417", null ],
        [ "Source Files (<tt>src/core/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2418", null ],
        [ "Interfaces (<tt>include/kcenon/logger/interfaces/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2419", null ],
        [ "Root-Level Headers (<tt>include/kcenon/logger/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2420", null ]
      ] ],
      [ "Builder Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2422", [
        [ "writer_builder.h / writer_builder.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2423", null ]
      ] ],
      [ "Writer Implementations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2425", [
        [ "Header Files (<tt>include/kcenon/logger/writers/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2426", null ],
        [ "Source Files (<tt>src/impl/writers/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2427", null ],
        [ "Async Processing (<tt>src/impl/async/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2428", null ]
      ] ],
      [ "Filter Implementations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2430", [
        [ "log_filter.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2431", null ]
      ] ],
      [ "Formatter Implementations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2433", [
        [ "Header Files (<tt>include/kcenon/logger/formatters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2434", null ]
      ] ],
      [ "Factory Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2436", [
        [ "Header Files (<tt>include/kcenon/logger/factories/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2437", null ]
      ] ],
      [ "Adapter Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2439", [
        [ "Header Files (<tt>include/kcenon/logger/adapters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2440", null ]
      ] ],
      [ "Backend Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2442", [
        [ "Header Files (<tt>include/kcenon/logger/backends/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2443", null ]
      ] ],
      [ "Sink Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2445", [
        [ "Header Files (<tt>include/kcenon/logger/sinks/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2446", null ]
      ] ],
      [ "Integration Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2448", [
        [ "Header Files (<tt>include/kcenon/logger/integration/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2449", null ],
        [ "Source Files (<tt>src/integration/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2450", null ]
      ] ],
      [ "Sampling Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2452", [
        [ "Header Files (<tt>include/kcenon/logger/sampling/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2453", null ],
        [ "Source Files (<tt>src/sampling/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2454", null ]
      ] ],
      [ "Utility Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2456", [
        [ "Header Files (<tt>include/kcenon/logger/utils/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2457", null ]
      ] ],
      [ "Security Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2459", [
        [ "Header Files (<tt>include/kcenon/logger/security/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2460", null ],
        [ "Source Files (<tt>src/security/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2461", null ]
      ] ],
      [ "Additional Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2463", [
        [ "Dependency Injection (<tt>include/kcenon/logger/di/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2464", null ],
        [ "OpenTelemetry (<tt>include/kcenon/logger/otlp/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2465", null ],
        [ "Routing (<tt>include/kcenon/logger/routing/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2466", null ],
        [ "Safety (<tt>include/kcenon/logger/safety/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2467", null ],
        [ "Analysis (<tt>include/kcenon/logger/analysis/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2468", null ],
        [ "Structured Logging (<tt>include/kcenon/logger/structured/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2469", null ],
        [ "Server (<tt>include/kcenon/logger/server/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2470", null ],
        [ "Internal Implementation Details (<tt>src/impl/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2471", null ]
      ] ],
      [ "Testing Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2473", [
        [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2474", null ],
        [ "Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2476", null ],
        [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2478", null ]
      ] ],
      [ "Build System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2480", [
        [ "CMakeLists.txt (Root)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2481", null ],
        [ "CMake Modules", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2483", null ]
      ] ],
      [ "Module Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2485", [
        [ "Dependency Graph", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2486", null ],
        [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2487", null ]
      ] ],
      [ "File Naming Conventions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2489", [
        [ "Header Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2490", null ],
        [ "Source Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2491", null ],
        [ "Documentation Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2492", null ]
      ] ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2494", null ]
    ] ],
    [ "README.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html", [
      [ "Logger System 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2495", [
        [ "📚 문서 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2496", [
          [ "핵심 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2497", null ],
          [ "가이드 (<tt>docs/guides/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2498", null ],
          [ "고급 주제 (<tt>docs/advanced/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2499", null ]
        ] ],
        [ "🚀 빠른 탐색", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2500", [
          [ "신규 사용자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2501", null ],
          [ "기존 사용자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2502", null ],
          [ "기여자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2503", null ],
          [ "성능 엔지니어", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2504", null ]
        ] ],
        [ "🔧 문서 유지관리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2505", [
          [ "최근 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2506", null ]
        ] ],
        [ "📖 외부 리소스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2507", null ],
        [ "🤝 도움 및 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2508", null ]
      ] ]
    ] ],
    [ "Security Module Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2528", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2530", [
        [ "Purpose", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2531", null ],
        [ "Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2532", null ]
      ] ],
      [ "Threat Model", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2534", [
        [ "Threats Defended Against", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2535", null ],
        [ "Security Assumptions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2536", null ],
        [ "Out of Scope", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2537", null ]
      ] ],
      [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2539", [
        [ "Component Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2540", null ],
        [ "Dependency Graph", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2541", null ]
      ] ],
      [ "Audit Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2543", [
        [ "Purpose", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2544", null ],
        [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2545", null ],
        [ "Audit Event Types", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2546", null ],
        [ "API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2547", [
          [ "Initialize Audit Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2548", null ],
          [ "Log an Audit Event", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2549", null ],
          [ "Verify Entry Integrity", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2550", null ]
        ] ],
        [ "Example Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2551", null ],
        [ "Compliance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2552", null ]
      ] ],
      [ "Log Sanitizer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2554", [
        [ "Purpose", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2555", null ],
        [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2556", null ],
        [ "Sensitive Data Types", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2557", null ],
        [ "API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2558", [
          [ "Add Built-in Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2559", null ],
          [ "Add Custom Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2560", null ],
          [ "Remove Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2561", null ],
          [ "Sanitize a String", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2562", null ],
          [ "Check for Sensitive Data", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2563", null ],
          [ "Get Active Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2564", null ],
          [ "Add All Common Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2565", null ]
        ] ],
        [ "Example Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2566", null ],
        [ "Performance Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2567", null ],
        [ "Integration with Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2568", null ]
      ] ],
      [ "Path Validator", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2570", [
        [ "Purpose", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2571", null ],
        [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2572", null ],
        [ "Security Checks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2573", null ],
        [ "API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2574", [
          [ "Constructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2575", null ],
          [ "Validate Path", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2576", null ],
          [ "Static: Check Filename Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2577", null ],
          [ "Static: Sanitize Filename", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2578", null ],
          [ "Static: Safe Path Join", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2579", null ]
        ] ],
        [ "Example Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2580", null ],
        [ "Integration with Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2581", null ]
      ] ],
      [ "Secure Key Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2583", [
        [ "Purpose", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2584", null ],
        [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2585", null ],
        [ "Classes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2586", [
          [ "<tt>secure_key</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2587", null ],
          [ "<tt>secure_key_storage</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2588", null ]
        ] ],
        [ "API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2589", [
          [ "Generate Secure Random Key", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2590", null ],
          [ "Save Key to File", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2591", null ],
          [ "Load Key from File", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2592", null ]
        ] ],
        [ "Example Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2593", null ],
        [ "Key Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2594", null ],
        [ "Platform-Specific Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2595", [
          [ "Linux/macOS", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2596", null ],
          [ "Windows", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2597", null ]
        ] ]
      ] ],
      [ "Signal Manager", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2599", [
        [ "Purpose", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2600", null ],
        [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2601", null ],
        [ "Handled Signals", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2602", null ],
        [ "Signal-Safe Guarantees", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2603", null ],
        [ "API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2604", [
          [ "Register Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2605", null ],
          [ "Unregister Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2606", null ],
          [ "Check Handler Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2607", null ],
          [ "Get Logger Count", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2608", null ]
        ] ],
        [ "Critical Logger Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2609", null ],
        [ "Example Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2610", null ],
        [ "Emergency Flush Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2611", null ],
        [ "Testing Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2612", null ]
      ] ],
      [ "Configuration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2614", [
        [ "Minimal Security Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2615", null ],
        [ "High-Security Setup (Compliance)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2616", null ],
        [ "Custom Security Layer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2617", null ]
      ] ],
      [ "Compliance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2619", [
        [ "SOX (Sarbanes-Oxley)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2620", null ],
        [ "HIPAA (Health Insurance Portability and Accountability Act)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2621", null ],
        [ "GDPR (General Data Protection Regulation)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2622", null ],
        [ "PCI-DSS (Payment Card Industry Data Security Standard)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2623", null ],
        [ "ISO 27001 (Information Security Management)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2624", null ]
      ] ],
      [ "Frequently Asked Questions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2626", [
        [ "Q: Can I use the security module without OpenSSL?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2627", null ],
        [ "Q: How do I rotate encryption keys?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2628", null ],
        [ "Q: What's the performance overhead of log sanitization?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2629", null ],
        [ "Q: How do I verify audit log integrity?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2630", null ],
        [ "Q: Can I use multiple loggers with one signal manager?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2631", null ],
        [ "Q: How do I handle signal manager cleanup?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2632", null ]
      ] ],
      [ "Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2SECURITY__GUIDE.html#autotoc_md2634", null ]
    ] ],
    [ "Writer Composition and Decorator Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2638", null ],
      [ "Architecture Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2640", [
        [ "Decorator Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2641", null ],
        [ "Inheritance Chain", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2642", null ],
        [ "Stack Visualization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2643", null ]
      ] ],
      [ "Writer Catalog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2645", [
        [ "Base Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2646", [
          [ "1. <tt>console_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2647", null ],
          [ "2. <tt>file_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2649", null ],
          [ "3. <tt>rotating_file_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2651", null ],
          [ "4. <tt>network_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2653", null ],
          [ "5. <tt>otlp_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2655", null ]
        ] ],
        [ "Decorator Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2657", [
          [ "6. <tt>async_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2658", null ],
          [ "7. <tt>batch_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2660", null ],
          [ "8. <tt>buffered_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2662", null ],
          [ "9. <tt>filtered_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2664", null ],
          [ "10. <tt>formatted_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2666", null ],
          [ "11. <tt>thread_safe_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2668", null ],
          [ "12. <tt>encrypted_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2670", null ],
          [ "13. <tt>critical_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2672", null ],
          [ "14. <tt>composite_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2674", null ],
          [ "15. <tt>queued_writer_base</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2676", null ],
          [ "16. <tt>legacy_writer_adapter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2678", null ]
        ] ]
      ] ],
      [ "Composition Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2680", [
        [ "Pattern 1: Simple Console (Development)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2681", null ],
        [ "Pattern 2: Async Console (Development + Performance)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2683", null ],
        [ "Pattern 3: Filtered Formatted File (Production)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2685", null ],
        [ "Pattern 4: High-Throughput Production", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2687", null ],
        [ "Pattern 5: Encrypted Compliance Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2689", null ],
        [ "Pattern 6: Multi-Destination (File + Network)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2691", null ]
      ] ],
      [ "Recommended Stacks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2693", [
        [ "Development", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2694", null ],
        [ "Production File Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2695", null ],
        [ "Compliance & Security", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2696", null ],
        [ "Distributed Systems", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2697", null ],
        [ "Resource-Constrained", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2698", null ]
      ] ],
      [ "Code Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2700", [
        [ "Example 1: Development Setup (3 lines)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2701", null ],
        [ "Example 2: Production File Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2703", null ],
        [ "Example 3: Encrypted Compliance Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2705", null ],
        [ "Example 4: Multi-Destination Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2707", null ],
        [ "Example 5: Custom Stack Order", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2709", null ]
      ] ],
      [ "Performance Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2711", [
        [ "Throughput per Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2712", null ],
        [ "Latency per Decorator", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2713", null ],
        [ "Memory Usage per Decorator", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2714", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2716", [
        [ "1. Filter Early, Format Late", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2717", null ],
        [ "2. Use Async for I/O-Heavy Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2719", null ],
        [ "3. Batch + Buffer for Maximum Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2721", null ],
        [ "4. Encrypt Only What's Necessary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2723", null ],
        [ "5. Use <tt>composite_writer</tt> for Multi-Destination", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2725", null ],
        [ "6. Flush on Critical Logs", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2727", null ],
        [ "7. Thread-Safety Only When Needed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2729", null ],
        [ "8. Start Async Writers Explicitly", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2731", null ]
      ] ],
      [ "Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2733", null ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"index.html",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md839",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CONFIGURATION__STRATEGIES.html#autotoc_md1234",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2238",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2WRITER__GUIDE.html#autotoc_md2674",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md395",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md633",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1662",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1888"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';