#include "Person.h"

Person::Person()
{
	ID = "";
	name = "";
	password = "";
	contact = "";
}

Person::Person(string i, string n, string p, string c)
{
	ID = i;
	name = n;
	password = p;
	contact = c;
}

Person::~Person() {}

string Person::getID()       const { return ID; }
string Person::getName()     const { return name; }
string Person::getPassword() const { return password; }
string Person::getContact()  const { return contact; }

void Person::setName(string n) { name = n; }
void Person::setPassword(string p) { password = p; }
void Person::setContact(string c) { contact = c; }