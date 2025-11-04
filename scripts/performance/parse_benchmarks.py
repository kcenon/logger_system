#!/usr/bin/env python3
"""
Parse Google Benchmark JSON output and extract key metrics.

This script processes benchmark results from logger_system and extracts
performance metrics for CI/CD automation.

Usage:
    python3 parse_benchmarks.py <benchmark_results.json>

Output:
    benchmarks/results/summary.json
"""

import json
import sys
from pathlib import Path
from typing import Dict, Any, List


def extract_metrics(benchmark_json: Dict[str, Any]) -> Dict[str, Any]:
    """
    Extract key performance metrics from benchmark results.

    Args:
        benchmark_json: Google Benchmark JSON output

    Returns:
        Dictionary containing organized metrics by category
    """
    metrics = {
        "throughput": {},
        "latency": {},
        "memory": {},
        "rotation": {},
        "context": benchmark_json.get("context", {}),
        "timestamp": ""
    }

    # Extract timestamp from context
    if "date" in metrics["context"]:
        metrics["timestamp"] = metrics["context"]["date"]

    benchmarks = benchmark_json.get("benchmarks", [])

    for benchmark in benchmarks:
        name = benchmark.get("name", "")

        # Skip aggregate results (we want raw data)
        if "_mean" in name or "_median" in name or "_stddev" in name:
            continue

        # Extract throughput benchmarks
        if "Throughput" in name or "throughput" in name.lower():
            items_per_sec = benchmark.get("items_per_second")
            if items_per_sec:
                metrics["throughput"][name] = {
                    "messages_per_second": items_per_sec,
                    "cpu_time_ms": benchmark.get("cpu_time", 0) / 1e6,  # ns to ms
                    "real_time_ms": benchmark.get("real_time", 0) / 1e6,
                    "iterations": benchmark.get("iterations", 0)
                }

        # Extract latency/write benchmarks
        elif "Latency" in name or "Write" in name or "latency" in name.lower():
            metrics["latency"][name] = {
                "cpu_time_ns": benchmark.get("cpu_time", 0),
                "real_time_ns": benchmark.get("real_time", 0),
                "iterations": benchmark.get("iterations", 0)
            }

        # Extract memory benchmarks
        elif "Memory" in name or "Pool" in name:
            bytes_per_sec = benchmark.get("bytes_per_second")
            if bytes_per_sec:
                metrics["memory"][name] = {
                    "bytes_per_second": bytes_per_sec,
                    "cpu_time_ms": benchmark.get("cpu_time", 0) / 1e6,
                    "iterations": benchmark.get("iterations", 0)
                }

        # Extract rotation benchmarks
        elif "Rotation" in name or "rotation" in name.lower():
            metrics["rotation"][name] = {
                "cpu_time_ms": benchmark.get("cpu_time", 0) / 1e6,
                "real_time_ms": benchmark.get("real_time", 0) / 1e6,
                "iterations": benchmark.get("iterations", 0)
            }

    return metrics


def calculate_summary(metrics: Dict[str, Any]) -> Dict[str, Any]:
    """
    Calculate summary statistics from detailed metrics.

    Args:
        metrics: Detailed metrics organized by category

    Returns:
        Summary statistics suitable for badges and README
    """
    summary = {}

    # Calculate peak throughput
    if metrics["throughput"]:
        throughput_values = [
            m["messages_per_second"]
            for m in metrics["throughput"].values()
            if "messages_per_second" in m
        ]
        if throughput_values:
            summary["peak_throughput_msg_per_sec"] = int(max(throughput_values))
            summary["avg_throughput_msg_per_sec"] = int(
                sum(throughput_values) / len(throughput_values)
            )

    # Calculate average latency
    if metrics["latency"]:
        latency_values = [
            m["cpu_time_ns"]
            for m in metrics["latency"].values()
            if "cpu_time_ns" in m
        ]
        if latency_values:
            summary["avg_latency_ns"] = round(
                sum(latency_values) / len(latency_values), 2
            )
            summary["min_latency_ns"] = round(min(latency_values), 2)
            summary["max_latency_ns"] = round(max(latency_values), 2)

    # Calculate rotation performance
    if metrics["rotation"]:
        rotation_times = [
            m["real_time_ms"]
            for m in metrics["rotation"].values()
            if "real_time_ms" in m
        ]
        if rotation_times:
            summary["avg_rotation_time_ms"] = round(
                sum(rotation_times) / len(rotation_times), 2
            )

    # Calculate memory efficiency
    if metrics["memory"]:
        memory_values = [
            m["bytes_per_second"]
            for m in metrics["memory"].values()
            if "bytes_per_second" in m
        ]
        if memory_values:
            summary["avg_memory_throughput_mb_per_sec"] = round(
                sum(memory_values) / len(memory_values) / (1024 * 1024), 2
            )

    return summary


