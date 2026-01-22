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
        [ "Installation", "index.html#autotoc_md5", null ],
        [ "Requirements", "index.html#autotoc_md6", [
          [ "Dependency Flow", "index.html#autotoc_md7", null ],
          [ "Building with Dependencies", "index.html#autotoc_md8", null ]
        ] ]
      ] ],
      [ "Core Features", "index.html#autotoc_md10", [
        [ "Asynchronous Logging", "index.html#autotoc_md11", null ],
        [ "Multiple Writer Types", "index.html#autotoc_md12", null ],
        [ "OpenTelemetry Integration (v3.0.0)", "index.html#autotoc_md13", null ],
        [ "Security Features (v3.0.0)", "index.html#autotoc_md14", null ],
        [ "Structured Logging (v3.1.0)", "index.html#autotoc_md15", null ]
      ] ],
      [ "Performance Highlights", "index.html#autotoc_md17", [
        [ "Throughput", "index.html#autotoc_md18", null ],
        [ "Latency", "index.html#autotoc_md19", null ],
        [ "Memory Efficiency", "index.html#autotoc_md20", null ]
      ] ],
      [ "Architecture Overview", "index.html#autotoc_md22", [
        [ "Modular Design", "index.html#autotoc_md23", null ],
        [ "Key Components", "index.html#autotoc_md24", null ]
      ] ],
      [ "Ecosystem Integration", "index.html#autotoc_md26", [
        [ "Dependencies", "index.html#autotoc_md27", null ],
        [ "Integration Pattern", "index.html#autotoc_md28", null ]
      ] ],
      [ "Documentation", "index.html#autotoc_md30", [
        [ "Getting Started", "index.html#autotoc_md31", null ],
        [ "Core Documentation", "index.html#autotoc_md32", null ],
        [ "Advanced Topics", "index.html#autotoc_md33", null ],
        [ "Development", "index.html#autotoc_md34", null ]
      ] ],
      [ "Configuration Templates", "index.html#autotoc_md36", [
        [ "Advanced Configuration", "index.html#autotoc_md37", null ]
      ] ],
      [ "Build Configuration", "index.html#autotoc_md39", [
        [ "CMake Feature Flags", "index.html#autotoc_md40", null ]
      ] ],
      [ "Platform Support", "index.html#autotoc_md42", [
        [ "Officially Supported", "index.html#autotoc_md43", null ]
      ] ],
      [ "Testing", "index.html#autotoc_md45", [
        [ "Test Coverage", "index.html#autotoc_md46", null ],
        [ "Running Tests", "index.html#autotoc_md47", null ],
        [ "CI/CD Status", "index.html#autotoc_md48", null ]
      ] ],
      [ "Contributing", "index.html#autotoc_md50", [
        [ "Development Workflow", "index.html#autotoc_md51", null ],
        [ "Code Standards", "index.html#autotoc_md52", null ]
      ] ],
      [ "Support", "index.html#autotoc_md54", null ],
      [ "License", "index.html#autotoc_md56", null ],
      [ "Acknowledgments", "index.html#autotoc_md58", null ]
    ] ],
    [ "Architecture - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md61", null ],
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md63", null ],
      [ "Design Philosophy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md65", [
        [ "Core Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md66", null ]
      ] ],
      [ "Layered Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md68", [
        [ "Layer Responsibilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md69", null ]
      ] ],
      [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md71", [
        [ "1. Logger (<tt>core/logger.h</tt>, <tt>core/logger.cpp</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md72", null ],
        [ "2. Logger Builder (<tt>core/logger_builder.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md74", null ],
        [ "3. Log Collector (<tt>core/log_collector.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md76", null ],
        [ "4. Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md78", [
          [ "Console Writer (<tt>writers/console_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md79", null ],
          [ "File Writer (<tt>writers/file_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md80", null ],
          [ "Rotating File Writer (<tt>writers/rotating_file_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md81", null ],
          [ "Network Writer (<tt>writers/network_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md82", null ],
          [ "Critical Writer (<tt>writers/critical_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md83", null ],
          [ "Async Writer (<tt>writers/async_writer.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md84", null ]
        ] ],
        [ "5. Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md86", [
          [ "Base Formatter (<tt>formatters/base_formatter.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md87", null ],
          [ "JSON Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md88", null ],
          [ "XML Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md89", null ]
        ] ],
        [ "6. Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md91", [
          [ "Level Filter (<tt>filters/log_filter.h</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md92", null ],
          [ "Regex Filter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md93", null ],
          [ "Function Filter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md94", null ]
        ] ]
      ] ],
      [ "Integration Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md96", [
        [ "Common System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md97", null ],
        [ "Thread System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md98", null ],
        [ "Monitoring System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md99", null ]
      ] ],
      [ "Threading Model", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md101", [
        [ "Asynchronous Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md102", null ],
        [ "Thread Safety Guarantees", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md103", null ]
      ] ],
      [ "Memory Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md105", [
        [ "Small String Optimization (SSO)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md106", null ],
        [ "Log Entry Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md107", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md109", [
        [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md110", null ],
        [ "Optimization Techniques", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md111", null ]
      ] ],
      [ "Design Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md113", [
        [ "1. Observer Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md114", null ],
        [ "2. Strategy Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md115", null ],
        [ "3. Builder Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md116", null ],
        [ "4. Template Method Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md117", null ],
        [ "5. Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md118", null ],
        [ "6. Factory Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md119", null ]
      ] ],
      [ "Build Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md121", [
        [ "CMake Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md122", null ],
        [ "Dependency Resolution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md123", null ],
        [ "Compiler Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md124", null ]
      ] ],
      [ "Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md126", [
        [ "Planned Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md127", null ],
        [ "Research Areas", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md128", null ]
      ] ],
      [ "Error Code Allocation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md130", null ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ARCHITECTURE.html#autotoc_md132", null ]
    ] ],
    [ "비동기 Writer 구현체 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html", [
      [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md135", null ],
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md137", null ],
      [ "빠른 선택 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md139", [
        [ "선택 플로우차트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md140", null ]
      ] ],
      [ "구현체 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md142", null ],
      [ "상세 설명", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md144", [
        [ "1. async_writer (공개 API)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md145", [
          [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md146", null ],
          [ "아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md147", null ],
          [ "API 레퍼런스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md148", null ],
          [ "사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md149", null ]
        ] ],
        [ "2. async_worker (내부)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md151", [
          [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md152", null ],
          [ "아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md153", null ],
          [ "사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md154", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md155", [
        [ "3. high_performance_async_writer (내부)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md156", [
          [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md157", null ],
          [ "아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md158", null ],
          [ "설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md159", null ],
          [ "사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md160", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md161", null ],
      [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md162", [
        [ "벤치마크 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md163", null ],
        [ "지연 시간 분포", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md164", null ]
      ] ],
      [ "사용 예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md166", [
        [ "예제 1: 기본 async_writer 사용", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md167", null ],
        [ "예제 2: 콘솔 출력과 async_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md168", null ],
        [ "예제 3: 큐 가득 참 처리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md169", null ],
        [ "예제 4: 정상 종료 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md170", null ]
      ] ],
      [ "모범 사례", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md172", [
        [ "1. 큐 크기 선택", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md173", null ],
        [ "2. 플러시 타임아웃 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md174", null ],
        [ "3. 에러 처리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md175", null ],
        [ "4. 생명주기 관리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md176", null ],
        [ "5. 스레드 안전성 고려사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md177", null ]
      ] ],
      [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md179", [
        [ "동기에서 async_writer로", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md180", null ],
        [ "큐 가득 참 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md181", null ]
      ] ],
      [ "관련 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS_8kr.html#autotoc_md183", null ]
    ] ],
    [ "Asynchronous Writer Implementations Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md186", null ],
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md188", null ],
      [ "Quick Decision Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md190", [
        [ "Decision Flowchart", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md191", null ]
      ] ],
      [ "Implementation Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md193", null ],
      [ "Detailed Descriptions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md195", [
        [ "1. async_writer (Public API)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md196", [
          [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md197", null ],
          [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md198", null ],
          [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md199", null ],
          [ "When to Use", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md200", null ]
        ] ],
        [ "2. async_worker (Internal)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md202", [
          [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md203", null ],
          [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md204", null ],
          [ "When to Use", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md205", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md206", [
        [ "3. high_performance_async_writer (Internal)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md207", [
          [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md208", null ],
          [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md209", null ],
          [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md210", null ],
          [ "When to Use", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md211", null ]
        ] ]
      ] ],
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md212", null ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md213", [
        [ "Benchmark Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md214", null ],
        [ "Latency Distribution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md215", null ]
      ] ],
      [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md217", [
        [ "Example 1: Basic async_writer Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md218", null ],
        [ "Example 2: async_writer with Console Output", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md219", null ],
        [ "Example 3: Queue Full Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md220", null ],
        [ "Example 4: Graceful Shutdown Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md221", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md223", [
        [ "1. Queue Size Selection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md224", null ],
        [ "2. Flush Timeout Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md225", null ],
        [ "3. Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md226", null ],
        [ "4. Lifecycle Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md227", null ],
        [ "5. Thread Safety Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md228", null ]
      ] ],
      [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md230", [
        [ "From Synchronous to async_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md231", null ],
        [ "Queue Full Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md232", null ]
      ] ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html#autotoc_md234", null ]
    ] ],
    [ "CI_CD_DASHBOARD.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD_8kr.html", [
      [ "Logger System CI/CD 대시보드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD_8kr.html#autotoc_md236", null ]
    ] ],
    [ "CI_CD_DASHBOARD", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html", [
      [ "Logger System CI/CD Dashboard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md239", [
        [ "🚀 Build Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md240", [
          [ "Main Branch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md241", null ]
        ] ],
        [ "📊 Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md242", [
          [ "Latest Benchmark Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md243", null ],
          [ "Performance Trends", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md244", null ]
        ] ],
        [ "🔍 Code Quality", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md245", [
          [ "Static Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md246", null ],
          [ "Test Coverage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md247", null ]
        ] ],
        [ "🧪 Test Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md248", [
          [ "Test Execution Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md249", null ],
          [ "Recent Test Failures", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md250", null ]
        ] ],
        [ "🔥 Sanitizer Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md251", null ],
        [ "📈 Build Time Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md252", [
          [ "Average Build Times", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md253", null ]
        ] ],
        [ "🚨 Recent Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md254", [
          [ "Build Failures (Last 7 Days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md255", null ],
          [ "Performance Regressions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md256", null ]
        ] ],
        [ "📝 Dependency Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md257", [
          [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md258", null ]
        ] ],
        [ "🔄 CI/CD Pipeline Health", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md259", [
          [ "Workflow Execution Stats (Last 30 Days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md260", null ],
          [ "Resource Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md261", null ]
        ] ],
        [ "📅 Release History", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md262", [
          [ "Recent Releases", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md263", null ]
        ] ],
        [ "🔗 Quick Links", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md264", null ],
        [ "📊 Dashboard Update", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md265", null ],
        [ "<em>This dashboard is automatically updated by CI/CD workflows. For real-time data, check the <a href=\"https://github.com/kcenon/logger_system/actions\" >GitHub Actions page</a>.</em>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CI__CD__DASHBOARD.html#autotoc_md267", null ]
      ] ]
    ] ],
    [ "Conditional Compilation Refactoring Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html", [
      [ "Problem Statement", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md269", [
        [ "Current State", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md270", null ],
        [ "Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md271", null ],
        [ "Example of Current Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md272", null ]
      ] ],
      [ "Proposed Solution: Adapter Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md273", [
        [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md274", null ],
        [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md275", null ],
        [ "Trade-offs", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md276", null ]
      ] ],
      [ "Implementation Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md277", [
        [ "Phase 3.1: Analysis and Documentation ✅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md278", null ],
        [ "Phase 3.2: Core Interface Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md279", null ],
        [ "Phase 3.3: Adapter Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md280", null ],
        [ "Phase 3.4: Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md281", null ],
        [ "Phase 3.5: Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md282", null ]
      ] ],
      [ "Detailed Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md283", [
        [ "Core Logger Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md284", null ],
        [ "Adapter Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md285", null ],
        [ "Thread System Adapter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md286", null ],
        [ "Common System Adapter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md287", null ],
        [ "Factory Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md288", null ]
      ] ],
      [ "Migration Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md289", [
        [ "Step 1: Extract Core (1 week)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md290", null ],
        [ "Step 2: Implement Adapters (1 week)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md291", null ],
        [ "Step 3: Update Public Interface (3 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md292", null ],
        [ "Step 4: Remove Conditional Compilation (2 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md293", null ],
        [ "Step 5: Testing and Validation (3 days)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md294", null ]
      ] ],
      [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md295", [
        [ "Virtual Function Overhead", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md296", null ],
        [ "Binary Size Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md297", null ]
      ] ],
      [ "Risk Mitigation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md298", [
        [ "Risks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md299", null ],
        [ "Rollback Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md300", null ]
      ] ],
      [ "Success Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md301", null ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CONDITIONAL__COMPILATION__REFACTORING.html#autotoc_md302", null ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html", [
      [ "크리티컬 로그 손실 방지 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION_8kr.html#autotoc_md304", null ]
    ] ],
    [ "CRITICAL_LOG_PREVENTION", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html", [
      [ "Critical Log Loss Prevention Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md308", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md307", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md309", [
          [ "Problem", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md310", null ],
          [ "Solution", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md311", null ]
        ] ],
        [ "Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md313", null ],
        [ "Class Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md315", [
          [ "1. <tt>critical_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md316", [
            [ "Key Features:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md317", null ],
            [ "Configuration:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md318", null ],
            [ "Usage Example:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md319", null ]
          ] ],
          [ "2. <tt>hybrid_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md320", [
            [ "Features:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md321", null ],
            [ "Usage Example:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md322", null ]
          ] ]
        ] ],
        [ "How It Works", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md324", [
          [ "1. Level-Based Routing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md325", null ],
          [ "2. Critical Write Flow", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md326", [
            [ "Step-by-Step Explanation:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md327", null ]
          ] ],
          [ "3. Write-Ahead Logging (WAL)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md328", [
            [ "WAL Format:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md329", null ],
            [ "WAL Usage:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md330", null ]
          ] ],
          [ "4. Signal Handler", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md331", [
            [ "Handled Signals:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md332", null ],
            [ "Signal Handler Implementation:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md333", null ],
            [ "Important Notes:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md334", null ]
          ] ],
          [ "5. File Descriptor Sync", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md335", [
            [ "Buffer Layers:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md336", null ],
            [ "Implementation:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md337", null ]
          ] ]
        ] ],
        [ "Performance Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md339", [
          [ "1. Critical Log Overhead", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md340", null ],
          [ "2. Normal Log Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md341", null ],
          [ "3. Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md342", [
            [ "Strategy 1: Disable <tt>sync_on_critical</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md343", null ],
            [ "Strategy 2: Disable <tt>force_flush_on_error</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md344", null ],
            [ "Strategy 3: Use Hybrid Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md345", null ]
          ] ],
          [ "4. Benchmark Results (Estimated)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md346", null ]
        ] ],
        [ "Production Recommendations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md348", [
          [ "1. Default Configuration (General Services)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md349", null ],
          [ "2. High Reliability Configuration (Finance/Medical)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md350", null ],
          [ "3. High Performance Configuration (Games/Real-time Systems)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md351", null ]
        ] ],
        [ "Testing & Verification", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md353", [
          [ "1. Critical Log Loss Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md354", [
            [ "Verification Method:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md355", null ]
          ] ],
          [ "2. WAL Recovery Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md356", null ],
          [ "3. Performance Benchmark", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md357", null ]
        ] ],
        [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md359", [
          [ "Issue 1: WAL File Not Created", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md360", null ],
          [ "Issue 2: Signal Handler Not Working", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md361", null ],
          [ "Issue 3: Performance Degradation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md362", null ]
        ] ],
        [ "FAQ", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md364", [
          [ "Q1: Should I wrap all logs with critical_writer?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md365", null ],
          [ "Q2: When should I use WAL?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md366", null ],
          [ "Q3: fsync() overhead is too high.", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md367", null ],
          [ "Q4: What can I do in a signal handler?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md368", null ]
        ] ],
        [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md370", null ],
        [ "Version History", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md372", null ],
        [ "License", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOG__PREVENTION.html#autotoc_md374", null ]
      ] ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html", [
      [ "크리티컬 로깅 빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START_8kr.html#autotoc_md376", null ]
    ] ],
    [ "CRITICAL_LOGGING_QUICK_START", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html", [
      [ "Critical Logging Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md380", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md379", null ],
        [ "🎯 Goal", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md382", null ],
        [ "🚀 Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md384", [
          [ "Step 1: Include Headers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md385", null ],
          [ "Step 2: Create Hybrid Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md386", null ],
          [ "Step 3: Verify", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md387", null ]
        ] ],
        [ "📊 3 Usage Scenarios", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md389", [
          [ "Scenario 1: General Web Services (Recommended)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md390", null ],
          [ "Scenario 2: Finance/Medical Systems (Maximum Reliability)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md391", null ],
          [ "Scenario 3: Games/Real-time Systems (High Performance)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md392", null ]
        ] ],
        [ "🔧 Configuration Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md394", null ],
        [ "✅ Verification Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md396", [
          [ "Test 1: Check Critical Logs", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md397", null ],
          [ "Test 2: Crash Simulation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md398", null ],
          [ "Test 3: Ctrl+C Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md399", null ]
        ] ],
        [ "📚 Learn More", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md401", null ],
        [ "🐛 Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md403", [
          [ "\"WAL file not created\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md404", null ],
          [ "\"Performance too slow\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md405", null ],
          [ "\"Signal handler not working\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md406", null ]
        ] ],
        [ "💡 Key Points", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CRITICAL__LOGGING__QUICK__START.html#autotoc_md408", null ]
      ] ]
    ] ],
    [ "CUSTOM_WRITERS.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS_8kr.html", [
      [ "커스텀 작성기 생성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS_8kr.html#autotoc_md411", null ]
    ] ],
    [ "CUSTOM_WRITERS", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html", [
      [ "Creating Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md414", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md415", null ],
        [ "Writer Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md416", null ],
        [ "Recommended: Using thread_safe_writer (Since v1.3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md417", [
          [ "Benefits of thread_safe_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md418", null ]
        ] ],
        [ "Base Writer Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md419", null ],
        [ "Simple Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md420", [
          [ "1. Simple File Writer (Using thread_safe_writer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md421", null ],
          [ "2. Rotating File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md422", null ],
          [ "3. Network Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md423", null ],
          [ "4. Database Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md424", null ]
        ] ],
        [ "Advanced Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md425", [
          [ "1. Filtering Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md426", null ],
          [ "2. Async Writer Wrapper", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md427", null ]
        ] ],
        [ "Choosing the Right Base Class", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md428", [
          [ "When to Use thread_safe_writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md429", null ],
          [ "When to Use base_writer Directly", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md430", null ]
        ] ],
        [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md431", null ],
        [ "Testing Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md432", null ]
      ] ]
    ] ],
    [ "Log Level Semantic Standard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html", [
      [ "Problem Statement", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md436", [
        [ "logger_system (Ascending: Low → High)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md437", null ],
        [ "thread_system (Descending: High → Low)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md438", null ],
        [ "Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md439", null ]
      ] ],
      [ "Decision", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md441", [
        [ "Rationale", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md442", null ]
      ] ],
      [ "Migration Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md444", [
        [ "Phase 1: Documentation (Sprint 1 - Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md445", null ],
        [ "Phase 2: thread_system Unification (Sprint 2)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md446", null ],
        [ "Phase 3: Adapter Simplification (Sprint 3)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md447", null ]
      ] ],
      [ "Implementation Details", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md449", [
        [ "Standard Definition (logger_system - already compliant)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md450", null ],
        [ "thread_system Changes (Sprint 2)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md451", null ],
        [ "Adapter Simplification (Sprint 3)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md452", null ]
      ] ],
      [ "Compatibility Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md454", [
        [ "Breaking Change Assessment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md455", null ],
        [ "Mitigation Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md456", null ],
        [ "User Impact", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md457", null ]
      ] ],
      [ "Testing Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md459", [
        [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md460", null ],
        [ "Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md461", null ],
        [ "Regression Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md462", null ]
      ] ],
      [ "Documentation Updates Required", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md464", null ],
      [ "Success Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md466", null ],
      [ "Timeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md468", null ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md470", null ],
      [ "Approval", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOG__LEVEL__SEMANTIC__STANDARD.html#autotoc_md472", null ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html", [
      [ "로거 시스템 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md474", [
        [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md475", [
          [ "주요 기능 (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md476", null ]
        ] ],
        [ "아키텍처 다이어그램", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md477", null ],
        [ "핵심 컴포넌트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md478", [
          [ "1. ILogger 인터페이스 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md479", null ],
          [ "2. Dual API 설계", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md480", null ],
          [ "3. 백엔드 추상화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md481", null ]
        ] ],
        [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md482", [
          [ "벤치마크 (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md483", null ],
          [ "멀티스레드 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE_8kr.html#autotoc_md484", null ]
        ] ]
      ] ]
    ] ],
    [ "LOGGER_SYSTEM_ARCHITECTURE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html", [
      [ "Logger System Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md487", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md488", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md489", [
          [ "Key Features (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md490", null ]
        ] ],
        [ "Architecture Diagram", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md491", null ],
        [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md492", [
          [ "1. ILogger Interface Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md493", null ],
          [ "2. Dual API Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md494", null ],
          [ "3. Configuration Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md495", [
            [ "Logger Configuration with Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md496", null ],
            [ "Configuration Templates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md497", null ]
          ] ],
          [ "4. Builder Pattern with Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md498", null ],
          [ "5. Backend Abstraction", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md499", null ],
          [ "6. Interface Segregation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md500", [
            [ "Writer Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md501", null ],
            [ "Filter Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md502", null ],
            [ "Formatter Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md503", null ]
          ] ],
          [ "7. Log Entry Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md504", null ]
        ] ],
        [ "Advanced Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md505", [
          [ "1. Asynchronous Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md506", null ],
          [ "2. Error Handling with Result Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md507", null ],
          [ "3. C++20 Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md508", null ],
          [ "4. Performance Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md509", null ],
          [ "5. Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md510", null ]
        ] ],
        [ "Threading Model", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md511", [
          [ "Synchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md512", null ],
          [ "Asynchronous Mode (Default)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md513", null ],
          [ "Thread Safety Guarantees", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md514", null ]
        ] ],
        [ "Memory Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md515", [
          [ "Buffer Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md516", null ],
          [ "Object Lifetime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md517", null ]
        ] ],
        [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md518", [
          [ "Benchmarks (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md519", null ],
          [ "Multi-threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md520", null ],
          [ "Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md521", null ]
        ] ],
        [ "Integration Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md522", [
          [ "ILogger Interface Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md523", null ],
          [ "Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md524", null ],
          [ "With Monitoring (Phase 2.2.4)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md525", null ]
        ] ],
        [ "Extension Points", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md526", [
          [ "Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md527", null ],
          [ "Custom Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md528", null ],
          [ "Custom Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md529", null ]
        ] ],
        [ "Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md530", [
          [ "Performance Improvements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md531", null ],
          [ "Feature Additions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md532", null ],
          [ "Platform Extensions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md533", null ]
        ] ],
        [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md534", [
          [ "For Library Users", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md535", null ],
          [ "For Contributors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md536", null ]
        ] ],
        [ "Platform Notes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2LOGGER__SYSTEM__ARCHITECTURE.html#autotoc_md537", null ]
      ] ]
    ] ],
    [ "Migration Guide - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md540", null ],
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md542", null ],
      [ "Quick Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md544", null ],
      [ "Migration Path 1: From Integrated thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md546", [
        [ "Background", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md547", null ],
        [ "Step 1: Update CMakeLists.txt", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md548", null ],
        [ "Step 2: Update Header Includes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md549", null ],
        [ "Step 3: Update Namespace", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md550", null ],
        [ "Step 4: Migrate Logger Creation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md551", null ],
        [ "Step 5: Update Thread System Integration (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md552", null ]
      ] ],
      [ "Migration Path 2: From spdlog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md554", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md555", null ],
        [ "API Mapping", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md556", null ],
        [ "Example Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md557", null ],
        [ "Pattern Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md558", null ]
      ] ],
      [ "Migration Path 3: From Custom Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md560", [
        [ "Common Custom Logger Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md561", [
          [ "Pattern 1: Direct File Writing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md562", null ],
          [ "Pattern 2: Multiple Output Targets", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md563", null ],
          [ "Pattern 3: Thread-Safe Singleton", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md564", null ]
        ] ]
      ] ],
      [ "Common Migration Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md566", [
        [ "Issue 1: Namespace Conflicts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md567", null ],
        [ "Issue 2: CMake Cache Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md568", null ],
        [ "Issue 3: Header Not Found", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md569", null ],
        [ "Issue 4: Result<T> Pattern Unfamiliar", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md570", null ],
        [ "Issue 5: Performance Regression", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md571", null ]
      ] ],
      [ "Testing Your Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md573", [
        [ "1. Compile Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md574", null ],
        [ "2. Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md575", null ],
        [ "3. Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md576", null ],
        [ "4. Performance Benchmark", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md577", null ]
      ] ],
      [ "Rollback Plan", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md579", [
        [ "Option 1: Revert Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md580", null ],
        [ "Option 2: Gradual Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md581", null ]
      ] ],
      [ "Migration Assistance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md583", [
        [ "Automated Migration Script", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md584", null ]
      ] ],
      [ "Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md586", null ],
      [ "Next Steps After Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2MIGRATION.html#autotoc_md588", null ]
    ] ],
    [ "Logger System - 프로젝트 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html", [
      [ "📁 디렉토리 레이아웃", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md592", null ],
      [ "🏗️ Namespace 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md593", [
        [ "Core Namespaces", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md594", null ],
        [ "Nested Namespaces", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md595", null ]
      ] ],
      [ "🔧 주요 컴포넌트 개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md596", [
        [ "🎯 Public API Layer (<tt>include/kcenon/logger/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md597", null ],
        [ "⚙️ Implementation Layer (<tt>src/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md598", null ]
      ] ],
      [ "📊 성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md599", null ],
      [ "🔄 마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md600", [
        [ "Step 1: 현재 설정 백업", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md601", null ],
        [ "Step 2: Include 경로 업데이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md602", null ],
        [ "Step 3: Namespace 사용 업데이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md603", null ],
        [ "Step 4: 마이그레이션 스크립트 실행", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md604", null ]
      ] ],
      [ "🚀 새로운 구조로 빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE_8kr.html#autotoc_md605", null ]
    ] ],
    [ "Logger System - Project Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md608", null ],
      [ "Directory Layout", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md609", null ],
      [ "Namespace Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md610", [
        [ "Primary Namespace Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md611", null ],
        [ "Namespace Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md612", null ]
      ] ],
      [ "Component Layout", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md613", [
        [ "Core Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md614", [
          [ "Logger Core (<tt>core/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md615", null ],
          [ "Interfaces (<tt>interfaces/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md616", null ],
          [ "Writers (<tt>writers/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md617", null ],
          [ "Formatters (<tt>formatters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md618", null ],
          [ "Filters (<tt>filters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md619", null ],
          [ "Routing (<tt>routing/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md620", null ],
          [ "Analysis (<tt>analysis/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md621", null ],
          [ "Structured Logging (<tt>structured/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md622", null ],
          [ "Safety (<tt>safety/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md623", null ],
          [ "Server (<tt>server/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md624", null ],
          [ "Adapters (<tt>adapters/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md625", null ]
        ] ],
        [ "Monitoring (<tt>core/monitoring/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md626", null ]
      ] ],
      [ "Module Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md627", [
        [ "Internal Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md628", null ],
        [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md629", null ],
        [ "Compilation Units", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md630", null ]
      ] ],
      [ "Key Components Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md631", [
        [ "Public API Layer (<tt>include/kcenon/logger/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md632", null ],
        [ "Implementation Layer (<tt>src/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md633", null ],
        [ "Test Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md634", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md635", [
        [ "Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md636", null ],
        [ "Memory", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md637", null ],
        [ "Threading", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md638", null ],
        [ "Resource Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md639", null ]
      ] ],
      [ "Build Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md640", [
        [ "CMake Targets", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md641", null ],
        [ "Feature Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md642", null ]
      ] ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2STRUCTURE.html#autotoc_md643", null ]
    ] ],
    [ "WRITER_HIERARCHY", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html", [
      [ "Writer Hierarchy and Categories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md644", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md645", null ],
        [ "Category System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md646", [
          [ "Writer Categories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md647", null ],
          [ "Category Tags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md648", null ]
        ] ],
        [ "Complete Writer Hierarchy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md649", null ],
        [ "Category Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md650", [
          [ "Synchronous Writers (sync_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md651", null ],
          [ "Asynchronous Writers (async_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md652", null ],
          [ "Decorator Writers (decorator_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md653", null ],
          [ "Composite Writers (composite_writer_tag)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md654", null ]
        ] ],
        [ "Using Category Information", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md655", [
          [ "Compile-Time Type Traits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md656", null ],
          [ "C++20 Concepts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md657", null ],
          [ "Runtime Category Query", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md658", null ]
        ] ],
        [ "Design Patterns Used", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md659", [
          [ "Template Method Pattern (thread_safe_writer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md660", null ],
          [ "Decorator Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md661", null ],
          [ "Pipeline Pattern (composite_writer)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md662", null ]
        ] ],
        [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__HIERARCHY.html#autotoc_md663", null ]
      ] ]
    ] ],
    [ "WRITER_SELECTION_GUIDE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html", [
      [ "Writer Selection Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md665", [
        [ "Quick Selection Flowchart", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md666", null ],
        [ "Writer Selection by Use Case", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md667", [
          [ "1. Simple Console Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md668", null ],
          [ "2. Basic File Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md670", null ],
          [ "3. Production File Logging with Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md672", null ],
          [ "4. High-Throughput Logging (>10K msg/sec)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md674", null ],
          [ "5. Network/Remote Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md676", null ],
          [ "6. Encrypted/Secure Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md678", null ],
          [ "7. Critical/Safety-Critical Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md680", null ],
          [ "8. Custom Pipeline (Formatter + Sink)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md682", null ]
        ] ],
        [ "Decorator Stacking Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md684", [
          [ "Pattern 1: Async + Encrypted", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md685", null ],
          [ "Pattern 2: Critical + Async + Rotating", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md686", null ],
          [ "Pattern 3: Batch + Network", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md687", null ]
        ] ],
        [ "Performance Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md688", null ],
        [ "Decision Matrix", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md689", null ],
        [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2WRITER__SELECTION__GUIDE.html#autotoc_md690", null ]
      ] ]
    ] ],
    [ "API_REFERENCE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html", [
      [ "API 레퍼런스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md692", [
        [ "v3.0 주요 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md693", [
          [ "네임스페이스 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md694", null ],
          [ "인터페이스 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md695", null ],
          [ "헤더 경로 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md696", null ],
          [ "주요 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md697", null ]
        ] ],
        [ "빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE_8kr.html#autotoc_md698", null ]
      ] ]
    ] ],
    [ "API_REFERENCE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html", [
      [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md701", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md702", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md703", [
          [ "Namespace", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md704", null ]
        ] ],
        [ "Core Classes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md705", [
          [ "<tt>kcenon::logger::logger</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md706", [
            [ "Constructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md707", null ],
            [ "Destructor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md708", null ]
          ] ]
        ] ],
        [ "ILogger Interface (Phase 2.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md709", [
          [ "Log Methods (ILogger Interface)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md710", null ],
          [ "Level Management (ILogger Interface)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md711", null ],
          [ "Native API (Backward Compatible)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md712", null ],
          [ "Writer Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md713", null ],
          [ "Lifecycle Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md714", null ],
          [ "Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md715", null ],
          [ "Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md716", null ],
          [ "Emergency Flush (Signal Safety)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md717", null ]
        ] ],
        [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md718", [
          [ "<tt>kcenon::logger::logger_config</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md719", null ],
          [ "Static Factory Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md720", null ]
        ] ],
        [ "Builder Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md721", [
          [ "<tt>kcenon::logger::logger_builder</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md722", [
            [ "Basic Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md723", null ],
            [ "Feature Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md724", null ],
            [ "Output Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md725", null ],
            [ "Writers and Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md726", null ],
            [ "Backend Selection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md727", null ],
            [ "Environment and Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md728", null ],
            [ "Build", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md729", null ]
          ] ]
        ] ],
        [ "Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md730", [
          [ "Deployment Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md731", null ],
          [ "Performance Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md732", null ],
          [ "Environment Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md733", null ],
          [ "Composite Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md734", null ]
        ] ],
        [ "Interfaces", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md735", [
          [ "<tt>kcenon::logger::log_writer_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md736", null ],
          [ "<tt>kcenon::logger::log_filter_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md737", null ],
          [ "<tt>kcenon::logger::log_formatter_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md738", null ],
          [ "<tt>kcenon::logger::log_entry</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md739", null ]
        ] ],
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md740", [
          [ "Error Codes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md741", null ],
          [ "Result Types", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md742", null ],
          [ "Helper Functions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md743", null ]
        ] ],
        [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md744", [
          [ "<tt>kcenon::logger::console_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md745", null ],
          [ "<tt>kcenon::logger::file_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md746", null ],
          [ "<tt>kcenon::logger::rotating_file_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md747", null ],
          [ "<tt>kcenon::logger::network_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md748", null ],
          [ "<tt>kcenon::logger::critical_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md749", null ],
          [ "<tt>kcenon::logger::batch_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md750", null ]
        ] ],
        [ "Filters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md751", [
          [ "<tt>kcenon::logger::filters::level_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md752", null ],
          [ "<tt>kcenon::logger::filters::regex_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md753", null ],
          [ "<tt>kcenon::logger::filters::function_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md754", null ],
          [ "<tt>kcenon::logger::filters::composite_filter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md755", null ]
        ] ],
        [ "Formatters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md756", [
          [ "<tt>kcenon::logger::plain_formatter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md757", null ],
          [ "<tt>kcenon::logger::json_formatter</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md758", null ]
        ] ],
        [ "Backend Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md759", [
          [ "Integration Backend Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md760", null ],
          [ "Standalone Backend (Default)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md761", null ]
        ] ],
        [ "Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md762", [
          [ "IMonitor Integration (Phase 2.2.4)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md763", null ]
        ] ],
        [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md764", [
          [ "Basic Usage with ILogger Interface", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md765", null ],
          [ "Native API (Backward Compatible)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md766", null ],
          [ "Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md767", null ],
          [ "Custom Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md768", null ],
          [ "Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md769", null ]
        ] ],
        [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md770", null ],
        [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md771", null ],
        [ "Migration from v2.x", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2API__REFERENCE.html#autotoc_md772", null ]
      ] ]
    ] ],
    [ "ARCHITECTURE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html", [
      [ "스레딩 생태계 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md774", [
        [ "v3.0 주요 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md775", [
          [ "의존성 구조 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md776", null ],
          [ "핵심 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md777", null ],
          [ "독립 실행 모드 vs 통합 모드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md778", null ],
          [ "성능 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE_8kr.html#autotoc_md779", null ]
        ] ]
      ] ]
    ] ],
    [ "ARCHITECTURE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html", [
      [ "Threading Ecosystem Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md782", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md783", null ],
        [ "🏗️ Ecosystem Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md784", null ],
        [ "📋 Project Roles & Responsibilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md785", [
          [ "1. common_system (Foundation) - <strong>Required</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md786", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md787", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md788", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md789", null ]
          ] ],
          [ "2. logger_system (Logging) - <strong>Standalone Capable</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md791", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md792", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md793", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md794", null ]
          ] ],
          [ "3. thread_system (Threading) - <strong>Optional</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md796", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md797", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md798", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md799", null ]
          ] ],
          [ "4. monitoring_system (Metrics)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md801", [
            [ "Responsibilities:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md802", null ],
            [ "Key Components:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md803", null ],
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md804", null ]
          ] ],
          [ "5. integrated_thread_system (Integration Hub)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md806", [
            [ "Dependencies:", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md807", null ]
          ] ]
        ] ],
        [ "🔄 Dependency Flow & Interface Contracts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md808", [
          [ "Interface Hierarchy (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md809", null ],
          [ "Dependency Graph (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md810", null ],
          [ "Build Order Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md811", null ],
          [ "Standalone vs Integrated Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md812", null ]
        ] ],
        [ "🔧 Integration Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md813", [
          [ "1. Interface-Based Integration (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md814", null ],
          [ "2. Dependency Injection Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md815", null ],
          [ "3. Configuration Management", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md816", null ],
          [ "4. Backend Selection Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md817", null ]
        ] ],
        [ "📊 Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md818", [
          [ "Design Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md819", null ],
          [ "Performance Metrics (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md820", null ],
          [ "Standalone vs Thread System Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md821", null ]
        ] ],
        [ "🔄 Evolution: Monolithic → Modular → Standalone", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md822", [
          [ "Phase 1: Monolithic (v1.x)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md823", null ],
          [ "Phase 2: Modular (v2.x)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md824", null ],
          [ "Phase 3: Standalone (v3.0) - Current", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md825", null ],
          [ "Migration Benefits (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md826", null ]
        ] ],
        [ "🚀 Getting Started", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md827", [
          [ "1. Minimal Setup (Standalone)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md828", null ],
          [ "2. Full Ecosystem Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md829", null ],
          [ "3. Build Order", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md830", null ],
          [ "4. CMake Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md831", null ]
        ] ],
        [ "📚 Documentation Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md832", [
          [ "common_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md833", null ],
          [ "logger_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md834", null ],
          [ "thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md835", null ],
          [ "monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md836", null ]
        ] ],
        [ "🔮 Future Roadmap", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md837", [
          [ "Phase 3.1: Enhancement (Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md838", null ],
          [ "Phase 3.2: Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md839", null ],
          [ "Phase 4: Ecosystem Expansion", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2ARCHITECTURE.html#autotoc_md840", null ]
        ] ]
      ] ]
    ] ],
    [ "Logger System 성능 벤치마크", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html", [
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md845", null ],
      [ "요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md847", [
        [ "핵심 하이라이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md848", null ],
        [ "경쟁 우위", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md849", null ]
      ] ],
      [ "테스트 환경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md851", [
        [ "하드웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md852", null ],
        [ "소프트웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md853", null ]
      ] ],
      [ "핵심 성능 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md855", [
        [ "단일 스레드 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md856", null ],
        [ "멀티스레드 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md857", [
          [ "스레드 수별 처리량", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md858", null ]
        ] ]
      ] ],
      [ "업계 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md860", [
        [ "vs spdlog (가장 인기 있는 C++ 로거)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md861", [
          [ "단일 스레드 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md862", null ],
          [ "멀티스레드 비교 (4 스레드)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md863", null ]
        ] ],
        [ "vs Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md864", null ],
        [ "vs glog (Google 로깅 라이브러리)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md865", null ]
      ] ],
      [ "지연시간 벤치마크", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md867", [
        [ "지연시간 분포 (단일 스레드, 비동기)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md868", null ],
        [ "부하 시 지연시간", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md869", null ]
      ] ],
      [ "메모리 프로파일링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md871", [
        [ "구성별 메모리 풋프린트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md872", null ],
        [ "경쟁사와 메모리 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md873", null ]
      ] ],
      [ "성능 회귀 기준", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md875", null ]
    ] ],
    [ "Logger System Performance Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md880", null ],
      [ "Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md882", [
        [ "Key Highlights", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md883", null ],
        [ "Competitive Advantages", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md884", null ]
      ] ],
      [ "Test Environment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md886", [
        [ "Hardware Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md887", null ],
        [ "Software Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md888", null ]
      ] ],
      [ "Core Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md890", [
        [ "Single-Threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md891", null ],
        [ "Multi-Threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md892", [
          [ "Throughput by Thread Count", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md893", null ],
          [ "Performance Scaling Graph", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md894", null ]
        ] ]
      ] ],
      [ "Industry Comparisons", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md896", [
        [ "vs spdlog (Most Popular C++ Logger)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md897", [
          [ "Single-Threaded Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md898", null ],
          [ "Multi-Threaded Comparison (4 Threads)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md899", null ]
        ] ],
        [ "vs Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md900", null ],
        [ "vs glog (Google's Logging Library)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md901", null ],
        [ "vs log4cxx", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md902", null ]
      ] ],
      [ "Scalability Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md904", [
        [ "Thread Scaling Efficiency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md905", null ],
        [ "Queue Utilization vs Thread Count", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md906", null ],
        [ "CPU Utilization Efficiency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md907", null ]
      ] ],
      [ "Latency Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md909", [
        [ "Latency Distribution (Single Thread, Async)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md910", null ],
        [ "Latency Comparison (p99)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md911", null ],
        [ "Latency Under Load", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md912", null ]
      ] ],
      [ "Memory Profiling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md914", [
        [ "Memory Footprint by Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md915", null ],
        [ "Memory Comparison with Competitors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md916", null ],
        [ "Memory Allocation Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md917", null ]
      ] ],
      [ "Benchmark Methodology", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md919", [
        [ "Test Scenarios", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md920", [
          [ "1. Single-Threaded Throughput Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md921", null ],
          [ "2. Multi-Threaded Contention Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md922", null ],
          [ "3. Latency Measurement Test", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md923", null ]
        ] ],
        [ "Measurement Tools", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md924", null ],
        [ "Benchmark Caveats", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md925", null ]
      ] ],
      [ "Performance Regression Baselines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md927", null ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS.html#autotoc_md929", null ]
    ] ],
    [ "변경 이력 - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html", [
      [ "[3.0.0] - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md932", [
        [ "제거됨 - Deprecated API 정리 (Issues #268, #324)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md933", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md934", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md935", null ]
        ] ]
      ] ],
      [ "[Unreleased]", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md937", [
        [ "실시간 로그 분석 통합 (Issue #281)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md938", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md939", null ],
          [ "예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md940", null ]
        ] ],
        [ "고용량 시나리오를 위한 로그 샘플링 (Issue #282)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md942", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md943", null ],
          [ "예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md944", null ]
        ] ],
        [ "Phase 3.4: 구조화된 로깅 통합 (Issue #311) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md946", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md947", null ],
          [ "예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md948", null ]
        ] ],
        [ "Coverage 빌드 수정 (PR #291) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md950", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md951", null ]
        ] ],
        [ "CI 워크플로우 수정 (PR #290) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md953", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md954", null ]
        ] ],
        [ "OpenTelemetry 통합 수정 (Issue #283) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md956", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md957", null ]
        ] ],
        [ "C++20 모듈 파일 (Issue #275) - 2026-01-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md959", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md960", null ],
          [ "모듈 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md961", null ],
          [ "CMake 사용법", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md962", null ]
        ] ],
        [ "중복 Writer 추가 로직 수정 (Issue #270) - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md964", [
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md965", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md966", null ],
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md967", null ]
        ] ],
        [ "KCENON 기능 감지 사용 (Issue #250) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md969", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md970", null ],
          [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md971", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md972", null ]
        ] ],
        [ "Deprecated common_system API 마이그레이션 (Issue #248) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md974", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md975", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md976", null ]
        ] ],
        [ "thread_system v3.0 호환성 (Issue #244) - 2025-12-19", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md978", [
          [ "주요 변경 (Breaking Changes)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md979", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md980", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md981", null ]
        ] ],
        [ "#226 API 변경 후 손상된 예제 파일 수정 (Issue #228) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md983", [
          [ "제거됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md984", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md985", null ],
          [ "비활성화됨 (기능 구현 대기)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md986", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md987", null ]
        ] ],
        [ "common::interfaces::ILogger 구현 - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md989", [
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md990", null ],
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md991", null ],
          [ "사용 중단됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md992", null ],
          [ "하위 호환성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md993", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md994", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md995", null ]
        ] ],
        [ "독립형 비동기 구현 (Issue #222) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md997", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md998", null ],
          [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md999", null ],
          [ "이점", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1000", null ],
          [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1001", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1002", null ]
        ] ],
        [ "CMake fmt Fallback 제거 - 2025-12-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1004", [
          [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1005", null ],
          [ "제거됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1006", null ],
          [ "이점", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1007", null ],
          [ "관련 이슈", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1008", null ]
        ] ]
      ] ],
      [ "[1.0.0] - 2025-10-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1010", [
        [ "추가됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1011", null ],
        [ "변경됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1012", null ],
        [ "사용 중단됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1013", null ],
        [ "제거됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1014", null ],
        [ "수정됨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1015", null ],
        [ "보안", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1016", null ]
      ] ],
      [ "프로젝트 정보", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1018", null ],
      [ "버전 지원 매트릭스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1020", null ],
      [ "마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG_8kr.html#autotoc_md1022", null ]
    ] ],
    [ "Changelog - Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html", [
      [ "[4.0.0] - Unreleased", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1026", [
        [ "Removed - Deprecated Context ID Convenience Methods (Issue #326)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1027", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1028", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1029", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1030", null ]
        ] ],
        [ "Added - Decorator Pattern Writer Architecture (Issue #356)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1032", [
          [ "New Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1033", null ],
          [ "Usage Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1034", null ],
          [ "Architecture Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1035", null ],
          [ "Custom Writer Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1036", null ]
        ] ]
      ] ],
      [ "[3.0.0] - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1038", [
        [ "Removed - Deprecated log_level Types and Converters (Issue #339)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1039", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1040", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1041", null ]
        ] ],
        [ "Removed - Deprecated API Cleanup (Issues #268, #324)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1043", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1044", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1045", null ]
        ] ]
      ] ],
      [ "[Unreleased]", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1047", [
        [ "Real-time Log Analysis Integration (Issue #281)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1048", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1049", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1050", null ]
        ] ],
        [ "Log Sampling for High-Volume Scenarios (Issue #282)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1052", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1053", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1054", null ]
        ] ],
        [ "Phase 3.4: Structured Logging Integration (Issue #311) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1056", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1057", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1058", null ]
        ] ],
        [ "Phase 3.3: Structured Logging Formatters (Issue #310) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1060", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1061", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1062", null ]
        ] ],
        [ "Phase 3.1: Structured Logging API Design (Issue #308) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1064", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1065", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1066", null ]
        ] ],
        [ "Phase 3.2: Structured Logging Core Implementation (Issue #309) - 2026-01-11", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1068", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1069", null ],
          [ "Example", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1070", null ]
        ] ],
        [ "Coverage Build Fix (PR #291) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1072", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1073", null ]
        ] ],
        [ "CI Workflow Fix (PR #290) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1075", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1076", null ]
        ] ],
        [ "OpenTelemetry Integration Fix (Issue #283) - 2026-01-08", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1078", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1079", null ]
        ] ],
        [ "C++20 Module Files (Issue #275) - 2026-01-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1081", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1082", null ],
          [ "Module Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1083", null ],
          [ "CMake Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1084", null ]
        ] ],
        [ "Fix Duplicate Writer Add Logic (Issue #270) - 2025-12-31", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1086", [
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1087", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1088", null ],
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1089", null ]
        ] ],
        [ "Use KCENON Feature Detection (Issue #250) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1091", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1092", null ],
          [ "Fixed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1093", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1094", null ]
        ] ],
        [ "Migrate from Deprecated common_system APIs (Issue #248) - 2025-12-22", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1096", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1097", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1098", null ]
        ] ],
        [ "thread_system v3.0 Compatibility (Issue #244) - 2025-12-19", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1100", [
          [ "Breaking Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1101", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1102", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1103", null ]
        ] ],
        [ "Fix broken example files after #226 API changes (Issue #228) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1105", [
          [ "Removed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1106", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1107", null ],
          [ "Disabled (awaiting feature implementation)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1108", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1109", null ]
        ] ],
        [ "common::interfaces::ILogger Implementation - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1111", [
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1112", null ],
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1113", null ],
          [ "Deprecated", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1114", null ],
          [ "Backward Compatibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1115", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1116", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1117", null ]
        ] ],
        [ "Standalone async implementation (Issue #222) - 2025-12-06", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1119", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1120", null ],
          [ "Added", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1121", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1122", null ],
          [ "Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1123", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1124", null ]
        ] ],
        [ "CMake fmt Fallback Removal - 2025-12-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1126", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1127", null ],
          [ "Removed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1128", null ],
          [ "Benefits", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1129", null ],
          [ "Related Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1130", null ]
        ] ],
        [ "thread_system Required Dependency - 2025-12-03", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1132", [
          [ "Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2CHANGELOG.html#autotoc_md1133", null ]
        ] ]
      ] ]
    ] ],
    [ "CONTRIBUTING.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING_8kr.html", [
      [ "Logger System 기여하기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING_8kr.html#autotoc_md1134", null ]
    ] ],
    [ "CONTRIBUTING", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html", [
      [ "Contributing to Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1137", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1138", null ],
        [ "Code of Conduct", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1139", null ],
        [ "Getting Started", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1140", null ],
        [ "Development Process", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1141", [
          [ "1. Before You Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1142", null ],
          [ "2. Making Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1143", null ],
          [ "3. Commit Message Format", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1144", null ]
        ] ],
        [ "Code Style", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1145", [
          [ "C++ Guidelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1146", null ],
          [ "Code Formatting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1147", null ],
          [ "Naming Conventions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1148", null ]
        ] ],
        [ "Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1149", [
          [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1150", null ],
          [ "Running Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1151", null ],
          [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1152", null ]
        ] ],
        [ "Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1153", [
          [ "Code Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1154", null ],
          [ "Documentation Updates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1155", null ]
        ] ],
        [ "Submitting Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1156", [
          [ "Pull Request Process", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1157", null ],
          [ "Review Process", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1158", null ]
        ] ],
        [ "Questions?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1159", null ],
        [ "Thank you for contributing!", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1160", null ]
      ] ]
    ] ],
    [ "Test Coverage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TESTING__GUIDE.html", null ],
    [ "Korean Translation Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html", [
      [ "Completion Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1163", null ],
      [ "Statistics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1164", null ],
      [ "Files Created", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1165", [
        [ "Root Directory (19 files)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1166", null ],
        [ "advanced/ Directory (2 files)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1167", null ],
        [ "guides/ Directory (4 files)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1168", null ]
      ] ],
      [ "Language Switcher Format", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1169", [
        [ "English Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1170", null ],
        [ "Korean Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1171", null ]
      ] ],
      [ "Translation Approach", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1172", null ],
      [ "File Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1173", null ],
      [ "Verification", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1174", null ],
      [ "Notes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1175", null ],
      [ "Next Steps (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2TRANSLATION__SUMMARY.html#autotoc_md1176", null ]
    ] ],
    [ "Logger System - 상세 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html", [
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1180", null ],
      [ "핵심 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1182", [
        [ "설계 원칙", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1183", null ],
        [ "주요 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1184", null ]
      ] ],
      [ "로그 레벨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1186", [
        [ "레벨 정의", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1187", null ],
        [ "레벨별 사용 지침", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1188", null ],
        [ "사용 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1189", null ]
      ] ],
      [ "로그 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1191", [
        [ "콘솔 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1192", null ],
        [ "파일 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1193", null ],
        [ "회전 파일 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1194", null ],
        [ "원격 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1195", null ],
        [ "커스텀 작성자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1196", null ]
      ] ],
      [ "포맷팅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1198", [
        [ "기본 포맷", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1199", null ],
        [ "JSON 포맷", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1200", null ],
        [ "커스텀 포맷터", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1201", null ]
      ] ],
      [ "필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1203", [
        [ "레벨 필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1204", null ],
        [ "패턴 필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1205", null ],
        [ "조건부 필터링", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1206", null ]
      ] ],
      [ "비동기 로깅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1208", [
        [ "비동기 모드 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1209", null ],
        [ "성능 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1210", null ]
      ] ],
      [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1212", [
        [ "벤치마크 결과", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1213", null ],
        [ "최적화 팁", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1214", null ]
      ] ],
      [ "통합 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1216", [
        [ "thread_system 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1217", null ],
        [ "monitoring_system 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1218", null ],
        [ "common_system 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1219", null ]
      ] ],
      [ "구성 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1221", [
        [ "프로덕션 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1222", null ],
        [ "개발 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1223", null ]
      ] ],
      [ "참고사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1225", [
        [ "스레드 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1226", null ],
        [ "주의사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1227", null ]
      ] ]
    ] ],
    [ "Logger System Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1232", null ],
      [ "Core Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1234", [
        [ "🎯 High-Performance Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1235", [
          [ "Asynchronous Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1236", null ],
          [ "Thread-Safe Operations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1237", null ],
          [ "Configuration Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1238", null ]
        ] ],
        [ "🔧 Advanced Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1239", [
          [ "Result-Based Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1240", null ],
          [ "Builder Pattern API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1241", null ],
          [ "Configuration Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1242", null ],
          [ "Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1243", null ]
        ] ]
      ] ],
      [ "Writer Types", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1245", [
        [ "Console Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1246", null ],
        [ "File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1247", null ],
        [ "Rotating File Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1248", null ],
        [ "Network Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1249", null ],
        [ "Critical Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1250", null ],
        [ "Hybrid Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1251", null ]
      ] ],
      [ "Formatting Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1253", [
        [ "Plain Text Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1254", null ],
        [ "JSON Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1255", null ],
        [ "Logfmt Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1256", null ],
        [ "Custom Formatter", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1257", null ]
      ] ],
      [ "Filtering Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1259", [
        [ "Level-Based Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1260", null ],
        [ "Regex-Based Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1261", null ],
        [ "Function-Based Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1262", null ],
        [ "Composite Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1263", null ]
      ] ],
      [ "Rotation Policies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1265", [
        [ "Size-Based Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1266", null ],
        [ "Time-Based Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1267", null ],
        [ "Hybrid Rotation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1268", null ],
        [ "Compression Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1269", null ]
      ] ],
      [ "Security Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1271", [
        [ "Secure Key Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1272", null ],
        [ "Path Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1273", null ],
        [ "Signal Handler Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1274", null ],
        [ "Security Audit Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1275", null ],
        [ "Sensitive Data Sanitization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1276", null ],
        [ "Access Control", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1277", null ],
        [ "Compliance Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1278", null ]
      ] ],
      [ "Advanced Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1280", [
        [ "Structured Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1281", null ],
        [ "Log Routing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1282", null ],
        [ "Performance Metrics Collection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1283", null ],
        [ "Real-time Log Analysis", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1284", null ],
        [ "Distributed Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1285", null ]
      ] ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES.html#autotoc_md1287", null ]
    ] ],
    [ "BEST_PRACTICES.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES_8kr.html", [
      [ "Logger System 모범 사례 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES_8kr.html#autotoc_md1288", null ]
    ] ],
    [ "BEST_PRACTICES", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html", [
      [ "Logger System Best Practices Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1291", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1292", null ],
        [ "Design Principles", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1293", [
          [ "1. Single Responsibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1294", null ],
          [ "2. Dependency Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1295", null ],
          [ "3. Interface Segregation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1296", null ]
        ] ],
        [ "Configuration Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1297", [
          [ "Environment-Specific Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1298", null ],
          [ "Configuration Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1299", null ],
          [ "Dynamic Reconfiguration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1300", null ]
        ] ],
        [ "Performance Guidelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1301", [
          [ "1. Minimize Allocations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1302", null ],
          [ "2. Lazy Evaluation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1303", null ],
          [ "3. Batch Operations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1304", null ],
          [ "4. Async Logging Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1305", null ]
        ] ],
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1306", [
          [ "Graceful Degradation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1307", null ],
          [ "Error Recovery", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1308", null ],
          [ "Circuit Breaker Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1309", null ]
        ] ],
        [ "Security Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1310", [
          [ "1. Sanitize Sensitive Data", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1311", null ],
          [ "2. Access Control", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1312", null ],
          [ "3. Log Injection Prevention", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1313", null ]
        ] ],
        [ "Testing Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1314", [
          [ "1. Mock Logger for Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1315", null ],
          [ "2. Performance Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1316", null ],
          [ "3. Stress Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1317", null ]
        ] ],
        [ "Production Deployment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1318", [
          [ "1. Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1319", null ],
          [ "2. Log Rotation Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1320", null ],
          [ "3. Graceful Shutdown", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1321", null ]
        ] ],
        [ "Common Pitfalls", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1322", [
          [ "1. Memory Leaks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1323", null ],
          [ "2. Deadlocks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1324", null ],
          [ "3. Performance Bottlenecks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1325", null ]
        ] ],
        [ "Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1326", null ],
        [ "Remember: Good logging is invisible when everything works but invaluable when things go wrong.", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2BEST__PRACTICES.html#autotoc_md1327", null ]
      ] ]
    ] ],
    [ "Decorator Pattern Writer Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1329", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1331", [
        [ "Why Decorator Pattern?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1332", null ],
        [ "Key Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1333", null ]
      ] ],
      [ "New Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1335", [
        [ "Base Classes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1336", [
          [ "<tt>log_writer_interface</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1337", null ],
          [ "<tt>decorator_writer_base</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1338", null ]
        ] ]
      ] ],
      [ "Built-in Decorators", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1340", [
        [ "<tt>filtered_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1341", null ],
        [ "<tt>buffered_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1342", null ],
        [ "<tt>formatted_writer</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1343", null ]
      ] ],
      [ "Migrating Custom Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1345", [
        [ "Before (v3.x): Monolithic Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1346", null ],
        [ "After (v4.0): Focused Leaf Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1347", null ],
        [ "Creating a Custom Decorator", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1348", null ]
      ] ],
      [ "Composing Decorators", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1350", [
        [ "Order Matters", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1351", null ],
        [ "Recommended Order", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1352", null ],
        [ "Using <tt>logger_builder</tt>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1353", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1355", [
        [ "1. Keep Leaf Writers Simple", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1356", null ],
        [ "2. Use Category Tags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1357", null ],
        [ "3. Handle Errors Gracefully", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1358", null ],
        [ "4. Ensure Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1359", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1361", [
        [ "Common Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1362", [
          [ "\"Writer not receiving entries\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1363", null ],
          [ "\"Entries not being formatted\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1364", null ],
          [ "\"Flush not working\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1365", null ]
        ] ],
        [ "Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1366", null ]
      ] ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2DECORATOR__MIGRATION.html#autotoc_md1368", null ]
    ] ],
    [ "Logger System - Frequently Asked Questions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1372", null ],
      [ "General Questions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1374", [
        [ "1. What is the logger_system?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1375", null ],
        [ "2. What C++ standard is required?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1377", null ]
      ] ],
      [ "Logging Basics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1379", [
        [ "3. How do I create a logger?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1380", null ],
        [ "4. How do I log messages?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1382", null ]
      ] ],
      [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1384", [
        [ "5. What log levels are available?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1385", null ],
        [ "6. How do I set the log level?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1387", null ],
        [ "7. Can I change log level at runtime?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1389", null ]
      ] ],
      [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1391", [
        [ "8. What writers are available?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1392", null ],
        [ "9. How do I use multiple writers?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1394", null ],
        [ "10. How does log rotation work?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1396", null ]
      ] ],
      [ "Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1398", [
        [ "11. What is the performance?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1399", null ],
        [ "12. How do I enable asynchronous logging?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1401", null ],
        [ "13. How do I optimize performance?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1403", null ]
      ] ],
      [ "Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1405", [
        [ "14. How do I integrate with monitoring_system?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1406", null ],
        [ "15. How do I integrate with thread_system?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1408", null ]
      ] ],
      [ "Advanced Topics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1410", [
        [ "16. Is it thread-safe?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1411", null ],
        [ "17. How do I create custom log formats?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1413", null ],
        [ "18. How do I filter logs?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1415", null ],
        [ "19. How do I handle critical logs?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1417", null ],
        [ "20. Where can I find more information?", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2FAQ.html#autotoc_md1419", null ]
      ] ]
    ] ],
    [ "GETTING_STARTED.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED_8kr.html", [
      [ "Logger System 시작하기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED_8kr.html#autotoc_md1421", null ]
    ] ],
    [ "GETTING_STARTED", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html", [
      [ "Getting Started with Logger System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1424", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1425", null ],
        [ "Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1426", null ],
        [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1427", [
          [ "Using CMake FetchContent (Recommended)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1428", null ],
          [ "Building from Source", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1429", null ],
          [ "Using as Installed Package", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1430", null ]
        ] ],
        [ "Basic Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1431", [
          [ "Simple Console Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1432", null ],
          [ "Logging with Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1433", null ],
          [ "Structured JSON Output", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1434", null ],
          [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1435", null ],
          [ "Filtering by Level", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1436", null ]
        ] ],
        [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1437", [
          [ "Synchronous vs Asynchronous Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1438", null ],
          [ "Multiple Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1439", null ],
          [ "Windows Notes (Networking)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1440", null ],
          [ "Console Writer Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1441", null ]
        ] ],
        [ "Integration with Thread System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1442", [
          [ "Using Service Container", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1443", null ],
          [ "Direct Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1444", null ]
        ] ],
        [ "Next Steps", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2GETTING__STARTED.html#autotoc_md1445", null ]
      ] ]
    ] ],
    [ "Logger System Integration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1449", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1450", null ],
      [ "System Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1451", [
        [ "Required Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1452", [
          [ "common_system (Header-only)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1453", null ]
        ] ],
        [ "Optional Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1454", [
          [ "thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1455", null ],
          [ "monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1456", null ]
        ] ]
      ] ],
      [ "Integration with common_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1457", [
        [ "ILogger Interface Implementation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1458", null ],
        [ "Result<T> Pattern Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1459", null ],
        [ "Adapter Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1460", null ]
      ] ],
      [ "Integration with thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1461", [
        [ "Asynchronous Logging with IExecutor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1462", null ],
        [ "Cross-System Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1463", null ]
      ] ],
      [ "Integration with monitoring_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1464", [
        [ "IMonitor Interface Injection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1465", null ],
        [ "Performance Metrics Collection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1466", null ],
        [ "Health Monitoring Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1467", null ]
      ] ],
      [ "Build Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1468", [
        [ "CMake Integration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1469", [
          [ "Minimal Configuration (common_system only)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1470", null ],
          [ "Full Ecosystem Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1471", null ]
        ] ],
        [ "Build Configuration Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1472", null ],
        [ "vcpkg Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1473", null ]
      ] ],
      [ "Code Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1474", [
        [ "Example 1: Basic Integration with common_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1475", null ],
        [ "Example 2: Full Stack Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1476", null ],
        [ "Example 3: Error Handling with Result<T>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1477", null ]
      ] ],
      [ "Result Handling Cheatsheet", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1478", null ],
      [ "Error Code Registry", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1479", [
        [ "Allocated Error Code Range", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1480", null ],
        [ "Error Message Mapping", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1481", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1482", [
        [ "Build Fails with \"common_system not found\"", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1483", null ],
        [ "Linker Errors: Undefined References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1484", null ],
        [ "Runtime: Logger Not Outputting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1485", null ],
        [ "Integration: IMonitor Not Injecting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1486", null ],
        [ "Performance: High Latency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1487", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1488", [
        [ "1. Use Result<T> for Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1489", null ],
        [ "2. Prefer Interface-Based Design", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1490", null ],
        [ "3. Leverage DI for Testing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1491", null ],
        [ "4. Monitor Performance in Production", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1492", null ]
      ] ],
      [ "References", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2INTEGRATION.html#autotoc_md1493", null ]
    ] ],
    [ "MIGRATION_GUIDE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html", [
      [ "로거 시스템 마이그레이션 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1494", [
        [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1495", null ],
        [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1496", null ],
        [ "CMake 구성 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1497", [
          [ "v2.x (이전)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1498", null ],
          [ "v3.0 (현재)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1499", null ],
          [ "CMake 플래그 변경 요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1500", null ],
          [ "타겟명 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1501", null ],
          [ "의존성 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1502", null ]
        ] ],
        [ "v2.x에서 v3.0으로 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1503", [
          [ "주요 변경사항 요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1504", null ],
          [ "1. 네임스페이스 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1505", null ],
          [ "2. 인터페이스 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1506", null ],
          [ "3. Result 타입 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1507", null ],
          [ "4. thread_system 의존성 변경", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1508", null ],
          [ "5. 백엔드 선택 (v3.0 신규)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1509", null ],
          [ "6. C++20 source_location (v3.0 신규)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1510", null ]
        ] ],
        [ "마이그레이션 체크리스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1511", [
          [ "v2.x → v3.0 마이그레이션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1512", null ]
        ] ],
        [ "일반적인 문제와 해결책", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1513", [
          [ "문제 1: 네임스페이스를 찾을 수 없음", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1514", null ],
          [ "문제 2: 인터페이스 타입 불일치", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1515", null ],
          [ "문제 3: C++20 미활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1516", null ]
        ] ],
        [ "지원 및 리소스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE_8kr.html#autotoc_md1517", null ]
      ] ]
    ] ],
    [ "MIGRATION_GUIDE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html", [
      [ "Logger System Migration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1520", [
        [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1521", null ],
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1522", [
          [ "Breaking Changes Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1523", null ]
        ] ],
        [ "From v3.x to v4.0", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1525", [
          [ "What Changed", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1526", null ],
          [ "Migration for Standard Users", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1527", null ],
          [ "Migration for Custom Writer Authors", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1528", null ],
          [ "Context ID API Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1529", null ]
        ] ],
        [ "CMake Configuration Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1531", [
          [ "v2.x (Previous)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1532", null ],
          [ "v3.0 (Current)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1533", null ],
          [ "CMake Flag Changes Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1534", null ],
          [ "Target Name Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1535", null ],
          [ "Dependency Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1536", null ]
        ] ],
        [ "Version Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1537", [
          [ "From v2.x to v3.0", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1538", [
            [ "Summary of Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1539", null ],
            [ "1. Namespace Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1540", null ],
            [ "2. Interface Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1541", null ],
            [ "3. Dual API Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1542", null ],
            [ "4. Result Type Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1543", null ],
            [ "5. thread_system Dependency Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1544", null ],
            [ "6. Backend Selection (New in v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1545", null ],
            [ "7. C++20 Source Location", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1546", null ],
            [ "8. Configuration Strategies (Enhanced in v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1547", null ],
            [ "9. Monitoring Integration (New in v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1548", null ],
            [ "Migration Script", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1549", null ]
          ] ],
          [ "From v1.x to v2.x", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1551", [
            [ "1. Error Handling Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1552", null ],
            [ "2. Memory Management Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1553", null ],
            [ "3. Configuration Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1554", null ],
            [ "4. Logging API Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1555", null ]
          ] ]
        ] ],
        [ "API Changes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1556", [
          [ "v3.0 API Changes Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1557", null ],
          [ "Deprecated Native log_level API (Planned for Removal in v3.0.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1558", null ],
          [ "Core Logger API", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1559", null ]
        ] ],
        [ "Configuration Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1560", [
          [ "From INI/XML Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1561", null ],
          [ "Environment-based Configuration (v3.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1562", null ]
        ] ],
        [ "Migration from Other Libraries", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1563", [
          [ "From spdlog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1564", null ],
          [ "From Boost.Log", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1565", null ],
          [ "From Google glog", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1566", null ]
        ] ],
        [ "Compatibility Wrappers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1567", [
          [ "v2.x to v3.0 Compatibility Header", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1568", null ],
          [ "Legacy API Wrapper (v1.x compatibility)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1569", null ]
        ] ],
        [ "Step-by-Step Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1570", [
          [ "Phase 1: Preparation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1571", null ],
          [ "Phase 2: Namespace Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1572", null ],
          [ "Phase 3: Interface Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1573", null ],
          [ "Phase 4: Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1574", null ]
        ] ],
        [ "Common Issues and Solutions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1575", [
          [ "Issue 1: Namespace Not Found", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1576", null ],
          [ "Issue 2: Interface Type Mismatch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1577", null ],
          [ "Issue 3: Result Type Mismatch", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1578", null ],
          [ "Issue 4: Missing thread_system", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1579", null ],
          [ "Issue 5: C++20 Not Enabled", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1580", null ]
        ] ],
        [ "Migration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1581", [
          [ "v2.x to v3.0 Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1582", null ],
          [ "v1.x to v2.x Migration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1583", null ]
        ] ],
        [ "Support and Resources", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1584", null ]
      ] ]
    ] ],
    [ "OpenTelemetry Integration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1587", null ],
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1588", null ],
      [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1589", [
        [ "With vcpkg", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1590", null ],
        [ "With CMake", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1591", null ],
        [ "Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1592", null ]
      ] ],
      [ "Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1593", [
        [ "Basic OTLP Export", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1594", null ],
        [ "With Trace Correlation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1595", null ],
        [ "Using RAII Scope Guard", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1596", null ]
      ] ],
      [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1597", [
        [ "OTLP Writer Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1598", null ],
        [ "Protocol Selection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1599", null ]
      ] ],
      [ "Trace Context", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1600", [
        [ "otel_context Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1601", null ],
        [ "Thread-Local Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1602", null ],
        [ "Context in Log Entries", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1603", null ]
      ] ],
      [ "OTLP Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1604", [
        [ "Statistics Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1605", null ],
        [ "Health Checking", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1606", null ],
        [ "Force Flush", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1607", null ]
      ] ],
      [ "Integration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1608", [
        [ "With OpenTelemetry Collector", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1609", null ],
        [ "Docker Compose Setup", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1610", null ],
        [ "W3C Trace Context Parsing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1611", null ]
      ] ],
      [ "Best Practices", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1612", [
        [ "1. Always Set Service Name", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1613", null ],
        [ "2. Use RAII Scope Guards", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1614", null ],
        [ "3. Configure Appropriate Batch Sizes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1615", null ],
        [ "4. Handle Collector Unavailability", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1616", null ],
        [ "5. Include Meaningful Resource Attributes", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1617", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1618", [
        [ "Logs Not Appearing in Collector", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1619", null ],
        [ "High Log Drop Rate", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1620", null ],
        [ "Connection Timeouts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1621", null ],
        [ "Missing Trace Context", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1622", null ]
      ] ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2OPENTELEMETRY.html#autotoc_md1623", null ]
    ] ],
    [ "PERFORMANCE.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE_8kr.html", [
      [ "Logger System 성능 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE_8kr.html#autotoc_md1624", null ]
    ] ],
    [ "PERFORMANCE", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html", [
      [ "Logger System Performance Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1627", [
        [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1628", null ],
        [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1629", [
          [ "Synchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1630", null ],
          [ "Asynchronous Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1631", null ]
        ] ],
        [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1632", [
          [ "Test Environment", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1633", null ],
          [ "Single Thread Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1634", null ],
          [ "Multi-threaded Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1635", null ],
          [ "Memory Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1636", null ]
        ] ],
        [ "Optimization Strategies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1637", [
          [ "1. Choose the Right Mode", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1638", null ],
          [ "2. Buffer Size Tuning", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1639", null ],
          [ "2.1 Batch Size Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1640", null ],
          [ "3. Level Filtering", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1641", null ],
          [ "4. Message Construction", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1642", null ],
          [ "5. Writer Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1643", [
            [ "Console Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1644", null ],
            [ "Custom High-Performance Writer", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1645", null ]
          ] ],
          [ "6. Batch Processing", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1646", null ]
        ] ],
        [ "Performance Anti-patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1647", [
          [ "1. Synchronous I/O in Hot Paths", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1648", null ],
          [ "2. Excessive String Formatting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1649", null ],
          [ "3. Logging in Tight Loops", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1650", null ]
        ] ],
        [ "Profiling and Monitoring", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1651", [
          [ "Built-in Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1652", null ],
          [ "External Profiling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1653", null ]
        ] ],
        [ "Best Practices Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1654", null ],
        [ "Platform-Specific Optimizations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1655", [
          [ "Linux", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1656", null ],
          [ "Windows", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1657", null ],
          [ "macOS", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2PERFORMANCE.html#autotoc_md1658", null ]
        ] ]
      ] ]
    ] ],
    [ "빠른 시작 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html", [
      [ "사전 요구사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1662", null ],
      [ "설치", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1663", [
        [ "1. 저장소 클론", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1664", null ],
        [ "2. 의존성 설치", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1665", null ],
        [ "3. 빌드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1666", null ],
        [ "4. 설치 확인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1667", null ]
      ] ],
      [ "첫 번째 로거 만들기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1669", [
        [ "애플리케이션 빌드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1670", null ]
      ] ],
      [ "핵심 개념", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1672", [
        [ "로거 빌더", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1673", null ],
        [ "라이터", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1674", null ],
        [ "로그 레벨", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1675", null ]
      ] ],
      [ "구성 템플릿", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1677", [
        [ "Production (프로덕션)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1678", null ],
        [ "Debug (디버그)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1679", null ],
        [ "High Performance (고성능)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1680", null ]
      ] ],
      [ "CMake 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1682", [
        [ "핵심 빌드 옵션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1683", null ],
        [ "기능 플래그", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1684", null ]
      ] ],
      [ "thread_system 통합 (선택 사항)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1686", [
        [ "빌드 시 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1687", null ],
        [ "런타임 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1688", null ]
      ] ],
      [ "일반적인 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1690", [
        [ "오류 처리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1691", null ],
        [ "여러 라이터 사용", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1692", null ],
        [ "정상 종료", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1693", null ]
      ] ],
      [ "다음 단계", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1695", null ],
      [ "문제 해결", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1697", [
        [ "일반적인 문제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START_8kr.html#autotoc_md1698", null ]
      ] ]
    ] ],
    [ "Quick Start Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html", [
      [ "Prerequisites", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1702", null ],
      [ "Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1703", [
        [ "1. Clone the Repositories", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1704", null ],
        [ "2. Install Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1705", null ],
        [ "3. Build", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1706", null ],
        [ "4. Verify Installation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1707", null ]
      ] ],
      [ "Your First Logger", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1709", [
        [ "Building Your Application", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1710", null ]
      ] ],
      [ "Key Concepts", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1712", [
        [ "Logger Builder", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1713", null ],
        [ "Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1714", null ],
        [ "Log Levels", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1715", null ]
      ] ],
      [ "Configuration Templates", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1717", [
        [ "Production", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1718", null ],
        [ "Debug", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1719", null ],
        [ "High Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1720", null ]
      ] ],
      [ "CMake Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1722", [
        [ "Core Build Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1723", null ],
        [ "Feature Flags", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1724", null ]
      ] ],
      [ "thread_system Integration (Optional)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1726", [
        [ "Enable at Build Time", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1727", null ],
        [ "Enable at Runtime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1728", null ]
      ] ],
      [ "Common Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1730", [
        [ "Error Handling", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1731", null ],
        [ "Multiple Writers", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1732", null ],
        [ "Graceful Shutdown", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1733", null ]
      ] ],
      [ "Next Steps", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1735", null ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1737", [
        [ "Common Issues", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2QUICK__START.html#autotoc_md1738", null ]
      ] ]
    ] ],
    [ "SECURITY.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html", [
      [ "보안 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1740", [
        [ "위협 모델 (범위)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1741", null ],
        [ "현재 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1742", null ],
        [ "중요한 제한사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1743", null ],
        [ "권장사항 (프로덕션)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1744", null ],
        [ "안전한 구성 체크리스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1745", null ],
        [ "로드맵", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY_8kr.html#autotoc_md1746", null ]
      ] ]
    ] ],
    [ "SECURITY", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html", [
      [ "Security Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1748", [
        [ "Threat Model (Scope)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1749", null ],
        [ "Current Capabilities", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1750", null ],
        [ "Important Limitations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1751", null ],
        [ "Recommendations (Production)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1752", null ],
        [ "Secure Configuration Checklist", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1753", null ],
        [ "Roadmap", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2SECURITY.html#autotoc_md1754", null ]
      ] ]
    ] ],
    [ "thread_system 통합 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html", [
      [ "개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1773", null ],
      [ "빠른 시작", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1774", [
        [ "빌드 시 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1775", null ],
        [ "런타임에 활성화", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1776", null ]
      ] ],
      [ "설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1777", [
        [ "CMake 옵션", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1778", null ],
        [ "컴파일 정의", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1779", null ]
      ] ],
      [ "의존성 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1780", [
        [ "양방향 의존성 위험 (Issue #252)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1781", null ],
        [ "권장 구성 매트릭스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1782", null ],
        [ "안전한 구성 예제", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1783", null ],
        [ "CMake 충돌 감지", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1784", null ]
      ] ],
      [ "API 레퍼런스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1785", [
        [ "thread_system_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1786", [
          [ "메서드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1787", null ]
        ] ],
        [ "async_backend_type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1788", null ],
        [ "컴파일 타임 감지", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1789", null ]
      ] ],
      [ "사용 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1790", [
        [ "패턴 1: 애플리케이션 전체 공유 풀", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1791", null ],
        [ "패턴 2: 사용자 정의 풀 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1792", null ],
        [ "패턴 3: 조건부 통합", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1793", null ]
      ] ],
      [ "스레드 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1794", null ],
      [ "성능 고려사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1795", [
        [ "thread_system 통합 사용 시기", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1796", null ],
        [ "오버헤드 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1797", null ]
      ] ],
      [ "문제 해결", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1798", [
        [ "thread_system 미발견", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1799", null ],
        [ "런타임 감지 미작동", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1800", null ]
      ] ],
      [ "관련 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM_8kr.html#autotoc_md1801", null ]
    ] ],
    [ "Async Integration Guide", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html", [
      [ "Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1804", null ],
      [ "Integration Approaches", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1805", [
        [ "Recommended: IExecutor Interface (v1.5.0+)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1806", null ],
        [ "Legacy: Direct thread_system Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1807", null ]
      ] ],
      [ "Quick Start", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1808", [
        [ "Enable at Build Time", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1809", null ],
        [ "Enable at Runtime", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1810", null ]
      ] ],
      [ "Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1811", [
        [ "CMake Options", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1812", null ],
        [ "Compile Definitions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1813", null ]
      ] ],
      [ "Dependency Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1814", [
        [ "Bidirectional Dependency Risk (Issue #252)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1815", null ],
        [ "Recommended Configuration Matrix", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1816", null ],
        [ "Safe Configuration Examples", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1817", null ],
        [ "CMake Conflict Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1818", null ]
      ] ],
      [ "API Reference", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1819", [
        [ "thread_system_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1820", [
          [ "Methods", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1821", null ]
        ] ],
        [ "async_backend_type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1822", null ],
        [ "Compile-time Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1823", null ]
      ] ],
      [ "Usage Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1824", [
        [ "Pattern 1: Application-wide Shared Pool", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1825", null ],
        [ "Pattern 2: Custom Pool Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1826", null ],
        [ "Pattern 3: Conditional Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1827", null ]
      ] ],
      [ "Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1828", null ],
      [ "Performance Considerations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1829", [
        [ "When to Use thread_system Integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1830", null ],
        [ "Overhead Comparison", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1831", null ]
      ] ],
      [ "Troubleshooting", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1832", [
        [ "thread_system not found", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1833", null ],
        [ "Runtime detection not working", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1834", null ]
      ] ],
      [ "IExecutor Integration API (v1.5.0+)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1835", [
        [ "executor_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1836", null ],
        [ "executor_type", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1837", null ],
        [ "standalone_executor", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1838", null ],
        [ "Compile-time Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1839", null ]
      ] ],
      [ "Migration from thread_system_integration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1840", null ],
      [ "Related Documentation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2integration_2THREAD__SYSTEM.html#autotoc_md1841", null ]
    ] ],
    [ "Logger System - 성능 기준 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html", [
      [ "시스템 정보", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1846", [
        [ "하드웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1847", null ],
        [ "소프트웨어 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1848", null ]
      ] ],
      [ "성능 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1850", [
        [ "로깅 Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1851", null ],
        [ "Latency (Async Mode)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1852", null ],
        [ "메모리 성능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1853", null ]
      ] ],
      [ "벤치마크 결과", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1855", null ],
      [ "주요 특징", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1857", null ],
      [ "Baseline 검증", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1859", [
        [ "Phase 0 요구사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1860", null ],
        [ "수락 기준", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE_8kr.html#autotoc_md1861", null ]
      ] ]
    ] ],
    [ "Logger System - Performance Baseline Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html", [
      [ "</blockquote>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1864", null ],
      [ "System Information", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1866", [
        [ "Hardware Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1867", null ],
        [ "Software Configuration", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1868", null ]
      ] ],
      [ "Performance Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1870", [
        [ "Logging Throughput", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1871", null ],
        [ "Latency (Async Mode)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1872", null ],
        [ "Memory Performance", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1873", null ]
      ] ],
      [ "Benchmark Results", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1875", null ],
      [ "Key Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1877", null ],
      [ "Baseline Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1879", [
        [ "Phase 0 Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1880", null ],
        [ "Acceptance Criteria", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html#autotoc_md1881", null ]
      ] ]
    ] ],
    [ "CI/CD 성능 지표 자동화 제안서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html", [
      [ "📊 Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1885", null ],
      [ "🔍 현재 상태 분석", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1887", [
        [ "✅ 장점", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1888", null ],
        [ "❌ 개선 필요 사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1889", null ]
      ] ],
      [ "🎯 제안 솔루션 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1891", [
        [ "전체 워크플로우", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1892", null ]
      ] ],
      [ "📁 제안 디렉토리 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1894", null ],
      [ "🛠️ 구현 상세 (4단계)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1896", [
        [ "Phase 1: 벤치마크 재활성화 및 JSON 출력 설정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1897", [
          [ "1.1 GitHub Actions 워크플로우 수정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1898", null ],
          [ "1.2 벤치마크 실행 및 JSON 저장", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1899", null ],
          [ "1.3 API 변경에 따른 벤치마크 코드 수정", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1900", null ]
        ] ],
        [ "Phase 2: 결과 파싱 및 기준선 비교", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1902", [
          [ "2.1 벤치마크 결과 파싱 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1903", null ],
          [ "2.2 기준선 비교 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1904", null ]
        ] ],
        [ "Phase 3: 배지 및 차트 생성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1906", [
          [ "3.1 성능 배지 생성 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1907", null ],
          [ "3.2 트렌드 차트 생성 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1908", null ]
        ] ],
        [ "Phase 4: README.md 자동 업데이트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1910", [
          [ "4.1 README 업데이트 스크립트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1911", null ],
          [ "4.2 GitHub Actions에서 자동 커밋", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1912", null ]
        ] ]
      ] ],
      [ "🎨 README.md 성능 섹션 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1914", [
        [ "제안 1: 배지 중심 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1915", null ],
        [ "제안 2: 테이블 중심 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1916", null ],
        [ "제안 3: 카드 스타일 디자인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1917", null ]
      ] ],
      [ "🔄 CI/CD 워크플로우 전체 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1919", null ],
      [ "📈 예상 결과물", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1921", [
        [ "1. GitHub Actions 실행 결과", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1922", null ],
        [ "2. PR 자동 코멘트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1923", null ],
        [ "3. 업데이트된 README.md", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1924", null ]
      ] ],
      [ "✅ 구현 체크리스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1926", [
        [ "Phase 1: 인프라 구축 (예상 소요: 2시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1927", null ],
        [ "Phase 2: 배지 및 차트 (예상 소요: 1.5시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1928", null ],
        [ "Phase 3: README 자동 업데이트 (예상 소요: 1시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1929", null ],
        [ "Phase 4: CI/CD 통합 (예상 소요: 2시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1930", null ],
        [ "Phase 5: 테스트 및 검증 (예상 소요: 1.5시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1931", null ],
        [ "Phase 6: 문서화 (예상 소요: 1시간)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1932", null ]
      ] ],
      [ "🔧 트러블슈팅 가이드", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1934", [
        [ "문제 1: 벤치마크 빌드 실패", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1935", null ],
        [ "문제 2: Python 스크립트 실행 오류", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1936", null ],
        [ "문제 3: Git 자동 커밋 권한 오류", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1937", null ],
        [ "문제 4: 배지가 README에 표시되지 않음", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1938", null ]
      ] ],
      [ "📚 참고 자료", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1940", [
        [ "도구 및 라이브러리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1941", null ],
        [ "관련 프로젝트 예시", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1942", null ],
        [ "내부 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1943", null ]
      ] ],
      [ "🎯 성공 지표", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1945", null ],
      [ "📝 다음 단계", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2CI__CD__PERFORMANCE__PROPOSAL.html#autotoc_md1947", null ]
    ] ],
    [ "Logger System 프로덕션 품질", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html", [
      [ "요약", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1951", [
        [ "개발 상태: ✅ <strong>개발 중</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1952", null ]
      ] ],
      [ "빌드 & 테스팅 인프라", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1954", [
        [ "지속적 통합 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1955", [
          [ "1. 메인 CI 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1956", null ],
          [ "2. 새니타이저 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1957", null ],
          [ "3. 성능 벤치마크 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1958", null ],
          [ "4. 코드 커버리지 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1959", null ],
          [ "5. 정적 분석 파이프라인", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1960", null ]
        ] ]
      ] ],
      [ "스레드 안전성 & 동시성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1962", [
        [ "스레드 안전성 등급: <strong>A+</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1963", null ],
        [ "Writer 스레드 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1964", null ],
        [ "비동기 처리 아키텍처", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1965", null ]
      ] ],
      [ "리소스 관리 (RAII)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1967", [
        [ "RAII 준수 등급: <strong>A</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1968", null ],
        [ "스마트 포인터 사용", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1969", null ],
        [ "메모리 누수 탐지", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1970", null ],
        [ "예외 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1971", null ]
      ] ],
      [ "오류 처리 전략", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1973", [
        [ "오류 처리 등급: <strong>A</strong> (90% 완료)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1974", null ],
        [ "Result<T> 패턴", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1975", null ],
        [ "오류 코드 레지스트리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1976", null ]
      ] ],
      [ "성능 특성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1978", [
        [ "기준선 메트릭", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1979", null ]
      ] ],
      [ "보안 기능", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1981", [
        [ "보안 등급: <strong>A</strong> (v3.0.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1982", null ],
        [ "1. 보안 키 저장소", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1983", null ],
        [ "2. 경로 검증", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1984", null ],
        [ "3. 시그널 핸들러 안전성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1985", null ],
        [ "4. 보안 감사 로깅", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1986", null ],
        [ "5. 컴플라이언스 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1987", null ]
      ] ],
      [ "플랫폼 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1989", [
        [ "공식 지원 플랫폼", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1990", null ],
        [ "최소 요구사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY_8kr.html#autotoc_md1991", null ]
      ] ]
    ] ],
    [ "Logger System Production Quality", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md1997", null ],
      [ "Executive Summary", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md1999", [
        [ "Quality Status", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2000", null ]
      ] ],
      [ "Build & Testing Infrastructure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2002", [
        [ "Continuous Integration Pipelines", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2003", [
          [ "1. Main CI Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2004", null ],
          [ "2. Sanitizer Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2006", null ],
          [ "3. Performance Benchmark Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2008", null ],
          [ "4. Code Coverage Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2010", null ],
          [ "5. Static Analysis Pipeline", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2012", null ]
        ] ],
        [ "Test Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2014", [
          [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2015", null ],
          [ "Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2017", null ],
          [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2019", null ]
        ] ]
      ] ],
      [ "Thread Safety & Concurrency", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2021", [
        [ "Thread Safety Grade: <strong>A+</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2022", null ],
        [ "Writer Thread Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2023", null ],
        [ "Asynchronous Processing Architecture", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2025", null ],
        [ "Concurrency Patterns", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2027", null ]
      ] ],
      [ "Resource Management (RAII)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2029", [
        [ "RAII Compliance Grade: <strong>A</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2030", null ],
        [ "Smart Pointer Usage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2031", null ],
        [ "Memory Leak Detection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2033", null ],
        [ "Exception Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2035", null ]
      ] ],
      [ "Error Handling Strategy", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2037", [
        [ "Error Handling Grade: <strong>A</strong> (90% Complete)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2038", null ],
        [ "Result<T> Pattern", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2039", null ],
        [ "Error Code Registry", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2041", null ],
        [ "Hot Path Optimization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2043", null ]
      ] ],
      [ "Performance Characteristics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2045", [
        [ "Baseline Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2046", null ],
        [ "Performance Baselines (CI/CD)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2048", null ]
      ] ],
      [ "Security Features", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2050", [
        [ "Security Grade: <strong>A</strong> (v3.0.0)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2051", null ],
        [ "1. Secure Key Storage", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2052", null ],
        [ "2. Path Validation", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2053", null ],
        [ "3. Signal Handler Safety", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2054", null ],
        [ "4. Security Audit Logging", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2055", null ],
        [ "5. Compliance Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2056", null ],
        [ "6. OWASP Top 10 Mitigations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2057", null ]
      ] ],
      [ "Operational Metrics", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2059", [
        [ "Built-in Metrics Collection", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2060", null ],
        [ "Health Checks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2062", null ]
      ] ],
      [ "Platform Support", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2064", [
        [ "Officially Supported Platforms", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2065", null ],
        [ "Minimum Requirements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2066", null ],
        [ "Cross-Platform Compatibility", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2068", null ]
      ] ],
      [ "Remaining Work & Future Enhancements", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2070", [
        [ "Current Status: <strong>90% Production Ready</strong>", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2071", null ]
      ] ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PRODUCTION__QUALITY.html#autotoc_md2073", null ]
    ] ],
    [ "Logger System 프로젝트 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html", [
      [ "목차", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2076", null ],
      [ "디렉토리 개요", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2078", null ],
      [ "코어 모듈 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2080", [
        [ "logger.h / logger.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2081", null ],
        [ "logger_builder.h / logger_builder.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2083", null ],
        [ "log_entry.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2085", null ],
        [ "result_types.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2087", null ]
      ] ],
      [ "Writer 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2089", [
        [ "console_writer.h / console_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2090", null ],
        [ "file_writer.h / file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2092", null ],
        [ "rotating_file_writer.h / rotating_file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2094", null ],
        [ "network_writer.h / network_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2096", null ],
        [ "critical_writer.h / critical_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2098", null ],
        [ "hybrid_writer.h / hybrid_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2100", null ]
      ] ],
      [ "Filter 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2102", [
        [ "level_filter.h / level_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2103", null ],
        [ "regex_filter.h / regex_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2105", null ],
        [ "function_filter.h / function_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2107", null ]
      ] ],
      [ "Formatter 구현", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2109", [
        [ "plain_formatter.h / plain_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2110", null ],
        [ "json_formatter.h / json_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2112", null ],
        [ "custom_formatter.h / custom_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2114", null ]
      ] ],
      [ "설정 시스템", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2116", [
        [ "config_templates.h / config_templates.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2117", null ],
        [ "config_validator.h / config_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2119", null ]
      ] ],
      [ "보안 컴포넌트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2121", [
        [ "secure_key_storage.h / secure_key_storage.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2122", null ],
        [ "path_validator.h / path_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2124", null ],
        [ "log_sanitizer.h / log_sanitizer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2126", null ]
      ] ],
      [ "테스팅 구성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2128", [
        [ "유닛 테스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2129", null ],
        [ "통합 테스트", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2131", null ],
        [ "벤치마크", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2133", null ]
      ] ],
      [ "빌드 시스템", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2135", [
        [ "CMakeLists.txt (루트)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2136", null ]
      ] ],
      [ "모듈 의존성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2138", [
        [ "의존성 그래프", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2139", null ],
        [ "외부 의존성", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2140", null ]
      ] ],
      [ "파일 명명 규칙", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2142", [
        [ "헤더 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2143", null ],
        [ "소스 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2144", null ],
        [ "문서 파일", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2145", null ]
      ] ],
      [ "참고 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE_8kr.html#autotoc_md2147", null ]
    ] ],
    [ "Logger System Project Structure", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html", [
      [ "Table of Contents", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2152", null ],
      [ "Directory Overview", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2154", null ],
      [ "Core Module Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2156", [
        [ "logger.h / logger.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2157", null ],
        [ "logger_builder.h / logger_builder.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2159", null ],
        [ "log_entry.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2161", null ],
        [ "result_types.h", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2163", null ]
      ] ],
      [ "Writer Implementations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2165", [
        [ "console_writer.h / console_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2166", null ],
        [ "file_writer.h / file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2168", null ],
        [ "rotating_file_writer.h / rotating_file_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2170", null ],
        [ "network_writer.h / network_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2172", null ],
        [ "critical_writer.h / critical_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2174", null ],
        [ "hybrid_writer.h / hybrid_writer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2176", null ]
      ] ],
      [ "Filter Implementations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2178", [
        [ "level_filter.h / level_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2179", null ],
        [ "regex_filter.h / regex_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2181", null ],
        [ "function_filter.h / function_filter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2183", null ]
      ] ],
      [ "Formatter Implementations", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2185", [
        [ "plain_formatter.h / plain_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2186", null ],
        [ "json_formatter.h / json_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2188", null ],
        [ "custom_formatter.h / custom_formatter.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2190", null ]
      ] ],
      [ "Configuration System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2192", [
        [ "config_templates.h / config_templates.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2193", null ],
        [ "config_validator.h / config_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2195", null ],
        [ "config_strategy.h / config_strategy.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2197", null ]
      ] ],
      [ "Security Components", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2199", [
        [ "secure_key_storage.h / secure_key_storage.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2200", null ],
        [ "path_validator.h / path_validator.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2202", null ],
        [ "log_sanitizer.h / log_sanitizer.cpp", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2204", null ]
      ] ],
      [ "Testing Organization", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2206", [
        [ "Unit Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2207", null ],
        [ "Integration Tests", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2209", null ],
        [ "Benchmarks", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2211", null ]
      ] ],
      [ "Build System", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2213", [
        [ "CMakeLists.txt (Root)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2214", null ],
        [ "CMake Modules", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2216", null ]
      ] ],
      [ "Module Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2218", [
        [ "Dependency Graph", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2219", null ],
        [ "External Dependencies", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2220", null ]
      ] ],
      [ "File Naming Conventions", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2222", [
        [ "Header Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2223", null ],
        [ "Source Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2224", null ],
        [ "Documentation Files", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2225", null ]
      ] ],
      [ "See Also", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2PROJECT__STRUCTURE.html#autotoc_md2227", null ]
    ] ],
    [ "README.kr", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html", [
      [ "Logger System 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2228", [
        [ "📚 문서 구조", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2229", [
          [ "핵심 문서", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2230", null ],
          [ "가이드 (<tt>docs/guides/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2231", null ],
          [ "고급 주제 (<tt>docs/advanced/</tt>)", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2232", null ]
        ] ],
        [ "🚀 빠른 탐색", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2233", [
          [ "신규 사용자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2234", null ],
          [ "기존 사용자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2235", null ],
          [ "기여자", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2236", null ],
          [ "성능 엔지니어", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2237", null ]
        ] ],
        [ "🔧 문서 유지관리", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2238", [
          [ "최근 변경사항", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2239", null ]
        ] ],
        [ "📖 외부 리소스", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2240", null ],
        [ "🤝 도움 및 지원", "md__2home_2runner_2work_2logger__system_2logger__system_2docs_2README_8kr.html#autotoc_md2241", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"index.html",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2BENCHMARKS_8kr.html#autotoc_md868",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2FEATURES_8kr.html#autotoc_md1223",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2ASYNC__WRITERS.html",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2advanced_2CUSTOM__WRITERS.html#autotoc_md430",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2contributing_2CONTRIBUTING.html#autotoc_md1158",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2guides_2MIGRATION__GUIDE.html#autotoc_md1578",
"md__2home_2runner_2work_2logger__system_2logger__system_2docs_2performance_2BASELINE.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';