#pragma once
#include "Storage.h"
#include "Person.h"
#include "FileHandler.h"
#include "Validation.h"
#include "Admin.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bills.h"
#include "Prescription.h"
using namespace std;

class Doctor;

class Patient : public Person
{
	int age;
	string gender;
	float balance;
public:
    Patient();
    Patient(string i, string n, string p, string c, int a, string g, float b);
	void displayMenu() override;
	void displayProfile() override;
	void displayAvailableSlots(Storage<Appointment>& apps, const string& docID, const string& date) const;
	void bookAppointment(Storage<Doctor>& doc, Storage<Appointment>& apps, Storage<Bills>& bills);
	void cancelAppointment(Storage<Appointment>& apps, Storage<Bills>& bills, Storage<Doctor>& docs);
	void viewMyAppointment(Storage<Appointment>& apps, Storage<Doctor>& docs) const;
	void viewMedicalRecords(Storage<Prescription>& ps) const;
	void viewBills(Storage<Bills>& bills) const;
	static string toLowerStr(const string& s);
	void PayBills(Storage<Bills>& bills);
	void upBalance(); 
	void operator +=(float amount);
	void operator -=(float amount);
	bool operator ==(const string &otherid) const;
	friend ostream& operator<<(ostream& os, const Patient& p);
	int getAge() const;
	string getGender() const;
	float getBalance() const;
};