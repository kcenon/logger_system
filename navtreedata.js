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
        [ "Composing Writers with Decorator Pattern", "index.html#autotoc_md5", null ],
        [ "Installation", "index.html#autotoc_md6", null ],
        [ "Requirements", "index.html#autotoc_md7", [
          [ "Dependency Flow", "index.html#autotoc_md8", null ],
          [ "Building with Dependencies", "index.html#autotoc_md9", null ]
        ] ]
      ] ],
      [ "Core Features", "index.html#autotoc_md11", [
        [ "Asynchronous Logging", "index.html#autotoc_md12", null ],
        [ "Multiple Writer Types", "index.html#autotoc_md13", null ],
        [ "OpenTelemetry Integration (v3.0.0)", "index.html#autotoc_md14", null ],
        [ "Security Features (v3.0.0)", "index.html#autotoc_md15", null ],
        [ "Structured Logging (v3.1.0)", "index.html#autotoc_md16", null ]
      ] ],
      [ "Performance Highlights", "index.html#autotoc_md18", [
        [ "Throughput", "index.html#autotoc_md19", null ],
        [ "Latency", "index.html#autotoc_md20", null ],
        [ "Memory Efficiency", "index.html#autotoc_md21", null ]
      ] ],
      [ "Architecture Overview", "index.html#autotoc_md23", [
        [ "Modular Design", "index.html#autotoc_md24", null ],
        [ "Key Components", "index.html#autotoc_md25", null ]
      ] ],
      [ "Ecosystem Integration", "index.html#autotoc_md27", [
        [ "Dependencies", "index.html#autotoc_md28", null ],
        [ "Integration Pattern", "index.html#autotoc_md29", null ]
      ] ],
      [ "Documentation", "index.html#autotoc_md31", [
        [ "Getting Started", "index.html#autotoc_md32", null ],
        [ "Core Documentation", "index.html#autotoc_md33", null ],
        [ "Advanced Topics", "index.html#autotoc_md34", null ],
        [ "Development", "index.html#autotoc_md35", null ]
      ] ],
      [ "Configuration Templates", "index.html#autotoc_md37", [
        [ "Advanced Configuration", "index.html#autotoc_md38", null ]
      ] ],
      [ "Build Configuration", "index.html#autotoc_md40", [
        [ "CMake Feature Flags", "index.html#autotoc_md41", null ]
      ] ],
      [ "Platform Support", "index.html#autotoc_md43", [
        [ "Officially Supported", "index.html#autotoc_md44", null ]
      ] ],
      [ "Testing", "index.html#autotoc_md46", [
        [ "Test Coverage", "index.html#autotoc_md47", null ],
        [ "Running Tests", "index.html#autotoc_md48", null ],
        [ "CI/CD Status", "index.html#autotoc_md49", null ]
      ] ],
      [ "Contributing", "index.html#autotoc_md51", [
        [ "Development Workflow", "index.html#autotoc_md52", null ],
        [ "Code Standards", "index.html#autotoc_md53", null ]
      ] ],
      [ "Support", "index.html#autotoc_md55", null ],
      [ "License", "index.html#autotoc_md57", null ],
      [ "Acknowledgments", "index.html#autotoc_md59", null ]
    ] ],
    [ "Architecture - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md62", null ],
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md64", null ],
      [ "Design Philosophy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md66", [
        [ "Core Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md67", null ]
      ] ],
      [ "Layered Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md69", [
        [ "Layer Responsibilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md70", null ]
      ] ],
      [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md72", [
        [ "1. Logger (<tt>core/logger.h</tt>, <tt>core/logger.cpp</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md73", null ],
        [ "2. Logger Builder (<tt>core/logger_builder.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md75", null ],
        [ "3. Log Collector (<tt>core/log_collector.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md77", null ],
        [ "4. Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md79", [
          [ "Console Writer (<tt>writers/console_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md80", null ],
          [ "File Writer (<tt>writers/file_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md81", null ],
          [ "Rotating File Writer (<tt>writers/rotating_file_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md82", null ],
          [ "Network Writer (<tt>writers/network_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md83", null ],
          [ "Critical Writer (<tt>writers/critical_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md84", null ],
          [ "Async Writer (<tt>writers/async_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md85", null ]
        ] ],
        [ "5. Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md87", [
          [ "Base Formatter (<tt>formatters/base_formatter.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md88", null ],
          [ "JSON Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md89", null ],
          [ "XML Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md90", null ]
        ] ],
        [ "6. Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md92", [
          [ "Level Filter (<tt>filters/log_filter.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md93", null ],
          [ "Regex Filter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md94", null ],
          [ "Function Filter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md95", null ]
        ] ]
      ] ],
      [ "Integration Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md97", [
        [ "Common System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md98", null ],
        [ "Thread System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md99", null ],
        [ "Monitoring System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md100", null ]
      ] ],
      [ "Threading Model", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md102", [
        [ "Asynchronous Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md103", null ],
        [ "Thread Safety Guarantees", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md104", null ]
      ] ],
      [ "Memory Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md106", [
        [ "Small String Optimization (SSO)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md107", null ],
        [ "Log Entry Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md108", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md110", [
        [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md111", null ],
        [ "Optimization Techniques", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md112", null ]
      ] ],
      [ "Design Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md114", [
        [ "1. Observer Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md115", null ],
        [ "2. Strategy Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md116", null ],
        [ "3. Builder Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md117", null ],
        [ "4. Template Method Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md118", null ],
        [ "5. Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md119", null ],
        [ "6. Factory Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md120", null ]
      ] ],
      [ "Build Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md122", [
        [ "CMake Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md123", null ],
        [ "Dependency Resolution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md124", null ],
        [ "Compiler Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md125", null ]
      ] ],
      [ "Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md127", [
        [ "Planned Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md128", null ],
        [ "Research Areas", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md129", null ]
      ] ],
      [ "Error Code Allocation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md131", null ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md133", null ]
    ] ],
    [ "비동기 Writer 구현체 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html", [
      [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md136", null ],
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md138", null ],
      [ "빠른 선택 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md140", [
        [ "선택 플로우차트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md141", null ]
      ] ],
      [ "구현체 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md143", null ],
      [ "상세 설명", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md145", [
        [ "1. async_writer (공개 API)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md146", [
          [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md147", null ],
          [ "아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md148", null ],
          [ "API 레퍼런스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md149", null ],
          [ "사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md150", null ]
        ] ],
        [ "2. async_worker (내부)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md152", [
          [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md153", null ],
          [ "아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md154", null ],
          [ "사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md155", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md156", [
        [ "3. high_performance_async_writer (내부)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md157", [
          [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md158", null ],
          [ "아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md159", null ],
          [ "설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md160", null ],
          [ "사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md161", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md162", null ],
      [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md163", [
        [ "벤치마크 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md164", null ],
        [ "지연 시간 분포", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md165", null ]
      ] ],
      [ "사용 예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md167", [
        [ "예제 1: 기본 async_writer 사용", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md168", null ],
        [ "예제 2: 콘솔 출력과 async_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md169", null ],
        [ "예제 3: 큐 가득 참 처리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md170", null ],
        [ "예제 4: 정상 종료 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md171", null ]
      ] ],
      [ "모범 사례", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md173", [
        [ "1. 큐 크기 선택", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md174", null ],
        [ "2. 플러시 타임아웃 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md175", null ],
        [ "3. 에러 처리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md176", null ],
        [ "4. 생명주기 관리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md177", null ],
        [ "5. 스레드 안전성 고려사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md178", null ]
      ] ],
      [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md180", [
        [ "동기에서 async_writer로", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md181", null ],
        [ "큐 가득 참 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md182", null ]
      ] ],
      [ "관련 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md184", null ]
    ] ],
    [ "Asynchronous Writer Implementations Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md187", null ],
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md189", null ],
      [ "Quick Decision Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md191", [
        [ "Decision Flowchart", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md192", null ]
      ] ],
      [ "Implementation Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md194", null ],
      [ "Detailed Descriptions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md196", [
        [ "1. async_writer (Public API)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md197", [
          [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md198", null ],
          [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md199", null ],
          [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md200", null ],
          [ "When to Use", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md201", null ]
        ] ],
        [ "2. async_worker (Internal)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md203", [
          [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md204", null ],
          [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md205", null ],
          [ "When to Use", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md206", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md207", [
        [ "3. high_performance_async_writer (Internal)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md208", [
          [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md209", null ],
          [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md210", null ],
          [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md211", null ],
          [ "When to Use", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md212", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md213", null ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md214", [
        [ "Benchmark Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md215", null ],
        [ "Latency Distribution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md216", null ]
      ] ],
      [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md218", [
        [ "Example 1: Basic async_writer Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md219", null ],
        [ "Example 2: async_writer with Console Output", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md220", null ],
        [ "Example 3: Queue Full Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md221", null ],
        [ "Example 4: Graceful Shutdown Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md222", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md224", [
        [ "1. Queue Size Selection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md225", null ],
        [ "2. Flush Timeout Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md226", null ],
        [ "3. Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md227", null ],
        [ "4. Lifecycle Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md228", null ],
        [ "5. Thread Safety Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md229", null ]
      ] ],
      [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md231", [
        [ "From Synchronous to async_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md232", null ],
        [ "Queue Full Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md233", null ]
      ] ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md235", null ]
    ] ],
    [ "CI_CD_DASHBOARD.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD_8kr.html", [
      [ "Logger System CI/CD 대시보드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD_8kr.html#autotoc_md237", null ]
    ] ],
    [ "CI_CD_DASHBOARD", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html", [
      [ "Logger System CI/CD Dashboard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md240", [
        [ "🚀 Build Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md241", [
          [ "Main Branch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md242", null ]
        ] ],
        [ "📊 Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md243", [
          [ "Latest Benchmark Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md244", null ],
          [ "Performance Trends", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md245", null ]
        ] ],
        [ "🔍 Code Quality", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md246", [
          [ "Static Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md247", null ],
          [ "Test Coverage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md248", null ]
        ] ],
        [ "🧪 Test Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md249", [
          [ "Test Execution Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md250", null ],
          [ "Recent Test Failures", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md251", null ]
        ] ],
        [ "🔥 Sanitizer Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md252", null ],
        [ "📈 Build Time Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md253", [
          [ "Average Build Times", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md254", null ]
        ] ],
        [ "🚨 Recent Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md255", [
          [ "Build Failures (Last 7 Days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md256", null ],
          [ "Performance Regressions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md257", null ]
        ] ],
        [ "📝 Dependency Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md258", [
          [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md259", null ]
        ] ],
        [ "🔄 CI/CD Pipeline Health", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md260", [
          [ "Workflow Execution Stats (Last 30 Days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md261", null ],
          [ "Resource Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md262", null ]
        ] ],
        [ "📅 Release History", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md263", [
          [ "Recent Releases", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md264", null ]
        ] ],
        [ "🔗 Quick Links", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md265", null ],
        [ "📊 Dashboard Update", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md266", null ],
        [ "<em>This dashboard is automatically updated by CI/CD workflows. For real-time data, check the <a href=\"https://github.com/kcenon/logger_system/actions\" >GitHub Actions page</a>.</em>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md268", null ]
      ] ]
    ] ],
    [ "Conditional Compilation Refactoring Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html", [
      [ "Problem Statement", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md270", [
        [ "Current State", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md271", null ],
        [ "Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md272", null ],
        [ "Example of Current Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md273", null ]
      ] ],
      [ "Proposed Solution: Adapter Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md274", [
        [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md275", null ],
        [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md276", null ],
        [ "Trade-offs", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md277", null ]
      ] ],
      [ "Implementation Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md278", [
        [ "Phase 3.1: Analysis and Documentation ✅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md279", null ],
        [ "Phase 3.2: Core Interface Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md280", null ],
        [ "Phase 3.3: Adapter Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md281", null ],
        [ "Phase 3.4: Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md282", null ],
        [ "Phase 3.5: Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md283", null ]
      ] ],
      [ "Detailed Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md284", [
        [ "Core Logger Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md285", null ],
        [ "Adapter Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md286", null ],
        [ "Thread System Adapter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md287", null ],
        [ "Common System Adapter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md288", null ],
        [ "Factory Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md289", null ]
      ] ],
      [ "Migration Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md290", [
        [ "Step 1: Extract Core (1 week)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md291", null ],
        [ "Step 2: Implement Adapters (1 week)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md292", null ],
        [ "Step 3: Update Public Interface (3 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md293", null ],
        [ "Step 4: Remove Conditional Compilation (2 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md294", null ],
        [ "Step 5: Testing and Validation (3 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md295", null ]
      ] ],
      [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md296", [
        [ "Virtual Function Overhead", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md297", null ],
        [ "Binary Size Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md298", null ]
      ] ],
      [ "Risk Mitigation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md299", [
        [ "Risks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md300", null ],
        [ "Rollback Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md301", null ]
      ] ],
      [ "Success Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md302", null ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md303", null ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html", [
      [ "크리티컬 로그 손실 방지 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html#autotoc_md305", null ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html", [
      [ "Critical Log Loss Prevention Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md309", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md308", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md310", [
          [ "Problem", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md311", null ],
          [ "Solution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md312", null ]
        ] ],
        [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md314", null ],
        [ "Class Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md316", [
          [ "1. <tt>critical_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md317", [
            [ "Key Features:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md318", null ],
            [ "Configuration:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md319", null ],
            [ "Usage Example:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md320", null ]
          ] ],
          [ "2. <tt>hybrid_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md321", [
            [ "Features:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md322", null ],
            [ "Usage Example:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md323", null ]
          ] ]
        ] ],
        [ "How It Works", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md325", [
          [ "1. Level-Based Routing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md326", null ],
          [ "2. Critical Write Flow", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md327", [
            [ "Step-by-Step Explanation:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md328", null ]
          ] ],
          [ "3. Write-Ahead Logging (WAL)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md329", [
            [ "WAL Format:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md330", null ],
            [ "WAL Usage:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md331", null ]
          ] ],
          [ "4. Signal Handler", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md332", [
            [ "Handled Signals:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md333", null ],
            [ "Signal Handler Implementation:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md334", null ],
            [ "Important Notes:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md335", null ]
          ] ],
          [ "5. File Descriptor Sync", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md336", [
            [ "Buffer Layers:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md337", null ],
            [ "Implementation:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md338", null ]
          ] ]
        ] ],
        [ "Performance Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md340", [
          [ "1. Critical Log Overhead", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md341", null ],
          [ "2. Normal Log Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md342", null ],
          [ "3. Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md343", [
            [ "Strategy 1: Disable <tt>sync_on_critical</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md344", null ],
            [ "Strategy 2: Disable <tt>force_flush_on_error</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md345", null ],
            [ "Strategy 3: Use Hybrid Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md346", null ]
          ] ],
          [ "4. Benchmark Results (Estimated)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md347", null ]
        ] ],
        [ "Production Recommendations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md349", [
          [ "1. Default Configuration (General Services)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md350", null ],
          [ "2. High Reliability Configuration (Finance/Medical)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md351", null ],
          [ "3. High Performance Configuration (Games/Real-time Systems)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md352", null ]
        ] ],
        [ "Testing & Verification", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md354", [
          [ "1. Critical Log Loss Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md355", [
            [ "Verification Method:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md356", null ]
          ] ],
          [ "2. WAL Recovery Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md357", null ],
          [ "3. Performance Benchmark", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md358", null ]
        ] ],
        [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md360", [
          [ "Issue 1: WAL File Not Created", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md361", null ],
          [ "Issue 2: Signal Handler Not Working", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md362", null ],
          [ "Issue 3: Performance Degradation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md363", null ]
        ] ],
        [ "FAQ", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md365", [
          [ "Q1: Should I wrap all logs with critical_writer?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md366", null ],
          [ "Q2: When should I use WAL?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md367", null ],
          [ "Q3: fsync() overhead is too high.", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md368", null ],
          [ "Q4: What can I do in a signal handler?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md369", null ]
        ] ],
        [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md371", null ],
        [ "Version History", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md373", null ],
        [ "License", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md375", null ]
      ] ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html", [
      [ "크리티컬 로깅 빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html#autotoc_md377", null ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html", [
      [ "Critical Logging Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md381", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md380", null ],
        [ "🎯 Goal", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md383", null ],
        [ "🚀 Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md385", [
          [ "Step 1: Include Headers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md386", null ],
          [ "Step 2: Create Hybrid Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md387", null ],
          [ "Step 3: Verify", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md388", null ]
        ] ],
        [ "📊 3 Usage Scenarios", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md390", [
          [ "Scenario 1: General Web Services (Recommended)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md391", null ],
          [ "Scenario 2: Finance/Medical Systems (Maximum Reliability)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md392", null ],
          [ "Scenario 3: Games/Real-time Systems (High Performance)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md393", null ]
        ] ],
        [ "🔧 Configuration Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md395", null ],
        [ "✅ Verification Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md397", [
          [ "Test 1: Check Critical Logs", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md398", null ],
          [ "Test 2: Crash Simulation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md399", null ],
          [ "Test 3: Ctrl+C Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md400", null ]
        ] ],
        [ "📚 Learn More", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md402", null ],
        [ "🐛 Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md404", [
          [ "\"WAL file not created\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md405", null ],
          [ "\"Performance too slow\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md406", null ],
          [ "\"Signal handler not working\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md407", null ]
        ] ],
        [ "💡 Key Points", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md409", null ]
      ] ]
    ] ],
    [ "CUSTOM_WRITERS.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS_8kr.html", [
      [ "커스텀 작성기 생성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS_8kr.html#autotoc_md412", null ]
    ] ],
    [ "CUSTOM_WRITERS", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html", [
      [ "Creating Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md415", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md416", null ],
        [ "Writer Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md417", null ],
        [ "Recommended: Using thread_safe_writer (Since v1.3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md418", [
          [ "Benefits of thread_safe_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md419", null ]
        ] ],
        [ "Base Writer Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md420", null ],
        [ "Simple Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md421", [
          [ "1. Simple File Writer (Using thread_safe_writer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md422", null ],
          [ "2. Rotating File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md423", null ],
          [ "3. Network Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md424", null ],
          [ "4. Database Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md425", null ]
        ] ],
        [ "Advanced Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md426", [
          [ "1. Filtering Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md427", null ],
          [ "2. Async Writer Wrapper", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md428", null ]
        ] ],
        [ "Choosing the Right Base Class", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md429", [
          [ "When to Use thread_safe_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md430", null ],
          [ "When to Use base_writer Directly", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md431", null ]
        ] ],
        [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md432", null ],
        [ "Testing Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md433", null ]
      ] ]
    ] ],
    [ "Log Level Semantic Standard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html", [
      [ "Problem Statement", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md437", [
        [ "logger_system (Ascending: Low → High)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md438", null ],
        [ "thread_system (Descending: High → Low)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md439", null ],
        [ "Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md440", null ]
      ] ],
      [ "Decision", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md442", [
        [ "Rationale", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md443", null ]
      ] ],
      [ "Migration Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md445", [
        [ "Phase 1: Documentation (Sprint 1 - Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md446", null ],
        [ "Phase 2: thread_system Unification (Sprint 2)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md447", null ],
        [ "Phase 3: Adapter Simplification (Sprint 3)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md448", null ]
      ] ],
      [ "Implementation Details", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md450", [
        [ "Standard Definition (logger_system - already compliant)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md451", null ],
        [ "thread_system Changes (Sprint 2)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md452", null ],
        [ "Adapter Simplification (Sprint 3)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md453", null ]
      ] ],
      [ "Compatibility Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md455", [
        [ "Breaking Change Assessment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md456", null ],
        [ "Mitigation Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md457", null ],
        [ "User Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md458", null ]
      ] ],
      [ "Testing Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md460", [
        [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md461", null ],
        [ "Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md462", null ],
        [ "Regression Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md463", null ]
      ] ],
      [ "Documentation Updates Required", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md465", null ],
      [ "Success Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md467", null ],
      [ "Timeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md469", null ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md471", null ],
      [ "Approval", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md473", null ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html", [
      [ "로거 시스템 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md475", [
        [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md476", [
          [ "주요 기능 (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md477", null ]
        ] ],
        [ "아키텍처 다이어그램", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md478", null ],
        [ "핵심 컴포넌트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md479", [
          [ "1. ILogger 인터페이스 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md480", null ],
          [ "2. Dual API 설계", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md481", null ],
          [ "3. 백엔드 추상화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md482", null ]
        ] ],
        [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md483", [
          [ "벤치마크 (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md484", null ],
          [ "멀티스레드 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md485", null ]
        ] ]
      ] ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html", [
      [ "Logger System Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md488", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md489", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md490", [
          [ "Key Features (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md491", null ]
        ] ],
        [ "Architecture Diagram", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md492", null ],
        [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md493", [
          [ "1. ILogger Interface Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md494", null ],
          [ "2. Dual API Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md495", null ],
          [ "3. Configuration Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md496", [
            [ "Logger Configuration with Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md497", null ],
            [ "Configuration Templates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md498", null ]
          ] ],
          [ "4. Builder Pattern with Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md499", null ],
          [ "5. Backend Abstraction", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md500", null ],
          [ "6. Interface Segregation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md501", [
            [ "Writer Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md502", null ],
            [ "Filter Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md503", null ],
            [ "Formatter Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md504", null ]
          ] ],
          [ "7. Log Entry Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md505", null ]
        ] ],
        [ "Advanced Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md506", [
          [ "1. Asynchronous Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md507", null ],
          [ "2. Error Handling with Result Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md508", null ],
          [ "3. C++20 Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md509", null ],
          [ "4. Performance Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md510", null ],
          [ "5. Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md511", null ]
        ] ],
        [ "Threading Model", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md512", [
          [ "Synchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md513", null ],
          [ "Asynchronous Mode (Default)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md514", null ],
          [ "Thread Safety Guarantees", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md515", null ]
        ] ],
        [ "Memory Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md516", [
          [ "Buffer Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md517", null ],
          [ "Object Lifetime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md518", null ]
        ] ],
        [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md519", [
          [ "Benchmarks (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md520", null ],
          [ "Multi-threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md521", null ],
          [ "Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md522", null ]
        ] ],
        [ "Integration Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md523", [
          [ "ILogger Interface Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md524", null ],
          [ "Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md525", null ],
          [ "With Monitoring (Phase 2.2.4)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md526", null ]
        ] ],
        [ "Extension Points", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md527", [
          [ "Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md528", null ],
          [ "Custom Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md529", null ],
          [ "Custom Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md530", null ]
        ] ],
        [ "Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md531", [
          [ "Performance Improvements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md532", null ],
          [ "Feature Additions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md533", null ],
          [ "Platform Extensions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md534", null ]
        ] ],
        [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md535", [
          [ "For Library Users", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md536", null ],
          [ "For Contributors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md537", null ]
        ] ],
        [ "Platform Notes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md538", null ]
      ] ]
    ] ],
    [ "Migration Guide - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md541", null ],
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md543", null ],
      [ "Quick Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md545", null ],
      [ "Migration Path 1: From Integrated thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md547", [
        [ "Background", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md548", null ],
        [ "Step 1: Update CMakeLists.txt", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md549", null ],
        [ "Step 2: Update Header Includes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md550", null ],
        [ "Step 3: Update Namespace", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md551", null ],
        [ "Step 4: Migrate Logger Creation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md552", null ],
        [ "Step 5: Update Thread System Integration (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md553", null ]
      ] ],
      [ "Migration Path 2: From spdlog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md555", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md556", null ],
        [ "API Mapping", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md557", null ],
        [ "Example Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md558", null ],
        [ "Pattern Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md559", null ]
      ] ],
      [ "Migration Path 3: From Custom Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md561", [
        [ "Common Custom Logger Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md562", [
          [ "Pattern 1: Direct File Writing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md563", null ],
          [ "Pattern 2: Multiple Output Targets", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md564", null ],
          [ "Pattern 3: Thread-Safe Singleton", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md565", null ]
        ] ]
      ] ],
      [ "Common Migration Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md567", [
        [ "Issue 1: Namespace Conflicts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md568", null ],
        [ "Issue 2: CMake Cache Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md569", null ],
        [ "Issue 3: Header Not Found", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md570", null ],
        [ "Issue 4: Result<T> Pattern Unfamiliar", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md571", null ],
        [ "Issue 5: Performance Regression", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md572", null ]
      ] ],
      [ "Testing Your Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md574", [
        [ "1. Compile Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md575", null ],
        [ "2. Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md576", null ],
        [ "3. Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md577", null ],
        [ "4. Performance Benchmark", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md578", null ]
      ] ],
      [ "Rollback Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md580", [
        [ "Option 1: Revert Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md581", null ],
        [ "Option 2: Gradual Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md582", null ]
      ] ],
      [ "Migration Assistance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md584", [
        [ "Automated Migration Script", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md585", null ]
      ] ],
      [ "Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md587", null ],
      [ "Next Steps After Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md589", null ]
    ] ],
    [ "Logger System - 프로젝트 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html", [
      [ "📁 디렉토리 레이아웃", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md593", null ],
      [ "🏗️ Namespace 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md594", [
        [ "Core Namespaces", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md595", null ],
        [ "Nested Namespaces", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md596", null ]
      ] ],
      [ "🔧 주요 컴포넌트 개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md597", [
        [ "🎯 Public API Layer (<tt>include/kcenon/logger/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md598", null ],
        [ "⚙️ Implementation Layer (<tt>src/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md599", null ]
      ] ],
      [ "📊 성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md600", null ],
      [ "🔄 마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md601", [
        [ "Step 1: 현재 설정 백업", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md602", null ],
        [ "Step 2: Include 경로 업데이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md603", null ],
        [ "Step 3: Namespace 사용 업데이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md604", null ],
        [ "Step 4: 마이그레이션 스크립트 실행", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md605", null ]
      ] ],
      [ "🚀 새로운 구조로 빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md606", null ]
    ] ],
    [ "Logger System - Project Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md609", null ],
      [ "Directory Layout", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md610", null ],
      [ "Namespace Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md611", [
        [ "Primary Namespace Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md612", null ],
        [ "Namespace Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md613", null ]
      ] ],
      [ "Component Layout", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md614", [
        [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md615", [
          [ "Logger Core (<tt>core/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md616", null ],
          [ "Interfaces (<tt>interfaces/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md617", null ],
          [ "Writers (<tt>writers/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md618", null ],
          [ "Formatters (<tt>formatters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md619", null ],
          [ "Filters (<tt>filters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md620", null ],
          [ "Routing (<tt>routing/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md621", null ],
          [ "Analysis (<tt>analysis/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md622", null ],
          [ "Structured Logging (<tt>structured/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md623", null ],
          [ "Safety (<tt>safety/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md624", null ],
          [ "Server (<tt>server/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md625", null ],
          [ "Adapters (<tt>adapters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md626", null ]
        ] ],
        [ "Monitoring (<tt>core/monitoring/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md627", null ]
      ] ],
      [ "Module Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md628", [
        [ "Internal Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md629", null ],
        [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md630", null ],
        [ "Compilation Units", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md631", null ]
      ] ],
      [ "Key Components Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md632", [
        [ "Public API Layer (<tt>include/kcenon/logger/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md633", null ],
        [ "Implementation Layer (<tt>src/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md634", null ],
        [ "Test Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md635", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md636", [
        [ "Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md637", null ],
        [ "Memory", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md638", null ],
        [ "Threading", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md639", null ],
        [ "Resource Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md640", null ]
      ] ],
      [ "Build Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md641", [
        [ "CMake Targets", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md642", null ],
        [ "Feature Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md643", null ]
      ] ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md644", null ]
    ] ],
    [ "WRITER_HIERARCHY", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html", [
      [ "Writer Hierarchy and Categories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md645", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md646", null ],
        [ "Category System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md647", [
          [ "Writer Categories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md648", null ],
          [ "Category Tags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md649", null ]
        ] ],
        [ "Complete Writer Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md650", null ],
        [ "Category Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md651", [
          [ "Synchronous Writers (sync_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md652", null ],
          [ "Asynchronous Writers (async_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md653", null ],
          [ "Decorator Writers (decorator_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md654", null ],
          [ "Composite Writers (composite_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md655", null ]
        ] ],
        [ "Using Category Information", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md656", [
          [ "Compile-Time Type Traits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md657", null ],
          [ "C++20 Concepts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md658", null ],
          [ "Runtime Category Query", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md659", null ]
        ] ],
        [ "Design Patterns Used", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md660", [
          [ "Template Method Pattern (thread_safe_writer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md661", null ],
          [ "Decorator Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md662", null ],
          [ "Pipeline Pattern (composite_writer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md663", null ]
        ] ],
        [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md664", null ]
      ] ]
    ] ],
    [ "WRITER_SELECTION_GUIDE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html", [
      [ "Writer Selection Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md666", [
        [ "Quick Selection Flowchart", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md667", null ],
        [ "Writer Selection by Use Case", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md668", [
          [ "1. Simple Console Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md669", null ],
          [ "2. Basic File Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md671", null ],
          [ "3. Production File Logging with Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md673", null ],
          [ "4. High-Throughput Logging (>10K msg/sec)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md675", null ],
          [ "5. Network/Remote Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md677", null ],
          [ "6. Encrypted/Secure Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md679", null ],
          [ "7. Critical/Safety-Critical Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md681", null ],
          [ "8. Custom Pipeline (Formatter + Sink)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md683", null ]
        ] ],
        [ "Decorator Stacking Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md685", [
          [ "Pattern 1: Async + Encrypted", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md686", null ],
          [ "Pattern 2: Critical + Async + Rotating", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md687", null ],
          [ "Pattern 3: Batch + Network", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md688", null ]
        ] ],
        [ "Performance Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md689", null ],
        [ "Decision Matrix", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md690", null ],
        [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md691", null ]
      ] ]
    ] ],
    [ "API_REFERENCE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html", [
      [ "API 레퍼런스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md693", [
        [ "v3.0 주요 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md694", [
          [ "네임스페이스 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md695", null ],
          [ "인터페이스 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md696", null ],
          [ "헤더 경로 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md697", null ],
          [ "주요 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md698", null ]
        ] ],
        [ "빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md699", null ]
      ] ]
    ] ],
    [ "API_REFERENCE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html", [
      [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md702", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md703", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md704", [
          [ "Namespace", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md705", null ]
        ] ],
        [ "Core Classes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md706", [
          [ "<tt>kcenon::logger::logger</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md707", [
            [ "Constructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md708", null ],
            [ "Destructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md709", null ]
          ] ]
        ] ],
        [ "ILogger Interface (Phase 2.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md710", [
          [ "Log Methods (ILogger Interface)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md711", null ],
          [ "Level Management (ILogger Interface)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md712", null ],
          [ "Native API (Backward Compatible)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md713", null ],
          [ "Writer Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md714", null ],
          [ "Lifecycle Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md715", null ],
          [ "Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md716", null ],
          [ "Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md717", null ],
          [ "Emergency Flush (Signal Safety)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md718", null ]
        ] ],
        [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md719", [
          [ "<tt>kcenon::logger::logger_config</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md720", null ],
          [ "Static Factory Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md721", null ]
        ] ],
        [ "Builder Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md722", [
          [ "<tt>kcenon::logger::logger_builder</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md723", [
            [ "Basic Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md724", null ],
            [ "Feature Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md725", null ],
            [ "Output Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md726", null ],
            [ "Writers and Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md727", null ],
            [ "Backend Selection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md728", null ],
            [ "Environment and Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md729", null ],
            [ "Build", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md730", null ]
          ] ]
        ] ],
        [ "Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md731", [
          [ "Deployment Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md732", null ],
          [ "Performance Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md733", null ],
          [ "Environment Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md734", null ],
          [ "Composite Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md735", null ]
        ] ],
        [ "Interfaces", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md736", [
          [ "<tt>kcenon::logger::log_writer_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md737", null ],
          [ "<tt>kcenon::logger::log_filter_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md738", null ],
          [ "<tt>kcenon::logger::log_formatter_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md739", null ],
          [ "<tt>kcenon::logger::log_entry</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md740", null ]
        ] ],
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md741", [
          [ "Error Codes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md742", null ],
          [ "Result Types", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md743", null ],
          [ "Helper Functions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md744", null ]
        ] ],
        [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md745", [
          [ "<tt>kcenon::logger::console_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md746", null ],
          [ "<tt>kcenon::logger::file_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md747", null ],
          [ "<tt>kcenon::logger::rotating_file_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md748", null ],
          [ "<tt>kcenon::logger::network_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md749", null ],
          [ "<tt>kcenon::logger::critical_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md750", null ],
          [ "<tt>kcenon::logger::batch_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md751", null ]
        ] ],
        [ "Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md752", [
          [ "<tt>kcenon::logger::filters::level_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md753", null ],
          [ "<tt>kcenon::logger::filters::regex_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md754", null ],
          [ "<tt>kcenon::logger::filters::function_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md755", null ],
          [ "<tt>kcenon::logger::filters::composite_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md756", null ]
        ] ],
        [ "Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md757", [
          [ "<tt>kcenon::logger::plain_formatter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md758", null ],
          [ "<tt>kcenon::logger::json_formatter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md759", null ]
        ] ],
        [ "Backend Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md760", [
          [ "Integration Backend Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md761", null ],
          [ "Standalone Backend (Default)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md762", null ]
        ] ],
        [ "Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md763", [
          [ "IMonitor Integration (Phase 2.2.4)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md764", null ]
        ] ],
        [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md765", [
          [ "Basic Usage with ILogger Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md766", null ],
          [ "Native API (Backward Compatible)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md767", null ],
          [ "Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md768", null ],
          [ "Custom Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md769", null ],
          [ "Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md770", null ]
        ] ],
        [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md771", null ],
        [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md772", null ],
        [ "Migration from v2.x", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md773", null ]
      ] ]
    ] ],
    [ "ARCHITECTURE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html", [
      [ "스레딩 생태계 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md775", [
        [ "v3.0 주요 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md776", [
          [ "의존성 구조 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md777", null ],
          [ "핵심 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md778", null ],
          [ "독립 실행 모드 vs 통합 모드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md779", null ],
          [ "성능 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md780", null ]
        ] ]
      ] ]
    ] ],
    [ "ARCHITECTURE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html", [
      [ "Threading Ecosystem Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md783", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md784", null ],
        [ "🏗️ Ecosystem Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md785", null ],
        [ "📋 Project Roles & Responsibilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md786", [
          [ "1. common_system (Foundation) - <strong>Required</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md787", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md788", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md789", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md790", null ]
          ] ],
          [ "2. logger_system (Logging) - <strong>Standalone Capable</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md792", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md793", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md794", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md795", null ]
          ] ],
          [ "3. thread_system (Threading) - <strong>Optional</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md797", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md798", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md799", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md800", null ]
          ] ],
          [ "4. monitoring_system (Metrics)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md802", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md803", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md804", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md805", null ]
          ] ],
          [ "5. integrated_thread_system (Integration Hub)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md807", [
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md808", null ]
          ] ]
        ] ],
        [ "🔄 Dependency Flow & Interface Contracts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md809", [
          [ "Interface Hierarchy (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md810", null ],
          [ "Dependency Graph (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md811", null ],
          [ "Build Order Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md812", null ],
          [ "Standalone vs Integrated Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md813", null ]
        ] ],
        [ "🔧 Integration Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md814", [
          [ "1. Interface-Based Integration (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md815", null ],
          [ "2. Dependency Injection Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md816", null ],
          [ "3. Configuration Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md817", null ],
          [ "4. Backend Selection Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md818", null ]
        ] ],
        [ "📊 Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md819", [
          [ "Design Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md820", null ],
          [ "Performance Metrics (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md821", null ],
          [ "Standalone vs Thread System Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md822", null ]
        ] ],
        [ "🔄 Evolution: Monolithic → Modular → Standalone", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md823", [
          [ "Phase 1: Monolithic (v1.x)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md824", null ],
          [ "Phase 2: Modular (v2.x)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md825", null ],
          [ "Phase 3: Standalone (v3.0) - Current", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md826", null ],
          [ "Migration Benefits (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md827", null ]
        ] ],
        [ "🚀 Getting Started", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md828", [
          [ "1. Minimal Setup (Standalone)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md829", null ],
          [ "2. Full Ecosystem Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md830", null ],
          [ "3. Build Order", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md831", null ],
          [ "4. CMake Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md832", null ]
        ] ],
        [ "📚 Documentation Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md833", [
          [ "common_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md834", null ],
          [ "logger_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md835", null ],
          [ "thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md836", null ],
          [ "monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md837", null ]
        ] ],
        [ "🔮 Future Roadmap", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md838", [
          [ "Phase 3.1: Enhancement (Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md839", null ],
          [ "Phase 3.2: Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md840", null ],
          [ "Phase 4: Ecosystem Expansion", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md841", null ]
        ] ]
      ] ]
    ] ],
    [ "Logger System 성능 벤치마크", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html", [
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md846", null ],
      [ "요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md848", [
        [ "핵심 하이라이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md849", null ],
        [ "경쟁 우위", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md850", null ]
      ] ],
      [ "테스트 환경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md852", [
        [ "하드웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md853", null ],
        [ "소프트웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md854", null ]
      ] ],
      [ "핵심 성능 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md856", [
        [ "단일 스레드 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md857", null ],
        [ "멀티스레드 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md858", [
          [ "스레드 수별 처리량", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md859", null ]
        ] ]
      ] ],
      [ "업계 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md861", [
        [ "vs spdlog (가장 인기 있는 C++ 로거)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md862", [
          [ "단일 스레드 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md863", null ],
          [ "멀티스레드 비교 (4 스레드)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md864", null ]
        ] ],
        [ "vs Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md865", null ],
        [ "vs glog (Google 로깅 라이브러리)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md866", null ]
      ] ],
      [ "지연시간 벤치마크", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md868", [
        [ "지연시간 분포 (단일 스레드, 비동기)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md869", null ],
        [ "부하 시 지연시간", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md870", null ]
      ] ],
      [ "메모리 프로파일링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md872", [
        [ "구성별 메모리 풋프린트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md873", null ],
        [ "경쟁사와 메모리 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md874", null ]
      ] ],
      [ "성능 회귀 기준", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md876", null ]
    ] ],
    [ "Logger System Performance Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md881", null ],
      [ "Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md883", [
        [ "Key Highlights", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md884", null ],
        [ "Competitive Advantages", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md885", null ]
      ] ],
      [ "Test Environment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md887", [
        [ "Hardware Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md888", null ],
        [ "Software Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md889", null ]
      ] ],
      [ "Core Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md891", [
        [ "Single-Threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md892", null ],
        [ "Multi-Threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md893", [
          [ "Throughput by Thread Count", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md894", null ],
          [ "Performance Scaling Graph", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md895", null ]
        ] ]
      ] ],
      [ "Industry Comparisons", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md897", [
        [ "vs spdlog (Most Popular C++ Logger)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md898", [
          [ "Single-Threaded Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md899", null ],
          [ "Multi-Threaded Comparison (4 Threads)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md900", null ]
        ] ],
        [ "vs Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md901", null ],
        [ "vs glog (Google's Logging Library)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md902", null ],
        [ "vs log4cxx", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md903", null ]
      ] ],
      [ "Scalability Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md905", [
        [ "Thread Scaling Efficiency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md906", null ],
        [ "Queue Utilization vs Thread Count", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md907", null ],
        [ "CPU Utilization Efficiency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md908", null ]
      ] ],
      [ "Latency Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md910", [
        [ "Latency Distribution (Single Thread, Async)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md911", null ],
        [ "Latency Comparison (p99)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md912", null ],
        [ "Latency Under Load", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md913", null ]
      ] ],
      [ "Memory Profiling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md915", [
        [ "Memory Footprint by Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md916", null ],
        [ "Memory Comparison with Competitors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md917", null ],
        [ "Memory Allocation Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md918", null ]
      ] ],
      [ "Benchmark Methodology", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md920", [
        [ "Test Scenarios", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md921", [
          [ "1. Single-Threaded Throughput Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md922", null ],
          [ "2. Multi-Threaded Contention Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md923", null ],
          [ "3. Latency Measurement Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md924", null ]
        ] ],
        [ "Measurement Tools", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md925", null ],
        [ "Benchmark Caveats", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md926", null ]
      ] ],
      [ "Performance Regression Baselines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md928", null ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md930", null ]
    ] ],
    [ "변경 이력 - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html", [
      [ "[3.0.0] - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md933", [
        [ "제거됨 - Deprecated API 정리 (Issues #268, #324)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md934", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md935", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md936", null ]
        ] ]
      ] ],
      [ "[Unreleased]", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md938", [
        [ "실시간 로그 분석 통합 (Issue #281)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md939", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md940", null ],
          [ "예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md941", null ]
        ] ],
        [ "고용량 시나리오를 위한 로그 샘플링 (Issue #282)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md943", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md944", null ],
          [ "예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md945", null ]
        ] ],
        [ "Phase 3.4: 구조화된 로깅 통합 (Issue #311) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md947", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md948", null ],
          [ "예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md949", null ]
        ] ],
        [ "Coverage 빌드 수정 (PR #291) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md951", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md952", null ]
        ] ],
        [ "CI 워크플로우 수정 (PR #290) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md954", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md955", null ]
        ] ],
        [ "OpenTelemetry 통합 수정 (Issue #283) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md957", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md958", null ]
        ] ],
        [ "C++20 모듈 파일 (Issue #275) - 2026-01-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md960", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md961", null ],
          [ "모듈 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md962", null ],
          [ "CMake 사용법", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md963", null ]
        ] ],
        [ "중복 Writer 추가 로직 수정 (Issue #270) - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md965", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md966", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md967", null ],
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md968", null ]
        ] ],
        [ "KCENON 기능 감지 사용 (Issue #250) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md970", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md971", null ],
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md972", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md973", null ]
        ] ],
        [ "Deprecated common_system API 마이그레이션 (Issue #248) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md975", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md976", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md977", null ]
        ] ],
        [ "thread_system v3.0 호환성 (Issue #244) - 2025-12-19", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md979", [
          [ "주요 변경 (Breaking Changes)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md980", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md981", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md982", null ]
        ] ],
        [ "#226 API 변경 후 손상된 예제 파일 수정 (Issue #228) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md984", [
          [ "제거됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md985", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md986", null ],
          [ "비활성화됨 (기능 구현 대기)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md987", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md988", null ]
        ] ],
        [ "common::interfaces::ILogger 구현 - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md990", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md991", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md992", null ],
          [ "사용 중단됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md993", null ],
          [ "하위 호환성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md994", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md995", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md996", null ]
        ] ],
        [ "독립형 비동기 구현 (Issue #222) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md998", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md999", null ],
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1000", null ],
          [ "이점", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1001", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1002", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1003", null ]
        ] ],
        [ "CMake fmt Fallback 제거 - 2025-12-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1005", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1006", null ],
          [ "제거됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1007", null ],
          [ "이점", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1008", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1009", null ]
        ] ]
      ] ],
      [ "[1.0.0] - 2025-10-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1011", [
        [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1012", null ],
        [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1013", null ],
        [ "사용 중단됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1014", null ],
        [ "제거됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1015", null ],
        [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1016", null ],
        [ "보안", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1017", null ]
      ] ],
      [ "프로젝트 정보", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1019", null ],
      [ "버전 지원 매트릭스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1021", null ],
      [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1023", null ]
    ] ],
    [ "Changelog - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html", [
      [ "[4.1.0] - Unreleased", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1027", [
        [ "Added - Writer Builder for Fluent Decorator Composition (Issue #413, #415, #416)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1028", [
          [ "New Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1029", null ],
          [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1030", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1031", null ],
          [ "Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1032", null ]
        ] ],
        [ "Deprecated - Legacy Writer Patterns (Issue #418)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1033", [
          [ "Deprecated Classes and Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1034", null ],
          [ "Deprecation Timeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1035", null ],
          [ "Migration Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1036", null ]
        ] ]
      ] ],
      [ "[4.0.0] - Unreleased", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1038", [
        [ "Removed - Deprecated Context ID Convenience Methods (Issue #326)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1039", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1040", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1041", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1042", null ]
        ] ],
        [ "Added - Decorator Pattern Writer Architecture (Issue #356)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1044", [
          [ "New Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1045", null ],
          [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1046", null ],
          [ "Architecture Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1047", null ],
          [ "Custom Writer Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1048", null ]
        ] ]
      ] ],
      [ "[3.0.0] - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1050", [
        [ "Removed - Deprecated log_level Types and Converters (Issue #339)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1051", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1052", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1053", null ]
        ] ],
        [ "Removed - Deprecated API Cleanup (Issues #268, #324)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1055", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1056", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1057", null ]
        ] ]
      ] ],
      [ "[Unreleased]", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1059", [
        [ "Real-time Log Analysis Integration (Issue #281)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1060", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1061", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1062", null ]
        ] ],
        [ "Log Sampling for High-Volume Scenarios (Issue #282)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1064", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1065", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1066", null ]
        ] ],
        [ "Phase 3.4: Structured Logging Integration (Issue #311) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1068", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1069", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1070", null ]
        ] ],
        [ "Phase 3.3: Structured Logging Formatters (Issue #310) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1072", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1073", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1074", null ]
        ] ],
        [ "Phase 3.1: Structured Logging API Design (Issue #308) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1076", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1077", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1078", null ]
        ] ],
        [ "Phase 3.2: Structured Logging Core Implementation (Issue #309) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1080", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1081", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1082", null ]
        ] ],
        [ "Coverage Build Fix (PR #291) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1084", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1085", null ]
        ] ],
        [ "CI Workflow Fix (PR #290) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1087", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1088", null ]
        ] ],
        [ "OpenTelemetry Integration Fix (Issue #283) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1090", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1091", null ]
        ] ],
        [ "C++20 Module Files (Issue #275) - 2026-01-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1093", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1094", null ],
          [ "Module Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1095", null ],
          [ "CMake Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1096", null ]
        ] ],
        [ "Fix Duplicate Writer Add Logic (Issue #270) - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1098", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1099", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1100", null ],
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1101", null ]
        ] ],
        [ "Use KCENON Feature Detection (Issue #250) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1103", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1104", null ],
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1105", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1106", null ]
        ] ],
        [ "Migrate from Deprecated common_system APIs (Issue #248) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1108", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1109", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1110", null ]
        ] ],
        [ "thread_system v3.0 Compatibility (Issue #244) - 2025-12-19", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1112", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1113", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1114", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1115", null ]
        ] ],
        [ "Fix broken example files after #226 API changes (Issue #228) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1117", [
          [ "Removed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1118", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1119", null ],
          [ "Disabled (awaiting feature implementation)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1120", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1121", null ]
        ] ],
        [ "common::interfaces::ILogger Implementation - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1123", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1124", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1125", null ],
          [ "Deprecated", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1126", null ],
          [ "Backward Compatibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1127", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1128", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1129", null ]
        ] ],
        [ "Standalone async implementation (Issue #222) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1131", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1132", null ],
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1133", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1134", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1135", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1136", null ]
        ] ],
        [ "CMake fmt Fallback Removal - 2025-12-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1138", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1139", null ],
          [ "Removed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1140", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1141", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1142", null ]
        ] ],
        [ "thread_system Required Dependency - 2025-12-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1144", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1145", null ]
        ] ]
      ] ]
    ] ],
    [ "CONTRIBUTING.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING_8kr.html", [
      [ "Logger System 기여하기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING_8kr.html#autotoc_md1146", null ]
    ] ],
    [ "CONTRIBUTING", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html", [
      [ "Contributing to Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1149", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1150", null ],
        [ "Code of Conduct", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1151", null ],
        [ "Getting Started", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1152", null ],
        [ "Development Process", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1153", [
          [ "1. Before You Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1154", null ],
          [ "2. Making Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1155", null ],
          [ "3. Commit Message Format", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1156", null ]
        ] ],
        [ "Code Style", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1157", [
          [ "C++ Guidelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1158", null ],
          [ "Code Formatting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1159", null ],
          [ "Naming Conventions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1160", null ]
        ] ],
        [ "Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1161", [
          [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1162", null ],
          [ "Running Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1163", null ],
          [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1164", null ]
        ] ],
        [ "Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1165", [
          [ "Code Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1166", null ],
          [ "Documentation Updates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1167", null ]
        ] ],
        [ "Submitting Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1168", [
          [ "Pull Request Process", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1169", null ],
          [ "Review Process", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1170", null ]
        ] ],
        [ "Questions?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1171", null ],
        [ "Thank you for contributing!", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1172", null ]
      ] ]
    ] ],
    [ "Test Coverage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TESTING__GUIDE.html", null ],
    [ "Korean Translation Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html", [
      [ "Completion Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1175", null ],
      [ "Statistics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1176", null ],
      [ "Files Created", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1177", [
        [ "Root Directory (19 files)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1178", null ],
        [ "advanced/ Directory (2 files)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1179", null ],
        [ "guides/ Directory (4 files)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1180", null ]
      ] ],
      [ "Language Switcher Format", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1181", [
        [ "English Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1182", null ],
        [ "Korean Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1183", null ]
      ] ],
      [ "Translation Approach", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1184", null ],
      [ "File Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1185", null ],
      [ "Verification", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1186", null ],
      [ "Notes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1187", null ],
      [ "Next Steps (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1188", null ]
    ] ],
    [ "Logger System - 상세 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html", [
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1192", null ],
      [ "핵심 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1194", [
        [ "설계 원칙", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1195", null ],
        [ "주요 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1196", null ]
      ] ],
      [ "로그 레벨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1198", [
        [ "레벨 정의", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1199", null ],
        [ "레벨별 사용 지침", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1200", null ],
        [ "사용 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1201", null ]
      ] ],
      [ "로그 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1203", [
        [ "콘솔 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1204", null ],
        [ "파일 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1205", null ],
        [ "회전 파일 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1206", null ],
        [ "원격 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1207", null ],
        [ "커스텀 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1208", null ]
      ] ],
      [ "포맷팅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1210", [
        [ "기본 포맷", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1211", null ],
        [ "JSON 포맷", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1212", null ],
        [ "커스텀 포맷터", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1213", null ]
      ] ],
      [ "필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1215", [
        [ "레벨 필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1216", null ],
        [ "패턴 필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1217", null ],
        [ "조건부 필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1218", null ]
      ] ],
      [ "비동기 로깅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1220", [
        [ "비동기 모드 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1221", null ],
        [ "성능 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1222", null ]
      ] ],
      [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1224", [
        [ "벤치마크 결과", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1225", null ],
        [ "최적화 팁", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1226", null ]
      ] ],
      [ "통합 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1228", [
        [ "thread_system 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1229", null ],
        [ "monitoring_system 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1230", null ],
        [ "common_system 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1231", null ]
      ] ],
      [ "구성 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1233", [
        [ "프로덕션 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1234", null ],
        [ "개발 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1235", null ]
      ] ],
      [ "참고사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1237", [
        [ "스레드 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1238", null ],
        [ "주의사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1239", null ]
      ] ]
    ] ],
    [ "Logger System Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1244", null ],
      [ "Core Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1246", [
        [ "🎯 High-Performance Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1247", [
          [ "Asynchronous Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1248", null ],
          [ "Thread-Safe Operations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1249", null ],
          [ "Configuration Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1250", null ]
        ] ],
        [ "🔧 Advanced Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1251", [
          [ "Result-Based Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1252", null ],
          [ "Builder Pattern API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1253", null ],
          [ "Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1254", null ],
          [ "Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1255", null ]
        ] ]
      ] ],
      [ "Writer Types", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1257", [
        [ "Console Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1258", null ],
        [ "File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1259", null ],
        [ "Rotating File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1260", null ],
        [ "Network Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1261", null ],
        [ "Critical Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1262", null ],
        [ "Hybrid Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1263", null ]
      ] ],
      [ "Formatting Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1265", [
        [ "Plain Text Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1266", null ],
        [ "JSON Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1267", null ],
        [ "Logfmt Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1268", null ],
        [ "Custom Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1269", null ]
      ] ],
      [ "Filtering Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1271", [
        [ "Level-Based Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1272", null ],
        [ "Regex-Based Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1273", null ],
        [ "Function-Based Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1274", null ],
        [ "Composite Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1275", null ]
      ] ],
      [ "Rotation Policies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1277", [
        [ "Size-Based Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1278", null ],
        [ "Time-Based Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1279", null ],
        [ "Hybrid Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1280", null ],
        [ "Compression Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1281", null ]
      ] ],
      [ "Security Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1283", [
        [ "Secure Key Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1284", null ],
        [ "Path Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1285", null ],
        [ "Signal Handler Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1286", null ],
        [ "Security Audit Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1287", null ],
        [ "Sensitive Data Sanitization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1288", null ],
        [ "Access Control", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1289", null ],
        [ "Compliance Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1290", null ]
      ] ],
      [ "Advanced Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1292", [
        [ "Structured Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1293", null ],
        [ "Log Routing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1294", null ],
        [ "Performance Metrics Collection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1295", null ],
        [ "Real-time Log Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1296", null ],
        [ "Distributed Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1297", null ]
      ] ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1299", null ]
    ] ],
    [ "BEST_PRACTICES.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES_8kr.html", [
      [ "Logger System 모범 사례 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES_8kr.html#autotoc_md1300", null ]
    ] ],
    [ "BEST_PRACTICES", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html", [
      [ "Logger System Best Practices Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1303", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1304", null ],
        [ "Design Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1305", [
          [ "1. Single Responsibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1306", null ],
          [ "2. Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1307", null ],
          [ "3. Interface Segregation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1308", null ]
        ] ],
        [ "Configuration Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1309", [
          [ "Environment-Specific Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1310", null ],
          [ "Configuration Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1311", null ],
          [ "Dynamic Reconfiguration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1312", null ]
        ] ],
        [ "Performance Guidelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1313", [
          [ "1. Minimize Allocations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1314", null ],
          [ "2. Lazy Evaluation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1315", null ],
          [ "3. Batch Operations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1316", null ],
          [ "4. Async Logging Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1317", null ]
        ] ],
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1318", [
          [ "Graceful Degradation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1319", null ],
          [ "Error Recovery", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1320", null ],
          [ "Circuit Breaker Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1321", null ]
        ] ],
        [ "Security Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1322", [
          [ "1. Sanitize Sensitive Data", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1323", null ],
          [ "2. Access Control", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1324", null ],
          [ "3. Log Injection Prevention", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1325", null ]
        ] ],
        [ "Testing Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1326", [
          [ "1. Mock Logger for Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1327", null ],
          [ "2. Performance Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1328", null ],
          [ "3. Stress Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1329", null ]
        ] ],
        [ "Production Deployment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1330", [
          [ "1. Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1331", null ],
          [ "2. Log Rotation Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1332", null ],
          [ "3. Graceful Shutdown", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1333", null ]
        ] ],
        [ "Common Pitfalls", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1334", [
          [ "1. Memory Leaks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1335", null ],
          [ "2. Deadlocks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1336", null ],
          [ "3. Performance Bottlenecks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1337", null ]
        ] ],
        [ "Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1338", null ],
        [ "Remember: Good logging is invisible when everything works but invaluable when things go wrong.", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1339", null ]
      ] ]
    ] ],
    [ "Decorator Pattern Writer Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1341", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1343", [
        [ "Why Decorator Pattern?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1344", null ],
        [ "Key Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1345", null ]
      ] ],
      [ "New Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1347", [
        [ "Base Classes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1348", [
          [ "<tt>log_writer_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1349", null ],
          [ "<tt>decorator_writer_base</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1350", null ]
        ] ]
      ] ],
      [ "Built-in Decorators", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1352", [
        [ "<tt>filtered_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1353", null ],
        [ "<tt>buffered_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1354", null ],
        [ "<tt>formatted_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1355", null ]
      ] ],
      [ "Migrating Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1357", [
        [ "Before (v3.x): Monolithic Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1358", null ],
        [ "After (v4.0): Focused Leaf Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1359", null ],
        [ "Creating a Custom Decorator", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1360", null ]
      ] ],
      [ "Composing Decorators", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1362", [
        [ "Order Matters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1363", null ],
        [ "Recommended Order", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1364", null ],
        [ "Using <tt>writer_builder</tt> (Recommended)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1365", null ],
        [ "Using <tt>logger_builder</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1366", null ]
      ] ],
      [ "Deprecation Timeline and Legacy Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1368", [
        [ "Deprecated Writer Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1369", null ],
        [ "Timeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1370", null ],
        [ "Migration Paths", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1371", [
          [ "Scenario 1: Simple File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1372", null ],
          [ "Scenario 2: Rotating File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1373", null ],
          [ "Scenario 3: Async + Encrypted Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1374", null ],
          [ "Scenario 4: Buffered + Filtered Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1375", null ],
          [ "Scenario 5: Multiple Decorators (Production Setup)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1376", null ],
          [ "Scenario 6: Custom Writer Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1377", null ]
        ] ],
        [ "Support Policy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1378", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1380", [
        [ "1. Keep Leaf Writers Simple", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1381", null ],
        [ "2. Use Category Tags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1382", null ],
        [ "3. Handle Errors Gracefully", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1383", null ],
        [ "4. Ensure Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1384", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1386", [
        [ "Common Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1387", [
          [ "\"Writer not receiving entries\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1388", null ],
          [ "\"Entries not being formatted\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1389", null ],
          [ "\"Flush not working\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1390", null ]
        ] ],
        [ "Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1391", null ]
      ] ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1393", null ]
    ] ],
    [ "Logger System - Frequently Asked Questions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1397", null ],
      [ "General Questions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1399", [
        [ "1. What is the logger_system?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1400", null ],
        [ "2. What C++ standard is required?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1402", null ]
      ] ],
      [ "Logging Basics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1404", [
        [ "3. How do I create a logger?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1405", null ],
        [ "4. How do I log messages?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1407", null ]
      ] ],
      [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1409", [
        [ "5. What log levels are available?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1410", null ],
        [ "6. How do I set the log level?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1412", null ],
        [ "7. Can I change log level at runtime?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1414", null ]
      ] ],
      [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1416", [
        [ "8. What writers are available?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1417", null ],
        [ "9. How do I use multiple writers?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1419", null ],
        [ "10. How does log rotation work?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1421", null ]
      ] ],
      [ "Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1423", [
        [ "11. What is the performance?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1424", null ],
        [ "12. How do I enable asynchronous logging?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1426", null ],
        [ "13. How do I optimize performance?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1428", null ]
      ] ],
      [ "Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1430", [
        [ "14. How do I integrate with monitoring_system?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1431", null ],
        [ "15. How do I integrate with thread_system?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1433", null ]
      ] ],
      [ "Advanced Topics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1435", [
        [ "16. Is it thread-safe?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1436", null ],
        [ "17. How do I create custom log formats?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1438", null ],
        [ "18. How do I filter logs?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1440", null ],
        [ "19. How do I handle critical logs?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1442", null ],
        [ "20. Where can I find more information?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1444", null ]
      ] ]
    ] ],
    [ "GETTING_STARTED.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED_8kr.html", [
      [ "Logger System 시작하기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED_8kr.html#autotoc_md1446", null ]
    ] ],
    [ "GETTING_STARTED", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html", [
      [ "Getting Started with Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1449", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1450", null ],
        [ "Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1451", null ],
        [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1452", [
          [ "Using CMake FetchContent (Recommended)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1453", null ],
          [ "Building from Source", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1454", null ],
          [ "Using as Installed Package", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1455", null ]
        ] ],
        [ "Basic Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1456", [
          [ "Simple Console Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1457", null ],
          [ "Logging with Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1458", null ],
          [ "Structured JSON Output", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1459", null ],
          [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1460", null ],
          [ "Filtering by Level", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1461", null ]
        ] ],
        [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1462", [
          [ "Synchronous vs Asynchronous Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1463", null ],
          [ "Multiple Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1464", null ],
          [ "Windows Notes (Networking)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1465", null ],
          [ "Console Writer Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1466", null ]
        ] ],
        [ "Integration with Thread System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1467", [
          [ "Using Service Container", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1468", null ],
          [ "Direct Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1469", null ]
        ] ],
        [ "Next Steps", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1470", null ]
      ] ]
    ] ],
    [ "Logger System Integration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1474", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1475", null ],
      [ "System Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1476", [
        [ "Required Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1477", [
          [ "common_system (Header-only)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1478", null ]
        ] ],
        [ "Optional Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1479", [
          [ "thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1480", null ],
          [ "monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1481", null ]
        ] ]
      ] ],
      [ "Integration with common_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1482", [
        [ "ILogger Interface Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1483", null ],
        [ "Result<T> Pattern Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1484", null ],
        [ "Adapter Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1485", null ]
      ] ],
      [ "Integration with thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1486", [
        [ "Asynchronous Logging with IExecutor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1487", null ],
        [ "Cross-System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1488", null ]
      ] ],
      [ "Integration with monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1489", [
        [ "IMonitor Interface Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1490", null ],
        [ "Performance Metrics Collection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1491", null ],
        [ "Health Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1492", null ]
      ] ],
      [ "Build Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1493", [
        [ "CMake Integration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1494", [
          [ "Minimal Configuration (common_system only)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1495", null ],
          [ "Full Ecosystem Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1496", null ]
        ] ],
        [ "Build Configuration Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1497", null ],
        [ "vcpkg Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1498", null ]
      ] ],
      [ "Code Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1499", [
        [ "Example 1: Basic Integration with common_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1500", null ],
        [ "Example 2: Full Stack Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1501", null ],
        [ "Example 3: Error Handling with Result<T>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1502", null ]
      ] ],
      [ "Result Handling Cheatsheet", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1503", null ],
      [ "Error Code Registry", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1504", [
        [ "Allocated Error Code Range", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1505", null ],
        [ "Error Message Mapping", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1506", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1507", [
        [ "Build Fails with \"common_system not found\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1508", null ],
        [ "Linker Errors: Undefined References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1509", null ],
        [ "Runtime: Logger Not Outputting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1510", null ],
        [ "Integration: IMonitor Not Injecting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1511", null ],
        [ "Performance: High Latency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1512", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1513", [
        [ "1. Use Result<T> for Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1514", null ],
        [ "2. Prefer Interface-Based Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1515", null ],
        [ "3. Leverage DI for Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1516", null ],
        [ "4. Monitor Performance in Production", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1517", null ]
      ] ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1518", null ]
    ] ],
    [ "MIGRATION_GUIDE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html", [
      [ "로거 시스템 마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1519", [
        [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1520", null ],
        [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1521", null ],
        [ "CMake 구성 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1522", [
          [ "v2.x (이전)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1523", null ],
          [ "v3.0 (현재)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1524", null ],
          [ "CMake 플래그 변경 요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1525", null ],
          [ "타겟명 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1526", null ],
          [ "의존성 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1527", null ]
        ] ],
        [ "v2.x에서 v3.0으로 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1528", [
          [ "주요 변경사항 요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1529", null ],
          [ "1. 네임스페이스 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1530", null ],
          [ "2. 인터페이스 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1531", null ],
          [ "3. Result 타입 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1532", null ],
          [ "4. thread_system 의존성 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1533", null ],
          [ "5. 백엔드 선택 (v3.0 신규)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1534", null ],
          [ "6. C++20 source_location (v3.0 신규)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1535", null ]
        ] ],
        [ "마이그레이션 체크리스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1536", [
          [ "v2.x → v3.0 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1537", null ]
        ] ],
        [ "일반적인 문제와 해결책", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1538", [
          [ "문제 1: 네임스페이스를 찾을 수 없음", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1539", null ],
          [ "문제 2: 인터페이스 타입 불일치", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1540", null ],
          [ "문제 3: C++20 미활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1541", null ]
        ] ],
        [ "지원 및 리소스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1542", null ]
      ] ]
    ] ],
    [ "MIGRATION_GUIDE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html", [
      [ "Logger System Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1545", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1546", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1547", [
          [ "Breaking Changes Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1548", null ]
        ] ],
        [ "From v3.x to v4.0", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1550", [
          [ "What Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1551", null ],
          [ "Migration for Standard Users", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1552", null ],
          [ "Migration for Custom Writer Authors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1553", null ],
          [ "Context ID API Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1554", null ],
          [ "Unified Context API (New in v4.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1555", [
            [ "Why This Change?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1556", null ],
            [ "Migration Table", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1557", null ],
            [ "Context Categories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1558", null ],
            [ "Code Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1559", null ],
            [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1560", null ],
            [ "Migration Script", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1561", null ],
            [ "Deprecation Timeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1562", null ]
          ] ]
        ] ],
        [ "CMake Configuration Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1564", [
          [ "v2.x (Previous)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1565", null ],
          [ "v3.0 (Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1566", null ],
          [ "CMake Flag Changes Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1567", null ],
          [ "Target Name Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1568", null ],
          [ "Dependency Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1569", null ]
        ] ],
        [ "Version Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1570", [
          [ "From v2.x to v3.0", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1571", [
            [ "Summary of Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1572", null ],
            [ "1. Namespace Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1573", null ],
            [ "2. Interface Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1574", null ],
            [ "3. Dual API Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1575", null ],
            [ "4. Result Type Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1576", null ],
            [ "5. thread_system Dependency Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1577", null ],
            [ "6. Backend Selection (New in v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1578", null ],
            [ "7. C++20 Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1579", null ],
            [ "8. Configuration Strategies (Enhanced in v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1580", null ],
            [ "9. Monitoring Integration (New in v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1581", null ],
            [ "Migration Script", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1582", null ]
          ] ],
          [ "From v1.x to v2.x", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1584", [
            [ "1. Error Handling Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1585", null ],
            [ "2. Memory Management Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1586", null ],
            [ "3. Configuration Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1587", null ],
            [ "4. Logging API Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1588", null ]
          ] ]
        ] ],
        [ "API Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1589", [
          [ "v3.0 API Changes Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1590", null ],
          [ "Deprecated Native log_level API (Planned for Removal in v3.0.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1591", null ],
          [ "Core Logger API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1592", null ]
        ] ],
        [ "Configuration Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1593", [
          [ "From INI/XML Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1594", null ],
          [ "Environment-based Configuration (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1595", null ]
        ] ],
        [ "Migration from Other Libraries", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1596", [
          [ "From spdlog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1597", null ],
          [ "From Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1598", null ],
          [ "From Google glog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1599", null ]
        ] ],
        [ "Compatibility Wrappers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1600", [
          [ "v2.x to v3.0 Compatibility Header", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1601", null ],
          [ "Legacy API Wrapper (v1.x compatibility)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1602", null ]
        ] ],
        [ "Step-by-Step Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1603", [
          [ "Phase 1: Preparation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1604", null ],
          [ "Phase 2: Namespace Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1605", null ],
          [ "Phase 3: Interface Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1606", null ],
          [ "Phase 4: Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1607", null ]
        ] ],
        [ "Common Issues and Solutions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1608", [
          [ "Issue 1: Namespace Not Found", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1609", null ],
          [ "Issue 2: Interface Type Mismatch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1610", null ],
          [ "Issue 3: Result Type Mismatch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1611", null ],
          [ "Issue 4: Missing thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1612", null ],
          [ "Issue 5: C++20 Not Enabled", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1613", null ]
        ] ],
        [ "Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1614", [
          [ "v2.x to v3.0 Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1615", null ],
          [ "v1.x to v2.x Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1616", null ]
        ] ],
        [ "Support and Resources", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1617", null ]
      ] ]
    ] ],
    [ "OpenTelemetry Integration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1620", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1621", null ],
      [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1622", [
        [ "With vcpkg", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1623", null ],
        [ "With CMake", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1624", null ],
        [ "Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1625", null ]
      ] ],
      [ "Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1626", [
        [ "Basic OTLP Export", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1627", null ],
        [ "With Trace Correlation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1628", null ],
        [ "Using RAII Scope Guard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1629", null ]
      ] ],
      [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1630", [
        [ "OTLP Writer Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1631", null ],
        [ "Protocol Selection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1632", null ]
      ] ],
      [ "Trace Context", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1633", [
        [ "otel_context Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1634", null ],
        [ "Thread-Local Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1635", null ],
        [ "Context in Log Entries", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1636", null ]
      ] ],
      [ "OTLP Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1637", [
        [ "Statistics Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1638", null ],
        [ "Health Checking", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1639", null ],
        [ "Force Flush", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1640", null ]
      ] ],
      [ "Integration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1641", [
        [ "With OpenTelemetry Collector", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1642", null ],
        [ "Docker Compose Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1643", null ],
        [ "W3C Trace Context Parsing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1644", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1645", [
        [ "1. Always Set Service Name", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1646", null ],
        [ "2. Use RAII Scope Guards", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1647", null ],
        [ "3. Configure Appropriate Batch Sizes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1648", null ],
        [ "4. Handle Collector Unavailability", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1649", null ],
        [ "5. Include Meaningful Resource Attributes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1650", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1651", [
        [ "Logs Not Appearing in Collector", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1652", null ],
        [ "High Log Drop Rate", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1653", null ],
        [ "Connection Timeouts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1654", null ],
        [ "Missing Trace Context", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1655", null ]
      ] ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1656", null ]
    ] ],
    [ "PERFORMANCE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE_8kr.html", [
      [ "Logger System 성능 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE_8kr.html#autotoc_md1657", null ]
    ] ],
    [ "PERFORMANCE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html", [
      [ "Logger System Performance Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1660", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1661", null ],
        [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1662", [
          [ "Synchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1663", null ],
          [ "Asynchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1664", null ]
        ] ],
        [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1665", [
          [ "Test Environment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1666", null ],
          [ "Single Thread Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1667", null ],
          [ "Multi-threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1668", null ],
          [ "Memory Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1669", null ]
        ] ],
        [ "Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1670", [
          [ "1. Choose the Right Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1671", null ],
          [ "2. Buffer Size Tuning", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1672", null ],
          [ "2.1 Batch Size Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1673", null ],
          [ "3. Level Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1674", null ],
          [ "4. Message Construction", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1675", null ],
          [ "5. Writer Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1676", [
            [ "Console Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1677", null ],
            [ "Custom High-Performance Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1678", null ]
          ] ],
          [ "6. Batch Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1679", null ]
        ] ],
        [ "Performance Anti-patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1680", [
          [ "1. Synchronous I/O in Hot Paths", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1681", null ],
          [ "2. Excessive String Formatting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1682", null ],
          [ "3. Logging in Tight Loops", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1683", null ]
        ] ],
        [ "Profiling and Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1684", [
          [ "Built-in Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1685", null ],
          [ "External Profiling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1686", null ]
        ] ],
        [ "Best Practices Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1687", null ],
        [ "Platform-Specific Optimizations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1688", [
          [ "Linux", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1689", null ],
          [ "Windows", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1690", null ],
          [ "macOS", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1691", null ]
        ] ]
      ] ]
    ] ],
    [ "빠른 시작 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html", [
      [ "사전 요구사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1695", null ],
      [ "설치", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1696", [
        [ "1. 저장소 클론", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1697", null ],
        [ "2. 의존성 설치", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1698", null ],
        [ "3. 빌드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1699", null ],
        [ "4. 설치 확인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1700", null ]
      ] ],
      [ "첫 번째 로거 만들기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1702", [
        [ "애플리케이션 빌드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1703", null ]
      ] ],
      [ "핵심 개념", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1705", [
        [ "로거 빌더", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1706", null ],
        [ "라이터", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1707", null ],
        [ "로그 레벨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1708", null ]
      ] ],
      [ "구성 템플릿", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1710", [
        [ "Production (프로덕션)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1711", null ],
        [ "Debug (디버그)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1712", null ],
        [ "High Performance (고성능)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1713", null ]
      ] ],
      [ "CMake 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1715", [
        [ "핵심 빌드 옵션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1716", null ],
        [ "기능 플래그", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1717", null ]
      ] ],
      [ "thread_system 통합 (선택 사항)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1719", [
        [ "빌드 시 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1720", null ],
        [ "런타임 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1721", null ]
      ] ],
      [ "일반적인 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1723", [
        [ "오류 처리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1724", null ],
        [ "여러 라이터 사용", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1725", null ],
        [ "정상 종료", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1726", null ]
      ] ],
      [ "다음 단계", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1728", null ],
      [ "문제 해결", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1730", [
        [ "일반적인 문제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1731", null ]
      ] ]
    ] ],
    [ "Quick Start Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html", [
      [ "Prerequisites", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1735", null ],
      [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1736", [
        [ "1. Clone the Repositories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1737", null ],
        [ "2. Install Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1738", null ],
        [ "3. Build", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1739", null ],
        [ "4. Verify Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1740", null ]
      ] ],
      [ "Your First Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1742", [
        [ "Building Your Application", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1743", null ]
      ] ],
      [ "Key Concepts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1745", [
        [ "Logger Builder", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1746", null ],
        [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1747", null ],
        [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1748", null ]
      ] ],
      [ "Configuration Templates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1750", [
        [ "Production", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1751", null ],
        [ "Debug", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1752", null ],
        [ "High Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1753", null ]
      ] ],
      [ "CMake Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1755", [
        [ "Core Build Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1756", null ],
        [ "Feature Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1757", null ]
      ] ],
      [ "thread_system Integration (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1759", [
        [ "Enable at Build Time", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1760", null ],
        [ "Enable at Runtime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1761", null ]
      ] ],
      [ "Common Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1763", [
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1764", null ],
        [ "Multiple Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1765", null ],
        [ "Graceful Shutdown", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1766", null ]
      ] ],
      [ "Next Steps", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1768", null ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1770", [
        [ "Common Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1771", null ]
      ] ]
    ] ],
    [ "SECURITY.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html", [
      [ "보안 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1773", [
        [ "위협 모델 (범위)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1774", null ],
        [ "현재 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1775", null ],
        [ "중요한 제한사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1776", null ],
        [ "권장사항 (프로덕션)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1777", null ],
        [ "안전한 구성 체크리스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1778", null ],
        [ "로드맵", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1779", null ]
      ] ]
    ] ],
    [ "SECURITY", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html", [
      [ "Security Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1781", [
        [ "Threat Model (Scope)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1782", null ],
        [ "Current Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1783", null ],
        [ "Important Limitations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1784", null ],
        [ "Recommendations (Production)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1785", null ],
        [ "Secure Configuration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1786", null ],
        [ "Roadmap", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1787", null ]
      ] ]
    ] ],
    [ "thread_system 통합 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html", [
      [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1806", null ],
      [ "빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1807", [
        [ "빌드 시 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1808", null ],
        [ "런타임에 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1809", null ]
      ] ],
      [ "설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1810", [
        [ "CMake 옵션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1811", null ],
        [ "컴파일 정의", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1812", null ]
      ] ],
      [ "의존성 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1813", [
        [ "양방향 의존성 위험 (Issue #252)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1814", null ],
        [ "권장 구성 매트릭스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1815", null ],
        [ "안전한 구성 예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1816", null ],
        [ "CMake 충돌 감지", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1817", null ]
      ] ],
      [ "API 레퍼런스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1818", [
        [ "thread_system_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1819", [
          [ "메서드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1820", null ]
        ] ],
        [ "async_backend_type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1821", null ],
        [ "컴파일 타임 감지", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1822", null ]
      ] ],
      [ "사용 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1823", [
        [ "패턴 1: 애플리케이션 전체 공유 풀", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1824", null ],
        [ "패턴 2: 사용자 정의 풀 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1825", null ],
        [ "패턴 3: 조건부 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1826", null ]
      ] ],
      [ "스레드 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1827", null ],
      [ "성능 고려사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1828", [
        [ "thread_system 통합 사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1829", null ],
        [ "오버헤드 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1830", null ]
      ] ],
      [ "문제 해결", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1831", [
        [ "thread_system 미발견", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1832", null ],
        [ "런타임 감지 미작동", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1833", null ]
      ] ],
      [ "관련 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1834", null ]
    ] ],
    [ "Async Integration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1837", null ],
      [ "Integration Approaches", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1838", [
        [ "Recommended: IExecutor Interface (v1.5.0+)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1839", null ],
        [ "Legacy: Direct thread_system Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1840", null ]
      ] ],
      [ "Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1841", [
        [ "Enable at Build Time", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1842", null ],
        [ "Enable at Runtime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1843", null ]
      ] ],
      [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1844", [
        [ "CMake Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1845", null ],
        [ "Compile Definitions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1846", null ]
      ] ],
      [ "Dependency Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1847", [
        [ "Bidirectional Dependency Risk (Issue #252)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1848", null ],
        [ "Recommended Configuration Matrix", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1849", null ],
        [ "Safe Configuration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1850", null ],
        [ "CMake Conflict Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1851", null ]
      ] ],
      [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1852", [
        [ "thread_system_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1853", [
          [ "Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1854", null ]
        ] ],
        [ "async_backend_type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1855", null ],
        [ "Compile-time Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1856", null ]
      ] ],
      [ "Usage Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1857", [
        [ "Pattern 1: Application-wide Shared Pool", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1858", null ],
        [ "Pattern 2: Custom Pool Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1859", null ],
        [ "Pattern 3: Conditional Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1860", null ]
      ] ],
      [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1861", null ],
      [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1862", [
        [ "When to Use thread_system Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1863", null ],
        [ "Overhead Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1864", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1865", [
        [ "thread_system not found", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1866", null ],
        [ "Runtime detection not working", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1867", null ]
      ] ],
      [ "IExecutor Integration API (v1.5.0+)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1868", [
        [ "executor_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1869", null ],
        [ "executor_type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1870", null ],
        [ "standalone_executor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1871", null ],
        [ "Compile-time Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1872", null ]
      ] ],
      [ "Migration from thread_system_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1873", null ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1874", null ]
    ] ],
    [ "Logger System - 성능 기준 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html", [
      [ "시스템 정보", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1879", [
        [ "하드웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1880", null ],
        [ "소프트웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1881", null ]
      ] ],
      [ "성능 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1883", [
        [ "로깅 Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1884", null ],
        [ "Latency (Async Mode)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1885", null ],
        [ "메모리 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1886", null ]
      ] ],
      [ "벤치마크 결과", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1888", null ],
      [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1890", null ],
      [ "Baseline 검증", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1892", [
        [ "Phase 0 요구사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1893", null ],
        [ "수락 기준", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1894", null ]
      ] ]
    ] ],
    [ "Logger System - Performance Baseline Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html", [
      [ "System Information", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1899", [
        [ "Hardware Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1900", null ],
        [ "Software Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1901", null ]
      ] ],
      [ "Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1903", [
        [ "Logging Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1904", null ],
        [ "Latency (Async Mode)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1905", null ],
        [ "Memory Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1906", null ]
      ] ],
      [ "Benchmark Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1908", null ],
      [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1910", null ],
      [ "Baseline Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1912", [
        [ "Phase 0 Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1913", null ],
        [ "Acceptance Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1914", null ]
      ] ]
    ] ],
    [ "CI/CD 성능 지표 자동화 제안서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html", [
      [ "📊 Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1918", null ],
      [ "🔍 현재 상태 분석", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1920", [
        [ "✅ 장점", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1921", null ],
        [ "❌ 개선 필요 사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1922", null ]
      ] ],
      [ "🎯 제안 솔루션 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1924", [
        [ "전체 워크플로우", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1925", null ]
      ] ],
      [ "📁 제안 디렉토리 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1927", null ],
      [ "🛠️ 구현 상세 (4단계)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1929", [
        [ "Phase 1: 벤치마크 재활성화 및 JSON 출력 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1930", [
          [ "1.1 GitHub Actions 워크플로우 수정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1931", null ],
          [ "1.2 벤치마크 실행 및 JSON 저장", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1932", null ],
          [ "1.3 API 변경에 따른 벤치마크 코드 수정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1933", null ]
        ] ],
        [ "Phase 2: 결과 파싱 및 기준선 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1935", [
          [ "2.1 벤치마크 결과 파싱 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1936", null ],
          [ "2.2 기준선 비교 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1937", null ]
        ] ],
        [ "Phase 3: 배지 및 차트 생성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1939", [
          [ "3.1 성능 배지 생성 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1940", null ],
          [ "3.2 트렌드 차트 생성 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1941", null ]
        ] ],
        [ "Phase 4: README.md 자동 업데이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1943", [
          [ "4.1 README 업데이트 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1944", null ],
          [ "4.2 GitHub Actions에서 자동 커밋", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1945", null ]
        ] ]
      ] ],
      [ "🎨 README.md 성능 섹션 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1947", [
        [ "제안 1: 배지 중심 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1948", null ],
        [ "제안 2: 테이블 중심 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1949", null ],
        [ "제안 3: 카드 스타일 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1950", null ]
      ] ],
      [ "🔄 CI/CD 워크플로우 전체 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1952", null ],
      [ "📈 예상 결과물", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1954", [
        [ "1. GitHub Actions 실행 결과", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1955", null ],
        [ "2. PR 자동 코멘트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1956", null ],
        [ "3. 업데이트된 README.md", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1957", null ]
      ] ],
      [ "✅ 구현 체크리스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1959", [
        [ "Phase 1: 인프라 구축 (예상 소요: 2시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1960", null ],
        [ "Phase 2: 배지 및 차트 (예상 소요: 1.5시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1961", null ],
        [ "Phase 3: README 자동 업데이트 (예상 소요: 1시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1962", null ],
        [ "Phase 4: CI/CD 통합 (예상 소요: 2시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1963", null ],
        [ "Phase 5: 테스트 및 검증 (예상 소요: 1.5시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1964", null ],
        [ "Phase 6: 문서화 (예상 소요: 1시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1965", null ]
      ] ],
      [ "🔧 트러블슈팅 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1967", [
        [ "문제 1: 벤치마크 빌드 실패", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1968", null ],
        [ "문제 2: Python 스크립트 실행 오류", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1969", null ],
        [ "문제 3: Git 자동 커밋 권한 오류", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1970", null ],
        [ "문제 4: 배지가 README에 표시되지 않음", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1971", null ]
      ] ],
      [ "📚 참고 자료", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1973", [
        [ "도구 및 라이브러리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1974", null ],
        [ "관련 프로젝트 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1975", null ],
        [ "내부 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1976", null ]
      ] ],
      [ "🎯 성공 지표", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1978", null ],
      [ "📝 다음 단계", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1980", null ]
    ] ],
    [ "Logger System 프로덕션 품질", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html", [
      [ "요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1984", [
        [ "개발 상태: ✅ <strong>개발 중</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1985", null ]
      ] ],
      [ "빌드 & 테스팅 인프라", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1987", [
        [ "지속적 통합 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1988", [
          [ "1. 메인 CI 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1989", null ],
          [ "2. 새니타이저 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1990", null ],
          [ "3. 성능 벤치마크 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1991", null ],
          [ "4. 코드 커버리지 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1992", null ],
          [ "5. 정적 분석 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1993", null ]
        ] ]
      ] ],
      [ "스레드 안전성 & 동시성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1995", [
        [ "스레드 안전성 등급: <strong>A+</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1996", null ],
        [ "Writer 스레드 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1997", null ],
        [ "비동기 처리 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1998", null ]
      ] ],
      [ "리소스 관리 (RAII)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2000", [
        [ "RAII 준수 등급: <strong>A</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2001", null ],
        [ "스마트 포인터 사용", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2002", null ],
        [ "메모리 누수 탐지", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2003", null ],
        [ "예외 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2004", null ]
      ] ],
      [ "오류 처리 전략", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2006", [
        [ "오류 처리 등급: <strong>A</strong> (90% 완료)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2007", null ],
        [ "Result<T> 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2008", null ],
        [ "오류 코드 레지스트리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2009", null ]
      ] ],
      [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2011", [
        [ "기준선 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2012", null ]
      ] ],
      [ "보안 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2014", [
        [ "보안 등급: <strong>A</strong> (v3.0.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2015", null ],
        [ "1. 보안 키 저장소", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2016", null ],
        [ "2. 경로 검증", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2017", null ],
        [ "3. 시그널 핸들러 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2018", null ],
        [ "4. 보안 감사 로깅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2019", null ],
        [ "5. 컴플라이언스 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2020", null ]
      ] ],
      [ "플랫폼 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2022", [
        [ "공식 지원 플랫폼", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2023", null ],
        [ "최소 요구사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md2024", null ]
      ] ]
    ] ],
    [ "Logger System Production Quality", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2030", null ],
      [ "Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2032", [
        [ "Quality Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2033", null ]
      ] ],
      [ "Build & Testing Infrastructure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2035", [
        [ "Continuous Integration Pipelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2036", [
          [ "1. Main CI Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2037", null ],
          [ "2. Sanitizer Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2039", null ],
          [ "3. Performance Benchmark Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2041", null ],
          [ "4. Code Coverage Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2043", null ],
          [ "5. Static Analysis Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2045", null ]
        ] ],
        [ "Test Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2047", [
          [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2048", null ],
          [ "Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2050", null ],
          [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2052", null ]
        ] ]
      ] ],
      [ "Thread Safety & Concurrency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2054", [
        [ "Thread Safety Grade: <strong>A+</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2055", null ],
        [ "Writer Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2056", null ],
        [ "Asynchronous Processing Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2058", null ],
        [ "Concurrency Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2060", null ]
      ] ],
      [ "Resource Management (RAII)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2062", [
        [ "RAII Compliance Grade: <strong>A</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2063", null ],
        [ "Smart Pointer Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2064", null ],
        [ "Memory Leak Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2066", null ],
        [ "Exception Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2068", null ]
      ] ],
      [ "Error Handling Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2070", [
        [ "Error Handling Grade: <strong>A</strong> (90% Complete)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2071", null ],
        [ "Result<T> Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2072", null ],
        [ "Error Code Registry", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2074", null ],
        [ "Hot Path Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2076", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2078", [
        [ "Baseline Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2079", null ],
        [ "Performance Baselines (CI/CD)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2081", null ]
      ] ],
      [ "Security Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2083", [
        [ "Security Grade: <strong>A</strong> (v3.0.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2084", null ],
        [ "1. Secure Key Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2085", null ],
        [ "2. Path Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2086", null ],
        [ "3. Signal Handler Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2087", null ],
        [ "4. Security Audit Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2088", null ],
        [ "5. Compliance Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2089", null ],
        [ "6. OWASP Top 10 Mitigations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2090", null ]
      ] ],
      [ "Operational Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2092", [
        [ "Built-in Metrics Collection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2093", null ],
        [ "Health Checks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2095", null ]
      ] ],
      [ "Platform Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2097", [
        [ "Officially Supported Platforms", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2098", null ],
        [ "Minimum Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2099", null ],
        [ "Cross-Platform Compatibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2101", null ]
      ] ],
      [ "Remaining Work & Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2103", [
        [ "Current Status: <strong>90% Production Ready</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2104", null ]
      ] ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2106", null ]
    ] ],
    [ "Logger System 프로젝트 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html", [
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2109", null ],
      [ "디렉토리 개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2111", null ],
      [ "코어 모듈 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2113", [
        [ "logger.h / logger.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2114", null ],
        [ "logger_builder.h / logger_builder.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2116", null ],
        [ "log_entry.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2118", null ],
        [ "result_types.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2120", null ]
      ] ],
      [ "Writer 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2122", [
        [ "console_writer.h / console_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2123", null ],
        [ "file_writer.h / file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2125", null ],
        [ "rotating_file_writer.h / rotating_file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2127", null ],
        [ "network_writer.h / network_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2129", null ],
        [ "critical_writer.h / critical_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2131", null ],
        [ "hybrid_writer.h / hybrid_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2133", null ]
      ] ],
      [ "Filter 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2135", [
        [ "level_filter.h / level_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2136", null ],
        [ "regex_filter.h / regex_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2138", null ],
        [ "function_filter.h / function_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2140", null ]
      ] ],
      [ "Formatter 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2142", [
        [ "plain_formatter.h / plain_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2143", null ],
        [ "json_formatter.h / json_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2145", null ],
        [ "custom_formatter.h / custom_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2147", null ]
      ] ],
      [ "설정 시스템", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2149", [
        [ "config_templates.h / config_templates.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2150", null ],
        [ "config_validator.h / config_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2152", null ]
      ] ],
      [ "보안 컴포넌트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2154", [
        [ "secure_key_storage.h / secure_key_storage.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2155", null ],
        [ "path_validator.h / path_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2157", null ],
        [ "log_sanitizer.h / log_sanitizer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2159", null ]
      ] ],
      [ "테스팅 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2161", [
        [ "유닛 테스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2162", null ],
        [ "통합 테스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2164", null ],
        [ "벤치마크", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2166", null ]
      ] ],
      [ "빌드 시스템", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2168", [
        [ "CMakeLists.txt (루트)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2169", null ]
      ] ],
      [ "모듈 의존성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2171", [
        [ "의존성 그래프", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2172", null ],
        [ "외부 의존성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2173", null ]
      ] ],
      [ "파일 명명 규칙", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2175", [
        [ "헤더 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2176", null ],
        [ "소스 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2177", null ],
        [ "문서 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2178", null ]
      ] ],
      [ "참고 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2180", null ]
    ] ],
    [ "Logger System Project Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2185", null ],
      [ "Directory Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2187", null ],
      [ "Core Module Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2189", [
        [ "logger.h / logger.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2190", null ],
        [ "logger_builder.h / logger_builder.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2192", null ],
        [ "log_entry.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2194", null ],
        [ "result_types.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2196", null ]
      ] ],
      [ "Writer Implementations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2198", [
        [ "console_writer.h / console_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2199", null ],
        [ "file_writer.h / file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2201", null ],
        [ "rotating_file_writer.h / rotating_file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2203", null ],
        [ "network_writer.h / network_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2205", null ],
        [ "critical_writer.h / critical_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2207", null ],
        [ "hybrid_writer.h / hybrid_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2209", null ]
      ] ],
      [ "Filter Implementations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2211", [
        [ "level_filter.h / level_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2212", null ],
        [ "regex_filter.h / regex_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2214", null ],
        [ "function_filter.h / function_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2216", null ]
      ] ],
      [ "Formatter Implementations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2218", [
        [ "plain_formatter.h / plain_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2219", null ],
        [ "json_formatter.h / json_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2221", null ],
        [ "custom_formatter.h / custom_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2223", null ]
      ] ],
      [ "Configuration System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2225", [
        [ "config_templates.h / config_templates.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2226", null ],
        [ "config_validator.h / config_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2228", null ],
        [ "config_strategy.h / config_strategy.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2230", null ]
      ] ],
      [ "Security Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2232", [
        [ "secure_key_storage.h / secure_key_storage.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2233", null ],
        [ "path_validator.h / path_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2235", null ],
        [ "log_sanitizer.h / log_sanitizer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2237", null ]
      ] ],
      [ "Testing Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2239", [
        [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2240", null ],
        [ "Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2242", null ],
        [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2244", null ]
      ] ],
      [ "Build System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2246", [
        [ "CMakeLists.txt (Root)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2247", null ],
        [ "CMake Modules", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2249", null ]
      ] ],
      [ "Module Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2251", [
        [ "Dependency Graph", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2252", null ],
        [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2253", null ]
      ] ],
      [ "File Naming Conventions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2255", [
        [ "Header Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2256", null ],
        [ "Source Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2257", null ],
        [ "Documentation Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2258", null ]
      ] ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2260", null ]
    ] ],
    [ "README.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html", [
      [ "Logger System 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2261", [
        [ "📚 문서 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2262", [
          [ "핵심 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2263", null ],
          [ "가이드 (<tt>docs/guides/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2264", null ],
          [ "고급 주제 (<tt>docs/advanced/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2265", null ]
        ] ],
        [ "🚀 빠른 탐색", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2266", [
          [ "신규 사용자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2267", null ],
          [ "기존 사용자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2268", null ],
          [ "기여자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2269", null ],
          [ "성능 엔지니어", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2270", null ]
        ] ],
        [ "🔧 문서 유지관리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2271", [
          [ "최근 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2272", null ]
        ] ],
        [ "📖 외부 리소스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2273", null ],
        [ "🤝 도움 및 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2274", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"index.html",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md868",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1221",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md87",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md420",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1159",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1579",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1874"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';