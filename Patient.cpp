#define _CRT_SECURE_NO_WARNINGS
#include "Patient.h"
#include "Person.h"
#include "Validation.h"
#include "InsufficientFundsException.h"
#include "SlotUnavailableException .h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include "FileHandler.h"

Patient::Patient() : Person()
{
	age = 0;
	gender = "";
	balance = 0.0f;
}

Patient::Patient(string i, string n, string p, string c, int a, string g, float b) : Person(i, n, p, c)
{
	age = a;
	gender = g;
	balance = b;
}

void Patient::displayMenu()
{
	cout << "===========================================================" << endl;
	cout << "                    PATIENT'S MAIN MENU                    " << endl;
	cout << "===========================================================" << endl;
	cout << "1.  View My Profile" << endl;
	cout << "2.  Book An Appointment" << endl;
	cout << "3.  Cancel An Appointment" << endl;
	cout << "4.  View My Appointments (those which are BOOKED)" << endl;
	cout << "5.  View All Medical Records (those which are RECORDED)" << endl;
	cout << "6.  View My Bills (those which are in RECORD)" << endl;
	cout << "7.  Pay pending Bills (From MENTIONED or COLLECTED data)" << endl;
	cout << "8.  Deposit Balance (only cash accepted)" << endl;
	cout << "0.  LOG OUT" << endl << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << " WHAT DO YOU WANT DO!" << endl;
	cout << "Enter Choice: " ;

}

void Patient::displayProfile()
{
	cout << "=============================================" << endl;
	cout << "              PATIENT'S PROFILE              " << endl;
	cout << "=============================================" << endl;
	cout << "ID:             " << ID << endl;
	cout << "Name:           " << name << endl;
	cout << "Age:            " << age << endl;
	cout << "Gender:         " << gender << endl;
	cout << "Contact:        " << contact << endl;
	cout << "Balance: Rs.    " << fixed << setprecision(2) << balance << endl;
}

void Patient::displayAvailableSlots(Storage<Appointment>& apps, const string& docID, const string& date) const
{
	const char* allSlots[8] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
	cout << "Available slots " << date << ":" << endl;

	for (int i = 0; i < 8; i++)
	{
		bool taken = false;
		for (int j = 0; j < apps.getCount(); j++)
		{
			if (apps.getAT(j)->getDocID() == docID && apps.getAT(j)->getDate() == date && apps.getAT(j)->getSlot() == string(allSlots[i]) && apps.getAT(j)->getStatus() != "cancelled")
			{
				taken = true;
				break;
			}


		}
		if (!taken)
		{
			cout << " - " << allSlots[i] << endl;
		}
	}
}

