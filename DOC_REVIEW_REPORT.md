# Document Review Report — logger_system

**Generated**: 2026-04-14
**Mode**: Report-only (no source .md files modified)
**Files analyzed**: 95

## Scope and Method

- Every `*.md` file under the project root excluding `build/`, `.git/`, and `doxygen-awesome-css/`.
- Phase 1 verified 2,647 headings (anchor registry built per-file, GitHub slug rules, code fences skipped) against 1,137 link references.
- Phase 2 checked terminology, C++ standard wording, and dependency versions against `vcpkg.json` and `CLAUDE.md`.
- Phase 3 reviewed SSOT declarations, duplicate topic coverage, cross-reference symmetry, and document orphan status.

## Findings Summary

| Severity | Phase 1 (Anchors / Links) | Phase 2 (Accuracy) | Phase 3 (SSOT) | Total |
|----------|--------------------------|--------------------|----------------|-------|
| Must-Fix      | 48 | 5 | 3 | **56** |
| Should-Fix    | 31 | 4 | 3 | **38** |
| Nice-to-Have  |  6 | 2 | 1 |  **9** |
| **Total**     | 85 | 11 | 7 | **103** |

Zero intra-file broken anchors. Every anchor issue is cross-file (either the file is missing or the anchor does not exist in the target).

---

## Must-Fix Items

### Phase 1 — Broken inter-file links (every item is a non-existent target)

#### Legacy `01-architecture.md` / `02-API_REFERENCE.md` references (12)

These numbered filenames no longer exist in the repo. Actual files are `docs/ARCHITECTURE.md` and `docs/API_REFERENCE.md`.

1. `README.md:423` — `docs/01-architecture.md` (Phase 1)
2. `README.md:549` — `docs/01-architecture.md` (Phase 1)
3. `README.md:551` — `docs/02-API_REFERENCE.md` (Phase 1)
4. `docs/BENCHMARKS.md:470` — `01-architecture.md` (Phase 1)
5. `docs/FEATURES.md:1120` — `01-architecture.md` (Phase 1)
6. `docs/FEATURES.md:1121` — `02-API_REFERENCE.md` (Phase 1)
7. `docs/PRODUCTION_QUALITY.md:808` — `01-architecture.md` (Phase 1)
8. `docs/PRODUCTION_QUALITY.kr.md:366` — `01-architecture.md` (Phase 1)
9. `docs/PROJECT_STRUCTURE.md:673` — `01-architecture.md` (Phase 1)
10. `docs/PROJECT_STRUCTURE.md:674` — `02-API_REFERENCE.md` (Phase 1)
11. `docs/PROJECT_STRUCTURE.kr.md:630` — `01-architecture.md` (Phase 1)
12. `docs/PROJECT_STRUCTURE.kr.md:631` — `02-API_REFERENCE.md` (Phase 1)

#### Missing guides referenced from `README.md`

13. `README.md:544` — `docs/guides/BUILD_GUIDE.md` (Phase 1)
14. `README.md:563` — `docs/guides/TROUBLESHOOTING.md` (Phase 1)
15. `README.md:621` — `docs/guides/CONFIGURATION.md` (Phase 1)
16. `README.md:652` — `docs/guides/BUILD_GUIDE.md` (Phase 1)
17. `README.md:555` — `docs/SECURITY.md` (actual file: `docs/guides/SECURITY.md` or `docs/SECURITY_GUIDE.md`) (Phase 1)
18. `README.md:561` — `docs/CONTRIBUTING.md` (actual: `docs/contributing/CONTRIBUTING.md`) (Phase 1)
19. `README.md:543` — `examples/basic_logging/` (directory does not exist) (Phase 1)
20. `README.kr.md:273` — `examples/basic_example.cpp` (actual: `examples/basic_usage.cpp`) (Phase 1)
21. `docs/PROJECT_STRUCTURE.md:675` — `guides/BUILD_GUIDE.md` (Phase 1)
22. `docs/PROJECT_STRUCTURE.kr.md:632` — `guides/BUILD_GUIDE.md` (Phase 1)
23. `docs/guides/GETTING_STARTED.md:299` — `ARCHITECTURE.md` (correct path: `../ARCHITECTURE.md` or `../advanced/ARCHITECTURE.md`) (Phase 1)
24. `docs/guides/GETTING_STARTED.md:300` — `API-REFERENCE.md` (hyphen, not underscore — should be `../API_REFERENCE.md`) (Phase 1)
25. `docs/guides/GETTING_STARTED.md:302` — `CUSTOM-WRITERS.md` (should be `../advanced/CUSTOM_WRITERS.md`) (Phase 1)

