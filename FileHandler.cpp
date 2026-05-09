#define _CRT_SECURE_NO_WARNINGS
#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>
#include <iostream>
#include "Storage.h"
#include "Admin.h"
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Bills.h"
#include "Prescription.h"
#include "Person.h"

using namespace std;

void FileHandler::parseCSV(const string& line, string* fields, int maxFields, int& count)
{
    count = 0;
    string cur = "";
    for (int i = 0; i <= (int)line.size(); i++)
    {
        if (i == (int)line.size() || line[i] == ',')
        {
            if (count < maxFields)
            {
                fields[count++] = cur;
            }
            cur = "";
        }
        else
        {
            cur += line[i];
        }
    }
}

void FileHandler::loadingPatients(const string& fn, Storage<Patient>& patients)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        string f[7];
        int cnt;
        parseCSV(line, f, 7, cnt);
        if (cnt < 7)
        {
            continue;
        }
        patients.add(Patient(f[0], f[1], f[5], f[4], stoi(f[2]), f[3], stof(f[6])));
    }
}

void FileHandler::loadingDoctors(const string& fn, Storage<Doctor>& docs)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        string f[6];
        int cnt;
        parseCSV(line, f, 6, cnt);
        if (cnt < 6)
        {
            continue;
        }
        docs.add(Doctor(f[0], f[1], f[4], f[3], f[2], stof(f[5])));
    }
}

void FileHandler::loadingAdmins(const string& fn, Storage<Admin>& adds)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        string f[3];
        int cnt;
        parseCSV(line, f, 3, cnt);
        if (cnt < 3)
        {
            continue;
        }
        adds.add(Admin(f[0], f[1], f[2], ""));
    }
}

void FileHandler::loadingAppointments(const string& fn, Storage<Appointment>& apps)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        string f[6];
        int cnt;
        parseCSV(line, f, 6, cnt);
        if (cnt < 6)
        {
            continue;
        }
        apps.add(Appointment(f[0], f[1], f[2], f[3], f[4], f[5]));
    }
}

void FileHandler::loadingBills(const string& fn, Storage<Bills>& bills)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        string f[6];
        int cnt;
        parseCSV(line, f, 6, cnt);
        if (cnt < 6)
        {
            continue;
        }
        bills.add(Bills(f[2], f[1], f[0], f[5], f[3], f[4]));
    }
}

void FileHandler::loadingPrescriptions(const string& fn, Storage<Prescription>& ps)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        string f[7];
        int cnt;
        parseCSV(line, f, 7, cnt);
        if (cnt < 7)
        {
            continue;
        }
        ps.add(Prescription(f[1], f[0], f[2], f[3], f[4], f[5].c_str(), f[6].c_str()));
    }
}

void FileHandler::savePatients(const string& fn, const Storage<Patient>& patients)
{
    ofstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    for (int i = 0; i < patients.getCount(); i++)
    {
        Patient* p = patients.getAT(i);
        file << p->getID() << "," << p->getName() << "," << p->getAge() << ","<< p->getGender() << "," << p->getContact() << "," << p->getPassword() << "," << fixed << setprecision(2) << p->getBalance() << "\n";
    }
}

void FileHandler::saveDoctors(const string& fn, const Storage<Doctor>& docs)
{
    ofstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    for (int i = 0; i < docs.getCount(); i++)
    {
        Doctor* d = docs.getAT(i);
        file << d->getID() << "," << d->getName() << "," << d->getSpec() << "," << d->getContact() << "," << d->getPassword() << "," << d->getfee() << "\n";
    }
}

void FileHandler::saveAppointments(const string& fn, const Storage<Appointment>& apps)
{
    ofstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    for (int i = 0; i < apps.getCount(); i++)
    {
        Appointment* a = apps.getAT(i);
        file << a->getAppID() << "," << a->getPatientID() << "," << a->getDocID() << ","
            << a->getDate() << "," << a->getSlot() << "," << a->getStatus() << "\n";
    }
}

void FileHandler::saveBills(const string& fn, const Storage<Bills>& bills)
{
    ofstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    for (int i = 0; i < bills.getCount(); i++)
    {
        Bills* b = bills.getAT(i);
        file << b->getBill() << "," << b->getPatientID() << "," << b->getAppID() << ","
            << b->getAmount() << "," << b->getStatus() << "," << b->getDate() << "\n";
    }
}

void FileHandler::savePrescriptions(const string& fn, const Storage<Prescription>& ps)
{
    ofstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    for (int i = 0; i < ps.getCount(); i++)
    {
        Prescription* p = ps.getAT(i);
        file << p->getPrescriptionID() << "," << p->getAppointmentID() << ","
            << p->getPatientID() << "," << p->getDoctorID() << "," << p->getDate() << ","
            << p->getMedicines() << "," << p->getNotes() << "\n";
    }
}