void Patient::bookAppointment(Storage<Doctor>& doc, Storage<Appointment>& apps, Storage<Bills>& bills)
{
	cout << "============================================" << endl;
	cout << "              BOOK APPOINTMENT              " << endl;
	cout << "============================================" << endl;
	string spec;
	cout << "Enter specialization you are looking for: " << endl;
	cin >> spec;

	string specLower = toLowerStr(spec);

	cout << "Type of Doctors u want to meet! " << endl << endl;

	bool ifFound = false;

	for (int i = 0; i < doc.getCount(); i++)
	{
		if (toLowerStr(doc.getAT(i)->getSpec()) == specLower)
		{
			cout << "ID:   " << doc.getAT(i)->getID() << endl;
			cout << "Name: " << doc.getAT(i)->getName() << endl;
			cout << "Fees: " << doc.getAT(i)->getfee() << endl << endl;
			ifFound = true;
		}
	}

	if (!ifFound)
	{
		cout << "Doctor not found." << endl;
		return;
	}

	string doctorId;
	cout << "Enter Doctor ID: ";
	cin >> doctorId;

	Doctor* attendingDoc = doc.findById(doctorId);
	if (!attendingDoc)
	{
		cout << "Doctor not found." << endl;
		return;
	}

	string date;
	int dateAttempts = 0;
	while (dateAttempts < 3)
	{
		cout << "Enter date (DD-MM-YYYY): ";
		cin >> date;

		if (Validation::validateDate(date))
		{
			break;
		}

		cout << "Invalid date. Use format DD-MM-YYYY." << endl;
		dateAttempts++;

		if (dateAttempts == 3)
		{
			cout << "Too many invalid attempts." << endl;
			return;
		}
	}

	string timeSlot;
	int slotAttempts = 0;

	while (slotAttempts < 3)
	{
		cout << "Enter time slot (like 09:00): ";
		cin >> timeSlot;

		if (!Validation::validateTimeSlot(timeSlot))
		{
			cout << "Invalid Slot! Please enter the slot from \"8\" restricted slot." << endl;
			slotAttempts++;
			continue;
		}

		Appointment newAppointment("", ID, doctorId, date, timeSlot, "pending");
		bool Conflict = false;

		for (int i = 0; i < apps.getCount(); i++)
		{
			if (*apps.getAT(i) == newAppointment)
			{
				Conflict = true;
				break;
			}
		}

		if (Conflict)
		{
			try
			{
				throw SlotUnavailableException();
			}
			catch (const SlotUnavailableException& e)
			{
				cout << e.what() << endl;
				displayAvailableSlots(apps, doctorId, date);
				slotAttempts++;
				continue;
			}
		}
		break;
	}

	if (slotAttempts >= 3)
	{
		cout << "Too many invalid attempts." << endl;
		return;
	}

	if (balance < attendingDoc->getfee())
	{
		try
		{
			throw InsufficientFundsException();
		}

		catch (const InsufficientFundsException& e)
		{
			cout << e.what() << endl;
			return;
		}

	}
	*this -= attendingDoc->getfee();

	int newNum = FileHandler::getMaxAppointmentID("appointments.txt") + 1;

	string appId = to_string(newNum);
	string billId = to_string(newNum + 500);

	Appointment finalApp(appId, ID, doctorId, date, timeSlot, "pending");
	apps.add(finalApp);

	Bills newBill(billId, ID, appId, date, to_string((int)attendingDoc->getfee()), "unpaid");
	bills.add(newBill);

	string appLine = appId + "," + ID + "," + doctorId + "," + date + "," + timeSlot + ",pending";
	FileHandler::appendLine("appointments.txt", appLine);

	string billLine = billId + "," + ID + "," + appId + "," + to_string((int)attendingDoc->getfee()) + ",unpaid," + date;

	FileHandler::appendLine("bills.txt", billLine);
	FileHandler::updatePatientBalance("patients.txt", ID, balance);

	cout << "Appointment booked successfully. Appointment ID: " << appId << endl;
}

void Patient::cancelAppointment(Storage<Appointment>& apps, Storage<Bills>& bills, Storage<Doctor>& docs)
{
	cout << "==============================================" << endl;
	cout << "              CANCEL APPOINTMENT              " << endl;
	cout << "==============================================" << endl;
	cout << " Your pending Appointments: " << endl;

	bool ifFound = false;

	for (int i = 0; i < apps.getCount(); i++)
	{
		if (apps.getAT(i)->getPatientID() == ID && apps.getAT(i)->getStatus() == "pending")
		{
			cout << "Appointment ID: " << apps.getAT(i)->getAppID() << endl;
			cout << "Doctor's ID: " << apps.getAT(i)->getDocID() << endl;
			cout << "Date: " << apps.getAT(i)->getDate() << endl << endl;
			cout << "Time Slot: " << apps.getAT(i)->getSlot() << endl << endl;
			ifFound = true;
		}
	}

	if (!ifFound)
	{
		cout << "You have no pending appointments!" << endl;
		return;
	}

	string cancelApp;

	cout << " Please appointment Id you want to cancel: " << endl;
	cin >> cancelApp;

	Appointment* appt = apps.findById(cancelApp);
	if (!appt || appt->getPatientID() != ID || appt->getStatus() != "pending")
	{
		cout << "Invalid appointment ID is entered!" << endl;
		return;
	}

	float fee = 0.0f;
	Doctor* doc = docs.findById(appt->getDocID());
	if (doc) fee = doc->getfee();

	appt->setStatus("cancelled");
	FileHandler::updateAppointmentStatus("appointments.txt", cancelApp, "cancelled");

	for (int i = 0; i < bills.getCount(); i++)
	{
		if (bills.getAT(i)->getAppID() == cancelApp)
		{
			bills.getAT(i)->setStatus("cancelled");
			FileHandler::updateBillStatusByAppID("bills.txt", cancelApp, "cancelled");
			break;
		}
	}

	*this += fee;
	FileHandler::updatePatientBalance("patients.txt", ID, balance);
	cout << "Appointment cancelled. PKR " << fee << " refunded to your balance." << endl;
}