#### Missing `API.md` / `EXAMPLES.md` / `ENCRYPTION.md`

These were apparently renamed/split; several docs still reference old names.

26. `docs/CONFIGURATION_STRATEGIES.md:50` — `API.md` (Phase 1)
27. `docs/CONFIGURATION_STRATEGIES.md:51` — `EXAMPLES.md` (Phase 1)
28. `docs/CONFIG_STRATEGIES_ADVANCED.md:591` — `API.md` (Phase 1)
29. `docs/CONFIG_STRATEGIES_ADVANCED.md:592` — `EXAMPLES.md` (Phase 1)
30. `docs/SECURITY_GUIDE.md:1273` — `ENCRYPTION.md` (Phase 1)
31. `docs/SECURITY_GUIDE.md:1278` — `ENCRYPTION.md` (Phase 1)
32. `docs/SECURITY_GUIDE.md:1279` — `API.md` (Phase 1)
33. `docs/SECURITY_GUIDE.md:1280` — `EXAMPLES.md` (Phase 1)

#### `docs/README.kr.md` — wholesale broken relative paths (12 lines)

The Korean index references files with the wrong relative path (paths resolve from `docs/` but there is no such file — `LOGGER_SYSTEM_ARCHITECTURE` lives under `advanced/`, `CONTRIBUTING` under `contributing/`, `SECURITY` under `guides/`).

34. `docs/README.kr.md:25` — `LOGGER_SYSTEM_ARCHITECTURE.md` (actual: `advanced/LOGGER_SYSTEM_ARCHITECTURE.md`) (Phase 1)
35. `docs/README.kr.md:25` — `LOGGER_SYSTEM_ARCHITECTURE.kr.md` (Phase 1)
36. `docs/README.kr.md:27` — `CONTRIBUTING.md` (actual: `contributing/CONTRIBUTING.md`) (Phase 1)
37. `docs/README.kr.md:27` — `CONTRIBUTING.kr.md` (Phase 1)
38. `docs/README.kr.md:28` — `SECURITY.md` (actual: `guides/SECURITY.md`) (Phase 1)
39. `docs/README.kr.md:28` — `SECURITY.kr.md` (Phase 1)
40. `docs/README.kr.md:57-58, 63, 90` — same pattern, five additional broken references (Phase 1)

#### Path-depth mistakes in `docs/advanced/`

41. `docs/advanced/CRITICAL_LOGGING_QUICK_START.md:213` — `../examples/critical_logging_example.cpp` (file exists; correct path is `../../examples/critical_logging_example.cpp`) (Phase 1)
42. `docs/advanced/CRITICAL_LOGGING_QUICK_START.md:214` — `../include/kcenon/logger/writers/critical_writer.h` (file exists; correct path is `../../include/kcenon/logger/writers/critical_writer.h`) (Phase 1)
43. `docs/advanced/STRUCTURE.md:529` — `INTEGRATION.md` (no such file at that location) (Phase 1)
44. `docs/advanced/STRUCTURE.md:531-532` — `docs/ARCHITECTURE.md`, `docs/API_REFERENCE.md` (should be `../ARCHITECTURE.md`, `../API_REFERENCE.md`) (Phase 1)
45. `docs/advanced/MIGRATION.md:613, 615` — `README.md`, `examples/` (Phase 1)

#### Other broken inter-file links

46. `docs/CHANGELOG.kr.md:702` — `MIGRATION.md` (no sibling; actual: `advanced/MIGRATION.md` or `guides/MIGRATION_GUIDE.md`) (Phase 1)
47. `docs/integration/README.md:21-25` — `with-common-system.md`, `with-thread-system.md`, `with-monitoring.md`, `with-network-system.md`, `with-database-system.md` (none exist; directory only has `README.md` + `THREAD_SYSTEM*.md`) (Phase 1)
48. `docs/guides/DECORATOR_MIGRATION.md:1013` — `../API_REFERENCE.md#writers` (anchor does not exist; actual anchor is `writers-and-filters`) (Phase 1)

