# 🖥️ OST Dashboard  
**Version:** v0.1.0  
**Author:** Tyron Clark  
**Status:** In Development – Proof of Concept  

---

## 📖 Overview

**OST Dashboard** is a secure desktop utility built in C++ for **banking IT departments** to discover, analyze, and manage **Microsoft Outlook OST files** across local or networked systems.  
It addresses one of the most common IT pain points in enterprise environments — oversized, orphaned, or outdated OST files consuming critical storage and degrading workstation performance.

This project emphasizes **security, auditability, and offline operation**, simulating real-world engineering practices in regulated industries.

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

OST-Dashboard/
├── README.md
├── VERSION
├── config
├── docs
├── include
├── resources
├── src
└── tests

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
| **0.1.0** | 2025-11-02 | Initial project structure, documentation, and baseline setup. |

---

## 📫 Contact / Maintainer

**Tyron Clark**  
Developer & Author 
GitHub: [@tyronclark03](https://github.com/tyronclark03)

> *This repository is for internal learning and simulation purposes only.  
> No proprietary or client data is processed, stored, or transmitted.*
