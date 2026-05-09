#include "Prescription.h"
#include <iostream>

Prescription::Prescription()
{
	AppointmentID = "";
	PrescriptionID = "";
	patientID = "";
	doctorID = "";
	date = "";
	medicines = "";
	notes = "";
}

Prescription::Prescription(string aid, string pid, string ptid, string did, string date, string m, string n)
{
	AppointmentID = aid;
	PrescriptionID = pid;
	patientID = ptid;
	doctorID = did;
	this->date = date;
	medicines = m;
	notes = n;
}

void Prescription::displayPrescription() const
{
	cout << "===========================================================" << endl;
	cout << "                        PRESCRIPTION                       " << endl;
	cout << "===========================================================" << endl;
	cout << "Appointment ID:       " << AppointmentID << endl;
	cout << "Prescription ID:      " << PrescriptionID << endl;
	cout << "Patient's ID:         " << patientID << endl;
	cout << "Doctor's ID:          " << doctorID << endl;
	cout << "Date:                 " << date << endl;
	cout << "Medicines :           " << medicines << endl;
	cout << "Important Advices:    " << notes << endl;
	cout << "-----------------------------------------------------------" << endl;
}

void Prescription::updateMedicine(string newmeds)
{
	medicines = newmeds;
	cout << "Medicines are updated according to need!" << endl;
}

bool Prescription::ifForPatient(string pid) const
{
	if (this->patientID == pid)
	{
		return true;
	}

	return false;
}

ostream& operator<<(ostream& os, const Prescription& p)
{
	os << "-------------------------------------------------" << endl;
	os << "Patient's ID:         " << p.patientID << endl;
	os << "Doctor's ID:          " << p.doctorID << endl;
	os << "Date:                 " << p.date << endl;
	os << "-------------------------------------------------" << endl;
	return os;
}

string Prescription::getAppointmentID() const
{
	return AppointmentID;
}

string Prescription::getPrescriptionID() const
{
	return PrescriptionID;
}

string Prescription::getPatientID() const
{
	return patientID;
}

string Prescription::getDoctorID() const
{
	return doctorID;
}

string Prescription::getDate() const
{
	return date;
}

string Prescription::getMedicines() const
{
	return medicines;
}

string Prescription::getNotes() const
{
	return notes;
}