#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<iostream>
#include<cstring>
#include<string>
using namespace std;
class Hospital_Exception
{
protected:
	char message[200];

public:
	Hospital_Exception()
	{
		strncpy(message, "Some kind of Error is detected!", 199);
		message[199] = '\0';
	}

	Hospital_Exception(const char* msg)
	{
		strncpy(message, msg , 199);
		message[199] = '\0';
	}

	virtual const char* what() const throw()
	{
		return message;
	}

	virtual ~Hospital_Exception(){}
};