#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class Person {
protected:
	string ID;
	string name;
	string password;
	string contact;

	int    failedAttempts = 0;
	bool   isLocked = false;
	time_t lockedAt = 0;


public:
	static const int MAX_ATTEMPTS = 3;
	static const int LOCKOUT_SECS = 60;

	Person();
	Person(string i, string n, string p, string c);
	virtual ~Person();

	bool checkLocked() {
		if (!isLocked) return false;
		time_t now = time(0);
		if (difftime(now, lockedAt) >= LOCKOUT_SECS) {
			isLocked = false;
			failedAttempts = 0;
		}
		return isLocked;
	}

	bool registerFailedAttempt() {
		failedAttempts++;
		if (failedAttempts >= MAX_ATTEMPTS) {
			isLocked = true;
			lockedAt = time(0);
			return true;
		}
		return false;
	}

	void resetAttempts() {
		failedAttempts = 0;
		isLocked = false;
		lockedAt = 0;
	}

	int  getFailedAttempts()   const { return failedAttempts; }
	bool getIsLocked()         const { return isLocked; }

	int getRemainingLockSecs() const {
		if (!isLocked) return 0;
		time_t now = time(0);
		int rem = LOCKOUT_SECS - (int)difftime(now, lockedAt);
		return rem > 0 ? rem : 0;
	}

	virtual void displayMenu() = 0;
	virtual void displayProfile() = 0;

	string getID()       const;
	string getName()     const;
	string getPassword() const;
	string getContact()  const;

	void setName(string n);
	void setPassword(string p);
	void setContact(string c);


};