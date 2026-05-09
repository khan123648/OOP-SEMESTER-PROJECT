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
#include "Person.h"
#include "Prescription.h"

using namespace std;

Doctor::Doctor() :Person()
{
	consultationFee = 0.0;
	specialization = "";
}

Doctor::Doctor(string i, string n, string p, string c, string spec, float cf) : Person(i, n, p, c)
{
	consultationFee = cf;
	specialization = spec;
}
void Doctor::displayMenu()
{

	cout << "============================================================" << endl;
	cout << "                     DOCTOR'S MAIN MENU                     " << endl;
	cout << "============================================================" << endl;
	cout << "1.  View My Profile" << endl;
	cout << "2.  View Today's Appointment (those which are BOOKED)" << endl;
	cout << "3.  Mark Appointment Complete" << endl;
	cout << "4.  Mark Appointment No-Show" << endl;
	cout << "5.  Write Prescription" << endl;
	cout << "6.  View Patient's History (if EXISTS)" << endl;
	cout << "0.  LOG OUT" << endl << endl;
	cout << "------------------------------------------------------------" << endl;
	cout << " WHAT DO YOU WANT DO!" << endl;
	cout << "Choice: ";
}

void Doctor::displayProfile()
{
	cout << "==============================================" << endl;
	cout << "               DOCTOR'S PROFILE              " << endl;
	cout << "==============================================" << endl;
	cout << "ID:                    " << ID << endl;
	cout << "Name:                  " << name << endl;
	cout << "Specialization:        " << specialization << endl;
	cout << "Contact:               " << contact << endl;
	cout << "Consultation Fee Rs:   " << consultationFee << "/-" << endl;
}

void Doctor::viewTodaysAppointment(Storage<Appointment>& apps) const
{
	cout << "=================================================" << endl;
	cout << "               TODAY'S APPOINTMENTS              " << endl;
	cout << "=================================================" << endl;

	time_t now = time(0);
	tm* ltm = localtime(&now);
	char dateBuf[20];
	strftime(dateBuf, sizeof(dateBuf), "%d-%m-%Y", ltm);
	string today(dateBuf);

	int indices[200]; int cnt = 0;
	for (int i = 0; i < apps.getCount(); i++)
	{
		if (apps.getAT(i)->getDocID() == ID && apps.getAT(i)->getDate() == today)
		{
			indices[cnt++] = i;
		}
	}
	if (cnt == 0)
	{
		cout << "No appointments scheduled for today." << endl;
		return;
	}

	for (int i = 0; i < cnt - 1; i++)
	{
		for (int j = 0; j < cnt - i - 1; j++)
		{
			if (apps.getAT(indices[j])->getSlot() > apps.getAT(indices[j + 1])->getSlot())
			{
				int tmp = indices[j]; indices[j] = indices[j + 1]; indices[j + 1] = tmp;
			}
		}
	}


	for (int i = 0; i < cnt; i++)
	{
		apps.getAT(indices[i])->displayAppointment();
	}
}

void Doctor::markAppointment(Storage<Appointment>& apps, Storage<Bills>& bills)
{
	string appid;
	cout << "==========================================================" << endl;
	cout << "                 MARK APPOINTMENT AS Done                 " << endl;
	cout << "==========================================================" << endl;

	time_t now = time(0);
	tm* ltm = localtime(&now);
	char dateBuf[20];
	strftime(dateBuf, sizeof(dateBuf), "%d-%m-%Y", ltm);
	string today(dateBuf);

	cout << "Enter Appointment ID: "; cin >> appid;

	Appointment* appoint = apps.findById(appid);

	if (!appoint || appoint->getDocID() != ID)
	{
		cout << "Appointment not found or unauthorized." << endl; return;
	}

	if (appoint->getStatus() != "pending")
	{
		cout << "Appointment is not pending." << endl; return;
	}

	if (appoint->getDate() != today)
	{
		cout << "Appointment is not scheduled for today." << endl; return;
	}

	appoint->setStatus("completed");

	FileHandler::updateAppointmentStatus("appointments.txt", appid, "completed");

	for (int i = 0; i < bills.getCount(); i++)
	{
		if (bills.getAT(i)->getAppID() == appid)
		{
			bills.getAT(i)->setStatus("paid");
			FileHandler::updateBillStatusByAppID("bills.txt", appid, "paid");
			break;
		}
	}
	cout << "Appointment marked as completed." << endl;
}