void FileHandler::appendLine(const string& fn, const string& line)
{
    ofstream file(fn, ios::app);
    if (!file.is_open())
    {
        return;
    }
    file << line << "\n";
}

int FileHandler::getMaxAppointmentID(const string& fn)
{
    ifstream file(fn);
    int maxID = 0;
    if (!file.is_open())
    {
        return maxID;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        int commaPos = (int)line.find(',');
        if (commaPos == (int)string::npos)
        {
            continue;
        }
        string idStr = line.substr(0, commaPos);
        string numPart = "";
        for (char c : idStr)
        {
            if (c >= '0' && c <= '9')
            {
                numPart += c;
            }
        }
        if (!numPart.empty())
        {
            int val = stoi(numPart);
            if (val > maxID)
            {
                maxID = val;
            }
        }
    }
    return maxID;
}

int FileHandler::getMaxPrescriptionID(const string& fn)
{
    ifstream file(fn);
    int maxID = 0;
    if (!file.is_open())
    {
        return maxID;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        int commaPos = (int)line.find(',');
        if (commaPos == (int)string::npos)
        {
            continue;
        }
        string idStr = line.substr(0, commaPos);
        string numPart = "";
        for (char c : idStr)
        {
            if (c >= '0' && c <= '9')
            {
                numPart += c;
            }
        }
        if (!numPart.empty())
        {
            int val = stoi(numPart);
            if (val > maxID)
            {
                maxID = val;
            }
        }
    }
    return maxID;
}

int FileHandler::getMaxDoctorID(const string& fn)
{
    ifstream file(fn);
    int maxID = 0;
    if (!file.is_open())
    {
        return maxID;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        int commaPos = (int)line.find(',');
        if (commaPos == (int)string::npos)
        {
            continue;
        }
        string idStr = line.substr(0, commaPos);
        string numPart = "";
        for (char c : idStr)
        {
            if (c >= '0' && c <= '9')
            {
                numPart += c;
            }
        }
        if (!numPart.empty())
        {
            int val = stoi(numPart);
            if (val > maxID)
            {
                maxID = val;
            }
        }
    }
    return maxID;
}

void FileHandler::updateAppointmentStatus(const string& fn, const string& appID, const string& newStatus)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string allLines, line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            allLines += "\n";
            continue;
        }
        int pos = (int)line.find(',');
        if (pos != (int)string::npos && line.substr(0, pos) == appID)
        {
            int lastComma = (int)line.rfind(',');
            line = line.substr(0, lastComma + 1) + newStatus;
        }
        allLines += line + "\n";
    }
    file.close();
    ofstream out(fn);
    out << allLines;
}

void FileHandler::updateBillStatusByAppID(const string& fn, const string& appID, const string& newStatus)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string allLines, line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            allLines += "\n";
            continue;
        }

        string fields[6];
        int f = 0;
        string cur = "";
        for (int i = 0; i <= (int)line.size(); i++)
        {
            if (i == (int)line.size() || line[i] == ',')
            {
                if (f < 6)
                {
                    fields[f++] = cur;
                }
                cur = "";
            }
            else
            {
                cur += line[i];
            }
        }
        if (f >= 3 && fields[2] == appID)
        {
            fields[4] = newStatus;
        }

        string rebuilt = "";
        for (int i = 0; i < f; i++)
        {
            rebuilt += fields[i];
            if (i < f - 1)
            {
                rebuilt += ",";
            }
        }
        allLines += rebuilt + "\n";
    }
    file.close();
    ofstream out(fn);
    out << allLines;
}

void FileHandler::updateBillStatusByBillID(const string& fn, const string& billID, const string& newStatus)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string allLines, line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            allLines += "\n";
            continue;
        }

        string fields[6];
        int f = 0;
        string cur = "";
        for (int i = 0; i <= (int)line.size(); i++)
        {
            if (i == (int)line.size() || line[i] == ',')
            {
                if (f < 6)
                {
                    fields[f++] = cur;
                }
                cur = "";
            }
            else
            {
                cur += line[i];
            }
        }
        if (f >= 1 && fields[0] == billID)
        {
            fields[4] = newStatus;
        }

        string rebuilt = "";
        for (int i = 0; i < f; i++)
        {
            rebuilt += fields[i];
            if (i < f - 1)
            {
                rebuilt += ",";
            }
        }
        allLines += rebuilt + "\n";
    }
    file.close();
    ofstream out(fn);
    out << allLines;
}

