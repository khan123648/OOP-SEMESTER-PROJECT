#include "Validation.h"
#include <iostream>
#include <string>


bool isNumber(char c)
{
	if (c >= '0' && c <= '9')
	{
		return true;
	}

	return false;
}

//ID
bool Validation::validateID(const string ID)
{

	if (ID.length() == 0)
	{
		return false;
	}

	if (ID.find(',') != string::npos)
	{
		return false;
	}

	return true;
}

//Date
bool Validation::validateDate(const string& date)
{
	//Format checking DD-MM-YYYY
	if (date.length() != 10)
	{
		return false;
	}

	//syntex check
	if (date[2] != '-' || date[5] != '-')
	{
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		if (i == 2 || i == 5)
		{
			continue;
		}
		if (isNumber(date[i]) == false)
		{
			return false;
		}
	}

	int d = stoi(date.substr(0, 2));
	int m = stoi(date.substr(3, 2));
	int y = stoi(date.substr(6, 4));

	if (d < 1 || d > 31)
	{
		return false;
	}

	if (m < 1 || m > 12)
	{
		return false;
	}

	if (y < 2025)
	{
		return false;
	}

	return true;
}

//Time
bool Validation::validateTimeSlot(const string& slot)
{
	const string validSlots[8] = { "09:00", "10:00", "11:00", "12:00","13:00", "14:00", "15:00", "16:00" };

	for (int i = 0; i < 8; i++)
	{
		if (slot == validSlots[i])
		{
			return true;
		}
	}
	return false;
}


//0000-0000000
bool Validation::validateContact(const string& phone)
{
	if (phone.length() != 12)
	{
		return false;
	}

	//syntex check
	if (phone[4] != '-')
	{
		return false;
	}

	for (int i = 0; i < 12; i++)
	{
		if (i == 4)
		{
			continue;
		}

		if (isNumber(phone[i]) == false)
		{
			return false;
		}
	}
	return true;
}

bool Validation::validatePassword(const string& password)
{
	if (password.length() >= 6 && password.find(',') == string::npos)
	{
		return true;
	}

	return false;
}

bool Validation::validateFee(float fee)
{
	if (fee > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Validation::validatePositiveFloat(float val)
{
	if (val > 0.0)
	{
		return true;
	}

	else
	{
		return false;
	}
}