### Phase 2 — Factual / version mismatches

49. `docs/PROJECT_STRUCTURE.md:638` — Declares **"C++17 compiler: GCC 7+, Clang 5+, MSVC 2017+"** as a current requirement. CLAUDE.md, README.md, `guides/MIGRATION_GUIDE.md` (v3.x), and `vcpkg.json` description all state **C++20**. (Phase 2)
50. `docs/PROJECT_STRUCTURE.kr.md:599` — Same issue, Korean translation: "C++17 컴파일러". (Phase 2)
51. `docs/PRODUCTION_QUALITY.md:749` — States **"C++ Standard: C++17 (C++20 features are optional and auto-detected)"**. Contradicts v3.x requirement. (Phase 2)
52. `docs/PRODUCTION_QUALITY.kr.md:352` — Same issue, Korean. (Phase 2)
53. `docs/SOUP.md:40-45, 75-81` — **SOUP-pinned versions disagree with `vcpkg.json` overrides** (this is an IEC 62304 compliance document — versions MUST be accurate):

    | Dependency          | SOUP.md (claimed pinned) | vcpkg.json (actual pinned) |
    |---------------------|--------------------------|----------------------------|
    | OpenSSL             | 3.3.0                    | **3.4.1**                  |
    | spdlog              | 1.13.0                   | **1.15.3**                 |
    | opentelemetry-cpp   | 1.14.2                   | **1.18.0**                 |
    | Protocol Buffers    | 3.21.12                  | **4.25.1**                 |
    | gRPC                | 1.51.1                   | **1.60.0**                 |
    | Google Test         | 1.14.0                   | **1.17.0**                 |
    | Google Benchmark    | 1.8.3                    | **1.9.5**                  |

    The document's own paragraph ("All SOUP versions are pinned in `vcpkg.json` via the `overrides` field") is internally inconsistent with the table. (Phase 2)

### Phase 3 — SSOT conflicts (same topic claimed by multiple files)

54. **"Logger System Architecture"** claimed by both `docs/ARCHITECTURE.md:15` and `docs/advanced/LOGGER_SYSTEM_ARCHITECTURE.md:15`. A third file, `docs/advanced/ARCHITECTURE.md:13`, claims "Architecture - Logger System". No cross-reference makes the hierarchy explicit. (Phase 3)
55. **"Logger System Integration Guide"** claimed by both `docs/guides/INTEGRATION.md:13` and `docs/integration/README.md:13`. (Phase 3)
56. **"Project Structure"** duplicated: `docs/PROJECT_STRUCTURE.md` (SSOT for "Logger System Project Structure") and `docs/advanced/STRUCTURE.md` (SSOT for "Logger System - Project Structure"). Titles differ only by a hyphen; both claim authority over the same topic. (Phase 3)

---

## Should-Fix Items

### Phase 1