void FileHandler::updatePatientBalance(const string& fn, const string& patID, float newBalance)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string allLines, line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            allLines += "\n";
            continue;
        }

        string fields[7];
        int f = 0;
        string cur = "";
        for (int i = 0; i <= (int)line.size(); i++)
        {
            if (i == (int)line.size() || line[i] == ',')
            {
                if (f < 7)
                {
                    fields[f++] = cur;
                }
                cur = "";
            }
            else
            {
                cur += line[i];
            }
        }
        if (f >= 1 && fields[0] == patID)
        {
            ostringstream oss;
            oss << fixed << setprecision(2) << newBalance;
            fields[6] = oss.str();
        }
        string rebuilt = "";
        for (int i = 0; i < f; i++)
        {
            rebuilt += fields[i];
            if (i < f - 1)
            {
                rebuilt += ",";
            }
        }
        allLines += rebuilt + "\n";
    }
    file.close();
    ofstream out(fn);
    out << allLines;
}

void FileHandler::deleteRecordByID(const string& fn, const string& id)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string allLines, line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        int pos = (int)line.find(',');
        string firstField = (pos != (int)string::npos) ? line.substr(0, pos) : line;
        if (firstField != id)
        {
            allLines += line + "\n";
        }
    }
    file.close();
    ofstream out(fn);
    out << allLines;
}

void FileHandler::deleteRecordsByField(const string& fn, int fieldIdx, const string& value)
{
    ifstream file(fn);
    if (!file.is_open())
    {
        return;
    }

    string allLines, line;
    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        string fields[10];
        int f = 0;
        string cur = "";
        for (int i = 0; i <= (int)line.size(); i++)
        {
            if (i == (int)line.size() || line[i] == ',')
            {
                if (f < 10)
                {
                    fields[f++] = cur;
                }
                cur = "";
            }
            else
            {
                cur += line[i];
            }
        }
        if (fieldIdx < f && fields[fieldIdx] == value)
        {
            continue;
        }
        allLines += line + "\n";
    }
    file.close();
    ofstream out(fn);
    out << allLines;
}

void FileHandler::writeSecurityLog(const string& role, const string& enteredID, const string& result)
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[30];
    strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", ltm);

    string entry = string(buf) + "," + role + "," + enteredID + "," + result;
    appendLine("security_log.txt", entry);
}

void FileHandler::archivePatient(const string& patID)
{
    {
        ifstream f("patients.txt");
        ofstream d("discharged.txt", ios::app);
        string line;
        while (getline(f, line))
        {
            if (line.empty())
            {
                continue;
            }
            int pos = (int)line.find(',');
            if (pos != (int)string::npos && line.substr(0, pos) == patID)
            {
                d << line << "\n";
            }
        }
    }

    {
        ifstream f("appointments.txt");
        ofstream d("discharged.txt", ios::app);
        string line;
        while (getline(f, line))
        {
            if (line.empty())
            {
                continue;
            }
            string fields[6];
            int fc = 0;
            string cur = "";
            for (int i = 0; i <= (int)line.size(); i++)
            {
                if (i == (int)line.size() || line[i] == ',')
                {
                    if (fc < 6)
                    {
                        fields[fc++] = cur;
                    }
                    cur = "";
                }
                else
                {
                    cur += line[i];
                }
            }
            if (fc >= 2 && fields[1] == patID)
            {
                d << line << "\n";
            }
        }
    }

    {
        ifstream f("bills.txt");
        ofstream d("discharged.txt", ios::app);
        string line;
        while (getline(f, line))
        {
            if (line.empty())
            {
                continue;
            }
            string fields[6];
            int fc = 0;
            string cur = "";
            for (int i = 0; i <= (int)line.size(); i++)
            {
                if (i == (int)line.size() || line[i] == ',')
                {
                    if (fc < 6)
                    {
                        fields[fc++] = cur;
                    }
                    cur = "";
                }
                else
                {
                    cur += line[i];
                }
            }
            if (fc >= 2 && fields[1] == patID)
            {
                d << line << "\n";
            }
        }
    }

    {
        ifstream f("prescriptions.txt");
        ofstream d("discharged.txt", ios::app);
        string line;
        while (getline(f, line))
        {
            if (line.empty())
            {
                continue;
            }
            string fields[7];
            int fc = 0;
            string cur = "";
            for (int i = 0; i <= (int)line.size(); i++)
            {
                if (i == (int)line.size() || line[i] == ',')
                {
                    if (fc < 7)
                    {
                        fields[fc++] = cur;
                    }
                    cur = "";
                }
                else
                {
                    cur += line[i];
                }
            }
            if (fc >= 3 && fields[2] == patID)
            {
                d << line << "\n";
            }
        }
    }
}

void FileHandler::initAdminFile(const string& fn)
{
    ifstream check(fn);
    if (check.good())
    {
        return;
    }
    check.close();

    ofstream file(fn);
    file << "1,Admin,admin123\n";
    file.close();

    cout << "[INFO] admin.txt created with default admin (ID: 1, Pass: admin123)" << endl;
}