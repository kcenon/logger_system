---
doc_id: "LOG-PERF-007"
doc_title: "CI/CD 성능 지표 자동화 제안서"
doc_version: "1.0.0"
doc_date: "2026-04-04"
doc_status: "Released"
project: "logger_system"
category: "PERF"
---

# CI/CD 성능 지표 자동화 제안서

> **SSOT**: This document is the single source of truth for **CI/CD 성능 지표 자동화 제안서**.

**문서 버전:** 1.0
**작성일:** 2025-11-04
**프로젝트:** logger_system
**목적:** 실시간 성능 지표 추적 및 README 자동 업데이트

---

## 📊 Executive Summary

logger_system의 성능 지표를 CI/CD 파이프라인을 통해 자동으로 수집하고, README.md 및 문서에 동적으로 표시하는 시스템을 구축합니다.

**기대 효과:**
- ✅ 모든 커밋에서 자동 성능 측정
- ✅ 성능 회귀 즉시 감지 (±5% 임계값)
- ✅ README.md에 실시간 성능 배지 표시
- ✅ 히스토리컬 트렌드 차트 자동 생성
- ✅ 벤치마크 결과 아티팩트로 보관

---

## 🔍 현재 상태 분석

### ✅ 장점
1. **벤치마크 인프라 완비**
   - Google Benchmark 기반 4개 벤치마크 파일
   - `logger_throughput_bench.cpp`: 처리량 측정
   - `logger_async_bench.cpp`: 비동기 성능
   - `logger_write_bench.cpp`: 쓰기 지연시간
   - `logger_rotation_bench.cpp`: 파일 회전 성능

2. **문서화 구조 우수**
   - `BASELINE.md`: 기준 성능 지표 기록
   - `benchmarks/README.md`: 벤치마크 사용법 문서화

3. **GitHub Actions 워크플로우 존재**
   - `.github/workflows/benchmarks.yml` 이미 구현

### ❌ 개선 필요 사항

1. **벤치마크 비활성화**
   ```yaml
   # 현재 상태 (.github/workflows/benchmarks.yml:68)
   -DBUILD_BENCHMARKS=OFF  # ❌ 비활성화됨
   ```
   - 이유: "API changes" (API 변경)
   - 영향: 성능 측정 불가

2. **하드코딩된 성능 지표**
   ```markdown
   <!-- README.md:150-153 -->
   - **Peak Throughput**: Up to 4.34M messages/second
   - **Multi-threaded Performance**:
     - 4 threads: 1.07M messages/s
   ```
   - 수동 업데이트 필요
   - 최신성 보장 불가

3. **성능 회귀 감지 미구현**
   ```yaml
   # benchmarks.yml:113-115
   - name: Check for performance regression
     run: |
       echo "Performance regression detection: TBD"
   ```

4. **시각화 부재**
   - 성능 트렌드 차트 없음
   - 배지(badge) 없음
   - 비교 그래프 없음

---

## 🎯 제안 솔루션 아키텍처

### 전체 워크플로우

```
┌──────────────────────────────────────────────────────────────────────┐
│                       GitHub Actions Trigger                          │
│  (Push to main, PR, Manual dispatch, Scheduled daily)                │
└────────────────────────┬─────────────────────────────────────────────┘
                         │
                         ▼
┌──────────────────────────────────────────────────────────────────────┐
│ Step 1: Build & Run Benchmarks                                       │
│  - Build logger_system with -DBUILD_BENCHMARKS=ON                    │
│  - Execute: ./build/benchmarks/logger_benchmarks                     │
│  - Output: benchmark_results.json (Google Benchmark JSON format)     │
└────────────────────────┬─────────────────────────────────────────────┘
                         │
                         ▼
┌──────────────────────────────────────────────────────────────────────┐
│ Step 2: Parse & Extract Metrics                                      │
│  - Parse JSON with Python script                                     │
│  - Extract key metrics:                                               │
│    * Throughput (messages/second)                                    │
│    * Latency (P50, P95, P99)                                         │
│    * Memory usage                                                     │
│  - Generate summary.json                                              │
└────────────────────────┬─────────────────────────────────────────────┘
                         │
                         ▼
┌──────────────────────────────────────────────────────────────────────┐
│ Step 3: Compare with Baseline                                        │
│  - Load baseline from benchmarks/baselines/baseline.json             │
│  - Calculate delta percentage                                         │
│  - Check regression threshold (±5%)                                   │
│  - Generate comparison report                                         │
└────────────────────────┬─────────────────────────────────────────────┘
                         │
                         ▼
┌──────────────────────────────────────────────────────────────────────┐
│ Step 4: Generate Visualizations                                      │
│  - Create performance badges (shields.io format)                     │
│  - Generate trend charts (Chart.js/matplotlib)                       │
│  - Update performance_history.json                                   │
└────────────────────────┬─────────────────────────────────────────────┘
                         │
                         ▼
┌──────────────────────────────────────────────────────────────────────┐
│ Step 5: Update Documentation                                         │
│  - Update README.md performance section                              │
│  - Update BASELINE.md if new baseline                                │
│  - Commit changes (bot account)                                       │
└────────────────────────┬─────────────────────────────────────────────┘
                         │
                         ▼
┌──────────────────────────────────────────────────────────────────────┐
│ Step 6: Upload Artifacts & Report                                    │
│  - Upload benchmark_results.json                                     │
│  - Upload comparison_report.md                                        │
│  - Upload trend_charts.png                                            │
│  - Post PR comment with results                                       │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 📁 제안 디렉토리 구조

```
logger_system/
├── .github/
│   └── workflows/
│       ├── benchmarks.yml              # 업데이트됨
│       └── update-performance-badges.yml  # 신규
│
├── benchmarks/
│   ├── baselines/
│   │   ├── baseline.json               # 공식 기준점
│   │   └── history/                    # 히스토리컬 데이터
│   │       ├── 2025-11-04.json
│   │       └── ...
│   └── results/                        # 신규 (자동 생성)
│       └── latest.json
│
├── docs/
│   └── performance/
│       ├── badges/                     # 신규 (자동 생성)
│       │   ├── throughput.svg
│       │   ├── latency.svg
│       │   └── memory.svg
│       ├── charts/                     # 신규 (자동 생성)
│       │   ├── throughput_trend.png
│       │   └── latency_trend.png
│       └── performance_history.json    # 신규 (시계열 데이터)
│
└── scripts/
    └── performance/                    # 신규
        ├── parse_benchmarks.py         # JSON 파싱
        ├── compare_baselines.py        # 회귀 검사
        ├── generate_badges.py          # 배지 생성
        ├── generate_charts.py          # 차트 생성
        └── update_readme.py            # README 업데이트
