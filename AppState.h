#pragma once
#include <string>
using namespace std;

enum class Screen {
    MAIN_LOGIN,
    ADMIN_MENU,
    ADMIN_ADD_DOCTOR,
    ADMIN_ADD_PATIENT,
    ADMIN_REMOVE_DOCTOR,
    ADMIN_VIEW_DOCTORS,
    ADMIN_VIEW_PATIENTS,
    ADMIN_VIEW_APPOINTMENTS,
    ADMIN_VIEW_BILLS,
    ADMIN_DISCHARGE,
    ADMIN_DAILY_REPORT,
    ADMIN_SECURITY_LOG,
    DOCTOR_MENU,
    DOCTOR_VIEW_TODAY,
    DOCTOR_MARK_COMPLETE,
    DOCTOR_MARK_NOSHOW,
    DOCTOR_WRITE_PRESCRIPTION,
    DOCTOR_PATIENT_HISTORY,
    PATIENT_MENU,
    PATIENT_BOOK_APPOINTMENT,
    PATIENT_CANCEL_APPOINTMENT,
    PATIENT_VIEW_APPOINTMENTS,
    PATIENT_MEDICAL_RECORDS,
    PATIENT_VIEW_BILLS,
    PATIENT_PAY_BILLS,
    PATIENT_DEPOSIT,
    EXIT
};

struct AppState {
    Screen      current = Screen::MAIN_LOGIN;
    Screen      previous = Screen::MAIN_LOGIN;
    string      loggedInID = "";
    string      loggedInRole = "";  // "admin", "doctor", "patient"
    string      statusMsg = "";
    bool        msgIsError = false;
};
