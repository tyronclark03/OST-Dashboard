# 🧩 Mock OST Dataset – Scanner Module Testing

**Project:** OST Dashboard  
**Author:** Tyron Clark  
**Last Updated:** 2025-11-03  
**Purpose:**  
This mock dataset simulates a realistic collection of Outlook `.ost` files used for testing the Scanner module in a secure, offline environment.  
All files are synthetic (generated via `truncate`) and contain **no real email data**.

---

## 📁 Directory Structure

```
tests/
└── mock_data
    ├── dummy_folder
    │   └── empty.ost
    ├── readme.md
    ├── user01_mailbox.ost
    ├── user02_mailbox.ost
    ├── user03_mailbox.ost
    ├── user04_mailbox.ost
    ├── user05_archive.ost
    ├── user06_archive.ost
    └── user07_legacy.ost
```

---

## 🧱 File Descriptions

| File Name | Simulated Size | Category | Purpose |
|------------|----------------|-----------|----------|
| `user01_mailbox.ost` | 5 MB | Normal | Baseline test (small OST) |
| `user02_mailbox.ost` | 18 MB | Normal | Medium OST within safe range |
| `user03_mailbox.ost` | 28 GB | Normal | Large but below threshold |
| `user04_mailbox.ost` | 31 GB | Warning | Slightly exceeds 30 GB threshold |
| `user05_archive.ost` | 33 GB | Warning | Over warning limit (flag expected) |
| `user06_archive.ost` | 36 GB | Critical | Exceeds critical 35 GB limit |
| `user07_legacy.ost` | 42 GB | Critical | Large legacy OST – should trigger alert |
| `temp_notes.txt` | 1 KB | Non-OST | Used to validate file-type filtering |
| `dummy_folder/empty.ost` | 0 B | Edge Case | Test handling of empty OSTs |
| `readme.md` | – | Documentation | Describes mock dataset purpose |

---

## ⚙️ Generation Method (WSL/Linux)
All `.ost` files were generated using **sparse allocation** to simulate real sizes without consuming actual disk space.

```bash
truncate -s 5M  user01_mailbox.ost
truncate -s 18M user02_mailbox.ost
truncate -s 28G user03_mailbox.ost
truncate -s 31G user04_mailbox.ost
truncate -s 33G user05_archive.ost
truncate -s 36G user06_archive.ost
truncate -s 42G user07_legacy.ost
truncate -s 0   dummy_folder/empty.ost
```

---

### 📘 Revision Tracking
See the central [Revision Log](../../docs/REVISION_LOG.md) for change history related to this dataset.

