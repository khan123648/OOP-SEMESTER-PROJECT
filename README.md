# 🏥 MediCore — Hospital Management System

> A full-featured Hospital Management System built in **C++ with SFML 3.1.0**, developed as an Object-Oriented Programming semester project at **FAST-NUCES Lahore**.

---

## ✨ Features

### 👤 Three Role System
| Role | Capabilities |
|------|-------------|
| **Admin** | Add/remove doctors & patients, view all records, discharge patients, generate daily reports, view security logs |
| **Doctor** | View today's appointments, mark complete/no-show, write prescriptions, view patient history |
| **Patient** | Book/cancel appointments, view medical records & bills, pay bills, deposit balance |

### 🔐 Security
- Account lockout after **3 failed login attempts** (60-second cooldown) for Doctor & Patient roles
- Full **security event logging** with timestamps
- Admin access is unrestricted (no lockout)

### 💾 Persistent Storage
- All data stored in plain-text CSV files (`doctors.txt`, `patients.txt`, `appointments.txt`, `bills.txt`, `prescriptions.txt`)
- Auto-save on logout and on window close
- Patient discharge archives all records before deletion

### 🎨 SFML GUI
- Dark warm-peach theme (`#0D1112` background, peach `#FFB28C` accents)
- Animated blinking cursor in text boxes
- Toast notifications for success / warning / error
- Scrollable lists with mouse-wheel support
- Role-coloured sidebar with avatar initials

---

## 🗂️ Project Structure

```
MediCore/
├── Admin.h / Admin.cpp
├── Doctor.h / Doctor.cpp
├── Patient.h / Patient.cpp
├── Person.h / Person.cpp          # Base class with lockout logic
├── Appointment.h / Appointment.cpp
├── Bills.h / Bills.cpp
├── Prescription.h / Prescription.cpp
├── Storage.h                       # Generic templated container
├── FileHandler.h / FileHandler.cpp # All file I/O
├── Validation.h / Validation.cpp   # Input validation
├── HospitalException.h             # Base exception
├── FileNotFoundException.h
├── InsufficientFundsException.h
├── InvalidInputException.h
├── SlotUnavailableException.h
├── GUI.h                           # Button, TextBox, Toast, ScrollList
├── AppState.h                      # Screen enum + state struct
├── HospitalApp.h
├── HospitalApp.cpp                 # Constructor + widget init
├── HospitalApp_Draw.cpp            # All rendering
├── HospitalApp_Events.cpp          # Event handling
├── HospitalApp_Logic.cpp           # Business logic (doLogin, doBook...)
├── main.cpp
└── arial.ttf                       # Required font file
```

---

## ⚙️ Requirements

- **C++17** or later
- **SFML 3.1.0** (Graphics + Window + System + Audio)
- **Visual Studio 2022** (x64) — recommended
- `arial.ttf` in the same directory as the `.exe`

---

## 🚀 Build Instructions (Visual Studio)

1. Clone the repository
2. Open the `.sln` file in Visual Studio
3. Set configuration to `Debug` or `Release`, platform `x64`
4. In **Project Properties → C/C++ → Additional Include Directories**, add your SFML `include/` path
5. In **Project Properties → Linker → Additional Library Directories**, add your SFML `lib/` path
6. Under **Linker → Input → Additional Dependencies**, add:
   ```
   sfml-graphics.lib
   sfml-window.lib
   sfml-system.lib
   sfml-audio.lib
   ```
7. Copy all SFML `.dll` files and `arial.ttf` next to the compiled `.exe`
8. Build and run (`F5`)

---

## 🗓️ Default Admin Credentials

On first run, `admin.txt` is auto-created with:
```
ID:       1
Password: admin123
```

---

## 📋 Data File Format

| File | Format |
|------|--------|
| `admin.txt` | `id,name,password` |
| `doctors.txt` | `id,name,specialization,contact,password,fee` |
| `patients.txt` | `id,name,age,gender,contact,password,balance` |
| `appointments.txt` | `id,patientID,doctorID,date,slot,status` |
| `bills.txt` | `billID,patientID,appointmentID,amount,status,date` |
| `prescriptions.txt` | `presID,appointmentID,patientID,doctorID,date,medicines,notes` |

---

## 🧱 OOP Concepts Used

- **Inheritance** — `Admin`, `Doctor`, `Patient` all extend `Person`
- **Polymorphism** — `displayMenu()` and `displayProfile()` are pure virtual in `Person`
- **Templates** — `Storage<T>` generic container used for all entity types
- **Operator Overloading** — `+=`, `-=` on `Patient` balance; `==` on entities; `<<` stream output
- **Exception Handling** — custom exception hierarchy (`HospitalException` base)
- **Encapsulation** — all data private/protected with getters/setters
- **Friend Functions** — `operator<<` for `Doctor`, `Patient`, `Bills`, `Prescription`
- **Static Members** — `MAX_ATTEMPTS`, `LOCKOUT_SECS` in `Person`; `GUIFont::ptr` global font

---

## 👩‍💻 Author

**Aleena Khan**  
Roll No: 25L-2516  
BS Computer Science — Section 2A  
FAST-NUCES Lahore  
OOP Semester Project — Spring 2026

---

## 📄 License

This project is submitted as academic coursework. All rights reserved © 2026 Aleena Khan.
