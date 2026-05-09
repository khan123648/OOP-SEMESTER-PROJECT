#pragma once
#include "HospitalException.h"

class InsufficientFundsException : public Hospital_Exception
{
public:
	InsufficientFundsException() : Hospital_Exception("Cannot locate/Find the required funds.ERROR!.") {}
};