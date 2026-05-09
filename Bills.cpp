#include <iomanip>
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

Bills::Bills()
{
	AppointmentID = "";
	patientID = "";
	billID = "";
	date = "";
	amount = "";
	status = "unpaid";
}

Bills::Bills(string AppID, string pID, string bID, string dat, string amt, string s)
{
	AppointmentID = AppID;
	patientID = pID;
	billID = bID;
	date = dat;
	amount = amt;
	status = s;
}

void Bills::displayBill() const
{
	cout << "===========================================================" << endl;
	cout << "                           BILL                            " << endl;
	cout << "===========================================================" << endl;
	cout << "Appointment's ID: " << AppointmentID << endl;
	cout << "Patient's ID:     " << patientID << endl;
	cout << "Bill's ID:        " << billID << endl;
	cout << "Date:             " << date << endl;
	cout << "Amount Rs:        " << amount << "/-" << endl;
	cout << "Status:           " << status << endl;
	cout << "-----------------------------------------------------------" << endl;
}

void Bills::overDue()
{
	if (this->status == "unpaid")
	{
		cout << "ALERT!" << endl;
		cout << "Bill Id: " << billID << " is currently over due.";
	}
}

void Bills::markAsPaid()
{
	status = "paid";
	cout << "Bill Id: " << billID << " is now marked as Paid.";
}

void Bills::generateBillSummary()
{
	cout << "Bill Id: " << billID << endl;
	cout << "Patient Id: " << patientID << endl;
	cout << "Amount Rs: " << amount << "/-" << endl;
	cout << "The bill of patient is " << status << endl;
}

ostream& operator<<(ostream& os, const Bills& b)
{
	os << "Bill ID: " << b.billID << endl;
	os << "Patient ID:" << b.patientID << endl;
	os << "Amount Rs:" << b.amount << "/-" << endl;
	os << "Stat:" << b.status << endl;
	return os;
}

string Bills::getBill() const
{
	return billID;
}

string Bills::getID() const
{
	return billID;
}

string Bills::getPatientID() const
{
	return patientID;
}

string Bills::getAppID() const
{
	return AppointmentID;
}

string Bills::getAmount() const
{
	return amount;
}

string Bills::getStatus() const
{
	return status;
}

string Bills::getDate() const
{
	return date;
}
void Bills::setStatus(string s)
{
	status = s;
}