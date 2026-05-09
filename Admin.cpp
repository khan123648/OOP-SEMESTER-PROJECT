#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
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

Admin::Admin() : Person() {}


Admin::Admin(string i, string n, string p, string c) : Person(i, n, p, c) {}

void Admin::displayMenu()
{
	cout << "==============================================" << endl;
	cout << "              ADMIN'S MAIN MENU               " << endl;
	cout << "==============================================" << endl;
	cout << "1.  View My Profile" << endl;
	cout << "2.  Add New Doctor" << endl;
	cout << "3.  Remove Any Doctor" << endl;
	cout << "4.  View All Doctors (those who are REGISTERED )" << endl;
	cout << "5.  View All Patients (those who are REGISTERED)" << endl;
	cout << "6.  View All Appointments (those which are BOOKED)" << endl;
	cout << "7.  View All Unpaid Bills (those which are RECORDED)" << endl;
	cout << "8.  Discharge Patient (RECOVERED)" << endl;
	cout << "9.  Generate Daily Reports (From MENTIONED or COLLECTED data)" << endl;
	cout << "10. View Security Logs (Only those ALLOWED)" << endl;
	cout << "0.  Log Out" << endl;
	cout << "----------------------------------------------" << endl;
	cout << " WHAT DO YOU WANT DO!" << endl;
}

void Admin::displayProfile()
{
	cout << "===========================================" << endl;
	cout << "              ADMIN'S PROFILE              " << endl;
	cout << "===========================================" << endl;
	cout << "Admin's ID: " << ID << endl;
	cout << "Admin's Name:" << name << endl;
	cout << "Admin's Contact: " << contact << endl;
}

void Admin::addDoctor(Storage<Doctor>& docs)
{
	string nam;
	string pass;
	string cont;
	string spec;
	float fee;

	cout << "=============================================" << endl;
	cout << "             REGISTER NEW DOCTOR             " << endl;
	cout << "=============================================" << endl;

	int newE = FileHandler::getMaxDoctorID("doctors.txt") + 1;
	string id = to_string(newE);

	cout << "The system has generated the DOCTOR's id: " << id << endl;
	cin.ignore(1000, '\n');

	cout << "Name of Doctor:(max 50 credentials) ";
	getline(cin, nam);

	cout << "Password (minimum 6): ";
	getline(cin, pass);

	cout << "Enter contact number (0000-0000000): ";
	cin >> cont;
	cin.ignore(1000, '\n');

	cout << "Enter Specialization: ";
	getline(cin, spec);

	cout << "Enter Consultation Fee: ";
	cin >> fee;
	cin.ignore(1000, '\n');

	if (!Validation::validatePassword(pass))
	{
		cout << "Errror! Password too short." << endl;
		return;
	}

	if (!Validation::validateContact(cont))
	{
		cout << "Errror! invalid number." << endl;
		return;
	}

	if (!Validation::validateFee(fee))
	{
		cout << "Errror! out of bound." << endl;
		return;
	}

	Doctor newDoc(id, nam, pass, cont, spec, fee);
	docs.add(newDoc);

	string line = id + "," + nam + "," + spec + "," + cont + "," + pass + "," + to_string(fee);
	FileHandler::appendLine("doctors.txt", line);

	cout << "A doctor is added successfully!" << endl;
}

void Admin::removeDoctor(Storage<Doctor>& docs, Storage<Appointment>& apps)
{
	viewAllDoctors(docs);
	string id;
	cout << "Enter Doctor's ID who is to be removed: ";
	cin >> id;

	Doctor* docToRemove = docs.findById(id);
	if (docToRemove == nullptr)
	{
		cout << "Error: Doctor with ID " << id << " not found!" << endl;
		return;
	}

	for (int i = 0; i < apps.getCount(); i++)
	{
		if (apps.getAT(i)->getDocID() == id && apps.getAT(i)->getStatus() == "pending")
		{
			cout << " Cannot remove an On-Duty Doctor (Doctor with pending Appointments)" << endl;
			return;
		}
	}

	docs.removeById(id);
	FileHandler::deleteRecordByID("doctors.txt", id);

	cout << "The Doctor with ID: " << id << " has been successfully removed." << endl;
}

