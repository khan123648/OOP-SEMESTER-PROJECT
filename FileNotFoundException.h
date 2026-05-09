#pragma once
#include "HospitalException.h"

class FileNotFoundException : public Hospital_Exception
{
	public:
		FileNotFoundException(): Hospital_Exception("Cannot locate/Find the required file.ERROR!."){}
};
