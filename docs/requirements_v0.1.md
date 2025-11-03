# Software Requirements Specification
**Project:** OST Dashboard  
**Version:** 0.1  
**Date:** 2025-11-02  

---

## 1. Functional Requirements

| ID | Requirement | Priority |
|----|--------------|----------|
| FR-1 | Scan directories for `.ost` files and collect metadata (path, size, modified date). | High |
| FR-2 | Flag files exceeding a configurable size threshold. | High |
| FR-3 | Archive or move selected OSTs to a secure path. | High |
| FR-4 | Log every action with timestamp, user, and outcome. | High |
| FR-5 | Provide a GUI for viewing, filtering, and managing results. | Medium |
| FR-6 | Export results to CSV or JSON. | Medium |

---

## 2. Non-Functional Requirements
- **Security:** Runs with least privilege; uses signed binaries; all actions auditable.  
- **Performance:** Scans 1000 files ≤ 120 seconds on SSD hardware.  
- **Reliability:** Archive and delete operations are atomic and recoverable.  
- **Usability:** GUI comprehensible to IT technicians with minimal training.  
- **Compliance:** Produces audit-ready logs and immutable records.  

---

## 3. System Assumptions
- Outlook OSTs reside under user `%AppData%\Local\Microsoft\Outlook`.  
- Administrator permissions available when needed.  
- No internet access required.

---

## 4. Dependencies
- Windows 10/11 OS  
- NTFS file system  
- C++ 17+ runtime environment  

---

## 5. Traceability Matrix

| Requirement | Module | Verification |
|--------------|---------|--------------|
| FR-1 | Scanner | Unit + Integration Test |
| FR-3 | Archiver | Functional Test + Log Review |
| FR-4 | Logger | Audit Log Inspection |
| FR-5 | GUI | UAT Observation |

---

## 6. References
Linked documents:  
- [Design Document](./design_doc.md)  
- [Project Charter](./project_charter.md)  
- [CHANGELOG](./CHANGELOG.md)
