#pragma once
#include "Person.h"
#include "Storage.h"

// ── Forward declarations (avoid circular includes) ────────────────────────
class Appointment;
class Bills;
class Patient;
class Prescription;   // <-- YEH MISSING THA — C2065 / C2923 ka root cause

using namespace std;

class Doctor : public Person
{
    float  consultationFee;
    string specialization;

public:
    Doctor();
    Doctor(string i, string n, string p, string c, string spec, float cf);

    void displayMenu()    override;
    void displayProfile() override;

    void viewTodaysAppointment(Storage<Appointment>& apps) const;
    void markAppointment(Storage<Appointment>& apps, Storage<Bills>& bills);
    void markNoShow(Storage<Appointment>& apps, Storage<Bills>& bills);
    void writePrescription(Storage<Prescription>& ps, Storage<Appointment>& apps);
    void viewPatientHistory(Storage<Prescription>& ps,
        Storage<Appointment>& apps,
        Storage<Patient>* patients) const;

    bool operator==(const string& id) const;
    friend ostream& operator<<(ostream& os, const Doctor& d);

    string getSpec()    const;
    string getContact() const;   // HospitalApp uses this — add if missing in .cpp
    float  getfee()     const;
};