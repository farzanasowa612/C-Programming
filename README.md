
# Healthcare Management System in C

A simple console-based healthcare management program written in C. It supports admin, doctor, and patient portals with file-based storage for patient and doctor records.

## Repository Contents

- `C Code HEALWISH.c` - Main C source file implementing the full application logic.
- `index.html` - Unrelated HTML file in the repository.
- `healthcare_data.txt` (generated at runtime) - Stores patient and doctor records.

## Features

- Admin portal with authentication (`admin` / `admin123`)
  - Add new patient records
  - View all patients
  - Search patients by ID
  - Delete patient records
  - Add new doctor profiles
  - View doctor list
- Doctor portal
  - Update patient diagnosis, treatment, and prescription
  - View patient medical history
- Patient portal
  - View patient details by ID

## How to Build

Use a C compiler such as `gcc`:

```bash
gcc "C_Code_HEALWISH.c" -o HEALWISH
```

## How to Run

```bash
./HEALWISH
```

On Windows PowerShell:

```powershell
.\

Capstone Project...