```

---

## 🛠️ 구현 상세 (4단계)

### Phase 1: 벤치마크 재활성화 및 JSON 출력 설정

#### 1.1 GitHub Actions 워크플로우 수정

**파일:** `.github/workflows/benchmarks.yml`

**변경 사항:**

```yaml
# Before
-DBUILD_BENCHMARKS=OFF

# After
-DBUILD_BENCHMARKS=ON
-DLOGGER_STANDALONE=ON
-DCMAKE_BUILD_TYPE=Release
```

#### 1.2 벤치마크 실행 및 JSON 저장

```yaml
- name: Run Benchmarks
  run: |
    cd build/benchmarks

    # 벤치마크 실행 및 JSON 출력
    ./logger_benchmarks \
      --benchmark_format=json \
      --benchmark_out=benchmark_results.json \
      --benchmark_repetitions=10 \
      --benchmark_report_aggregates_only=true \
      --benchmark_min_time=5.0

    # 결과 복사
    cp benchmark_results.json ../../benchmarks/results/latest.json

- name: Upload Benchmark Results
  uses: actions/upload-artifact@v4
  with:
    name: benchmark-results-${{ matrix.os }}
    path: benchmarks/results/latest.json
    retention-days: 90
```

#### 1.3 API 변경에 따른 벤치마크 코드 수정

**필요 작업:** 벤치마크 코드가 새 API와 호환되도록 업데이트

```cpp
// 예시: logger_throughput_bench.cpp
// Before (구 API)
auto logger = std::make_shared<logger_module::logger>();

// After (신 API)
auto logger = kcenon::logger::logger_builder()
    .use_template("production")
    .add_writer("null", std::make_unique<kcenon::logger::null_writer>())
    .build()
    .value();
```

---

### Phase 2: 결과 파싱 및 기준선 비교

#### 2.1 벤치마크 결과 파싱 스크립트

**파일:** `scripts/performance/parse_benchmarks.py`

```python
#!/usr/bin/env python3
"""
Parse Google Benchmark JSON output and extract key metrics.
"""
import json
import sys
from pathlib import Path
from typing import Dict, Any


def extract_metrics(benchmark_json: Dict[str, Any]) -> Dict[str, Any]:
    """Extract key performance metrics from benchmark results."""

    metrics = {
        "throughput": {},
        "latency": {},
        "memory": {},
        "timestamp": benchmark_json.get("context", {}).get("date", "")
    }

    for benchmark in benchmark_json.get("benchmarks", []):
        name = benchmark["name"]

        # Throughput benchmarks
        if "Throughput" in name:
            if "items_per_second" in benchmark:
                metrics["throughput"][name] = {
                    "messages_per_second": benchmark["items_per_second"],
                    "cpu_time": benchmark["cpu_time"],
                    "real_time": benchmark["real_time"]
                }

        # Latency benchmarks
        elif "Latency" in name or "WriteLatency" in name:
            metrics["latency"][name] = {
                "cpu_time_ns": benchmark["cpu_time"],
                "real_time_ns": benchmark["real_time"]
            }

        # Memory benchmarks
        elif "Memory" in name:
            metrics["memory"][name] = {
                "bytes_allocated": benchmark.get("bytes_per_second", 0)
            }

    return metrics


def calculate_summary(metrics: Dict[str, Any]) -> Dict[str, Any]:
    """Calculate summary statistics."""

    summary = {}

    # Max throughput
    if metrics["throughput"]:
        max_throughput = max(
            m["messages_per_second"]
            for m in metrics["throughput"].values()
        )
        summary["peak_throughput_msg_per_sec"] = round(max_throughput)

    # Average latency
    if metrics["latency"]:
        avg_latency = sum(
            m["cpu_time_ns"]
            for m in metrics["latency"].values()
        ) / len(metrics["latency"])
        summary["avg_latency_ns"] = round(avg_latency, 2)

    return summary


def main():
    if len(sys.argv) < 2:
        print("Usage: parse_benchmarks.py <benchmark_results.json>")
        sys.exit(1)

    input_file = Path(sys.argv[1])
    output_file = Path("benchmarks/results/summary.json")

    with open(input_file) as f:
        benchmark_data = json.load(f)

    metrics = extract_metrics(benchmark_data)
    summary = calculate_summary(metrics)

    result = {
        "summary": summary,
        "detailed_metrics": metrics
    }

    output_file.parent.mkdir(parents=True, exist_ok=True)
    with open(output_file, 'w') as f:
        json.dump(result, f, indent=2)

    print(f"✅ Metrics extracted to {output_file}")
    print(f"📊 Summary: {summary}")