void Doctor::markNoShow(Storage<Appointment>& apps, Storage<Bills>& bills)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);
	char dateBuf[20];
	strftime(dateBuf, sizeof(dateBuf), "%d-%m-%Y", ltm);
	string today(dateBuf);

	string appid; cout << "Enter Appointment ID: "; cin >> appid;

	Appointment* appoint = apps.findById(appid);

	if (!appoint || appoint->getDocID() != ID)
	{
		cout << "Appointment not found or unauthorized." << endl; return;
	}

	if (appoint->getStatus() != "pending")
	{
		cout << "Appointment is not pending." << endl; return;
	}

	if (appoint->getDate() != today)
	{
		cout << "Appointment is not scheduled for today." << endl; return;
	}

	appoint->setStatus("noshow");

	FileHandler::updateAppointmentStatus("appointments.txt", appid, "noshow");

	for (int i = 0; i < bills.getCount(); i++)
	{
		if (bills.getAT(i)->getAppID() == appid)
		{
			bills.getAT(i)->setStatus("cancelled");
			FileHandler::updateBillStatusByAppID("bills.txt", appid, "cancelled");
			break;
		}
	}
	cout << "Appointment marked as no-show." << endl;
}

void Doctor::writePrescription(Storage<Prescription>& ps, Storage<Appointment>& apps)
{
	cout << "================================================" << endl;
	cout << "                  PRESCRIPTION                  " << endl;
	cout << "================================================" << endl;


	string appid;
	char medicines[500];
	char notes[300];


	cout << "Enter Appointment ID: ";
	cin >> appid;
	cin.ignore();

	Appointment* appt = apps.findById(appid);
	if (!appt || appt->getDocID() != ID)
	{
		cout << "Appointment not found or unauthorized." << endl; return;
	}

	if (appt->getStatus() != "completed")
	{
		cout << "Appointment is not completed yet." << endl; return;
	}

	for (int i = 0; i < ps.getCount(); i++)
	{
		if (ps.getAT(i)->getAppointmentID() == appid)
		{
			cout << "Prescription already written for this appointment." << endl; return;
		}
	}

	cout << "Enter Prescribed Medicine (e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
	cin.getline(medicines, 500);

	cout << "Enter notes of  Medicine: ";
	cin.getline(notes, 300);

	int newNum = FileHandler::getMaxPrescriptionID("prescriptions.txt") + 1;
	string presID = to_string(newNum);

	Prescription p(appid, presID, appt->getPatientID(), ID, appt->getDate(), medicines, notes);
	ps.add(p);

	string line = presID + "," + appid + "," + appt->getPatientID() + "," + ID + "," + appt->getDate() + "," + string(medicines) + "," + string(notes);
	FileHandler::appendLine("prescriptions.txt", line);

	cout << "Prescription Successfully added!" << endl;
	cout << "-----------------------------------------------------------" << endl;
}

void Doctor::viewPatientHistory(Storage<Prescription>& ps, Storage<Appointment>& apps, Storage<Patient>* patients) const
{
	string Pid;

	cout << "================================================" << endl;
	cout << "               PATIENT'S HISTORY                " << endl;
	cout << "================================================" << endl;
	cout << " Patient's ID: " << endl;
	cin >> Pid;

	bool found = false;
	for (int i = 0; i < apps.getCount(); i++)
	{
		if (apps.getAT(i)->getPatientID() == Pid && apps.getAT(i)->getDocID() == ID && apps.getAT(i)->getStatus() == "completed")
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		cout << "Access denied. You can only view records of your own patients." << endl; return;
	}

	string patientName = Pid;
	if (patients != nullptr)
	{
		Person* p = (Person*)patients->findById(Pid);
		if (p) patientName = p->getName();
	}

	int indices[200];
	int count = 0;

	for (int i = 0; i < ps.getCount(); i++)
	{
		if (ps.getAT(i)->getPatientID() == Pid && ps.getAT(i)->getDoctorID() == ID)
		{
			indices[count++] = i;
		}
	}

	if (count == 0)
	{
		cout << "No records found." << endl;
		return;
	}

	for (int i = 0; i < count - 1; i++)
	{
		for (int j = 0; j < count - i - 1; j++)
		{
			string d1 = ps.getAT(indices[j])->getDate();
			string d2 = ps.getAT(indices[j + 1])->getDate();

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
	for (int i = 0; i < count; i++) ps.getAT(indices[i])->displayPrescription();
}

bool Doctor::operator ==(const string& id) const
{
	return (this->ID == id);
}

ostream& operator<<(ostream& os, const Doctor& d)
{
	os << "-------------------------------------------" << endl;
	os << "ID:                  " << d.ID << endl;
	os << "Name:                " << d.name << endl;
	os << "Specialization:      " << d.specialization << endl;
	os << "Consultation Fee Rs: " << d.consultationFee << endl;
	os << "contact:             " << d.contact << endl;
	os << "-------------------------------------------" << endl;
	return os;
}

string Doctor::getSpec() const
{
	return specialization;
}
float Doctor::getfee() const
{
	return consultationFee;
}
string Doctor::getContact() const
{
	return contact;   // Person base class ka member
}