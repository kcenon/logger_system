#!/usr/bin/env python3
"""
Benchmark Comparison Script for Logger System
Compares current benchmark results with baseline to detect performance regressions.

Usage:
    python compare_benchmarks.py baseline.json current.json --threshold 5.0
"""

import json
import sys
import argparse
from pathlib import Path
from typing import Dict, List, Tuple, Any, Optional
from dataclasses import dataclass
from enum import Enum


class ChangeType(Enum):
    """Type of performance change"""
    IMPROVEMENT = "improvement"
    REGRESSION = "regression"
    NEUTRAL = "neutral"


@dataclass
class BenchmarkComparison:
    """Result of comparing a single benchmark"""
    name: str
    baseline_time: float
    current_time: float
    change_percent: float
    change_type: ChangeType

    @property
    def is_regression(self) -> bool:
        return self.change_type == ChangeType.REGRESSION

    @property
    def is_improvement(self) -> bool:
        return self.change_type == ChangeType.IMPROVEMENT


class BenchmarkComparator:
    """Compares benchmark results and detects regressions"""

    def __init__(self, threshold_percent: float = 5.0, improvement_threshold: float = -5.0):
        """
        Initialize comparator.

        Args:
            threshold_percent: Percentage threshold for regression (positive value)
            improvement_threshold: Percentage threshold for improvement (negative value)
        """
        self.threshold_percent = threshold_percent
        self.improvement_threshold = improvement_threshold

    def load_benchmark_file(self, filepath: Path) -> Dict[str, Any]:
        """Load benchmark results from JSON file"""
        try:
            with open(filepath, 'r') as f:
                data = json.load(f)
            return data
        except FileNotFoundError:
            print(f"Error: File not found: {filepath}")
            sys.exit(1)
        except json.JSONDecodeError as e:
            print(f"Error: Invalid JSON in {filepath}: {e}")
            sys.exit(1)

    def extract_benchmarks(self, data: Dict[str, Any]) -> Dict[str, float]:
        """
        Extract benchmark name -> time mapping from Google Benchmark JSON format.

        Google Benchmark JSON format:
        {
            "context": {...},
            "benchmarks": [
                {
                    "name": "BM_LoggerWrite/1024",
                    "run_type": "iteration",
                    "real_time": 1234.5,
                    "cpu_time": 1230.0,
                    ...
                }
            ]
        }
        """
        benchmarks = {}

        if "benchmarks" not in data:
            print("Warning: No 'benchmarks' key found in JSON")
            return benchmarks

        for bench in data["benchmarks"]:
            # Skip aggregate results (mean, median, stddev)
            if bench.get("run_type") != "iteration":
                continue

            name = bench.get("name", "unknown")
            # Use real_time as the primary metric (wall clock time)
            # Fallback to cpu_time if real_time is not available
            time = bench.get("real_time", bench.get("cpu_time", 0.0))

            benchmarks[name] = time

        return benchmarks

    def compare_benchmarks(
        self,
        baseline: Dict[str, float],
        current: Dict[str, float]
    ) -> List[BenchmarkComparison]:
        """Compare baseline and current benchmark results"""
        comparisons = []

        # Compare benchmarks that exist in both baseline and current
        for name in baseline.keys():
            if name not in current:
                print(f"Warning: Benchmark '{name}' not found in current results")
                continue

            baseline_time = baseline[name]
            current_time = current[name]

            # Calculate percentage change
            # Positive = slower (regression), Negative = faster (improvement)
            change_percent = ((current_time - baseline_time) / baseline_time) * 100

            # Determine change type
            if change_percent > self.threshold_percent:
                change_type = ChangeType.REGRESSION
            elif change_percent < self.improvement_threshold:
                change_type = ChangeType.IMPROVEMENT
            else:
                change_type = ChangeType.NEUTRAL

            comparisons.append(BenchmarkComparison(
                name=name,
                baseline_time=baseline_time,
                current_time=current_time,
                change_percent=change_percent,
                change_type=change_type
            ))

        # Report benchmarks only in current (new benchmarks)
        for name in current.keys():
            if name not in baseline:
                print(f"Info: New benchmark found: '{name}'")

        return comparisons

    def format_time(self, time_ns: float) -> str:
        """Format time in appropriate unit"""
        if time_ns < 1000:
            return f"{time_ns:.2f}ns"
        elif time_ns < 1_000_000:
            return f"{time_ns / 1000:.2f}μs"
        elif time_ns < 1_000_000_000:
            return f"{time_ns / 1_000_000:.2f}ms"
        else:
            return f"{time_ns / 1_000_000_000:.2f}s"

    def print_summary(self, comparisons: List[BenchmarkComparison]):
        """Print comparison summary"""
        if not comparisons:
            print("No benchmarks to compare")
            return

        regressions = [c for c in comparisons if c.is_regression]
        improvements = [c for c in comparisons if c.is_improvement]
        neutral = [c for c in comparisons if c.change_type == ChangeType.NEUTRAL]

        print("\n" + "=" * 80)
        print("BENCHMARK COMPARISON SUMMARY")
        print("=" * 80)
        print(f"Total benchmarks: {len(comparisons)}")
        print(f"Regressions: {len(regressions)} (>{self.threshold_percent}% slower)")
        print(f"Improvements: {len(improvements)} (<{self.improvement_threshold}% faster)")
        print(f"Neutral: {len(neutral)}")
        print("=" * 80)

        # Print regressions
        if regressions:
            print("\n⚠️  PERFORMANCE REGRESSIONS DETECTED:")
            print("-" * 80)
            for comp in sorted(regressions, key=lambda x: x.change_percent, reverse=True):
                print(f"  {comp.name}")
                print(f"    Baseline: {self.format_time(comp.baseline_time)}")
                print(f"    Current:  {self.format_time(comp.current_time)}")
                print(f"    Change:   {comp.change_percent:+.2f}% slower")
                print()

        # Print improvements
        if improvements:
            print("\n✅ PERFORMANCE IMPROVEMENTS:")
            print("-" * 80)
            for comp in sorted(improvements, key=lambda x: x.change_percent):
                print(f"  {comp.name}")
                print(f"    Baseline: {self.format_time(comp.baseline_time)}")
                print(f"    Current:  {self.format_time(comp.current_time)}")
                print(f"    Change:   {comp.change_percent:+.2f}% faster")
                print()

        # Print neutral changes (optional, for verbose mode)
        # Uncomment if you want to see all results
        # if neutral:
        #     print("\nNEUTRAL CHANGES:")
        #     print("-" * 80)
        #     for comp in neutral:
        #         print(f"  {comp.name}: {comp.change_percent:+.2f}%")

    def generate_markdown_report(
        self,
        comparisons: List[BenchmarkComparison],
        output_file: Optional[Path] = None
    ) -> str:
        """Generate markdown report of comparison results"""
        lines = []
        lines.append("# Benchmark Comparison Report")
        lines.append("")
        lines.append(f"**Regression Threshold:** >{self.threshold_percent}% slower")
        lines.append(f"**Improvement Threshold:** <{self.improvement_threshold}% faster")
        lines.append("")

        regressions = [c for c in comparisons if c.is_regression]
        improvements = [c for c in comparisons if c.is_improvement]
        neutral = [c for c in comparisons if c.change_type == ChangeType.NEUTRAL]

        # Summary table
        lines.append("## Summary")
        lines.append("")
        lines.append("| Category | Count |")
        lines.append("|----------|-------|")
        lines.append(f"| Total Benchmarks | {len(comparisons)} |")
        lines.append(f"| Regressions | {len(regressions)} |")
        lines.append(f"| Improvements | {len(improvements)} |")
        lines.append(f"| Neutral | {len(neutral)} |")
        lines.append("")

        # Regressions table
        if regressions:
            lines.append("## ⚠️ Regressions")
            lines.append("")
            lines.append("| Benchmark | Baseline | Current | Change |")
            lines.append("|-----------|----------|---------|--------|")
            for comp in sorted(regressions, key=lambda x: x.change_percent, reverse=True):
                lines.append(
                    f"| {comp.name} | "
                    f"{self.format_time(comp.baseline_time)} | "
                    f"{self.format_time(comp.current_time)} | "
                    f"**{comp.change_percent:+.2f}%** |"
                )
            lines.append("")

        # Improvements table
        if improvements:
            lines.append("## ✅ Improvements")
            lines.append("")
            lines.append("| Benchmark | Baseline | Current | Change |")
            lines.append("|-----------|----------|---------|--------|")
            for comp in sorted(improvements, key=lambda x: x.change_percent):
                lines.append(
                    f"| {comp.name} | "
                    f"{self.format_time(comp.baseline_time)} | "
                    f"{self.format_time(comp.current_time)} | "
                    f"**{comp.change_percent:+.2f}%** |"
                )
            lines.append("")

        report = "\n".join(lines)

        # Save to file if specified
        if output_file:
            with open(output_file, 'w') as f:
                f.write(report)
            print(f"\nMarkdown report saved to: {output_file}")

        return report