void Admin::viewAllDoctors(Storage<Doctor>& docs)
{
	cout << "==========================================" << endl;
	cout << "             VIEW ALL DOCTORS             " << endl;
	cout << "==========================================" << endl;
	if (docs.getCount() == 0)
	{
		cout << "No Doctors Registered!" << endl;
		return;
	}

	for (int i = 0; i < docs.getCount(); i++)
	{
		cout << *docs.getAT(i) << endl;
	}
}

void Admin::viewAllPatients(Storage<Patient>& pat, Storage<Bills>& bills)
{
	cout << "==========================================" << endl;
	cout << "             VIEW ALL PATIENT             " << endl;
	cout << "==========================================" << endl;
	if (pat.getCount() == 0)
	{
		cout << "No Patients Registered!" << endl;
		return;
	}

	for (int i = 0; i < pat.getCount(); i++)
	{
		Patient* p = pat.getAT(i);
		int unpaid_count = 0;

		for (int j = 0; j < bills.getCount(); j++)
		{
			if (bills.getAT(j)->getPatientID() == p->getID() && bills.getAT(j)->getStatus() == "unpaid")
			{
				unpaid_count++;
			}
		}

		cout << *p << endl;
		cout << "Unpaid Bills count is: " << unpaid_count << endl;
	}
}

void Admin::viewAllAppointments(Storage<Appointment>& apps)
{
	cout << "==============================================" << endl;
	cout << "             VIEW ALL Appointments            " << endl;
	cout << "==============================================" << endl;
	if (apps.getCount() == 0)
	{
		cout << "No Appointments Traced!" << endl;
		return;
	}

	int indices[500];
	int cnt = apps.getCount();

	for (int i = 0; i < cnt; i++)
	{
		indices[i] = i;
	}

	for (int i = 0; i < cnt - 1; i++)
	{
		for (int j = 0; j < cnt - i - 1; j++)
		{
			string d1 = apps.getAT(indices[j])->getDate();
			string d2 = apps.getAT(indices[j + 1])->getDate();

			if (d1.length() < 10)
			{
				continue;
			}

			if (d2.length() < 10)
			{
				continue;
			}

			string k1 = d1.substr(6, 4) + d1.substr(3, 2) + d1.substr(0, 2);
			string k2 = d2.substr(6, 4) + d2.substr(3, 2) + d2.substr(0, 2);

			if (k1 < k2)
			{
				int tmp = indices[j];

				indices[j] = indices[j + 1];
				indices[j + 1] = tmp;
			}
		}
	}

	for (int i = 0; i < cnt; i++)
	{
		cout << *apps.getAT(indices[i]);
	}
}

void Admin::viewUnpaidBills(Storage<Bills>& bills)
{
	cout << "============================================" << endl;
	cout << "             VIEW PENDING BILLS             " << endl;
	cout << "============================================" << endl;

	bool found = false;
	time_t now = time(0);

	for (int i = 0; i < bills.getCount(); i++)
	{
		if (bills.getAT(i)->getStatus() != "unpaid")
		{
			continue;
		}

		bills.getAT(i)->displayBill();

		string d = bills.getAT(i)->getDate();

		if (d.length() == 10)
		{
			tm billTm = {};

			billTm.tm_mday = stoi(d.substr(0, 2));
			billTm.tm_mon = stoi(d.substr(3, 2)) - 1;
			billTm.tm_year = stoi(d.substr(6, 4)) - 1900;

			time_t billTime = mktime(&billTm);

			if (difftime(now, billTime) > 7.0 * 24 * 3600)
			{
				cout << "[OVERDUE]" << endl;
			}
		}
		found = true;
	}
	if (!found) cout << "No unpaid bills." << endl;
}

void Admin::dischargePatient(Storage<Patient>& patients, Storage<Bills>& bills, Storage<Appointment>& apps)
{
	string id;
	cout << "Enter Patient's ID to be discharged!" << endl;
	cin >> id;

	for (int i = 0; i < bills.getCount(); i++)
	{
		if (bills.getAT(i)->getPatientID() == id && bills.getAT(i)->getStatus() == "unpaid")
		{
			cout << "Cannot discharge patient with unpaid bill" << endl;
			return;
		}
	}

	for (int i = 0; i < apps.getCount(); i++)
	{
		if (apps.getAT(i)->getPatientID() == id && apps.getAT(i)->getStatus() == "pending")
		{
			cout << "Cannot discharge patient with pending appointments" << endl;
			return;
		}
	}

	FileHandler::archivePatient(id);
	FileHandler::deleteRecordByID("patients.txt", id);
	FileHandler::deleteRecordsByField("appointments.txt", 1, id);
	FileHandler::deleteRecordsByField("bills.txt", 1, id);
	FileHandler::deleteRecordsByField("prescriptions.txt", 2, id);

	patients.removeById(id);
	cout << "Patient discharged and archived successfully." << endl;
}

