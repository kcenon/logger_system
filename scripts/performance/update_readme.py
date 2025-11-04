#!/usr/bin/env python3
"""
Update README.md with latest performance metrics.

This script automatically updates the performance section in README.md
with the latest benchmark results, badges, and charts.

Usage:
    python3 update_readme.py

Input:
    benchmarks/results/summary.json
    docs/performance/badges/badges.json
    README.md

Output:
    README.md (updated)
"""

import json
import re
from pathlib import Path
from typing import Dict, Any, Optional
import sys


def load_summary() -> Dict[str, Any]:
    """
    Load performance summary.

    Returns:
        Performance summary dictionary

    Raises:
        FileNotFoundError: If summary file doesn't exist
    """
    summary_file = Path("benchmarks/results/summary.json")

    if not summary_file.exists():
        raise FileNotFoundError(f"Summary file not found: {summary_file}")

    with open(summary_file) as f:
        data = json.load(f)

    return data.get("summary", {})


def load_badge_info() -> Optional[Dict[str, Any]]:
    """
    Load badge information.

    Returns:
        Badge information or None if not available
    """
    badge_file = Path("docs/performance/badges/badges.json")

    if not badge_file.exists():
        return None

    with open(badge_file) as f:
        return json.load(f)


def format_throughput(value: float) -> str:
    """Format throughput value for display."""
    if value >= 1_000_000:
        return f"{value / 1_000_000:.2f}M"
    elif value >= 1_000:
        return f"{value / 1_000:.0f}K"
    else:
        return f"{value:.0f}"


def format_latency(value_ns: float) -> str:
    """Format latency value for display."""
    if value_ns >= 1_000_000:
        return f"{value_ns / 1_000_000:.2f} milliseconds"
    elif value_ns >= 1_000:
        return f"{value_ns / 1_000:.2f} microseconds"
    else:
        return f"{value_ns:.0f} nanoseconds"


def generate_performance_section(summary: Dict[str, Any], badge_info: Optional[Dict[str, Any]]) -> str:
    """
    Generate performance section content.

    Args:
        summary: Performance summary
        badge_info: Badge information

    Returns:
        Markdown content for performance section
    """
    lines = []

    # Header
    lines.append("### 📊 Performance Benchmarks\n\n")
    lines.append("*Benchmarked on Apple M1 (8-core) @ 3.2GHz, 16GB, macOS Sonoma*\n\n")

    # Auto-generated marker
    lines.append("<!-- AUTO-GENERATED: Do not edit manually -->\n")
    lines.append("<!-- Performance metrics are automatically updated by CI/CD -->\n\n")

    # Badges
    if badge_info:
        for badge in badge_info:
            badge_path = badge.get("file", "")
            if badge_path:
                lines.append(f"![{badge['name']}]({badge_path}) ")
        lines.append("\n\n")

    # Key metrics
    lines.append("**Key Performance Metrics:**\n\n")

    if "peak_throughput_msg_per_sec" in summary:
        throughput = summary["peak_throughput_msg_per_sec"]
        lines.append(f"- **Peak Throughput**: {format_throughput(throughput)} messages/second\n")

    if "avg_throughput_msg_per_sec" in summary:
        avg_throughput = summary["avg_throughput_msg_per_sec"]
        lines.append(f"- **Average Throughput**: {format_throughput(avg_throughput)} messages/second\n")

    if "avg_latency_ns" in summary:
        latency = summary["avg_latency_ns"]
        lines.append(f"- **Average Latency**: {format_latency(latency)}\n")

    if "min_latency_ns" in summary:
        min_latency = summary["min_latency_ns"]
        lines.append(f"- **Minimum Latency**: {format_latency(min_latency)}\n")

    if "avg_rotation_time_ms" in summary:
        rotation = summary["avg_rotation_time_ms"]
        lines.append(f"- **Average Rotation Time**: {rotation:.2f} milliseconds\n")

    # Performance trends
    lines.append("\n**Performance Trends:**\n\n")
    lines.append("![Throughput Trend](docs/performance/charts/throughput_trend.png)\n\n")
    lines.append("*See [detailed performance history](docs/performance/performance_history.json) for complete data.*\n\n")

    # End marker
    lines.append("<!-- END AUTO-GENERATED -->\n")

    return "".join(lines)


