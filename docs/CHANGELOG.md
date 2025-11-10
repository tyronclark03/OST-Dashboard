# OST Dashboard – Change Log

All notable changes to this project will be documented in this file.

---

## [Unreleased]
### Planned
- Add config-driven thresholds
- Add log retention (auto-clean old logs)
- Add CSV/JSON export

---

## [v0.5.0] – 2025-11-10
### Added
- Introduced `Config` module for dynamic runtime configuration
- Added `scanner.conf` for adjustable thresholds, directories, and flags
- Integrated `Config` with `Scanner` to replace hardcoded thresholds
- Logger now accepts configurable directory path via `logs_dir` entry
- Updated CMake build to include `config.cpp` for modular builds

---

## [v0.4.0] – 2025-11-09
### Added
- Dynamic project-root resolution so scanner works from `/bin` or repo root
- Per-run timestamped logging to `logs/scan_report_YYYY-MM-DD.txt`
- Inline comments across scanner/logger sources
- Updated README with build/run instructions

---

## [v0.3.0] – 2025-11-09
### Added
- File size threshold classification (NORMAL/WARNING/CRITICAL)
- Display policy: hide NORMAL, summarize flagged files
- Internal data model retains full metadata for logging

---

## [v0.2.0] – 2025-11-05
### Added
- Scanner module prototype (`scanner.cpp`)
- Recursive directory traversal and file size metrics
- Documentation updates to project charter, requirements, and design doc
- Version control and branching model established

---

## [v0.1.0] – 2025-11-02
### Added
- Project charter, requirements, and high-level design documentation.
- Repository and directory structure initialized.
- Semantic Versioning 2.0.0 policy adopted.




