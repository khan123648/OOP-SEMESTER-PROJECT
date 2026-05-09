#pragma once
#include "HospitalException.h"

class SlotUnavailableException : public Hospital_Exception
{
public:
	SlotUnavailableException() : Hospital_Exception("The slot is already filled try finding another one.ERROR!.") {}
};