void Patient::viewMyAppointment(Storage<Appointment>& apps, Storage<Doctor>& docs) const
{
	cout << "===============================================" << endl;
	cout << "              BOOKED APPOINTMENTS              " << endl;
	cout << "===============================================" << endl;

	int indices[200];
	int count = 0;
	for (int i = 0; i < apps.getCount(); i++)
	{
		if (apps.getAT(i)->getPatientID() == ID)
		{
			indices[count++] = i;
		}
	}

	if (count == 0)
	{
		cout << "No Bookings previously Recorded!" << endl;
		return;
	}

	for (int i = 0; i < count - 1; i++)
	{
		for (int j = 0; j < count - i - 1; j++)
		{
			string d1 = apps.getAT(indices[j])->getDate();
			string d2 = apps.getAT(indices[j + 1])->getDate();
			string k1 = d1.substr(6, 4) + d1.substr(3, 2) + d1.substr(0, 2);
			string k2 = d2.substr(6, 4) + d2.substr(3, 2) + d2.substr(0, 2);

			if (k1 > k2)
			{
				int temp = indices[j];
				indices[j] = indices[j + 1];
				indices[j + 1] = temp;
			}
		}
	}

	for (int i = 0; i < count; i++)
	{
		Appointment* a = apps.getAT(indices[i]);
		Doctor* doc = docs.findById(a->getDocID());
		string docName;
		string docSpec;

		if (doc != nullptr)
		{
			docName = doc->getName();
			docSpec = doc->getSpec();
		}

		else
		{
			docName = a->getDocID();
			docSpec = "";
		}

		cout << "Appointment ID: " << a->getAppID() << endl;
		cout << "Doctor's Name:  " << docName << endl;
		cout << "Specialization: " << docSpec << endl;
		cout << "Date:           " << a->getDate() << endl;
		cout << "Time Slot:      " << a->getSlot() << endl;
		cout << "Status:         " << a->getStatus() << endl;
	}
}

string Patient::toLowerStr(const string& s)
{
	string result = s;
	for (int i = 0; i < (int)result.size(); i++)
	{
		result[i] = (char)tolower((unsigned char)result[i]);
	}
		
	return result;
}

void Patient::viewMedicalRecords(Storage<Prescription>& ps) const
{
	cout << "===============================================" << endl;
	cout << "                 MEDICAL RECORDS               " << endl;
	cout << "===============================================" << endl;

	int indices[200];
	int count = 0;
	for (int i = 0; i < ps.getCount(); i++)
	{
		if (ps.getAT(i)->getPatientID() == ID)
		{
			indices[count++] = i;
		}
	}

	if (count == 0)
	{
		cout << "No Medical Records found!" << endl;
		return;
	}

	for (int i = 0; i < count - 1; i++)
	{
		for (int j = 0; j < count - i - 1; j++)
		{
			string d1 = ps.getAT(indices[j])->getDate();
			string d2 = ps.getAT(indices[j + 1])->getDate();
			string k1 = d1.substr(6, 4) + d1.substr(3, 2) + d1.substr(0, 2);
			string k2 = d2.substr(6, 4) + d2.substr(3, 2) + d2.substr(0, 2);

			if (k1 < k2)
			{
				int temp = indices[j];
				indices[j] = indices[j + 1];
				indices[j + 1] = temp;
			}
		}
	}

	for (int i = 0; i < count; i++)
	{
		ps.getAT(indices[i])->displayPrescription();
	}
}