if __name__ == "__main__":
    main()
```

#### 2.2 기준선 비교 스크립트

**파일:** `scripts/performance/compare_baselines.py`

```python
#!/usr/bin/env python3
"""
Compare current benchmark results with baseline and detect regressions.
"""
import json
import sys
from pathlib import Path
from typing import Dict, Any, Tuple


# Regression threshold: 5%
REGRESSION_THRESHOLD = 0.05


def load_json(file_path: Path) -> Dict[str, Any]:
    """Load JSON file."""
    with open(file_path) as f:
        return json.load(f)


def compare_metrics(
    baseline: Dict[str, Any],
    current: Dict[str, Any]
) -> Tuple[bool, Dict[str, Any]]:
    """
    Compare current metrics with baseline.

    Returns:
        (has_regression, comparison_report)
    """

    report = {
        "regression_detected": False,
        "improvements": [],
        "regressions": [],
        "stable": []
    }

    baseline_summary = baseline.get("summary", {})
    current_summary = current.get("summary", {})

    for metric_name, baseline_value in baseline_summary.items():
        current_value = current_summary.get(metric_name)

        if current_value is None:
            continue

        # Calculate percentage change
        delta = (current_value - baseline_value) / baseline_value
        delta_percent = delta * 100

        comparison = {
            "metric": metric_name,
            "baseline": baseline_value,
            "current": current_value,
            "delta_percent": round(delta_percent, 2)
        }

        # For throughput: higher is better
        if "throughput" in metric_name.lower():
            if delta < -REGRESSION_THRESHOLD:  # Significant decrease
                report["regressions"].append(comparison)
                report["regression_detected"] = True
            elif delta > REGRESSION_THRESHOLD:  # Significant increase
                report["improvements"].append(comparison)
            else:
                report["stable"].append(comparison)

        # For latency: lower is better
        elif "latency" in metric_name.lower():
            if delta > REGRESSION_THRESHOLD:  # Significant increase
                report["regressions"].append(comparison)
                report["regression_detected"] = True
            elif delta < -REGRESSION_THRESHOLD:  # Significant decrease
                report["improvements"].append(comparison)
            else:
                report["stable"].append(comparison)

    return report["regression_detected"], report


def generate_markdown_report(report: Dict[str, Any]) -> str:
    """Generate Markdown report."""

    md = ["# Performance Comparison Report\n"]

    if report["regression_detected"]:
        md.append("## ⚠️ Performance Regressions Detected\n")
        for item in report["regressions"]:
            md.append(f"- **{item['metric']}**: "
                     f"{item['baseline']} → {item['current']} "
                     f"({item['delta_percent']:+.2f}%)\n")
    else:
        md.append("## ✅ No Performance Regressions\n")

    if report["improvements"]:
        md.append("\n## 🚀 Performance Improvements\n")
        for item in report["improvements"]:
            md.append(f"- **{item['metric']}**: "
                     f"{item['baseline']} → {item['current']} "
                     f"({item['delta_percent']:+.2f}%)\n")

    if report["stable"]:
        md.append("\n## ⚖️ Stable Metrics\n")
        for item in report["stable"]:
            md.append(f"- **{item['metric']}**: "
                     f"{item['current']} "
                     f"({item['delta_percent']:+.2f}%)\n")

    return "".join(md)


def main():
    baseline_file = Path("benchmarks/baselines/baseline.json")
    current_file = Path("benchmarks/results/summary.json")
    report_file = Path("benchmarks/results/comparison_report.md")

    baseline = load_json(baseline_file)
    current = load_json(current_file)

    has_regression, report = compare_metrics(baseline, current)

    # Save JSON report
    report_json_file = Path("benchmarks/results/comparison_report.json")
    with open(report_json_file, 'w') as f:
        json.dump(report, f, indent=2)

    # Save Markdown report
    md_report = generate_markdown_report(report)
    with open(report_file, 'w') as f:
        f.write(md_report)

    print(md_report)

    # Exit with error code if regression detected
    if has_regression:
        print("\n❌ Performance regression detected!")
        sys.exit(1)
    else:
        print("\n✅ No performance regression!")
        sys.exit(0)


if __name__ == "__main__":
    main()
```

---

### Phase 3: 배지 및 차트 생성

#### 3.1 성능 배지 생성 스크립트

**파일:** `scripts/performance/generate_badges.py`

```python
#!/usr/bin/env python3
"""
Generate performance badges using shields.io format.
"""
import json
from pathlib import Path
from typing import Dict, Any


def format_throughput(value: float) -> str:
    """Format throughput value."""
    if value >= 1_000_000:
        return f"{value / 1_000_000:.2f}M"
    elif value >= 1_000:
        return f"{value / 1_000:.1f}K"
    else:
        return f"{value:.0f}"


def format_latency(value_ns: float) -> str:
    """Format latency value."""
    if value_ns >= 1_000_000:
        return f"{value_ns / 1_000_000:.2f}ms"
    elif value_ns >= 1_000:
        return f"{value_ns / 1_000:.2f}μs"
    else:
        return f"{value_ns:.0f}ns"


def get_badge_color(metric_name: str, value: float) -> str:
    """Determine badge color based on performance."""

    # Throughput thresholds (messages/sec)
    if "throughput" in metric_name.lower():
        if value >= 4_000_000:
            return "brightgreen"
        elif value >= 2_000_000:
            return "green"
        elif value >= 1_000_000:
            return "yellow"
        else:
            return "orange"

    # Latency thresholds (nanoseconds)
    elif "latency" in metric_name.lower():
        if value <= 500:
            return "brightgreen"
        elif value <= 1000:
            return "green"
        elif value <= 5000:
            return "yellow"
        else:
            return "orange"

    return "blue"


