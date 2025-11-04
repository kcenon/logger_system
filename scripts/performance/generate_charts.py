#!/usr/bin/env python3
"""
Generate performance trend charts using matplotlib.

This script creates visual charts showing performance trends over time,
based on historical benchmark data.

Usage:
    python3 generate_charts.py

Input:
    benchmarks/results/summary.json
    docs/performance/performance_history.json

Output:
    docs/performance/charts/*.png
    docs/performance/performance_history.json (updated)

Requirements:
    pip install matplotlib
"""

import json
from pathlib import Path
from datetime import datetime
from typing import Dict, Any, List, Tuple
import sys


def check_matplotlib():
    """Check if matplotlib is installed."""
    try:
        import matplotlib
        import matplotlib.pyplot
        import matplotlib.dates
        return True
    except ImportError:
        print("❌ Error: matplotlib is not installed", file=sys.stderr)
        print("\nInstall it with:", file=sys.stderr)
        print("  pip3 install matplotlib", file=sys.stderr)
        return False


def load_performance_history(history_file: Path) -> Dict[str, Any]:
    """
    Load performance history from JSON file.

    Args:
        history_file: Path to history JSON file

    Returns:
        Performance history data
    """
    if not history_file.exists():
        return {"history": []}

    with open(history_file) as f:
        return json.load(f)


def append_to_history(
    summary: Dict[str, Any],
    history_file: Path,
    max_entries: int = 90
):
    """
    Append current summary to performance history.

    Args:
        summary: Performance summary
        history_file: Path to history file
        max_entries: Maximum number of entries to keep (default: 90 days)
    """
    history = load_performance_history(history_file)

    entry = {
        "timestamp": datetime.now().isoformat(),
        "metrics": summary
    }

    history["history"].append(entry)

    # Keep only last N entries
    if len(history["history"]) > max_entries:
        history["history"] = history["history"][-max_entries:]

    # Ensure parent directory exists
    history_file.parent.mkdir(parents=True, exist_ok=True)

    # Save updated history
    with open(history_file, 'w') as f:
        json.dump(history, f, indent=2)

    print(f"✅ History updated: {len(history['history'])} entries")


def extract_time_series(
    history: Dict[str, Any],
    metric_name: str
) -> Tuple[List[datetime], List[float]]:
    """
    Extract time series data for a specific metric.

    Args:
        history: Performance history
        metric_name: Name of metric to extract

    Returns:
        Tuple of (timestamps, values)
    """
    timestamps = []
    values = []

    for entry in history.get("history", []):
        if metric_name in entry.get("metrics", {}):
            try:
                timestamp = datetime.fromisoformat(entry["timestamp"])
                value = entry["metrics"][metric_name]

                timestamps.append(timestamp)
                values.append(value)
            except (ValueError, KeyError):
                # Skip invalid entries
                continue

    return timestamps, values


def plot_throughput_trend(history: Dict[str, Any], output_file: Path):
    """
    Plot throughput trend over time.

    Args:
        history: Performance history
        output_file: Output PNG file path
    """
    import matplotlib.pyplot as plt
    import matplotlib.dates as mdates

    timestamps, throughputs = extract_time_series(
        history, "peak_throughput_msg_per_sec"
    )

    if not timestamps:
        print("⚠️ No throughput data to plot")
        return

    plt.figure(figsize=(12, 6))
    plt.plot(timestamps, throughputs, marker='o', linestyle='-',
             linewidth=2, markersize=6, color='#4c1')

    plt.title("Logger Throughput Trend", fontsize=16, fontweight='bold')
    plt.xlabel("Date", fontsize=12)
    plt.ylabel("Messages/Second", fontsize=12)
    plt.grid(True, alpha=0.3, linestyle='--')

    # Format y-axis
    ax = plt.gca()
    ax.yaxis.set_major_formatter(
        plt.FuncFormatter(lambda x, p: f'{x/1_000_000:.1f}M')
    )

    # Format x-axis
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d'))
    plt.xticks(rotation=45, ha='right')

    # Add horizontal line for baseline (if available)
    if throughputs:
        baseline = throughputs[0]
        plt.axhline(y=baseline, color='gray', linestyle='--',
                   alpha=0.5, label=f'Baseline: {baseline/1_000_000:.2f}M')
        plt.legend()

    plt.tight_layout()
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    plt.close()

    print(f"✅ Throughput chart saved to {output_file}")