def update_readme_performance_section(
    readme_content: str,
    summary: Dict[str, Any],
    badge_info: Optional[Dict[str, Any]]
) -> str:
    """
    Update performance section in README content.

    Args:
        readme_content: Current README content
        summary: Performance summary
        badge_info: Badge information

    Returns:
        Updated README content
    """
    # Pattern to match performance section
    # Look for the section between AUTO-GENERATED markers
    pattern = (
        r'<!-- AUTO-GENERATED: Do not edit manually -->.*?'
        r'<!-- END AUTO-GENERATED -->'
    )

    # Generate new performance content
    new_section = generate_performance_section(summary, badge_info)

    # Remove markers from new section for matching
    new_content_body = new_section.split('<!-- AUTO-GENERATED: Do not edit manually -->')[1]
    new_content_body = new_content_body.split('<!-- END AUTO-GENERATED -->')[0]

    # Try to replace existing auto-generated section
    if re.search(pattern, readme_content, flags=re.DOTALL):
        # Extract just the content between markers
        replacement = (
            '<!-- AUTO-GENERATED: Do not edit manually -->' +
            new_content_body +
            '<!-- END AUTO-GENERATED -->'
        )

        updated_content = re.sub(
            pattern,
            replacement,
            readme_content,
            flags=re.DOTALL
        )

        print("✅ Updated existing auto-generated section")
        return updated_content

    # If no auto-generated section exists, try to find performance benchmarks section
    perf_pattern = r'(###? 📊 Performance Benchmarks.*?)(?=\n###?[^#]|\Z)'

    if re.search(perf_pattern, readme_content, flags=re.DOTALL):
        updated_content = re.sub(
            perf_pattern,
            new_section,
            readme_content,
            flags=re.DOTALL,
            count=1
        )

        print("✅ Replaced performance benchmarks section")
        return updated_content

    # If no section found, append to end
    print("⚠️ Warning: No performance section found, appending to end")
    return readme_content + "\n\n" + new_section


def backup_readme(readme_file: Path):
    """
    Create backup of README before modification.

    Args:
        readme_file: Path to README file
    """
    backup_file = readme_file.with_suffix('.md.backup')
    backup_file.write_text(readme_file.read_text())
    print(f"✅ Backup created: {backup_file}")


def main():
    """Main entry point."""
    readme_file = Path("README.md")

    try:
        # Check if README exists
        if not readme_file.exists():
            print(f"❌ Error: README.md not found at {readme_file}")
            return 1

        # Load summary
        print("Loading performance summary...")
        summary = load_summary()

        if not summary:
            print("⚠️ Warning: No summary metrics found")
            return 1

        # Load badge info
        print("Loading badge information...")
        badge_info = load_badge_info()

        # Read current README
        print(f"Reading {readme_file}...")
        readme_content = readme_file.read_text()

        # Create backup
        backup_readme(readme_file)

        # Update README
        print("Updating README with latest metrics...")
        updated_content = update_readme_performance_section(
            readme_content,
            summary,
            badge_info
        )

        # Write updated README
        readme_file.write_text(updated_content)

        print(f"\n✅ README.md updated successfully!")
        print(f"   Peak Throughput: {format_throughput(summary.get('peak_throughput_msg_per_sec', 0))} msg/s")
        print(f"   Avg Latency: {format_latency(summary.get('avg_latency_ns', 0))}")

        return 0

    except FileNotFoundError as e:
        print(f"❌ Error: {e}")
        return 1
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
