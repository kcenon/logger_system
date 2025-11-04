#!/usr/bin/env python3
"""
Compare current benchmark results with baseline and detect regressions.

This script compares performance metrics against a baseline to detect
performance regressions or improvements.

Usage:
    python3 compare_baselines.py

Input Files:
    - benchmarks/baselines/baseline.json (baseline metrics)
    - benchmarks/results/summary.json (current metrics)

Output Files:
    - benchmarks/results/comparison_report.json
    - benchmarks/results/comparison_report.md

Exit Code:
    0: No regression detected
    1: Performance regression detected
"""

import json
import sys
from pathlib import Path
from typing import Dict, Any, Tuple, List


# Regression threshold: 5%
REGRESSION_THRESHOLD = 0.05


def load_json(file_path: Path) -> Dict[str, Any]:
    """
    Load JSON file.

    Args:
        file_path: Path to JSON file

    Returns:
        Parsed JSON data

    Raises:
        FileNotFoundError: If file doesn't exist
        json.JSONDecodeError: If file is not valid JSON
    """
    if not file_path.exists():
        raise FileNotFoundError(f"File not found: {file_path}")

    with open(file_path) as f:
        return json.load(f)


def compare_metrics(
    baseline: Dict[str, Any],
    current: Dict[str, Any]
) -> Tuple[bool, Dict[str, Any]]:
    """
    Compare current metrics with baseline.

    Args:
        baseline: Baseline metrics
        current: Current metrics

    Returns:
        Tuple of (has_regression, comparison_report)
    """
    report = {
        "regression_detected": False,
        "improvements": [],
        "regressions": [],
        "stable": [],
        "threshold_percent": REGRESSION_THRESHOLD * 100
    }

    baseline_summary = baseline.get("summary", {})
    current_summary = current.get("summary", {})

    for metric_name, baseline_value in baseline_summary.items():
        current_value = current_summary.get(metric_name)

        if current_value is None:
            # Metric not present in current results
            continue

        # Skip if not numeric
        if not isinstance(baseline_value, (int, float)) or \
           not isinstance(current_value, (int, float)):
            continue

        # Calculate percentage change
        if baseline_value == 0:
            # Avoid division by zero
            if current_value != 0:
                delta_percent = 100.0  # Arbitrary large change
            else:
                delta_percent = 0.0
        else:
            delta = (current_value - baseline_value) / baseline_value
            delta_percent = delta * 100

        comparison = {
            "metric": metric_name,
            "baseline": baseline_value,
            "current": current_value,
            "delta_percent": round(delta_percent, 2),
            "delta_absolute": round(current_value - baseline_value, 2)
        }

        # Determine if this is a regression, improvement, or stable
        # For throughput: higher is better
        if "throughput" in metric_name.lower():
            if delta_percent < -REGRESSION_THRESHOLD * 100:  # Significant decrease
                comparison["status"] = "regression"
                report["regressions"].append(comparison)
                report["regression_detected"] = True
            elif delta_percent > REGRESSION_THRESHOLD * 100:  # Significant increase
                comparison["status"] = "improvement"
                report["improvements"].append(comparison)
            else:
                comparison["status"] = "stable"
                report["stable"].append(comparison)

        # For latency and rotation time: lower is better
        elif "latency" in metric_name.lower() or "rotation" in metric_name.lower():
            if delta_percent > REGRESSION_THRESHOLD * 100:  # Significant increase
                comparison["status"] = "regression"
                report["regressions"].append(comparison)
                report["regression_detected"] = True
            elif delta_percent < -REGRESSION_THRESHOLD * 100:  # Significant decrease
                comparison["status"] = "improvement"
                report["improvements"].append(comparison)
            else:
                comparison["status"] = "stable"
                report["stable"].append(comparison)

        # For other metrics: treat as neutral (higher could be better or worse)
        else:
            if abs(delta_percent) <= REGRESSION_THRESHOLD * 100:
                comparison["status"] = "stable"
                report["stable"].append(comparison)
            else:
                comparison["status"] = "changed"
                report["stable"].append(comparison)  # Treat as stable for now

    return report["regression_detected"], report


