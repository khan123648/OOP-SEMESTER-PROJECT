#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "Storage.h"
#include "Person.h"
class Doctor;
class Patient;
class Appointment;
class Bills;

class Admin : public Person
{
public:

	Admin();
	Admin(string i, string n, string p, string c);
	void displayMenu() override;
	void displayProfile() override;
	void addDoctor(Storage<Doctor>& docs);
	void removeDoctor(Storage<Doctor>& docs, Storage<Appointment>& apps);
	void viewAllPatients(Storage<Patient>& pat, Storage<Bills>& bills);
	void viewAllDoctors(Storage<Doctor>& docs);
	void viewAllAppointments(Storage<Appointment>& apps);
	void viewUnpaidBills(Storage<Bills>& bills);
	void dischargePatient(Storage<Patient>& patients, Storage<Bills>& bills, Storage<Appointment>& apps);
	void viewSecurityLog();
	void generateDailyReport(Storage<Appointment>& apps, Storage<Bills>& bills, Storage<Patient>& patients, Storage<Doctor>& docs);
};