void Patient::viewBills(Storage<Bills>& bills) const
{
	cout << "==================================================" << endl;
	cout << "                      BILLS                       " << endl;
	cout << "==================================================" << endl;

	bool found = false;
	float totalUnpaid = 0.0f;

	for (int i = 0; i < bills.getCount(); i++)
	{
		if (bills.getAT(i)->getPatientID() == this->ID)
		{
			bills.getAT(i)->displayBill();
			if (bills.getAT(i)->getStatus() == "unpaid")
			{
				totalUnpaid += stof(bills.getAT(i)->getAmount());
			}
			found = true;
		}
	}

	if (!found)
	{
		cout << "No previous billing Records Found." << endl;
		return;
	}

	cout << "Total Outstanding (Unpaid): Rs. " << fixed << setprecision(2) << totalUnpaid << endl;
}

void Patient::PayBills(Storage<Bills>& bills)
{
	bool ifThere = false;
	for (int i = 0; i < bills.getCount(); i++)
	{
		if (bills.getAT(i)->getPatientID() == this->ID && bills.getAT(i)->getStatus() == "unpaid")
		{
			bills.getAT(i)->displayBill();
			ifThere = true;
		}
	}


	if (!ifThere)
	{
		cout << "No unpaid bills found!";
		return;
	}

	string billID;
	cout << "Enter Bill ID to pay" << endl;
	cin >> billID;

	Bills* bill = bills.findById(billID);
	if (!bill || bill->getPatientID() != ID || bill->getStatus() != "unpaid")
	{
		cout << "Invalid bill ID is entered or it is already paid!" << endl;
		return;
	}

	float amount = stof(bill->getAmount());
	if (balance < amount)
	{
		try
		{
			throw InsufficientFundsException();
		}

		catch (const InsufficientFundsException& e)
		{
			cout << e.what() << endl;
			return;
		}
	}

	*this -= amount;
	bill->setStatus("paid");
	FileHandler::updateBillStatusByBillID("bills.txt", billID, "paid");
	FileHandler::updatePatientBalance("patients.txt", ID, balance);
	cout << "Bill paid successfully. Remaining balance: PKR " << fixed << setprecision(2) << balance << endl;
}


void Patient::upBalance()
{


	int attempts = 0;
	float amount;

	while (attempts < 3)
	{
		cout << "\n-----------------------------------------------------------" << endl;
		cout << "Please Enter the amount of money you want to deposit (PKR): ";

		if (!(cin >> amount))
		{
			cout << "Invalid input! Please enter a numeric value." << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			attempts++;
			continue;
		}

		if (!Validation::validatePositiveFloat(amount))
		{
			cout << "Invalid amount! Deposit must be greater than zero." << endl;
			attempts++;
		}

		else
		{
			*this += amount;

			FileHandler::updatePatientBalance("patients.txt", ID, balance);

			cout << "-----------------------------------------------------------" << endl;
			cout << "Balance updated." << endl;
			cout << "New balance Rs: " << fixed << setprecision(2) << balance << endl;
			return;
		}
	}

	if (attempts >= 3)
	{
		cout << "Too many invalid attempts. Returning to menu." << endl;
	}

}

void Patient::operator +=(float amount)
{
	this->balance += amount;
}

void Patient::operator -=(float amount)
{
	this->balance -= amount;
}

bool Patient::operator ==(const string& otherid) const
{
	return this->ID == otherid;
}

ostream& operator<<(ostream& os, const Patient& p)
{
	os << "-------------------------------------------" << endl;
	os << "ID: " << p.ID << endl;
	os << "Name: " << p.name << endl;
	os << "Age: " << p.age << endl;
	os << "Balance: " << p.balance << endl;
	os << "Gender:  " << p.gender << endl;
	os << "Contact: " << p.contact << endl;
	os << "-------------------------------------------" << endl;
	return os;
}

int Patient::getAge() const
{
	return age;
}

string Patient::getGender() const
{
	return gender;
}

float Patient::getBalance() const
{
	return balance;
}