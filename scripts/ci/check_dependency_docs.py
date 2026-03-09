#!/usr/bin/env python3
"""Validate dependency category documentation against the vcpkg manifest."""

from __future__ import annotations

import json
import sys
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[2]
MANIFEST_PATH = REPO_ROOT / "vcpkg.json"
SOUP_PATH = REPO_ROOT / "docs" / "SOUP.md"
LICENSE_PATH = REPO_ROOT / "LICENSE-THIRD-PARTY"
README_PATH = REPO_ROOT / "README.md"

DISPLAY_NAMES = {
    "openssl": ("OpenSSL", "openssl"),
    "opentelemetry-cpp": ("OpenTelemetry C++ SDK", "opentelemetry-cpp"),
    "protobuf": ("Protocol Buffers", "protobuf"),
    "grpc": ("gRPC", "grpc"),
    "spdlog": ("spdlog",),
    "gtest": ("Google Test", "GTest", "gtest"),
    "benchmark": ("Google Benchmark", "benchmark"),
}


def load_manifest() -> dict:
    return json.loads(MANIFEST_PATH.read_text(encoding="utf-8"))


def collect_dependency_categories(manifest: dict) -> tuple[set[str], set[str], set[str]]:
    default_build = set()
    optional = set()
    test_only = set()

    for dep in manifest.get("dependencies", []):
        if isinstance(dep, str):
            default_build.add(dep)
        else:
            default_build.add(dep["name"])

    for feature_name, feature_data in manifest.get("features", {}).items():
        for dep in feature_data.get("dependencies", []):
            dep_name = dep if isinstance(dep, str) else dep["name"]
            if feature_name == "testing":
                test_only.add(dep_name)
            elif feature_name == "benchmarks" and dep_name == "benchmark":
                test_only.add(dep_name)
            else:
                optional.add(dep_name)

    return default_build, optional, test_only


def section_between(text: str, start: str, end: str | None) -> str:
    try:
        start_index = text.index(start) + len(start)
    except ValueError as exc:
        raise AssertionError(f"Missing section header: {start}") from exc

    if end is None:
        return text[start_index:]

    try:
        end_index = text.index(end, start_index)
    except ValueError as exc:
        raise AssertionError(f"Missing section header: {end}") from exc

    return text[start_index:end_index]


def has_any_display_name(section: str, dependency_name: str) -> bool:
    aliases = DISPLAY_NAMES.get(dependency_name, (dependency_name,))
    return any(alias in section for alias in aliases)


def assert_contains_all(section: str, expected: set[str], context: str) -> None:
    missing = sorted(name for name in expected if not has_any_display_name(section, name))
    if missing:
        raise AssertionError(f"{context} is missing: {', '.join(missing)}")


def assert_contains_none(section: str, unexpected: set[str], context: str) -> None:
    present = sorted(name for name in unexpected if has_any_display_name(section, name))
    if present:
        raise AssertionError(f"{context} should not contain: {', '.join(present)}")


def check_soup(default_build: set[str], optional: set[str], test_only: set[str]) -> None:
    soup = SOUP_PATH.read_text(encoding="utf-8")
    production = section_between(soup, "## Production SOUP", "## Optional SOUP")
    optional_section = section_between(soup, "## Optional SOUP", "## Development/Test SOUP (Not Deployed)")
    test_section = section_between(soup, "## Development/Test SOUP (Not Deployed)", "## Safety Classification Key")

    if default_build:
        assert_contains_all(production, default_build, "docs/SOUP.md production section")
    elif "*(none in default build)*" not in production:
        raise AssertionError("docs/SOUP.md production section must state that the default build has no SOUP")

    assert_contains_all(optional_section, optional, "docs/SOUP.md optional section")
    assert_contains_all(test_section, test_only, "docs/SOUP.md development/test section")
    assert_contains_none(production, optional | test_only, "docs/SOUP.md production section")


def check_license_inventory(default_build: set[str], optional: set[str], test_only: set[str]) -> None:
    license_doc = LICENSE_PATH.read_text(encoding="utf-8")
    default_section = section_between(license_doc, "## Default Build", "## Optional Feature Dependencies")
    optional_section = section_between(license_doc, "## Optional Feature Dependencies", "## Development and Test Dependencies")
    test_section = section_between(license_doc, "## Development and Test Dependencies", "## Dependency Summary")
    summary_section = section_between(license_doc, "## Dependency Summary", None)

    if default_build:
        assert_contains_all(default_section, default_build, "LICENSE-THIRD-PARTY default section")
    elif "no required third-party production dependencies" not in default_section:
        raise AssertionError("LICENSE-THIRD-PARTY default section must state that the default build has no third-party dependencies")

    assert_contains_all(optional_section, optional, "LICENSE-THIRD-PARTY optional section")
    assert_contains_all(test_section, test_only, "LICENSE-THIRD-PARTY test section")
    assert_contains_none(default_section, optional | test_only, "LICENSE-THIRD-PARTY default section")
    assert_contains_none(summary_section, {"fmt"}, "LICENSE-THIRD-PARTY summary")


def check_readme(default_build: set[str], optional: set[str], test_only: set[str]) -> None:
    readme = README_PATH.read_text(encoding="utf-8")
    optional_section = section_between(readme, "#### Optional Feature Dependencies", "#### Development and Benchmark Dependencies")
    benchmark_section = section_between(readme, "#### Development and Benchmark Dependencies", "#### Dependency Flow")

    if default_build:
        assert_contains_all(readme, default_build, "README.md default-build description")
    elif "no required third-party production dependencies" not in readme:
        raise AssertionError("README.md must describe the default build as having no required third-party production dependencies")

    assert_contains_all(optional_section, optional, "README.md optional dependency section")
    assert_contains_all(benchmark_section, test_only, "README.md development and benchmark dependency section")


def main() -> int:
    manifest = load_manifest()
    default_build, optional, test_only = collect_dependency_categories(manifest)

    check_soup(default_build, optional, test_only)
    check_license_inventory(default_build, optional, test_only)
    check_readme(default_build, optional, test_only)

    print("Dependency documentation is consistent with vcpkg.json")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as error:
        print(f"Dependency documentation check failed: {error}", file=sys.stderr)
        raise SystemExit(1)
