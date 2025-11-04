#!/usr/bin/env python3
"""
Generate performance badges using shields.io format.

This script creates SVG badges displaying key performance metrics
for inclusion in README.md and documentation.

Usage:
    python3 generate_badges.py

Input:
    benchmarks/results/summary.json

Output:
    docs/performance/badges/*.svg
    docs/performance/badges/badges.json (badge metadata)
"""

import json
from pathlib import Path
from typing import Dict, Any, List


def format_throughput(value: float) -> str:
    """
    Format throughput value for display.

    Args:
        value: Throughput in messages/second

    Returns:
        Formatted string (e.g., "4.34M", "1.5K")
    """
    if value >= 1_000_000:
        return f"{value / 1_000_000:.2f}M"
    elif value >= 1_000:
        return f"{value / 1_000:.1f}K"
    else:
        return f"{value:.0f}"


def format_latency(value_ns: float) -> str:
    """
    Format latency value for display.

    Args:
        value_ns: Latency in nanoseconds

    Returns:
        Formatted string (e.g., "148ns", "2.5μs", "1.2ms")
    """
    if value_ns >= 1_000_000:
        return f"{value_ns / 1_000_000:.2f}ms"
    elif value_ns >= 1_000:
        return f"{value_ns / 1_000:.2f}μs"
    else:
        return f"{value_ns:.0f}ns"


def format_memory(value_mb: float) -> str:
    """
    Format memory value for display.

    Args:
        value_mb: Memory in MB

    Returns:
        Formatted string (e.g., "2.5MB", "128KB")
    """
    if value_mb >= 1:
        return f"{value_mb:.1f}MB"
    else:
        return f"{value_mb * 1024:.0f}KB"


def get_badge_color(metric_name: str, value: float) -> str:
    """
    Determine badge color based on performance thresholds.

    Args:
        metric_name: Name of the metric
        value: Numeric value

    Returns:
        Color name (brightgreen, green, yellow, orange, red)
    """
    # Throughput thresholds (messages/sec)
    if "throughput" in metric_name.lower():
        if value >= 4_000_000:
            return "brightgreen"
        elif value >= 2_000_000:
            return "green"
        elif value >= 1_000_000:
            return "yellow"
        elif value >= 500_000:
            return "orange"
        else:
            return "red"

    # Latency thresholds (nanoseconds)
    elif "latency" in metric_name.lower():
        if value <= 200:
            return "brightgreen"
        elif value <= 500:
            return "green"
        elif value <= 1000:
            return "yellow"
        elif value <= 5000:
            return "orange"
        else:
            return "red"

    # Rotation time thresholds (milliseconds)
    elif "rotation" in metric_name.lower():
        if value <= 10:
            return "brightgreen"
        elif value <= 50:
            return "green"
        elif value <= 100:
            return "yellow"
        elif value <= 200:
            return "orange"
        else:
            return "red"

    # Default color
    return "blue"


def get_color_hex(color_name: str) -> str:
    """
    Convert color name to hex code.

    Args:
        color_name: Color name

    Returns:
        Hex color code (without #)
    """
    colors = {
        "brightgreen": "4c1",
        "green": "97ca00",
        "yellow": "dfb317",
        "orange": "fe7d37",
        "red": "e05d44",
        "blue": "007ec6",
        "lightgrey": "9f9f9f"
    }
    return colors.get(color_name, "007ec6")