void Admin::viewSecurityLog()
{
	time_t timeNow = time(0);
	string dateAndTime = ctime(&timeNow);

	cout << "=============================================" << endl;
	cout << "            SHOWING SECURITY LOG             " << endl;
	cout << "=============================================" << endl;
	cout << "Admin access granted at: " << dateAndTime << endl;

	ifstream file("security_log.txt");
	if (!file.is_open())
	{
		cout << "No security events logged." << endl;
		return;
	}

	string line;
	bool any = false;

	while (getline(file, line))
	{
		if (!line.empty())
		{
			cout << line << endl;
			any = true;
		}
	}

	if (!any) cout << "No security events logged." << endl;
}

void Admin::generateDailyReport(Storage<Appointment>& apps, Storage<Bills>& bills, Storage<Patient>& patients, Storage<Doctor>& docs)
{
	time_t timeNow = time(0);
	tm* ltm = localtime(&timeNow);
	char dateBuf[20];
	char timeBuf[20];
	strftime(dateBuf, sizeof(dateBuf), "%d-%m-%Y", ltm);
	strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", ltm);
	string today(dateBuf);

	cout << "===============================================" << endl;
	cout << "           HOSPITAL DAILY REPORT               " << endl;
	cout << "===============================================" << endl;

	cout << "Date: " << dateBuf << "  Time: " << timeBuf << endl;

	int total = 0;
	int pending = 0;
	int completed = 0;
	int noshow = 0;
	int cancelled = 0;

	for (int i = 0; i < apps.getCount(); i++)
	{
		if (apps.getAT(i)->getDate() == today)
		{
			total++;
			string s = apps.getAT(i)->getStatus();
			if (s == "pending")   pending++;
			else if (s == "completed") completed++;
			else if (s == "noshow")    noshow++;
			else if (s == "cancelled") cancelled++;
		}
	}

	cout << "Total appointments today: " << total << endl;
	cout << " Pending: " << pending << endl;
	cout << " Completed: " << completed << endl;
	cout << " No-show: " << noshow << endl;
	cout << " Cancelled: " << cancelled << endl;

	float revenue = 0.0f;

	for (int i = 0; i < bills.getCount(); i++)
	{

		if (bills.getAT(i)->getStatus() == "paid" && bills.getAT(i)->getDate() == today)
		{
			revenue += stof(bills.getAT(i)->getAmount());
		}
	}

	cout << "Revenue collected today (paid bills): PKR " << fixed << setprecision(2) << revenue << endl;

	cout << "--- Patients with outstanding unpaid bills ---" << endl;

	for (int i = 0; i < patients.getCount(); i++)
	{
		float owed = 0.0f;
		for (int j = 0; j < bills.getCount(); j++)
		{
			if (bills.getAT(j)->getPatientID() == patients.getAT(i)->getID() && bills.getAT(j)->getStatus() == "unpaid")
			{
				owed += stof(bills.getAT(j)->getAmount());
			}
		}

		if (owed > 0)
		{
			cout << patients.getAT(i)->getName() << "  Total Owed: Rs: " << owed << endl;
		}
	}

	cout << "--- Doctor-wise Summary for Today ---" << endl;

	for (int i = 0; i < docs.getCount(); i++)
	{
		string did = docs.getAT(i)->getID();
		int dComp = 0, dPend = 0, dNoshow = 0;
		for (int j = 0; j < apps.getCount(); j++)
		{
			if (apps.getAT(j)->getDocID() == did && apps.getAT(j)->getDate() == today)
			{
				string s = apps.getAT(j)->getStatus();
				if (s == "completed")
				{
					dComp++;
				}

				else if (s == "pending")
				{
					dPend++;
				}

				else if (s == "noshow")
				{
					dNoshow++;
				}
			}
		}

		if (dComp + dPend + dNoshow > 0)
		{

			cout << docs.getAT(i)->getName() << endl;
			cout << " Completed: " << dComp << endl;
			cout << " Pending: " << dPend << endl;
			cout << " No-show: " << dNoshow << endl;

		}
	}
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

}
