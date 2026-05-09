#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <sstream>
#include "GUI.h"          // makeText / makeCentered / makePanel as free functions
#include "AppState.h"
#include "Storage.h"
#include "Admin.h"
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Bills.h"
#include "Prescription.h"
#include "FileHandler.h"
#include "Validation.h"
using namespace std;

class HospitalApp {
public:
    HospitalApp();
    void run();

private:
    // ── Window & Font ──────────────────────────────────────────────────────
    sf::RenderWindow win;
    sf::Font         font;
    AppState         state;
    Toast            toast;

    // ── Data ──────────────────────────────────────────────────────────────
    Storage<Admin>        admins;
    Storage<Doctor>       doctors;
    Storage<Patient>      patients;
    Storage<Appointment>  appointments;
    Storage<Bills>        bills;
    Storage<Prescription> prescriptions;

    // ── Per-screen UI ──────────────────────────────────────────────────────
    // Login
    TextBox  tbLoginID;
    TextBox  tbLoginPass;
    Button   btnLoginAdmin;
    Button   btnLoginDoctor;
    Button   btnLoginPatient;
    Button   btnLoginGo;
    Button   btnExit;

    // Generic back / logout button (reused on every sub-screen)
    Button   btnBack;

    // Admin menu buttons
    static const int ADMIN_BTN_COUNT = 11;
    Button   adminBtns[ADMIN_BTN_COUNT];

    // Doctor menu buttons
    static const int DOC_BTN_COUNT = 7;
    Button   docBtns[DOC_BTN_COUNT];

    // Patient menu buttons
    static const int PAT_BTN_COUNT = 9;
    Button   patBtns[PAT_BTN_COUNT];

    // Forms — Add Doctor
    TextBox  tbDocName, tbDocPass, tbDocContact, tbDocSpec, tbDocFee;
    Button   btnDocSubmit;

    // Forms — Add Patient
    TextBox  tbPatName, tbPatPass, tbPatContact, tbPatAge, tbPatGender;
    Button   btnPatSubmit;

    // Forms — generic single-ID (remove doctor / discharge / mark appointment)
    TextBox  tbTargetID;
    Button   btnConfirmAction;

    // Forms — Book Appointment
    TextBox  tbBookSpec, tbBookDocID, tbBookDate, tbBookSlot;
    Button   btnBookSubmit;

    // Forms — Write Prescription
    TextBox  tbPresAppID, tbPresMeds, tbPresNotes;
    Button   btnPresSubmit;

    // Forms — Pay Bill
    TextBox  tbBillID;
    Button   btnBillPay;

    // Forms — Deposit
    TextBox  tbDepositAmt;
    Button   btnDepositSubmit;

    // Forms — Patient History
    TextBox  tbHistPatID;
    Button   btnHistSearch;

    // Forms — Cancel Appointment
    TextBox  tbCancelAppID;
    Button   btnCancelSubmit;

    // Scroll lists
    ScrollList mainList;
    ScrollList subList;

    // Report / log text lines
    vector<string> reportLines;

    // Login role selection: 0=none 1=admin 2=doctor 3=patient
    int loginRole = 0;

    // ── Init ───────────────────────────────────────────────────────────────
    void initLoginScreen();
    void initAdminMenu();
    void initDoctorMenu();
    void initPatientMenu();
    void initForms();
    void initScrollLists();

    // ── Events ─────────────────────────────────────────────────────────────
    void handleEvents();
    void handleLoginEvents(const sf::Event& e, sf::Vector2f mp);
    void handleAdminMenuEvents(const sf::Event& e, sf::Vector2f mp);
    void handleDoctorMenuEvents(const sf::Event& e, sf::Vector2f mp);
    void handlePatientMenuEvents(const sf::Event& e, sf::Vector2f mp);
    void handleFormEvents(const sf::Event& e, sf::Vector2f mp);

    // ── Drawing ────────────────────────────────────────────────────────────
    void draw();
    void drawLogin();
    void drawAdminMenu();
    void drawDoctorMenu();
    void drawPatientMenu();
    void drawScrollScreen(const string& title);
    void drawReportScreen(const string& title);
    void drawSidebar(const string& role, const string& name);
    void drawTopBar(const string& title);

    // ── Logic ──────────────────────────────────────────────────────────────
    void doLogin();
    void doAddDoctor();
    void doAddPatient();
    void doRemoveDoctor();
    void doDischargePatient();
    void doMarkComplete();
    void doMarkNoShow();
    void doWritePrescription();
    void doCancelAppointment();
    void doBookAppointment();
    void doPayBill();
    void doDeposit();
    void doViewDoctors();
    void doViewPatients();
    void doViewAppointments();
    void doViewUnpaidBills();
    void doViewTodayAppointments();
    void doViewMyAppointments();
    void doViewMedicalRecords();
    void doViewMyBills();
    void doPatientHistory();
    void doDailyReport();
    void doSecurityLog();
    void syncAndSave();

    // ── Utility ────────────────────────────────────────────────────────────
    void    goTo(Screen s);
    void    goBack();
    string  getTodayStr();
    // NOTE: makeText / makeCentered / makePanel are free functions in GUI.h
    // Declaring them as members here would create ambiguity — do NOT add them back.
    Admin* currentAdmin();
    Doctor* currentDoctor();
    Patient* currentPatient();
};