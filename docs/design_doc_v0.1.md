# Design Document
**Project:** OST Dashboard  
**Version:** 0.1  
**Date:** 2025-11-02  

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

## 6. Future Enhancements
- Centralized reporting dashboard for multiple PCs  
- Scheduling engine for automated scans  
- Integration with Exchange Online Archive status
