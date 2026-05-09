#pragma once
#include "Storage.h"
#include "Person.h"

using namespace std;
class Prescription
{
private:
	string AppointmentID;
	string PrescriptionID;
	string patientID;
	string doctorID;
	string date;
	string medicines;
	string notes;
public:
	Prescription();
	Prescription(string aid, string pid, string ptid, string did, string date, string m, string n );
	void displayPrescription() const;
	void updateMedicine(string newmeds);
	bool ifForPatient(string pid) const;
	friend ostream& operator<<(ostream& os, const Prescription& p);
	string getAppointmentID() const;
	string getPrescriptionID() const;
	string getPatientID() const;
	string getDoctorID() const;
	string getDate() const;
	string getMedicines() const;
	string getNotes() const;
};