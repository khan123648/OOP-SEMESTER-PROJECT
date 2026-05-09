#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string> 
#include "Storage.h"
#include "FileHandler.h"
#include "Validation.h"
#include "Admin.h"
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Bills.h"
#include "Prescription.h"
#include "Person.h"

using namespace std;

Appointment::Appointment()
{
	AppointmentID = "";
	patientID = "";
	doctorID = "";
	date = "";
	timeSlot = "";
	status = "";
}

Appointment::Appointment(string id, string pid, string did, string date, string ts, string s)
{
	AppointmentID = id;
	patientID = pid;
	doctorID = did;
	this->date = date;
	timeSlot = ts;
	status = s;
}

bool Appointment:: operator == (const Appointment& other) const 
{
	if (status == "cancelled" || other.status == "cancelled")
	{
		return false;
	}

	if (doctorID == other.doctorID && date == other.date && timeSlot == other.timeSlot)
	{
		return true;
	}

	return false;
}

ostream& operator <<(ostream& os, const Appointment& a)

{
	os << "-------------------------------------------" << endl;
	os << "Appointment's ID: " << a.AppointmentID << endl;
	os << "Patient's ID:     " << a.patientID << endl;
	os << "Doctor's ID:      " << a.doctorID << endl;
	os << "Date:             " << a.date << endl;
	os << "Time Slot:        " << a.timeSlot << endl;
	os << "Status:           " << a.status << endl;
	os << "-------------------------------------------" << endl;
	return os;
}


void Appointment::displayAppointment() const
{
	cout << "===========================================================" << endl;
	cout << "                        APPOINTMENT                        " << endl;
	cout << "===========================================================" << endl;
	cout << "Appointment ID: " << AppointmentID << endl;
	cout << "Date (DD-MM-YYYY): " << date << endl;
	cout << "Slot: " << timeSlot << endl;
	cout << "Status: " << status << endl;
	cout << "Doctor: " << doctorID << endl;
	cout << "-----------------------------------------------------------" << endl;
}

string Appointment::getAppID() const
{
	return AppointmentID;
}

string Appointment::getID() const
{
	return AppointmentID;
}

string Appointment::getPatientID() const
{
	return patientID;
}

string Appointment::getDocID() const
{
	return doctorID;
}

string Appointment::getStatus() const
{
	return status;
}

string Appointment::getDate() const
{
	return date;
}

string Appointment::getSlot() const
{
	return timeSlot;
}

void Appointment::setStatus(string s)
{
	status = s;
}