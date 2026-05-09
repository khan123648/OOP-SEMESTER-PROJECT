#pragma once
#include "HospitalException.h"

class InvalidInputException : public Hospital_Exception
{
public:
	InvalidInputException() : Hospital_Exception("Cannot locate/Find the required inout format.ERROR!.") {}
};