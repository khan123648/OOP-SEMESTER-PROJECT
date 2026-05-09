#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

class Appointment
{
	string AppointmentID;
	string patientID;
	string doctorID;
	string date;
	string timeSlot;
	string status;
public:

	Appointment();
	Appointment(string id, string pid, string did, string date, string ts, string s);
	bool operator == (const Appointment& other) const;
	friend ostream& operator <<(ostream& os, const Appointment& a);
	void displayAppointment() const;
	string getID() const;
	string getAppID() const; 
	string getPatientID() const;
	string getDocID() const;
	string getStatus() const; 
	void setStatus( string s); 
	string getDate() const; 
	string getSlot() const;
};