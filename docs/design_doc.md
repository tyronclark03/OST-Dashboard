# Design Document
**Project:** OST Dashboard  <br>
**Version:** 0.3 <br>
**Last Updated:** 2025-11-09  

---

## 1. Architecture Overview
The application follows a **modular architecture** composed of:

1. **GUI Module** – user interface and event control  
2. **Scanner Module** – discovers and analyzes `.ost` files  
3. **Archiver Module** – compresses, moves, or deletes large OSTs  
4. **Config Module** – loads, validates, and stores configuration settings  
5. **Logger Module** – records all activity for auditing

Each module communicates via controlled interfaces; no module bypasses the logger or directly accesses others’ internals.

---

## 2. Data Flow Summary
1. User initiates *Scan* in GUI.  
2. Scanner returns list of OST metadata.  
3. GUI highlights files exceeding size threshold.  
4. User triggers *Archive* on selected file(s).  
5. Archiver moves file to archive path → verifies checksum → logs result.  
6. Logger writes entry; GUI updates status.

---

## 3. Security Considerations
- All configuration and log files stored in secured directories (`%ProgramData%`).  
- No sensitive user data read or transmitted.  
- Input validation for all paths and parameters.  
- Logs append-only; tamper prevention via file ACLs.

---

## 4. Risks and Mitigations
| Risk | Mitigation |
|------|-------------|
| Locked OST file | Skip with logged warning |
| Disk I/O failure | Transactional copy + rollback |
| Misconfigured paths | Config validation on startup |
| Unauthorized usage | Admin check + signed binary enforcement |

---

## 5. Interfaces Overview
| Interface | Description |
|------------|-------------|
| **Scanner → GUI** | Returns list of OST metadata objects |
| **Archiver → Logger** | Sends status and metrics events |
| **Config → All** | Provides runtime settings to modules |

---

### 🧱 Module State Matrix
| Module | Version | Status | Next Objective |
|---------|----------|---------|----------------|
| Scanner | v0.2 | Functional prototype | Add thresholds + Logger integration |
| Logger | v0.0 | Planned | Define schema and file format |
| Archiver | v0.0 | Planned | Integrate with Logger for compression flow |
| Config | v0.0 | Planned | Introduce dynamic threshold + path inputs |

---

### 6. Scanner Module Prototype (v0.2.0 — Completed)
**Objective:**  
Build a lightweight component to test scanning logic with mock OST data.

**Implemented:**  
- Recursive directory traversal under `/tests/mock_data`  
- Metadata extraction (file name, size, date modified)  
- File size formatting (B, KB, MB, GB)  
- Error handling for inaccessible paths  
- Output to console (text log planned for v0.3)

---

### 7. Scanner Module Enhancement (v0.3.0 — Completed)
**Objective:**  
Introduce file size threshold categorization and refined console display.

**Implemented:**  
- Threshold logic: NORMAL, WARNING (≥10 GB), CRITICAL (>30 GB)
- Category classification via `classifyFileSize()` helper  
- Suppressed NORMAL entries in console output  
- Summary footer with total and flagged file counts  

---

### 8. v0.4.0 — Logging & Runtime Path Resolution

**Goal:**  
Make the scanner usable regardless of where the executable is run from, and capture every scan in a dated log file.

**Implemented:**
- Runtime detection of execution directory (`/bin` vs repo root)
- Construction of real project root at runtime
- Logs now stored in `<project-root>/logs/scan_report_YYYY-MM-DD.txt`
- Logger made compliant with strict C++ build flags (member init order, unused params)
- Scanner still suppresses NORMAL in console, but now logs all files for audit

---

## 9. Future Considerations
- Automated scheduling  
- Centralized network reporting  
- SHA256 validation before archival  
- Role-based access control

