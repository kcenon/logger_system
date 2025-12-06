#!/usr/bin/env python3
"""
Extract key metrics from benchmark summary for GitHub Actions step summary.

This script reads a benchmark summary JSON file and outputs
formatted metrics for the GitHub Actions step summary.
"""

import json
import sys


def extract_metrics(summary_file: str) -> None:
    """Extract and print key metrics from benchmark summary."""
    try:
        with open(summary_file) as f:
            data = json.load(f)
            summary = data.get('summary', {})

        if 'peak_throughput_msg_per_sec' in summary:
            throughput = summary['peak_throughput_msg_per_sec']
            print(f'  - **Peak Throughput**: {throughput/1_000_000:.2f}M msg/s')

        if 'avg_latency_ns' in summary:
            latency = summary['avg_latency_ns']
            if latency < 1000:
                print(f'  - **Avg Latency**: {latency:.1f} ns')
            else:
                print(f'  - **Avg Latency**: {latency/1000:.2f} μs')
    except (json.JSONDecodeError, IOError, KeyError):
        pass  # Silently ignore errors


if __name__ == '__main__':
    if len(sys.argv) > 1:
        extract_metrics(sys.argv[1])
    else:
        print("Usage: extract_benchmark_metrics.py <summary.json>", file=sys.stderr)
        sys.exit(1)