def main():
    parser = argparse.ArgumentParser(
        description="Compare benchmark results and detect performance regressions",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Compare with 5% threshold (default)
  python compare_benchmarks.py baseline.json current.json

  # Use custom threshold
  python compare_benchmarks.py baseline.json current.json --threshold 10.0

  # Generate markdown report
  python compare_benchmarks.py baseline.json current.json --report output.md

  # Verbose output
  python compare_benchmarks.py baseline.json current.json --verbose
        """
    )

    parser.add_argument(
        "baseline",
        type=Path,
        help="Path to baseline benchmark results (JSON)"
    )
    parser.add_argument(
        "current",
        type=Path,
        help="Path to current benchmark results (JSON)"
    )
    parser.add_argument(
        "--threshold",
        type=float,
        default=5.0,
        help="Regression threshold percentage (default: 5.0)"
    )
    parser.add_argument(
        "--improvement-threshold",
        type=float,
        default=-5.0,
        help="Improvement threshold percentage (default: -5.0)"
    )
    parser.add_argument(
        "--report",
        type=Path,
        help="Generate markdown report to file"
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Verbose output"
    )
    parser.add_argument(
        "--fail-on-regression",
        action="store_true",
        default=True,
        help="Exit with error code if regressions detected (default: true)"
    )

    args = parser.parse_args()

    # Create comparator
    comparator = BenchmarkComparator(
        threshold_percent=args.threshold,
        improvement_threshold=args.improvement_threshold
    )

    # Load benchmark results
    if args.verbose:
        print(f"Loading baseline: {args.baseline}")
    baseline_data = comparator.load_benchmark_file(args.baseline)

    if args.verbose:
        print(f"Loading current: {args.current}")
    current_data = comparator.load_benchmark_file(args.current)

    # Extract benchmarks
    baseline_benchmarks = comparator.extract_benchmarks(baseline_data)
    current_benchmarks = comparator.extract_benchmarks(current_data)

    if args.verbose:
        print(f"Baseline benchmarks: {len(baseline_benchmarks)}")
        print(f"Current benchmarks: {len(current_benchmarks)}")

    # Compare
    comparisons = comparator.compare_benchmarks(baseline_benchmarks, current_benchmarks)

    # Print summary
    comparator.print_summary(comparisons)

    # Generate report if requested
    if args.report:
        comparator.generate_markdown_report(comparisons, args.report)

    # Exit with error if regressions detected
    regressions = [c for c in comparisons if c.is_regression]
    if args.fail_on_regression and regressions:
        print(f"\n❌ FAILURE: {len(regressions)} performance regression(s) detected")
        sys.exit(1)
    else:
        print("\n✅ SUCCESS: No performance regressions detected")
        sys.exit(0)


if __name__ == "__main__":
    main()