def save_results(metrics: Dict[str, Any], summary: Dict[str, Any], output_dir: Path):
    """
    Save extracted metrics and summary to JSON files.

    Args:
        metrics: Detailed metrics
        summary: Summary statistics
        output_dir: Output directory path
    """
    # Create output directory
    output_dir.mkdir(parents=True, exist_ok=True)

    # Combine metrics and summary
    result = {
        "summary": summary,
        "detailed_metrics": metrics
    }

    # Save summary file
    summary_file = output_dir / "summary.json"
    with open(summary_file, 'w') as f:
        json.dump(result, f, indent=2)

    print(f"✅ Metrics extracted to {summary_file}")


def print_summary(summary: Dict[str, Any]):
    """
    Print summary statistics to console.

    Args:
        summary: Summary statistics
    """
    print("\n📊 Performance Summary:")
    print("=" * 60)

    if "peak_throughput_msg_per_sec" in summary:
        throughput = summary["peak_throughput_msg_per_sec"]
        print(f"  Peak Throughput:     {throughput:,} messages/second")

    if "avg_throughput_msg_per_sec" in summary:
        avg_throughput = summary["avg_throughput_msg_per_sec"]
        print(f"  Avg Throughput:      {avg_throughput:,} messages/second")

    if "avg_latency_ns" in summary:
        latency = summary["avg_latency_ns"]
        if latency < 1000:
            print(f"  Avg Latency:         {latency:.2f} nanoseconds")
        elif latency < 1_000_000:
            print(f"  Avg Latency:         {latency/1000:.2f} microseconds")
        else:
            print(f"  Avg Latency:         {latency/1_000_000:.2f} milliseconds")

    if "avg_rotation_time_ms" in summary:
        rotation = summary["avg_rotation_time_ms"]
        print(f"  Avg Rotation Time:   {rotation:.2f} milliseconds")

    if "avg_memory_throughput_mb_per_sec" in summary:
        memory = summary["avg_memory_throughput_mb_per_sec"]
        print(f"  Memory Throughput:   {memory:.2f} MB/second")

    print("=" * 60)


def main():
    """Main entry point."""
    if len(sys.argv) < 2:
        print("Usage: parse_benchmarks.py <benchmark_results.json>", file=sys.stderr)
        print("\nExample:", file=sys.stderr)
        print("  python3 parse_benchmarks.py build/benchmarks/benchmark_results.json",
              file=sys.stderr)
        sys.exit(1)

    # Parse input file
    input_file = Path(sys.argv[1])

    if not input_file.exists():
        print(f"❌ Error: Input file not found: {input_file}", file=sys.stderr)
        sys.exit(1)

    # Set output directory
    output_dir = Path("benchmarks/results")

    try:
        # Load benchmark data
        with open(input_file) as f:
            benchmark_data = json.load(f)

        # Extract metrics
        metrics = extract_metrics(benchmark_data)

        # Calculate summary
        summary = calculate_summary(metrics)

        # Save results
        save_results(metrics, summary, output_dir)

        # Print summary
        print_summary(summary)

        print("\n✅ Success!")

    except json.JSONDecodeError as e:
        print(f"❌ Error: Invalid JSON file: {e}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"❌ Error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
