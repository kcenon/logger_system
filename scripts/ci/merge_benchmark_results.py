#!/usr/bin/env python3
"""
Merge benchmark result JSON files into a single file.

This script is used by the CI benchmarks workflow to combine
individual benchmark results into one consolidated report.

Note: Google Benchmark may output NaN values for custom counters when using
--benchmark_report_aggregates_only=true. This script sanitizes such values
to ensure valid JSON output.
"""

import json
import glob
import os
import re
import sys
import math


def sanitize_nan_values(content: str) -> str:
    """
    Replace NaN values with null in JSON content.

    Google Benchmark outputs JavaScript-style NaN which is not valid JSON.
    This function replaces such values with null before parsing.

    Args:
        content: Raw JSON string that may contain NaN values

    Returns:
        Sanitized JSON string with NaN replaced by null
    """
    # Replace standalone NaN (not part of a string) with null
    # Pattern matches: "key": NaN or "key":NaN (with optional whitespace)
    return re.sub(r':\s*NaN\b', ': null', content)


def sanitize_benchmark_data(data: dict) -> dict:
    """
    Recursively sanitize benchmark data, replacing any remaining NaN/Inf values.

    Args:
        data: Dictionary that may contain float('nan') or float('inf') values

    Returns:
        Sanitized dictionary with invalid floats replaced by None
    """
    if isinstance(data, dict):
        return {k: sanitize_benchmark_data(v) for k, v in data.items()}
    elif isinstance(data, list):
        return [sanitize_benchmark_data(item) for item in data]
    elif isinstance(data, float):
        if math.isnan(data) or math.isinf(data):
            return None
        return data
    else:
        return data


def load_json_with_nan_handling(filepath: str) -> dict:
    """
    Load a JSON file that may contain NaN values.

    Args:
        filepath: Path to the JSON file

    Returns:
        Parsed and sanitized JSON data
    """
    with open(filepath, 'r') as f:
        content = f.read()

    # Sanitize NaN values in raw content
    sanitized_content = sanitize_nan_values(content)

    # Parse JSON
    data = json.loads(sanitized_content)

    # Double-check for any remaining invalid values
    return sanitize_benchmark_data(data)


def merge_results(output_pattern: str, matrix_os: str) -> None:
    """Merge all benchmark result files into a single JSON file."""
    results = {'benchmarks': []}

    for f in glob.glob(output_pattern):
        try:
            data = load_json_with_nan_handling(f)
            if 'benchmarks' in data:
                results['benchmarks'].extend(data['benchmarks'])
            if 'context' not in results and 'context' in data:
                results['context'] = data['context']
        except (json.JSONDecodeError, IOError) as e:
            print(f"Warning: Could not process {f}: {e}", file=sys.stderr)

    output_file = f'build/benchmark_results_{matrix_os}.json'
    with open(output_file, 'w') as out:
        json.dump(results, out, indent=2)

    print(f"Merged {len(results['benchmarks'])} benchmarks to {output_file}")


if __name__ == '__main__':
    matrix_os = os.environ.get('MATRIX_OS', 'unknown')
    # Search in both bin and benchmarks directories
    patterns = [
        'build/bin/*_results.json',
        'build/benchmarks/*_results.json',
    ]

    results = {'benchmarks': []}
    files_found = []
    nan_sanitized = False

    for pattern in patterns:
        for f in glob.glob(pattern):
            files_found.append(f)
            try:
                # Use NaN-safe loading
                data = load_json_with_nan_handling(f)
                if 'benchmarks' in data:
                    results['benchmarks'].extend(data['benchmarks'])
                if 'context' not in results and 'context' in data:
                    results['context'] = data['context']

                # Check if file had NaN values
                with open(f, 'r') as raw_file:
                    if 'NaN' in raw_file.read():
                        nan_sanitized = True
            except (json.JSONDecodeError, IOError) as e:
                print(f"Warning: Could not process {f}: {e}", file=sys.stderr)

    output_file = f'build/benchmark_results_{matrix_os}.json'
    with open(output_file, 'w') as out:
        json.dump(results, out, indent=2)

    print(f"Found files: {files_found}")
    if nan_sanitized:
        print("Note: NaN values were sanitized to null for JSON compliance")
    print(f"Merged {len(results['benchmarks'])} benchmarks to {output_file}")