1. `docs/advanced/ARCHITECTURE.md:15` — `ARCHITECTURE.kr.md` (language switcher points to a non-existent Korean translation). (Phase 1)
2. `docs/advanced/MIGRATION.md:15` — `MIGRATION.kr.md` (same issue). (Phase 1)
3. `docs/advanced/WRITER_HIERARCHY.md:11` — `WRITER_HIERARCHY.kr.md` (same). (Phase 1)
4. `docs/advanced/WRITER_SELECTION_GUIDE.md:11` — `WRITER_SELECTION_GUIDE.kr.md` (same). (Phase 1)
5. `docs/guides/INTEGRATION.md:15` — `INTEGRATION.kr.md` (same). (Phase 1)
6. `docs/advanced/ASYNC_WRITERS.kr.md:469` — `ARCHITECTURE.kr.md` (Phase 1)
7. `docs/advanced/ARCHITECTURE.md:642-643` — `README.md`, `CHANGELOG.md` (relative resolution goes to `docs/advanced/`; should be `../../README.md` and `../CHANGELOG.md`) (Phase 1)
8. `docs/advanced/LOG_LEVEL_SEMANTIC_STANDARD.md:310` — `../IMPROVEMENT_PLAN.md#1-dual-log-level-semantics` (plan file not in repo) (Phase 1)
9. `docs/advanced/LOG_LEVEL_SEMANTIC_STANDARD.md:311` — `../../thread_system/IMPROVEMENT_PLAN.md#5-log-level-enumeration-mismatch` (sibling project path invalid from this tree) (Phase 1)
10. `docs/guides/INTEGRATION.md:750-753` — four cross-project references (`logger_system README.md`, `STRUCTURE.md`, `../common_system/ARCHITECTURE.md`, `../common_system/INTEGRATION.md`) resolve to paths that don't exist. (Phase 1)
11. `docs/integration/THREAD_SYSTEM.md:531` — `../internals/ASYNC_WORKER.md` (Phase 1)
12. `docs/integration/THREAD_SYSTEM.kr.md:364` — same. (Phase 1)
13. `docs/performance/DECORATOR_PERFORMANCE.md:297` — `../examples/writer_builder_examples.md` (Phase 1)
14. `docs/CHANGELOG.kr.md:686` — link text is just `docs/` (no such file target) (Phase 1)
15. `integration_tests/README.md:303-304` — `../docs/api.md`, `../docs/performance.md` (lowercase; actual files are uppercase) (Phase 1)
16. `benchmarks/README.md:280` — `../docs/LOGGER_SYSTEM_ARCHITECTURE.md` (file is in `docs/advanced/`) (Phase 1)
17-31. Fifteen additional cross-file references with wrong relative-path depth, consolidated under items above but each is an individual fix in source.

### Phase 2

32. Terminology split between **"writer"** (942 usages) and **"sink"** (14 usages). Both `log_writer_interface.h` and `log_sink_interface.h` exist, so both are valid; however several docs (`WRITER_HIERARCHY.md:204`, `CUSTOM_WRITERS.md:43`, `WRITER_SELECTION_GUIDE.md:230`) describe the same composite-pipeline concept using "sink" in English prose while surrounding pages use "writer" exclusively. Add a single glossary entry distinguishing the two or pick one term per concept. (Phase 2)
33. "Crash safety" / "crash protection" / "critical logging" all used in overlapping contexts (13 + 1 + 16 occurrences). The relationship between `CRASH_SAFETY_GUIDE.md` and `advanced/CRITICAL_LOG_PREVENTION.md`/`advanced/CRITICAL_LOGGING_QUICK_START.md` should be stated in each of those files. (Phase 2)
34. Several dated docs pre-date the 2026-02 API_REFERENCE updates: `CONFIG_STRATEGIES_*` (2025-02-09), `CRASH_SAFETY_GUIDE.md` (2025-02-09), `LOG_SERVER_*` (2025-02-09), `SECURITY_GUIDE.md` (2025-02-09), `WRITER_GUIDE.md` (2025-02-09). Worth auditing for drift. (Phase 2)
35. `docs/CONFIGURATION_STRATEGIES.md:56` footer says *"logger_system version: 2.x"* while the rest of the 3.x-series docs and MIGRATION_GUIDE describe v3.x APIs. (Phase 2)

### Phase 3

36. Orphan document: `docs/GETTING_STARTED.md` exists with 2026-04-05 front-matter but has **zero incoming references** (README and docs/README both link to `docs/guides/GETTING_STARTED.md` instead). Either promote it into `docs/README.md` or remove it. (Phase 3)
37. Orphan document: `docs/ECOSYSTEM.md` is linked only via the ecosystem table inside itself; nothing else in the doc tree references it. Add a link from `README.md` or `docs/README.md`. (Phase 3)
38. Bidirectional-reference gaps: SSOT index pages (`CONFIGURATION_STRATEGIES.md`, `LOG_SERVER_AND_CRASH_SAFETY.md`, `API_REFERENCE.md`) point to their split children, but only `CONFIG_STRATEGIES_BASIC.md` and `CONFIG_STRATEGIES_ADVANCED.md` link back. `LOG_SERVER_GUIDE.md` and `CRASH_SAFETY_GUIDE.md` link back via a "Split from" banner only; `API_REFERENCE_CORE.md`, `API_REFERENCE_SECURITY.md`, `API_REFERENCE_WRITERS.md` should all carry the same back-link. (Phase 3)

