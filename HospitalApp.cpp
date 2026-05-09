#define _CRT_SECURE_NO_WARNINGS
#include "HospitalApp.h"
#include "InsufficientFundsException.h"
#include "SlotUnavailableException .h"
#include "FileNotFoundException.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>

static void drawLines(sf::RenderWindow& win, const sf::Font& font,
    const vector<string>& lines,
    float x, float startY, float lineH = 21.f,
    sf::Color c = Theme::TEXT, unsigned int sz = 13)
{
    for (int i = 0; i < (int)lines.size(); i++)
        win.draw(makeText(font, lines[i], x, startY + i * lineH, c, sz));
}

// ═══════════════════════════════════════════════════════════════════════════
// CONSTRUCTOR
// ═══════════════════════════════════════════════════════════════════════════
HospitalApp::HospitalApp()
    : win(sf::VideoMode({ 1280u, 780u }), "MediCore Hospital Management System",
        sf::Style::Titlebar | sf::Style::Close)
{
    win.setFramerateLimit(60);
    if (!font.openFromFile("arial.ttf"))
        font.openFromFile("C:/Windows/Fonts/arial.ttf");
    GUIFont::set(font);

    FileHandler::initAdminFile("admin.txt");
    FileHandler::loadingAdmins("admin.txt", admins);
    FileHandler::loadingDoctors("doctors.txt", doctors);
    FileHandler::loadingPatients("patients.txt", patients);
    FileHandler::loadingAppointments("appointments.txt", appointments);
    FileHandler::loadingBills("bills.txt", bills);
    FileHandler::loadingPrescriptions("prescriptions.txt", prescriptions);

    toast.init(font, 1280.f, 780.f);
    initLoginScreen();
    initAdminMenu();
    initDoctorMenu();
    initPatientMenu();
    initForms();
    initScrollLists();
}

// ═══════════════════════════════════════════════════════════════════════════
// LOGIN
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::initLoginScreen()
{
    float cardX = 390.f;   // card left edge  (640 - 250)
    float fieldX = 420.f;   // field left edge (cardX + 30)
    float fieldW = 440.f;

    // Card top = 100, logo area = 82px, divider at 182
    // label(16) + field(44) + gap(14) pattern from y=198
    // ID:    label@198, field@214
    // Pass:  label@274, field@290
    // Role:  label@350, pills@366
    // Btns:  @418
    float fy1 = 214.f;   // ID field
    float fy2 = 290.f;   // Pass field
    float fy3 = 366.f;   // Role pills
    float fy4 = 418.f;   // Action buttons

    tbLoginID.init(font, "Enter your ID", fieldX, fy1, fieldW, 44.f);
    tbLoginPass.init(font, "Enter your password", fieldX, fy2, fieldW, 44.f, true);

    // Three equal-width role pills
    float rw = (fieldW - 16.f) / 3.f;
    btnLoginAdmin.init(font, "Admin", fieldX, fy3, rw, 36.f,
        Theme::PANEL2, Theme::ACCENT_DARK, Theme::TEXT_DIM, 12);
    btnLoginDoctor.init(font, "Doctor", fieldX + rw + 8.f, fy3, rw, 36.f,
        Theme::PANEL2, Theme::ACCENT_DARK, Theme::TEXT_DIM, 12);
    btnLoginPatient.init(font, "Patient", fieldX + rw * 2 + 16.f, fy3, rw, 36.f,
        Theme::PANEL2, Theme::ACCENT_DARK, Theme::TEXT_DIM, 12);

    // Login / Exit buttons
    float abw = (fieldW - 12.f) / 2.f;
    btnLoginGo.init(font, "LOGIN", fieldX, fy4, abw, 44.f,
        Theme::ACCENT_DARK, Theme::ACCENT_DIM, Theme::ACCENT, 13, Theme::ACCENT_DIM);
    btnExit.init(font, "Exit", fieldX + abw + 12.f, fy4, abw, 44.f,
        Theme::PANEL2, Theme::DANGER_DK, Theme::TEXT_DIM, 13, Theme::BORDER);

    // Sidebar back / logout button
    btnBack.init(font, "<- Menu", 14.f, 696.f, 152.f, 36.f,
        Theme::ACCENT_DARK, { 110,48,32 }, Theme::ACCENT, 12, Theme::ACCENT_DIM);
}