def generate_markdown_report(report: Dict[str, Any]) -> str:
    """
    Generate Markdown report from comparison results.

    Args:
        report: Comparison report

    Returns:
        Markdown-formatted report
    """
    lines: List[str] = []

    lines.append("# 📊 Performance Comparison Report\n")
    lines.append(f"**Regression Threshold:** ±{report['threshold_percent']:.0f}%\n\n")

    # Regression section
    if report["regression_detected"]:
        lines.append("## ⚠️ Performance Regressions Detected\n")
        lines.append("The following metrics show performance degradation:\n\n")
        for item in report["regressions"]:
            symbol = "🔻" if item["delta_percent"] < 0 else "🔺"
            lines.append(
                f"- **{item['metric']}**: "
                f"{format_value(item['baseline'])} → {format_value(item['current'])} "
                f"({symbol} {item['delta_percent']:+.2f}%)\n"
            )
        lines.append("\n")
    else:
        lines.append("## ✅ No Performance Regressions\n")
        lines.append("All metrics are within acceptable thresholds.\n\n")

    # Improvements section
    if report["improvements"]:
        lines.append("## 🚀 Performance Improvements\n")
        for item in report["improvements"]:
            symbol = "⬆️" if item["delta_percent"] > 0 else "⬇️"
            lines.append(
                f"- **{item['metric']}**: "
                f"{format_value(item['baseline'])} → {format_value(item['current'])} "
                f"({symbol} {item['delta_percent']:+.2f}%)\n"
            )
        lines.append("\n")

    # Stable metrics section
    if report["stable"]:
        lines.append("## ⚖️ Stable Metrics\n")
        for item in report["stable"]:
            lines.append(
                f"- **{item['metric']}**: "
                f"{format_value(item['current'])} "
                f"({item['delta_percent']:+.2f}%)\n"
            )

    return "".join(lines)


def format_value(value: float) -> str:
    """
    Format numeric value for display.

    Args:
        value: Numeric value

    Returns:
        Formatted string
    """
    if value >= 1_000_000:
        return f"{value / 1_000_000:.2f}M"
    elif value >= 1_000:
        return f"{value / 1_000:.1f}K"
    elif value >= 1:
        return f"{value:.2f}"
    else:
        return f"{value:.4f}"


def save_reports(report: Dict[str, Any], output_dir: Path):
    """
    Save comparison reports to files.

    Args:
        report: Comparison report
        output_dir: Output directory
    """
    output_dir.mkdir(parents=True, exist_ok=True)

    # Save JSON report
    json_file = output_dir / "comparison_report.json"
    with open(json_file, 'w') as f:
        json.dump(report, f, indent=2)

    # Save Markdown report
    md_report = generate_markdown_report(report)
    md_file = output_dir / "comparison_report.md"
    with open(md_file, 'w') as f:
        f.write(md_report)

    print(f"✅ Reports saved:")
    print(f"  - JSON: {json_file}")
    print(f"  - Markdown: {md_file}")


def print_report(report: Dict[str, Any]):
    """
    Print comparison report to console.

    Args:
        report: Comparison report
    """
    md_report = generate_markdown_report(report)
    print("\n" + md_report)


def main():
    """Main entry point."""
    baseline_file = Path("benchmarks/baselines/baseline.json")
    current_file = Path("benchmarks/results/summary.json")
    output_dir = Path("benchmarks/results")

    try:
        # Load baseline
        if not baseline_file.exists():
            print(f"⚠️ Warning: Baseline file not found: {baseline_file}",
                  file=sys.stderr)
            print("Skipping comparison. Run with --save-baseline to create one.",
                  file=sys.stderr)
            sys.exit(0)

        baseline = load_json(baseline_file)

        # Load current results
        if not current_file.exists():
            print(f"❌ Error: Current results not found: {current_file}",
                  file=sys.stderr)
            print("Run parse_benchmarks.py first.", file=sys.stderr)
            sys.exit(1)

        current = load_json(current_file)

        # Compare metrics
        has_regression, report = compare_metrics(baseline, current)

        # Save reports
        save_reports(report, output_dir)

        # Print report
        print_report(report)

        # Exit with appropriate code
        if has_regression:
            print("\n❌ Performance regression detected!")
            print(f"One or more metrics degraded by more than {REGRESSION_THRESHOLD * 100:.0f}%")
            sys.exit(1)
        else:
            print("\n✅ No performance regression detected!")
            sys.exit(0)

    except FileNotFoundError as e:
        print(f"❌ Error: {e}", file=sys.stderr)
        sys.exit(1)
    except json.JSONDecodeError as e:
        print(f"❌ Error: Invalid JSON: {e}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"❌ Error: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == "__main__":
    main()
