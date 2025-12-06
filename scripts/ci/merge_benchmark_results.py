#!/usr/bin/env python3
"""
Merge benchmark result JSON files into a single file.

This script is used by the CI benchmarks workflow to combine
individual benchmark results into one consolidated report.
"""

import json
import glob
import os
import sys


def merge_results(output_pattern: str, matrix_os: str) -> None:
    """Merge all benchmark result files into a single JSON file."""
    results = {'benchmarks': []}

    for f in glob.glob(output_pattern):
        try:
            with open(f) as file:
                data = json.load(file)
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
    pattern = 'build/benchmarks/*_results.json'
    merge_results(pattern, matrix_os)