def generate_static_badge_svg(
    label: str,
    value: str,
    color: str,
    output_file: Path
):
    """
    Generate static SVG badge file.

    Args:
        label: Left side text (e.g., "throughput")
        value: Right side text (e.g., "4.34M msg/s")
        color: Color name for right side
        output_file: Output SVG file path
    """
    # Calculate text widths (approximate)
    label_width = len(label) * 7 + 10
    value_width = len(value) * 7 + 10
    total_width = label_width + value_width

    # Get color hex
    color_hex = get_color_hex(color)

    # SVG template
    svg_template = f"""<svg xmlns="http://www.w3.org/2000/svg" width="{total_width}" height="20">
  <linearGradient id="b" x2="0" y2="100%">
    <stop offset="0" stop-color="#bbb" stop-opacity=".1"/>
    <stop offset="1" stop-opacity=".1"/>
  </linearGradient>
  <clipPath id="a">
    <rect width="{total_width}" height="20" rx="3" fill="#fff"/>
  </clipPath>
  <g clip-path="url(#a)">
    <path fill="#555" d="M0 0h{label_width}v20H0z"/>
    <path fill="#{color_hex}" d="M{label_width} 0h{value_width}v20H{label_width}z"/>
    <path fill="url(#b)" d="M0 0h{total_width}v20H0z"/>
  </g>
  <g fill="#fff" text-anchor="middle" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="11">
    <text x="{label_width/2}" y="15" fill="#010101" fill-opacity=".3">{label}</text>
    <text x="{label_width/2}" y="14">{label}</text>
    <text x="{label_width + value_width/2}" y="15" fill="#010101" fill-opacity=".3">{value}</text>
    <text x="{label_width + value_width/2}" y="14">{value}</text>
  </g>
</svg>"""

    with open(output_file, 'w') as f:
        f.write(svg_template)


def generate_badge_url(label: str, value: str, color: str) -> str:
    """
    Generate shields.io badge URL.

    Args:
        label: Left side text
        value: Right side text
        color: Color name

    Returns:
        shields.io URL
    """
    # URL encode spaces and special characters
    label_encoded = label.replace(' ', '%20')
    value_encoded = value.replace(' ', '%20').replace('/', '%2F')

    return (
        f"https://img.shields.io/badge/"
        f"{label_encoded}-"
        f"{value_encoded}-"
        f"{color}"
    )


def generate_badges(summary: Dict[str, Any], output_dir: Path) -> List[Dict[str, str]]:
    """
    Generate all performance badges.

    Args:
        summary: Performance summary metrics
        output_dir: Output directory for SVG files

    Returns:
        List of badge metadata
    """
    output_dir.mkdir(parents=True, exist_ok=True)

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
            output_dir / "throughput.svg"
        )

        badges_info.append({
            "name": "Throughput",
            "label": "throughput",
            "value": value_str,
            "color": color,
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
            output_dir / "latency.svg"
        )

        badges_info.append({
            "name": "Latency",
            "label": "latency",
            "value": value_str,
            "color": color,
            "file": "docs/performance/badges/latency.svg",
            "url": generate_badge_url("latency", value_str, color)
        })

    # Rotation time badge (if available)
    if "avg_rotation_time_ms" in summary:
        rotation = summary["avg_rotation_time_ms"]
        value_str = f"{rotation:.1f}ms"
        color = get_badge_color("rotation", rotation)

        generate_static_badge_svg(
            "rotation",
            value_str,
            color,
            output_dir / "rotation.svg"
        )

        badges_info.append({
            "name": "Rotation Time",
            "label": "rotation",
            "value": value_str,
            "color": color,
            "file": "docs/performance/badges/rotation.svg",
            "url": generate_badge_url("rotation", value_str, color)
        })

    return badges_info


def save_badge_metadata(badges_info: List[Dict[str, str]], output_dir: Path):
    """
    Save badge metadata to JSON file.

    Args:
        badges_info: List of badge metadata
        output_dir: Output directory
    """
    metadata_file = output_dir / "badges.json"
    with open(metadata_file, 'w') as f:
        json.dump(badges_info, f, indent=2)

    print(f"✅ Badge metadata saved to {metadata_file}")


def main():
    """Main entry point."""
    summary_file = Path("benchmarks/results/summary.json")
    badges_dir = Path("docs/performance/badges")

    try:
        # Load performance summary
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

        # Generate badges
        print("Generating performance badges...")
        badges_info = generate_badges(summary, badges_dir)

        # Save metadata
        save_badge_metadata(badges_info, badges_dir)

        # Print summary
        print("\n✅ Performance badges generated:")
        for badge in badges_info:
            print(f"  - {badge['name']}: {badge['value']} ({badge['color']})")
            print(f"    File: {badge['file']}")

        print(f"\n📁 Output directory: {badges_dir}")

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
    import sys
    sys.exit(main())
