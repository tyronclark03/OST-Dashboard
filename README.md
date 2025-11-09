# 🖥️ OST Dashboard  
**Version:** v0.3.0  
**Author:** Tyron Clark  
**Status:** In Development – Scanner Prototype

---

## 📖 Overview

**OST Dashboard** is a secure desktop utility built in C++ for **enterprise IT departments** to discover, analyze, and manage **Microsoft Outlook OST files** across local or networked systems.  
It addresses one of the most common IT pain points in enterprise environments — oversized, orphaned, or outdated OST files consuming critical storage and degrading workstation performance.

This project emphasizes **security, auditability, and offline operation**, simulating real-world engineering practices in regulated industries.

---

## 🧭 Current Focus (v0.3.0)
The latest version introduces **threshold-based categorization** of OST files and refined output
display for IT operators. The scanner now classifies files as:
- CRITICAL > 30 GB
- WARNING ≥ 10 GB
- NORMAL < 10 GB (not shown in console)

Next release (v0.4) will focus on logging and configuration integration.

---

## 🧩 Core Objectives

- Detect and inventory OST files on local drives or mapped directories.  
- Identify large or outdated OSTs based on configurable thresholds.  
- Provide options to archive, move, or flag files securely.  
- Maintain a full audit trail through immutable logs.  
- Operate under least-privilege principles with no internet dependency.

---

## 🏗️ Architecture Summary

The OST Dashboard follows a **modular architecture** for maintainability and testing:

| Module | Description |
|--------|--------------|
| **GUI** | Desktop interface for IT personnel to view results, configure settings, and execute actions. |
| **Scanner** | Locates `.ost` files and collects metadata (path, size, modified date). |
| **Archiver** | Handles file compression, movement, and deletion while ensuring integrity checks. |
| **Logger** | Records all events and actions with timestamps for compliance. |
| **Config** | Loads and validates configuration data (thresholds, directories, policies). |

All modules communicate via defined interfaces and are designed for **audit-ready isolation** and **modular testing**.

---

## 🗂️ Directory Structure

```
OST-Dashboard/
├─ docs/          # Requirements, design, and changelog documentation
├─ src/           # Source code modules (Scanner, Archiver, Logger, GUI, Config)
├─ include/       # Public header files
├─ config/        # Default configuration templates
├─ resources/     # Icons, GUI assets, localization files
└─ tests/         # Unit and integration test files
```

---

## ⚙️ Development Standards

- **Language:** C++17+  
- **Build System:** CMake  
- **Versioning:** [Semantic Versioning 2.0.0](https://semver.org/)  
- **Branching Model:**  
  - `main` → stable, tagged releases  
  - `develop` → active development branch  
  - `feature/*` → module-specific or prototype branches  
  - `hotfix/*` → critical fixes for production releases  

---

## 🧱 Compliance and Security Notes

- All operations are **local and offline**.  
- Logs are append-only and timestamped.  
- No personal email content is accessed or parsed.  
- Configuration and log directories use restricted NTFS permissions.  

---

## 📜 Version History

| Version | Date | Description |
|----------|------|-------------|
| **0.3.0** | 2025-11-09 | Add size threshold classification helper for the scanner module. |
| **0.2.0** | 2025-11-05 | Implemented Scanner module prototype (`src/scanner/scanner.cpp`); recursive traversal, file size formatting, and documentation updates. |
| **0.1.0** | 2025-11-02 | Initial project structure, documentation, and baseline setup. |

---

## 📫 Contact / Maintainer

**Tyron Clark**  
Developer & Author 
GitHub: [@tyronclark03](https://github.com/tyronclark03)

> *This repository is for internal learning and simulation purposes only.  
> No proprietary or client data is processed, stored, or transmitted.*