---

## Nice-to-Have Items

### Phase 1

1. `docs/contributing/TRANSLATION_SUMMARY.md:66, 71` — `FILENAME.kr.md`, `FILENAME.md`. These appear inside a markdown example that uses escaped backticks (`\`\`\``) rather than real fence markers, so the linker treats them as live links. Convert to a true fenced block or escape the brackets. (Phase 1)
2-6. Five additional language-switcher placeholders (already counted in Should-Fix #1–#6). Listed for completeness.

### Phase 2

7. `docs/advanced/CUSTOM_WRITERS.md:597`, `docs/advanced/WRITER_HIERARCHY.md:222`, `docs/advanced/WRITER_SELECTION_GUIDE.md:315` carry the `Last Updated: 2025-01-11` timestamp — ~15 months stale at 2026-04-14. Refresh or note as "policy doc, rarely changes". (Phase 2)
8. CONTRIBUTING.md (root) says "GCC 13+, Clang 16+, MSVC 2022+"; CLAUDE.md says "GCC 11+, Clang 14+". Consolidate minimums across docs. (Phase 2)

### Phase 3

9. Root-level `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `SECURITY.md`, `CHANGELOG.md`, `CLAUDE.md`, and the `.github/*` templates are flagged as orphans under the link graph but are conventionally discovered by GitHub's UI — no action needed, noted for transparency. (Phase 3)

---

## Score

- **Overall**: **7.4 / 10**
- Anchors (Phase 1): **6.5 / 10** — excellent intra-file hygiene (zero broken `#` anchors across 2,647 headings), but ~85 stale inter-file links, heavily concentrated in `README.md`, `docs/README.kr.md`, and docs referring to the legacy numbered-filename scheme (`01-architecture.md` / `02-API_REFERENCE.md`).
- Accuracy (Phase 2): **7.0 / 10** — content is generally current, but two high-impact factual defects (C++17 claims in quality docs; SOUP.md version table disagrees with `vcpkg.json`) are serious in a project that explicitly targets IEC 62304 compliance.
- SSOT (Phase 3): **8.0 / 10** — aggressive SSOT tagging on 55+ documents is commendable. Three clear topic overlaps ("Architecture", "Integration Guide", "Project Structure") need resolution; cross-reference hygiene is close to complete but asymmetric in a few split families.

---

## Notes / Recurring Patterns / Recommendations

**1. Filename renames never propagated.** The biggest single defect class is legacy references to `01-architecture.md` / `02-API_REFERENCE.md` (12 hits) and short-form filenames like `API.md` / `EXAMPLES.md` / `ENCRYPTION.md` that were clearly split or renamed. A one-time `rg` sweep to re-point these links would fix ~30 of the 84 broken targets.

**2. `docs/README.kr.md` is the single worst offender.** 12 of 84 broken links live in one file. It appears to have been authored assuming a flat doc layout, then the reorganization into `advanced/`, `guides/`, `contributing/` was applied without updating this index.

**3. IEC 62304 risk: SOUP.md version drift.** The SOUP table is the compliance artifact — every version is wrong. Recommend adding a CI check that diffs `SOUP.md` versus `vcpkg.json` overrides and fails the build on mismatch.

**4. Language-switcher scaffolding assumes every doc has a .kr twin.** Five `.md` files link to a `*.kr.md` sibling that does not exist (and vice versa on some `.kr` files). Either generate the missing translations or drop the switcher banner.

**5. Three SSOT duplicates.** "Architecture", "Integration Guide", and "Project Structure" each have two files self-declaring authority. Pick one, replace the other with a stub `> See [canonical doc]` redirect, and keep the SSOT banner only on the chosen file.

**6. Relative-path depth errors in `docs/advanced/`.** Multiple files use `../` where `../../` is needed, particularly for links reaching into `examples/` and `include/`. A linter rule for "every `../` path must resolve" would catch these pre-merge.

**7. Orphans worth triaging.** `docs/GETTING_STARTED.md` (a full setup guide from 2026-04-05) and `docs/ECOSYSTEM.md` have no incoming references — either surface them in `docs/README.md` or delete them to prevent confusion with their sibling `docs/guides/GETTING_STARTED.md`.