def generate_badge_url(label: str, value: str, color: str) -> str:
    """Generate shields.io badge URL."""
    return (
        f"https://img.shields.io/badge/"
        f"{label.replace(' ', '%20')}-"
        f"{value.replace(' ', '%20')}-"
        f"{color}"
    )


def generate_static_badge_svg(
    label: str,
    value: str,
    color: str,
    output_file: Path
):
    """Generate static SVG badge file."""

    # Simple SVG badge template
    svg_template = f"""<svg xmlns="http://www.w3.org/2000/svg" width="160" height="20">
  <linearGradient id="b" x2="0" y2="100%">
    <stop offset="0" stop-color="#bbb" stop-opacity=".1"/>
    <stop offset="1" stop-opacity=".1"/>
  </linearGradient>
  <mask id="a">
    <rect width="160" height="20" rx="3" fill="#fff"/>
  </mask>
  <g mask="url(#a)">
    <path fill="#555" d="M0 0h90v20H0z"/>
    <path fill="#{get_color_hex(color)}" d="M90 0h70v20H90z"/>
    <path fill="url(#b)" d="M0 0h160v20H0z"/>
  </g>
  <g fill="#fff" text-anchor="middle" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="11">
    <text x="45" y="15" fill="#010101" fill-opacity=".3">{label}</text>
    <text x="45" y="14">{label}</text>
    <text x="125" y="15" fill="#010101" fill-opacity=".3">{value}</text>
    <text x="125" y="14">{value}</text>
  </g>
</svg>"""

    with open(output_file, 'w') as f:
        f.write(svg_template)


def get_color_hex(color_name: str) -> str:
    """Convert color name to hex."""
    colors = {
        "brightgreen": "4c1",
        "green": "97ca00",
        "yellow": "dfb317",
        "orange": "fe7d37",
        "red": "e05d44",
        "blue": "007ec6"
    }
    return colors.get(color_name, "007ec6")


def main():
    summary_file = Path("benchmarks/results/summary.json")
    badges_dir = Path("docs/performance/badges")
    badges_dir.mkdir(parents=True, exist_ok=True)

    with open(summary_file) as f:
        data = json.load(f)

    summary = data.get("summary", {})

    # Generate badges
    badges_info = []

    # Throughput badge
    if "peak_throughput_msg_per_sec" in summary:
        throughput = summary["peak_throughput_msg_per_sec"]
        value_str = format_throughput(throughput) + " msg/s"
        color = get_badge_color("throughput", throughput)

        generate_static_badge_svg(
            "throughput",
            value_str,
            color,
            badges_dir / "throughput.svg"
        )

        badges_info.append({
            "name": "Throughput",
            "file": "docs/performance/badges/throughput.svg",
            "url": generate_badge_url("throughput", value_str, color)
        })

    # Latency badge
    if "avg_latency_ns" in summary:
        latency = summary["avg_latency_ns"]
        value_str = format_latency(latency)
        color = get_badge_color("latency", latency)

        generate_static_badge_svg(
            "latency",
            value_str,
            color,
            badges_dir / "latency.svg"
        )

        badges_info.append({
            "name": "Latency",
            "file": "docs/performance/badges/latency.svg",
            "url": generate_badge_url("latency", value_str, color)
        })

    # Save badges info
    badges_json = badges_dir / "badges.json"
    with open(badges_json, 'w') as f:
        json.dump(badges_info, f, indent=2)

    print("✅ Performance badges generated:")
    for badge in badges_info:
        print(f"  - {badge['name']}: {badge['file']}")


if __name__ == "__main__":
    main()
```

#### 3.2 트렌드 차트 생성 스크립트

**파일:** `scripts/performance/generate_charts.py`

```python
#!/usr/bin/env python3
"""
Generate performance trend charts using matplotlib.
"""
import json
from pathlib import Path
from datetime import datetime
import matplotlib.pyplot as plt
import matplotlib.dates as mdates


def load_performance_history() -> dict:
    """Load performance history from JSON file."""
    history_file = Path("docs/performance/performance_history.json")

    if not history_file.exists():
        return {"history": []}

    with open(history_file) as f:
        return json.load(f)


def append_to_history(summary: dict):
    """Append current summary to performance history."""
    history_file = Path("docs/performance/performance_history.json")

    history = load_performance_history()

    entry = {
        "timestamp": datetime.now().isoformat(),
        "metrics": summary
    }

    history["history"].append(entry)

    # Keep last 90 days
    if len(history["history"]) > 90:
        history["history"] = history["history"][-90:]

    with open(history_file, 'w') as f:
        json.dump(history, f, indent=2)


def plot_throughput_trend(history: dict, output_file: Path):
    """Plot throughput trend over time."""

    timestamps = []
    throughputs = []

    for entry in history["history"]:
        if "peak_throughput_msg_per_sec" in entry["metrics"]:
            timestamps.append(datetime.fromisoformat(entry["timestamp"]))
            throughputs.append(entry["metrics"]["peak_throughput_msg_per_sec"])

    if not timestamps:
        print("⚠️ No throughput data to plot")
        return

    plt.figure(figsize=(12, 6))
    plt.plot(timestamps, throughputs, marker='o', linestyle='-', linewidth=2)
    plt.title("Logger Throughput Trend", fontsize=16, fontweight='bold')
    plt.xlabel("Date", fontsize=12)
    plt.ylabel("Messages/Second", fontsize=12)
    plt.grid(True, alpha=0.3)

    # Format y-axis
    ax = plt.gca()
    ax.yaxis.set_major_formatter(
        plt.FuncFormatter(lambda x, p: f'{x/1_000_000:.1f}M')
    )

    # Format x-axis
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d'))
    plt.xticks(rotation=45)

    plt.tight_layout()
    plt.savefig(output_file, dpi=150)
    print(f"✅ Throughput chart saved to {output_file}")