def plot_latency_trend(history: Dict[str, Any], output_file: Path):
    """
    Plot latency trend over time.

    Args:
        history: Performance history
        output_file: Output PNG file path
    """
    import matplotlib.pyplot as plt
    import matplotlib.dates as mdates

    timestamps, latencies = extract_time_series(history, "avg_latency_ns")

    if not timestamps:
        print("⚠️ No latency data to plot")
        return

    plt.figure(figsize=(12, 6))
    plt.plot(timestamps, latencies, marker='o', linestyle='-',
             linewidth=2, markersize=6, color='#fe7d37')

    plt.title("Logger Latency Trend", fontsize=16, fontweight='bold')
    plt.xlabel("Date", fontsize=12)
    plt.ylabel("Latency (nanoseconds)", fontsize=12)
    plt.grid(True, alpha=0.3, linestyle='--')

    # Format x-axis
    ax = plt.gca()
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d'))
    plt.xticks(rotation=45, ha='right')

    # Add horizontal line for baseline (if available)
    if latencies:
        baseline = latencies[0]
        plt.axhline(y=baseline, color='gray', linestyle='--',
                   alpha=0.5, label=f'Baseline: {baseline:.1f}ns')
        plt.legend()

    plt.tight_layout()
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    plt.close()

    print(f"✅ Latency chart saved to {output_file}")


def plot_combined_metrics(history: Dict[str, Any], output_file: Path):
    """
    Plot multiple metrics in subplots.

    Args:
        history: Performance history
        output_file: Output PNG file path
    """
    import matplotlib.pyplot as plt
    import matplotlib.dates as mdates

    # Extract data
    throughput_ts, throughput_vals = extract_time_series(
        history, "peak_throughput_msg_per_sec"
    )
    latency_ts, latency_vals = extract_time_series(
        history, "avg_latency_ns"
    )

    if not throughput_ts and not latency_ts:
        print("⚠️ No data to plot")
        return

    # Create subplots
    fig, axes = plt.subplots(2, 1, figsize=(12, 10))
    fig.suptitle("Logger Performance Trends", fontsize=16, fontweight='bold')

    # Throughput subplot
    if throughput_ts:
        axes[0].plot(throughput_ts, throughput_vals, marker='o',
                    linestyle='-', linewidth=2, markersize=6, color='#4c1')
        axes[0].set_ylabel("Messages/Second", fontsize=12)
        axes[0].set_title("Throughput", fontsize=14)
        axes[0].grid(True, alpha=0.3, linestyle='--')
        axes[0].yaxis.set_major_formatter(
            plt.FuncFormatter(lambda x, p: f'{x/1_000_000:.1f}M')
        )

        # Baseline
        baseline = throughput_vals[0]
        axes[0].axhline(y=baseline, color='gray', linestyle='--',
                       alpha=0.5, label=f'Baseline: {baseline/1_000_000:.2f}M')
        axes[0].legend()

    # Latency subplot
    if latency_ts:
        axes[1].plot(latency_ts, latency_vals, marker='o',
                    linestyle='-', linewidth=2, markersize=6, color='#fe7d37')
        axes[1].set_ylabel("Latency (nanoseconds)", fontsize=12)
        axes[1].set_xlabel("Date", fontsize=12)
        axes[1].set_title("Latency", fontsize=14)
        axes[1].grid(True, alpha=0.3, linestyle='--')

        # Baseline
        baseline = latency_vals[0]
        axes[1].axhline(y=baseline, color='gray', linestyle='--',
                       alpha=0.5, label=f'Baseline: {baseline:.1f}ns')
        axes[1].legend()

    # Format x-axes
    for ax in axes:
        ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d'))
        plt.setp(ax.xaxis.get_majorticklabels(), rotation=45, ha='right')

    plt.tight_layout()
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    plt.close()

    print(f"✅ Combined metrics chart saved to {output_file}")


def main():
    """Main entry point."""
    if not check_matplotlib():
        return 1

    summary_file = Path("benchmarks/results/summary.json")
    history_file = Path("docs/performance/performance_history.json")
    charts_dir = Path("docs/performance/charts")

    try:
        # Create charts directory
        charts_dir.mkdir(parents=True, exist_ok=True)

        # Load current summary
        if not summary_file.exists():
            print(f"❌ Error: Summary file not found: {summary_file}")
            print("Run parse_benchmarks.py first.")
            return 1

        with open(summary_file) as f:
            data = json.load(f)

        summary = data.get("summary", {})

        if not summary:
            print("⚠️ Warning: No summary metrics found")
            return 1

        # Append to history
        print("Updating performance history...")
        append_to_history(summary, history_file)

        # Load full history
        history = load_performance_history(history_file)

        # Generate charts
        print("\nGenerating charts...")
        plot_throughput_trend(history, charts_dir / "throughput_trend.png")
        plot_latency_trend(history, charts_dir / "latency_trend.png")
        plot_combined_metrics(history, charts_dir / "combined_metrics.png")

        print(f"\n📁 Output directory: {charts_dir}")
        print(f"📊 History entries: {len(history['history'])}")

        return 0

    except json.JSONDecodeError as e:
        print(f"❌ Error: Invalid JSON: {e}")
        return 1
    except Exception as e:
        print(f"❌ Error: {e}")
        import traceback
        traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())
