#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include "Storage.h"
class Patient;
class Doctor;
class Admin;
class Appointment;
class Bills;
class Prescription;
using namespace std;

class FileHandler
{
public:
    static void parseCSV(const string& line, string* fields, int maxFields, int& count);
    static void loadingPatients(const string& filename, Storage<Patient>& patients);
    static void loadingDoctors(const string& filename, Storage<Doctor>& docs);
    static void loadingAdmins(const string& filename, Storage<Admin>& adds);
    static void loadingAppointments(const string& filename, Storage<Appointment>& apps);
    static void loadingBills(const string& filename, Storage<Bills>& bills);
    static void loadingPrescriptions(const string& filename, Storage<Prescription>& ps);
    static void savePatients(const string& filename, const Storage<Patient>& patients);
    static void saveDoctors(const string& filename, const Storage<Doctor>& docs);
    static void saveAppointments(const string& filename, const Storage<Appointment>& apps);
    static void saveBills(const string& filename, const Storage<Bills>& bills);
    static void savePrescriptions(const string& filename, const Storage<Prescription>& ps);
    static void appendLine(const string& filename, const string& line);
    static int getMaxAppointmentID(const string& filename);
    static int getMaxPrescriptionID(const string& filename);
    static int getMaxDoctorID(const string& filename);
    static void updateAppointmentStatus(const string& filename, const string& appID, const string& newStatus);
    static void updateBillStatusByAppID(const string& filename, const string& appID, const string& newStatus);
    static void updateBillStatusByBillID(const string& filename, const string& billID, const string& newStatus);
    static void updatePatientBalance(const string& filename, const string& patID, float newBalance);
    static void deleteRecordByID(const string& filename, const string& id);
    static void deleteRecordsByField(const string& filename, int fieldIdx, const string& value);
    static void writeSecurityLog(const string& role, const string& enteredID, const string& result);
    static void archivePatient(const string& patID);
    static void initAdminFile(const string& filename);
};