def plot_latency_trend(history: dict, output_file: Path):
    """Plot latency trend over time."""

    timestamps = []
    latencies = []

    for entry in history["history"]:
        if "avg_latency_ns" in entry["metrics"]:
            timestamps.append(datetime.fromisoformat(entry["timestamp"]))
            latencies.append(entry["metrics"]["avg_latency_ns"])

    if not timestamps:
        print("⚠️ No latency data to plot")
        return

    plt.figure(figsize=(12, 6))
    plt.plot(timestamps, latencies, marker='o', linestyle='-',
             linewidth=2, color='orange')
    plt.title("Logger Latency Trend", fontsize=16, fontweight='bold')
    plt.xlabel("Date", fontsize=12)
    plt.ylabel("Latency (nanoseconds)", fontsize=12)
    plt.grid(True, alpha=0.3)

    # Format x-axis
    ax = plt.gca()
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d'))
    plt.xticks(rotation=45)

    plt.tight_layout()
    plt.savefig(output_file, dpi=150)
    print(f"✅ Latency chart saved to {output_file}")


def main():
    summary_file = Path("benchmarks/results/summary.json")
    charts_dir = Path("docs/performance/charts")
    charts_dir.mkdir(parents=True, exist_ok=True)

    # Load current summary
    with open(summary_file) as f:
        data = json.load(f)
    summary = data.get("summary", {})

    # Append to history
    append_to_history(summary)

    # Load full history
    history = load_performance_history()

    # Generate charts
    plot_throughput_trend(history, charts_dir / "throughput_trend.png")
    plot_latency_trend(history, charts_dir / "latency_trend.png")


if __name__ == "__main__":
    main()
```

---

### Phase 4: README.md 자동 업데이트

#### 4.1 README 업데이트 스크립트

**파일:** `scripts/performance/update_readme.py`

```python
#!/usr/bin/env python3
"""
Update README.md with latest performance metrics.
"""
import json
import re
from pathlib import Path
from typing import Dict, Any


def load_summary() -> Dict[str, Any]:
    """Load performance summary."""
    summary_file = Path("benchmarks/results/summary.json")
    with open(summary_file) as f:
        data = json.load(f)
    return data.get("summary", {})


def format_throughput(value: float) -> str:
    """Format throughput value."""
    if value >= 1_000_000:
        return f"{value / 1_000_000:.2f}M"
    elif value >= 1_000:
        return f"{value / 1_000:.0f}K"
    else:
        return f"{value:.0f}"


def update_readme_performance_section(
    readme_content: str,
    summary: Dict[str, Any]
) -> str:
    """Update performance section in README."""

    # Find performance section
    pattern = r'(### 📊 Performance Benchmarks.*?)\n\n(.*?)(?=\n###|\Z)'

    def replacement(match):
        header = match.group(1)

        # Build new performance section
        new_section = [header, "\n\n"]
        new_section.append("*Benchmarked on Apple M1 (8-core) @ 3.2GHz, 16GB, macOS Sonoma*\n\n")
        new_section.append("<!-- AUTO-GENERATED: Do not edit manually -->\n")

        if "peak_throughput_msg_per_sec" in summary:
            throughput = summary["peak_throughput_msg_per_sec"]
            new_section.append(
                f"- **Peak Throughput**: {format_throughput(throughput)} messages/second\n"
            )

        if "avg_latency_ns" in summary:
            latency = summary["avg_latency_ns"]
            if latency < 1000:
                latency_str = f"{latency:.0f} nanoseconds"
            else:
                latency_str = f"{latency / 1000:.2f} microseconds"
            new_section.append(f"- **Average Latency**: {latency_str}\n")

        new_section.append("\n![Throughput](docs/performance/badges/throughput.svg) ")
        new_section.append("![Latency](docs/performance/badges/latency.svg)\n")

        new_section.append("\n**Performance Trend:**\n\n")
        new_section.append("![Throughput Trend](docs/performance/charts/throughput_trend.png)\n")

        new_section.append("\n<!-- END AUTO-GENERATED -->\n")

        return "".join(new_section)

    updated = re.sub(pattern, replacement, readme_content, flags=re.DOTALL)

    return updated


def main():
    readme_file = Path("README.md")
    summary = load_summary()

    with open(readme_file) as f:
        readme_content = f.read()

    updated_content = update_readme_performance_section(readme_content, summary)

    with open(readme_file, 'w') as f:
        f.write(updated_content)

    print("✅ README.md updated with latest performance metrics")


if __name__ == "__main__":
    main()
```

#### 4.2 GitHub Actions에서 자동 커밋

**파일:** `.github/workflows/benchmarks.yml` (추가 부분)

```yaml
- name: Update Documentation with Performance Metrics
  if: github.event_name == 'push' && github.ref == 'refs/heads/main'
  run: |
    # Parse benchmarks
    python3 scripts/performance/parse_benchmarks.py benchmarks/results/latest.json

    # Compare with baseline
    python3 scripts/performance/compare_baselines.py || true

    # Generate badges
    python3 scripts/performance/generate_badges.py

    # Generate charts (requires matplotlib)
    pip3 install matplotlib
    python3 scripts/performance/generate_charts.py

    # Update README
    python3 scripts/performance/update_readme.py

