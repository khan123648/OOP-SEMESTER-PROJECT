#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string> 
using namespace std;
class Bills
{
protected:
	string AppointmentID;
	string patientID;
	string billID;
	string date;
	string amount;
	string status;

public:

	Bills();
	Bills(string AppID, string pID, string bID, string date, string amt, string s);
	void displayBill() const;
	void overDue();
	void markAsPaid();
	void generateBillSummary();
	friend ostream& operator<<(ostream& os, const Bills& b);
	string getBill() const;
	string getID() const;
	string getPatientID() const;
	string getAppID() const;
	string getAmount() const;
	string getDate() const;
	string getStatus() const;
	void setStatus(string s);
};