// ═══════════════════════════════════════════════════════════════════════════
// ADMIN MENU  — 2-col grid
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::initAdminMenu()
{
    const char* labels[ADMIN_BTN_COUNT] = {
        "My Profile",       "Add New Doctor",
        "Add New Patient",  "Remove Doctor",
        "View All Doctors", "View All Patients",
        "View Appointments","View Unpaid Bills",
        "Discharge Patient","Daily Report",
        "Security Log"
    };

    // 2 columns: each (1080-14)/2 = 533 px wide
    // sy=196 leaves room for: topbar(56) + cards(74) + section-header(32) + gap(34)
    float sx = 190.f, sy = 200.f, bw = 533.f, bh = 46.f, gx = 14.f, gy = 10.f;
    for (int i = 0; i < ADMIN_BTN_COUNT; i++) {
        float bx = sx + (i % 2) * (bw + gx);
        float by = sy + (i / 2) * (bh + gy);
        adminBtns[i].init(font, labels[i], bx, by, bw, bh,
            Theme::PANEL2, Theme::PANEL3, Theme::TEXT, 13, Theme::BORDER);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// DOCTOR MENU
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::initDoctorMenu()
{
    const char* labels[DOC_BTN_COUNT] = {
        "My Profile",
        "Today's Appointments",
        "Mark Appointment Complete",
        "Mark Appointment No-Show",
        "Write Prescription",
        "View Patient History",
        "Logout"
    };
    // topbar(56) + info-card(60) + section-header(32) + gap(8) = 156 → start at 164
    float bx = 190.f, by = 164.f, bw = 1080.f, bh = 46.f, gap = 10.f;
    Theme::PANEL2, Theme::PANEL3, Theme::TEXT, 13, Theme::BORDER;
}

// ═══════════════════════════════════════════════════════════════════════════
// PATIENT MENU
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::initPatientMenu()
{
    const char* labels[PAT_BTN_COUNT] = {
        "My Profile",
        "Book Appointment",
        "Cancel Appointment",
        "View My Appointments",
        "View Medical Records",
        "View My Bills",
        "Pay Bill",
        "Deposit Balance",
        "Logout"
    };
    float bx = 190.f, by = 164.f, bw = 1080.f, bh = 46.f, gap = 10.f;
    for (int i = 0; i < PAT_BTN_COUNT; i++)
        patBtns[i].init(font, labels[i], bx, by + i * (bh + gap), bw, bh,
            Theme::PANEL2, Theme::PANEL3, Theme::TEXT, 13, Theme::BORDER);
}

// ═══════════════════════════════════════════════════════════════════════════
// FORMS
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::initForms()
{
    float fx = 210.f, fw = 520.f, fh = 42.f, gap = 68.f, topY = 110.f;

    // Add Doctor
    tbDocName.init(font, "e.g.  Ali Hassan", fx, topY, fw, fh);
    tbDocPass.init(font, "min 6 characters", fx, topY + gap, fw, fh, true);
    tbDocContact.init(font, "0300-1234567", fx, topY + gap * 2, fw, fh);
    tbDocSpec.init(font, "e.g.  Cardiology", fx, topY + gap * 3, fw, fh);
    tbDocFee.init(font, "e.g.  1500", fx, topY + gap * 4, fw, fh);
    btnDocSubmit.init(font, "Add Doctor",
        fx, topY + gap * 5 + 8.f, fw, 44.f,
        Theme::ACCENT_DARK, Theme::ACCENT_DIM, Theme::ACCENT, 13, Theme::ACCENT_DIM);

    // Add Patient
    tbPatName.init(font, "e.g.  Sara Khan", fx, topY, fw, fh);
    tbPatPass.init(font, "min 6 characters", fx, topY + gap, fw, fh, true);
    tbPatContact.init(font, "0300-1234567", fx, topY + gap * 2, fw, fh);
    tbPatAge.init(font, "e.g.  34", fx, topY + gap * 3, fw, fh);
    tbPatGender.init(font, "M  or  F", fx, topY + gap * 4, fw, fh);
    btnPatSubmit.init(font, "Add Patient",
        fx, topY + gap * 5 + 8.f, fw, 44.f,
        Theme::ACCENT_DARK, Theme::ACCENT_DIM, Theme::ACCENT, 13, Theme::ACCENT_DIM);

    // Generic single-field (below short list at y≈100..490)
    tbTargetID.init(font, "Enter ID here", fx, 514.f, fw, fh);
    btnConfirmAction.init(font, "Confirm Action",
        fx, 572.f, fw, 44.f,
        Theme::DANGER_DK, { 130,30,30 }, Theme::TEXT, 13, Theme::DANGER);

    // Book Appointment
    tbBookSpec.init(font, "e.g.  Cardiology", fx, topY, fw, fh);
    tbBookDocID.init(font, "Doctor ID number", fx, topY + gap, fw, fh);
    tbBookDate.init(font, "DD-MM-YYYY", fx, topY + gap * 2, fw, fh);
    tbBookSlot.init(font, "09:00  10:00 ... 16:00", fx, topY + gap * 3, fw, fh);
    btnBookSubmit.init(font, "Book Appointment",
        fx, topY + gap * 4 + 8.f, fw, 44.f,
        Theme::ACCENT_DARK, Theme::ACCENT_DIM, Theme::ACCENT, 13, Theme::ACCENT_DIM);

    // Prescription
    tbPresAppID.init(font, "Appointment ID", fx, topY, fw, fh);
    tbPresMeds.init(font, "Paracetamol 500mg ; Amox...", fx, topY + gap, fw, fh);
    tbPresNotes.init(font, "Advice / follow-up notes", fx, topY + gap * 2, fw, fh);
    btnPresSubmit.init(font, "Write Prescription",
        fx, topY + gap * 3 + 8.f, fw, 44.f,
        Theme::ACCENT_DARK, Theme::ACCENT_DIM, Theme::ACCENT, 13, Theme::ACCENT_DIM);

    // Pay Bill
    tbBillID.init(font, "Bill ID number", fx, 514.f, fw, fh);
    btnBillPay.init(font, "Pay This Bill",
        fx, 572.f, fw, 44.f,
        Theme::SUCCESS_DK, { 20,110,60 }, Theme::TEXT, 13, Theme::SUCCESS);

    // Deposit
    tbDepositAmt.init(font, "Amount in PKR", fx, 320.f, fw, fh);
    btnDepositSubmit.init(font, "Deposit Funds",
        fx, 378.f, fw, 44.f,
        Theme::SUCCESS_DK, { 20,110,60 }, Theme::TEXT, 13, Theme::SUCCESS);

    // Patient History
    tbHistPatID.init(font, "Patient ID", fx, topY, fw, fh);
    btnHistSearch.init(font, "Search History",
        fx, topY + 58.f, fw, 44.f,
        Theme::ACCENT_DARK, Theme::ACCENT_DIM, Theme::ACCENT, 13, Theme::ACCENT_DIM);

    // Cancel Appointment
    tbCancelAppID.init(font, "Appointment ID to Cancel", fx, 514.f, fw, fh);
    btnCancelSubmit.init(font, "Cancel Appointment",
        fx, 572.f, fw, 44.f,
        Theme::DANGER_DK, { 130,30,30 }, Theme::TEXT, 13, Theme::DANGER);
}

// ═══════════════════════════════════════════════════════════════════════════
// SCROLL LISTS
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::initScrollLists()
{
    mainList.init(font, 190.f, 100.f, 1080.f, 648.f, 38.f);
    subList.init(font, 190.f, 100.f, 1080.f, 400.f, 38.f);
}

// ═══════════════════════════════════════════════════════════════════════════
// UTILITY
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::goTo(Screen s) { state.previous = state.current; state.current = s; }
void HospitalApp::goBack() {
    if (state.loggedInRole == "admin")   goTo(Screen::ADMIN_MENU);
    else if (state.loggedInRole == "doctor")  goTo(Screen::DOCTOR_MENU);
    else if (state.loggedInRole == "patient") goTo(Screen::PATIENT_MENU);
    else                                     goTo(Screen::MAIN_LOGIN);
}
string HospitalApp::getTodayStr() {
    time_t now = time(0); tm* ltm = localtime(&now); char buf[20];
    strftime(buf, sizeof(buf), "%d-%m-%Y", ltm); return string(buf);
}
Admin* HospitalApp::currentAdmin() { return admins.findById(state.loggedInID); }
Doctor* HospitalApp::currentDoctor() { return doctors.findById(state.loggedInID); }
Patient* HospitalApp::currentPatient() { return patients.findById(state.loggedInID); }
void HospitalApp::syncAndSave() {
    FileHandler::savePatients("patients.txt", patients);
    FileHandler::saveDoctors("doctors.txt", doctors);
    FileHandler::saveAppointments("appointments.txt", appointments);
    FileHandler::saveBills("bills.txt", bills);
    FileHandler::savePrescriptions("prescriptions.txt", prescriptions);
}