- name: Commit Performance Updates
  if: github.event_name == 'push' && github.ref == 'refs/heads/main'
  uses: stefanzweifel/git-auto-commit-action@v5
  with:
    commit_message: "chore: update performance metrics [skip ci]"
    file_pattern: |
      README.md
      docs/performance/**
      benchmarks/results/**
    commit_user_name: github-actions[bot]
    commit_user_email: github-actions[bot]@users.noreply.github.com
```

---

## 🎨 README.md 성능 섹션 디자인

### 제안 1: 배지 중심 디자인

```markdown
## 📊 Performance Benchmarks

*Benchmarked on Apple M1 (8-core) @ 3.2GHz, 16GB, macOS Sonoma*

<!-- AUTO-GENERATED: Do not edit manually -->

![Throughput](https://img.shields.io/badge/throughput-4.34M%20msg%2Fs-brightgreen)
![Latency](https://img.shields.io/badge/latency-148ns-brightgreen)
![Memory](https://img.shields.io/badge/memory-2MB-green)

- **Peak Throughput**: 4.34M messages/second (async mode)
- **Average Latency**: 148 nanoseconds (enqueue time)
- **Memory Footprint**: <2MB baseline with adaptive buffer management

**Performance Trend:**

![Throughput Trend](docs/performance/charts/throughput_trend.png)

<!-- END AUTO-GENERATED -->
```

### 제안 2: 테이블 중심 디자인

```markdown
## 📊 Performance Benchmarks

<!-- AUTO-GENERATED: Do not edit manually -->

| Metric | Value | Status |
|--------|-------|--------|
| **Peak Throughput** | 4.34M msg/s | ![](https://img.shields.io/badge/-brightgreen) |
| **Avg Latency (P50)** | 148 ns | ![](https://img.shields.io/badge/-brightgreen) |
| **Memory Usage** | 2 MB | ![](https://img.shields.io/badge/-green) |
| **Multi-thread (4 threads)** | 1.07M msg/s | ![](https://img.shields.io/badge/-green) |

**Latest Benchmark Run:** 2025-11-04 14:30:00 UTC
**Baseline Comparison:** ✅ No regressions detected

[View Detailed Report](benchmarks/results/comparison_report.md) | [Historical Data](docs/performance/performance_history.json)

<!-- END AUTO-GENERATED -->
```

### 제안 3: 카드 스타일 디자인

```markdown
## 📊 Performance Benchmarks

<!-- AUTO-GENERATED: Do not edit manually -->

<table>
<tr>
<td align="center">
  <img src="docs/performance/badges/throughput.svg" alt="Throughput" /><br>
  <strong>Peak Throughput</strong><br>
  4.34M msg/s
</td>
<td align="center">
  <img src="docs/performance/badges/latency.svg" alt="Latency" /><br>
  <strong>Avg Latency</strong><br>
  148 ns
</td>
<td align="center">
  <img src="docs/performance/badges/memory.svg" alt="Memory" /><br>
  <strong>Memory Usage</strong><br>
  2 MB
</td>
</tr>
</table>

**Performance Trends** (Last 30 days)

![](docs/performance/charts/throughput_trend.png)

<!-- END AUTO-GENERATED -->
```

---

## 🔄 CI/CD 워크플로우 전체 예시

**파일:** `.github/workflows/benchmarks.yml` (완전 버전)

```yaml
name: Benchmarks

on:
  push:
    branches: [ main, phase-* ]
  pull_request:
    branches: [ main ]
  workflow_dispatch:
    inputs:
      save_baseline:
        description: 'Save as new baseline'
        required: false
        default: 'false'
  schedule:
    # Run daily at 2 AM UTC
    - cron: '0 2 * * *'

jobs:
  benchmark:
    name: Run Performance Benchmarks
    runs-on: ${{ matrix.os }}
    timeout-minutes: 30

    strategy:
      fail-fast: false
      matrix:
        os: [ubuntu-22.04, macos-13]
        compiler: [clang]
        build_type: [Release]

    steps:
    - name: Checkout logger_system
      uses: actions/checkout@v4
      with:
        fetch-depth: 0
        path: logger_system

    - name: Checkout common_system
      uses: actions/checkout@v4
      with:
        repository: kcenon/common_system
        path: common_system

    - name: Checkout thread_system
      uses: actions/checkout@v4
      with:
        repository: kcenon/thread_system
        path: thread_system

    - name: Install dependencies (Ubuntu)
      if: runner.os == 'Linux'
      run: |
        sudo apt-get update
        sudo apt-get install -y \
          cmake ninja-build clang \
          libbenchmark-dev libgtest-dev libfmt-dev \
          python3-pip
        pip3 install matplotlib scipy

    - name: Install dependencies (macOS)
      if: runner.os == 'macOS'
      run: |
        brew install ninja google-benchmark googletest fmt
        pip3 install matplotlib scipy

    - name: Set up compiler
      run: |
        echo "CC=clang" >> $GITHUB_ENV
        echo "CXX=clang++" >> $GITHUB_ENV

    - name: Configure CMake
      run: |
        cmake -B build -S logger_system \
          -GNinja \
          -DCMAKE_BUILD_TYPE=${{ matrix.build_type }} \
          -DBUILD_BENCHMARKS=ON \
          -DBUILD_TESTS=OFF \
          -DLOGGER_STANDALONE=ON

    - name: Build logger_system
      run: cmake --build build --config ${{ matrix.build_type }} -j

    - name: Run Benchmarks
      run: |
        cd build/benchmarks

        # Run benchmarks with JSON output
        ./logger_benchmarks \
          --benchmark_format=json \
          --benchmark_out=benchmark_results.json \
          --benchmark_repetitions=10 \
          --benchmark_report_aggregates_only=true \
          --benchmark_min_time=5.0

        # Copy results
        mkdir -p ../../logger_system/benchmarks/results
        cp benchmark_results.json \
           ../../logger_system/benchmarks/results/latest_${{ matrix.os }}.json

    - name: Parse Benchmark Results
      run: |
        cd logger_system
        python3 scripts/performance/parse_benchmarks.py \
          benchmarks/results/latest_${{ matrix.os }}.json

    - name: Compare with Baseline
      continue-on-error: true
      run: |
        cd logger_system
        python3 scripts/performance/compare_baselines.py

    - name: Generate Performance Badges
      if: matrix.os == 'ubuntu-22.04'
      run: |
        cd logger_system
        python3 scripts/performance/generate_badges.py

    - name: Generate Performance Charts
      if: matrix.os == 'ubuntu-22.04'
      run: |
        cd logger_system
        python3 scripts/performance/generate_charts.py

    - name: Update README
      if: matrix.os == 'ubuntu-22.04' && github.event_name == 'push' && github.ref == 'refs/heads/main'
      run: |
        cd logger_system
        python3 scripts/performance/update_readme.py

    - name: Upload Benchmark Results
      uses: actions/upload-artifact@v4
      with:
        name: benchmark-results-${{ matrix.os }}
        path: |
          logger_system/benchmarks/results/
          logger_system/docs/performance/
        retention-days: 90

    - name: Post PR Comment
      if: github.event_name == 'pull_request' && matrix.os == 'ubuntu-22.04'
      uses: actions/github-script@v7
      with:
        script: |
          const fs = require('fs');
          const reportPath = 'logger_system/benchmarks/results/comparison_report.md';

          if (fs.existsSync(reportPath)) {
            const report = fs.readFileSync(reportPath, 'utf8');

            github.rest.issues.createComment({
              issue_number: context.issue.number,
              owner: context.repo.owner,
              repo: context.repo.repo,
              body: report
            });
          }

    - name: Commit Performance Updates
      if: matrix.os == 'ubuntu-22.04' && github.event_name == 'push' && github.ref == 'refs/heads/main'
      uses: stefanzweifel/git-auto-commit-action@v5
      with:
        repository: logger_system
        commit_message: "chore: update performance metrics [skip ci]"
        file_pattern: |
          README.md
          docs/performance/**
          benchmarks/results/**
        commit_user_name: github-actions[bot]
        commit_user_email: github-actions[bot]@users.noreply.github.com

    - name: Save as Baseline
      if: github.event.inputs.save_baseline == 'true' && github.ref == 'refs/heads/main'
      run: |
        cd logger_system
        cp benchmarks/results/summary.json \
           benchmarks/baselines/baseline.json

        # Archive old baseline
        mkdir -p benchmarks/baselines/history
        cp benchmarks/baselines/baseline.json \
           benchmarks/baselines/history/baseline_$(date +%Y%m%d).json

  report:
    name: Generate Benchmark Report
    needs: benchmark
    runs-on: ubuntu-22.04
    if: always()

    steps:
    - name: Download all artifacts
      uses: actions/download-artifact@v4

    - name: Generate summary
      run: |
        echo "# 📊 Benchmark Results Summary" >> $GITHUB_STEP_SUMMARY
        echo "" >> $GITHUB_STEP_SUMMARY

        for dir in benchmark-results-*; do
          if [ -d "$dir" ]; then
            echo "## $dir" >> $GITHUB_STEP_SUMMARY

            if [ -f "$dir/comparison_report.md" ]; then
              cat "$dir/comparison_report.md" >> $GITHUB_STEP_SUMMARY
            fi
          fi
        done
```

---

## 📈 예상 결과물

### 1. GitHub Actions 실행 결과

```
✅ Build logger_system
✅ Run Benchmarks
✅ Parse Benchmark Results
  📊 Peak Throughput: 4.34M msg/s
  ⏱️ Avg Latency: 148 ns
✅ Compare with Baseline
  ✅ No performance regression detected
✅ Generate Performance Badges
  - throughput.svg
  - latency.svg
  - memory.svg
✅ Generate Performance Charts
  - throughput_trend.png
  - latency_trend.png
✅ Update README.md
✅ Commit changes
```

### 2. PR 자동 코멘트

```markdown
# Performance Comparison Report

## ✅ No Performance Regressions

## 🚀 Performance Improvements

- **peak_throughput_msg_per_sec**: 4,200,000 → 4,340,000 (+3.33%)

## ⚖️ Stable Metrics

- **avg_latency_ns**: 150 → 148 (-1.33%)
```

### 3. 업데이트된 README.md

자동으로 최신 성능 지표가 반영된 README.md:

```markdown
## 📊 Performance Benchmarks

*Benchmarked on Apple M1 (8-core) @ 3.2GHz, 16GB, macOS Sonoma*

<!-- AUTO-GENERATED: Do not edit manually -->

![Throughput](docs/performance/badges/throughput.svg)
![Latency](docs/performance/badges/latency.svg)

- **Peak Throughput**: 4.34M messages/second
- **Average Latency**: 148 nanoseconds

**Performance Trend:**

![Throughput Trend](docs/performance/charts/throughput_trend.png)

<!-- END AUTO-GENERATED -->
```

---

## ✅ 구현 체크리스트

### Phase 1: 인프라 구축 (예상 소요: 2시간)
- [ ] `scripts/performance/` 디렉토리 생성
- [ ] `parse_benchmarks.py` 작성
- [ ] `compare_baselines.py` 작성
- [ ] Python 의존성 설치 (`matplotlib`, `scipy`)
- [ ] 로컬에서 스크립트 테스트

### Phase 2: 배지 및 차트 (예상 소요: 1.5시간)
- [ ] `generate_badges.py` 작성
- [ ] `generate_charts.py` 작성
- [ ] `docs/performance/` 디렉토리 구조 생성
- [ ] 샘플 배지 및 차트 생성 테스트

### Phase 3: README 자동 업데이트 (예상 소요: 1시간)
- [ ] `update_readme.py` 작성
- [ ] README.md에 자동 생성 마커 추가
- [ ] 로컬에서 README 업데이트 테스트

### Phase 4: CI/CD 통합 (예상 소요: 2시간)
- [ ] `.github/workflows/benchmarks.yml` 업데이트
- [ ] 벤치마크 빌드 활성화 (`-DBUILD_BENCHMARKS=ON`)
- [ ] 워크플로우 단계 추가 (파싱, 비교, 배지, 차트)
- [ ] 자동 커밋 액션 설정
- [ ] PR 코멘트 기능 추가

### Phase 5: 테스트 및 검증 (예상 소요: 1.5시간)
- [ ] 로컬에서 전체 워크플로우 시뮬레이션
- [ ] 테스트 브랜치에서 CI 실행 확인
- [ ] 성능 회귀 시나리오 테스트
- [ ] 배지 및 차트 시각화 검증

### Phase 6: 문서화 (예상 소요: 1시간)
- [ ] 이 제안서를 `docs/performance/CI_CD_PERFORMANCE_PROPOSAL.md`로 저장
- [ ] `benchmarks/README.md` 업데이트
- [ ] 트러블슈팅 가이드 작성

**총 예상 소요 시간: 9시간**

---

## 🔧 트러블슈팅 가이드

### 문제 1: 벤치마크 빌드 실패

**증상:**
```
CMake Error: Could not find benchmark
```

**해결책:**
```bash
# Ubuntu
sudo apt-get install libbenchmark-dev

# macOS
brew install google-benchmark

# From source
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -B build -DCMAKE_BUILD_TYPE=Release
sudo cmake --build build --target install
```

### 문제 2: Python 스크립트 실행 오류

**증상:**
```
ModuleNotFoundError: No module named 'matplotlib'
```

**해결책:**
```bash
pip3 install matplotlib scipy
```

### 문제 3: Git 자동 커밋 권한 오류

**증상:**
```
Error: refusing to allow a GitHub App to create or update workflow
```

**해결책:**

GitHub Actions 설정 변경:
```yaml
permissions:
  contents: write  # 추가 필요
```

또는 Personal Access Token 사용:
```yaml
- uses: stefanzweifel/git-auto-commit-action@v5
  with:
    token: ${{ secrets.GITHUB_TOKEN }}
```

### 문제 4: 배지가 README에 표시되지 않음

**증상:**
배지 이미지가 깨짐

**해결책:**

1. 상대 경로 확인:
   ```markdown
   ![Throughput](docs/performance/badges/throughput.svg)
   ```

2. GitHub Pages 활성화 필요 시:
   ```markdown
   ![Throughput](https://raw.githubusercontent.com/kcenon/logger_system/main/docs/performance/badges/throughput.svg)
   ```

---

## 📚 참고 자료

### 도구 및 라이브러리
- [Google Benchmark](https://github.com/google/benchmark) - C++ 벤치마킹 프레임워크
- [Shields.io](https://shields.io/) - 배지 생성 서비스
- [Matplotlib](https://matplotlib.org/) - Python 차트 라이브러리
- [GitHub Actions](https://docs.github.com/en/actions) - CI/CD 플랫폼

### 관련 프로젝트 예시
- [spdlog Benchmarks](https://github.com/gabime/spdlog#benchmarks)
- [fmt Performance](https://github.com/fmtlib/fmt#performance)

### 내부 문서
- `benchmarks/README.md` - 벤치마크 사용 가이드
- `BASELINE.md` - 기준 성능 지표
- `docs/LOGGER_SYSTEM_ARCHITECTURE.md` - 시스템 아키텍처

---

## 🎯 성공 지표

이 제안이 성공적으로 구현되면 다음을 달성합니다:

1. **자동화**
   - ✅ 모든 커밋에서 자동 벤치마크 실행
   - ✅ README.md 자동 업데이트 (수동 개입 불필요)
   - ✅ 성능 배지 실시간 갱신

2. **가시성**
   - ✅ README에 최신 성능 지표 항상 표시
   - ✅ 트렌드 차트로 시각적 이해 향상
   - ✅ PR마다 성능 영향 자동 리포트

3. **품질 보증**
   - ✅ 성능 회귀 즉시 감지 (±5% 임계값)
   - ✅ 90일간 성능 히스토리 보관
   - ✅ 기준선 대비 지속적 비교

4. **개발자 경험**
   - ✅ 벤치마크 결과 쉽게 접근
   - ✅ 성능 영향 사전 인지 (PR 단계)
   - ✅ 문서 작성 부담 감소

---

## 📝 다음 단계

1. **승인 및 리뷰**
   - 이 제안서 검토
   - 구현 우선순위 결정

2. **프로토타입 구현**
   - Phase 1-2 먼저 구현 (스크립트)
   - 로컬에서 테스트

3. **CI/CD 통합**
   - Phase 3-4 구현
   - 테스트 브랜치에서 검증

4. **프로덕션 배포**
   - main 브랜치에 머지
   - 첫 자동 업데이트 모니터링

5. **유지보수 및 개선**
   - 피드백 수집
   - 추가 메트릭 고려
   - 차트 디자인 개선

---

**문서 작성자:** Claude (AI Assistant)
**검토 필요:** 프로젝트 메인테이너
**마지막 업데이트:** 2025-11-04
