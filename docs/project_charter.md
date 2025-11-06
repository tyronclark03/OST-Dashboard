# Project Charter – OST Dashboard

**Project:** OST Dashboard <br>
**Version:** 0.2 <br>
**Last Updated:** 2025-11-05

---

## 1. Purpose
The **OST Dashboard** project provides a secure desktop utility for enterprise IT departments to discover, analyze, and manage Outlook OST files.  
It targets performance, storage, and compliance challenges caused by large or orphaned OSTs on corporate machines.

---

## 2. Objectives
- Identify and flag large OST files that exceed defined thresholds.  
- Enable safe archiving or movement of inactive OSTs.  
- Maintain full auditability through centralized logging.  
- Operate entirely offline with minimal privileges.

---

## 3. Scope

**In-Scope**
- File-level scanning of OST directories.
- Logging and audit reporting.
- Secure archival workflows.
- GUI for IT operators.
- Configurable thresholds and paths.

**Out-of-Scope**
- Parsing OST contents or emails.
- Direct integration with Exchange or Outlook.
- Cloud sync or network-distributed functionality.

---

## 5. Constraints
- Windows-only deployment (10/11, x64).  
- Must run without internet connectivity.

---

## 6. Risks and Mitigations
| Risk | Mitigation |
|------|-------------|
| User deletes live OST by mistake | Require confirmation prompts and logging |
| Access denied on protected directories | Use admin elevation or skip with warning |
| Large file operations stall | Background threading and progress updates |

---

## 7. Version Control
This project uses **Semantic Versioning 2.0.0** (MAJOR.MINOR.PATCH).
