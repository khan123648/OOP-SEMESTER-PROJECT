#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string> 
using namespace std;
class Validation
{
public:
	static bool validateID(const string ID);
	static bool validateDate(const string& date);
	static bool validateTimeSlot(const string& slot);
	static bool validateContact(const string& phone);
	static bool validatePassword(const string& password);
	static bool validateFee(float fee);
	static bool validatePositiveFloat(float val);
};