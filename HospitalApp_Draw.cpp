#define _CRT_SECURE_NO_WARNINGS
#include "HospitalApp.h"
#include <sstream>
#include <iomanip>

// ═══════════════════════════════════════════════════════════════════════════
// INTERNAL DRAW HELPERS
// ═══════════════════════════════════════════════════════════════════════════

// Subtle diagonal-line grid for background depth
static void drawBgGrid(sf::RenderWindow& win)
{
    const sf::Color lc{ 55, 28, 36, 22 };
    for (float x = -780.f; x < 2060.f; x += 64.f) {
        sf::RectangleShape ln({ 1.f, 1260.f });
        ln.setFillColor(lc);
        ln.setPosition({ x, 0.f });
        ln.setRotation(sf::degrees(45.f));
        win.draw(ln);
    }
}

// Stat card with accent top stripe + value + label
static void drawStatCard(sf::RenderWindow& win, const sf::Font& font,
    float x, float y, float w, float h,
    const string& value, const string& label, sf::Color accent)
{
    // Shadow offset
    auto shadow = makePanel(x + 3.f, y + 3.f, w, h,
        { 0,0,0,60 }, { 0,0,0,0 }, 0.f);
    win.draw(shadow);

    auto card = makePanel(x, y, w, h, Theme::PANEL2, accent, 1.f);
    win.draw(card);

    // Accent top stripe
    auto stripe = makePanel(x, y, w, 4.f, accent, accent, 0.f);
    win.draw(stripe);

    // Inner subtle glow in top-left corner
    auto glow = makePanel(x, y, w / 3.f, h / 2.f,
        { accent.r,accent.g,accent.b,18 }, { 0,0,0,0 }, 0.f);
    win.draw(glow);

    auto vt = makeCentered(font, value, x + w / 2.f, y + h * 0.42f, accent, 26);
    win.draw(vt);
    auto lt = makeCentered(font, label, x + w / 2.f, y + h * 0.74f, Theme::TEXT_DIM, 11);
    win.draw(lt);
}

// Section header bar — peach left-accent stripe + label
static void drawSectionHeader(sf::RenderWindow& win, const sf::Font& font,
    float x, float y, float w, const string& title)
{
    auto bar = makePanel(x, y, w, 32.f, Theme::PANEL2, Theme::BORDER);
    win.draw(bar);
    auto accent = makePanel(x, y, 4.f, 32.f, Theme::ACCENT, Theme::ACCENT, 0.f);
    win.draw(accent);
    auto fade = makePanel(x + w - 80.f, y, 80.f, 32.f,
        { Theme::ACCENT.r,Theme::ACCENT.g,Theme::ACCENT.b,14 }, { 0,0,0,0 }, 0.f);
    win.draw(fade);
    auto txt = makeText(font, title, x + 16.f, y + 9.f, Theme::ACCENT, 11);
    win.draw(txt);
}

// Label + field pair
static void drawField(sf::RenderWindow& win, const sf::Font& font,
    const string& lbl, TextBox& tb)
{
    auto lt = makeText(font, lbl, tb.bx, tb.by - 18.f, Theme::TEXT_DIM, 11);
    win.draw(lt);
    tb.draw(win);
}

// ═══════════════════════════════════════════════════════════════════════════
// TOP BAR
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::drawTopBar(const string& title)
{
    auto bar = makePanel(0.f, 0.f, 1280.f, 56.f, Theme::PANEL2, Theme::BORDER);
    win.draw(bar);

    // Three-segment shimmer: dim | bright | dim
    win.draw(makePanel(0.f, 54.f, 400.f, 2.f, Theme::ACCENT_DARK, Theme::ACCENT_DARK, 0.f));
    win.draw(makePanel(400.f, 54.f, 480.f, 2.f, Theme::ACCENT, Theme::ACCENT, 0.f));
    win.draw(makePanel(880.f, 54.f, 400.f, 2.f, Theme::ACCENT_DARK, Theme::ACCENT_DARK, 0.f));

    // Brand
    win.draw(makeText(font, "MEDI", 16.f, 17.f, Theme::ACCENT, 19));
    win.draw(makeText(font, "CORE", 64.f, 17.f, Theme::TEXT, 19));
    win.draw(makeText(font, "+", 106.f, 14.f, Theme::ACCENT_DIM, 24));

    // Screen title — centred
    auto titleTxt = makeCentered(font, title, 640.f, 27.f, Theme::TEXT, 15);
    win.draw(titleTxt);

    // Date — right
    win.draw(makeText(font, getTodayStr(), 1148.f, 21.f, Theme::TEXT_DIM, 11));
}

// ═══════════════════════════════════════════════════════════════════════════
// SIDEBAR
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::drawSidebar(const string& role, const string& name)
{
    // Body
    auto sb = makePanel(0.f, 56.f, 180.f, 724.f, Theme::PANEL, Theme::BORDER);
    win.draw(sb);

    sf::Color rc = (role == "Admin") ? Theme::DANGER
        : (role == "Doctor") ? Theme::ACCENT
        : Theme::SUCCESS;

    // Role colour top stripe
    win.draw(makePanel(0.f, 56.f, 180.f, 4.f, rc, rc, 0.f));

    // Avatar circle
    sf::CircleShape av(28.f);
    av.setFillColor(Theme::PANEL2);
    av.setOutlineThickness(2.f);
    av.setOutlineColor(rc);
    av.setPosition({ 62.f, 76.f });
    win.draw(av);

    string ini = name.empty() ? "?" : string(1, (char)toupper((unsigned char)name[0]));
    win.draw(makeCentered(font, ini, 90.f, 104.f, rc, 20));
    win.draw(makeCentered(font, role, 90.f, 144.f, rc, 11));
    win.draw(makeCentered(font, name, 90.f, 162.f, Theme::TEXT, 12));

    // Divider
    win.draw(makePanel(14.f, 182.f, 152.f, 1.f, Theme::BORDER, Theme::BORDER, 0.f));

    // Decorative hint lines (visual richness in empty space)
    for (int i = 0; i < 5; i++) {
        float lw = (i % 2 == 0) ? 100.f : 64.f;
        win.draw(makePanel(14.f, 196.f + i * 20.f, lw, 1.f,
            { Theme::BORDER.r,Theme::BORDER.g,Theme::BORDER.b,60 },
            { 0,0,0,0 }, 0.f));
    }

    // Nav label + separator above back button
    win.draw(makeText(font, "NAVIGATION", 18.f, 664.f, Theme::TEXT_MUTED, 9));
    win.draw(makePanel(14.f, 678.f, 152.f, 1.f, Theme::BORDER, Theme::BORDER, 0.f));

    // Back / Logout button
    btnBack.moveTo(14.f, 686.f);
    btnBack.draw(win);
}

// ═══════════════════════════════════════════════════════════════════════════
// LOGIN SCREEN
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::drawLogin()
{
    drawBgGrid(win);

    // ── Card geometry ─────────────────────────────────────────────────────
    // fieldX=420, fieldW=440  (from initLoginScreen)
    // Card spans fieldX-30 .. fieldX+fieldW+30 → x=390, w=500
    // Fields: ID@280, Pass@342, Pills@404, Buttons@458
    // Card needs to contain all that + logo(80px) + footer(30px)
    // Total height: logo(80) + margin(16) + label(14) + ID(44) + gap(6)
    //             + label(14) + Pass(44) + gap(6) + label(14) + pills(36)
    //             + gap(10) + buttons(44) + footer(40) = ~368 → use 510
    float cx = 640.f;
    float cw = 500.f;
    float ch = 530.f;
    float cx0 = cx - cw / 2.f;   // 390
    float cy0 = 100.f;

    // Drop shadow
    win.draw(makePanel(cx0 + 4.f, cy0 + 4.f, cw, ch, { 0,0,0,80 }, { 0,0,0,0 }, 0.f));

    // Card body
    win.draw(makePanel(cx0, cy0, cw, ch, Theme::PANEL, Theme::BORDER));

    // Peach top stripe
    win.draw(makePanel(cx0, cy0, cw, 5.f, Theme::ACCENT, Theme::ACCENT, 0.f));

    // ── Logo area ─────────────────────────────────────────────────────────
    win.draw(makePanel(cx0, cy0, cw, 82.f, Theme::PANEL2, Theme::PANEL2, 0.f));

    // "+" icon left-aligned, brand centred
    win.draw(makeText(font, "+", cx0 + 18.f, cy0 + 16.f, Theme::ACCENT, 26));
    // MEDI + CORE centred together
    // Measure both to place them as a unit
    sf::Text tMedi(font, "MEDI", 26); sf::Text tCore(font, "CORE", 26);
    float mw = tMedi.getLocalBounds().size.x;
    float ow = tCore.getLocalBounds().size.x;
    float totalW = mw + ow + 2.f;
    float lx = cx - totalW / 2.f;
    auto tM = makeText(font, "MEDI", lx, cy0 + 18.f, Theme::ACCENT, 26);
    auto tC = makeText(font, "CORE", lx + mw + 2.f, cy0 + 18.f, Theme::TEXT, 26);
    win.draw(tM); win.draw(tC);
    win.draw(makeCentered(font, "Hospital Management System",
        cx, cy0 + 58.f, Theme::TEXT_DIM, 11));

    // Divider below logo
    win.draw(makePanel(cx0 + 20.f, cy0 + 82.f, cw - 40.f, 1.f,
        Theme::BORDER, Theme::BORDER, 0.f));

    // ── Fields — labels drawn 16px above each field ───────────────────────
    // ID field is at y=280 (from init), label at 280-16=264
    // Pass field at y=342, label at 326
    // Pills  at y=404, label at 388
    // Buttons at y=458
    win.draw(makeText(font, "ID", tbLoginID.bx, tbLoginID.by - 16.f, Theme::TEXT_DIM, 11));
    win.draw(makeText(font, "PASSWORD", tbLoginPass.bx, tbLoginPass.by - 16.f, Theme::TEXT_DIM, 11));
    win.draw(makeText(font, "ROLE", btnLoginAdmin.shape.getPosition().x,
        btnLoginAdmin.shape.getPosition().y - 16.f, Theme::TEXT_DIM, 11));

    tbLoginID.draw(win);
    tbLoginPass.draw(win);

    // Role pills with active highlight
    auto hilite = [&](Button& btn, int role) {
        if (loginRole == role) {
            btn.shape.setOutlineColor(Theme::ACCENT);
            btn.shape.setOutlineThickness(2.f);
            btn.shape.setFillColor(Theme::ACCENT_DARK);
            btn.label.setFillColor(Theme::ACCENT);
        }
        else {
            btn.shape.setOutlineColor(Theme::BORDER);
            btn.shape.setOutlineThickness(1.f);
            btn.shape.setFillColor(Theme::PANEL2);
            btn.label.setFillColor(Theme::TEXT_DIM);
        }
        btn.draw(win);
        };
    hilite(btnLoginAdmin, 1);
    hilite(btnLoginDoctor, 2);
    hilite(btnLoginPatient, 3);

    btnLoginGo.draw(win);
    btnExit.draw(win);

    // ── Footer ────────────────────────────────────────────────────────────
    float footerY = cy0 + ch - 28.f;
    win.draw(makePanel(cx0 + 20.f, footerY - 6.f, cw - 40.f, 1.f,
        Theme::BORDER, Theme::BORDER, 0.f));
    // Use ASCII only — avoid any encoding issue with (c)
    win.draw(makeCentered(font, "(c) 2025 MediCore  -  Secure Health Management",
        cx, footerY + 6.f, Theme::TEXT_MUTED, 10));
}

// ═══════════════════════════════════════════════════════════════════════════
// ADMIN MENU
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::drawAdminMenu()
{
    drawBgGrid(win);
    drawTopBar("Admin Dashboard");
    drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "Admin");

    // Stat cards row
    int unpaid = 0;
    for (int i = 0;i < bills.getCount();i++)
        if (bills.getAT(i)->getStatus() == "unpaid") unpaid++;

    // Stat cards: topbar=56, gap=8 → cards at y=68, height=74 → bottom=142
    // Section header at y=152, buttons from y=192
    float cy = 68.f, cw = 258.f, ch = 74.f, cgap = 8.f, cx0 = 190.f;
    drawStatCard(win, font, cx0, cy, cw, ch, to_string(doctors.getCount()), "Doctors", Theme::ACCENT);
    drawStatCard(win, font, cx0 + cw + cgap, cy, cw, ch, to_string(patients.getCount()), "Patients", Theme::SUCCESS);
    drawStatCard(win, font, cx0 + (cw + cgap) * 2, cy, cw, ch, to_string(appointments.getCount()), "Appointments", Theme::WARNING);
    drawStatCard(win, font, cx0 + (cw + cgap) * 3, cy, cw, ch, to_string(unpaid), "Unpaid Bills", Theme::DANGER);

    drawSectionHeader(win, font, 190.f, 152.f, 1080.f, "QUICK ACTIONS");

    const sf::Color accents[ADMIN_BTN_COUNT] = {
        Theme::PANEL2, Theme::ACCENT_DARK, Theme::ACCENT_DARK,
        Theme::DANGER_DK,  Theme::PANEL2, Theme::PANEL2,
        Theme::PANEL2, Theme::WARNING_DK,  Theme::DANGER_DK,
        Theme::PANEL2, Theme::PANEL2
    };
    for (int i = 0;i < ADMIN_BTN_COUNT;i++) {
        adminBtns[i].normalColor = accents[i];
        adminBtns[i].shape.setFillColor(
            adminBtns[i].isHovered ? adminBtns[i].hoverColor : accents[i]);
        adminBtns[i].draw(win);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// DOCTOR MENU
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::drawDoctorMenu()
{
    drawBgGrid(win);
    drawTopBar("Doctor Dashboard");
    drawSidebar("Doctor", currentDoctor() ? currentDoctor()->getName() : "Doctor");

    Doctor* d = currentDoctor();
    if (d) {
        win.draw(makePanel(190.f, 68.f, 1080.f, 60.f, Theme::PANEL2, Theme::BORDER));
        win.draw(makePanel(190.f, 68.f, 4.f, 60.f, Theme::ACCENT, Theme::ACCENT, 0.f));
        win.draw(makeText(font, "Specialization:  " + d->getSpec(),
            208.f, 80.f, Theme::TEXT_DIM, 12));
        win.draw(makeText(font, "Fee:  Rs." + to_string((int)d->getfee()),
            208.f, 100.f, Theme::SUCCESS, 12));
        win.draw(makeText(font, "Contact:  " + d->getContact(),
            480.f, 90.f, Theme::TEXT_DIM, 12));
    }

    drawSectionHeader(win, font, 190.f, 138.f, 1080.f, "DOCTOR ACTIONS");

    for (int i = 0;i < DOC_BTN_COUNT;i++) {
        docBtns[i].normalColor = (i == DOC_BTN_COUNT - 1) ? Theme::DANGER_DK : Theme::PANEL2;
        docBtns[i].draw(win);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// PATIENT MENU
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::drawPatientMenu()
{
    drawBgGrid(win);
    drawTopBar("Patient Dashboard");
    drawSidebar("Patient", currentPatient() ? currentPatient()->getName() : "Patient");

    Patient* p = currentPatient();
    if (p) {
        ostringstream oss; oss << fixed << setprecision(2) << p->getBalance();
        win.draw(makePanel(190.f, 68.f, 1080.f, 60.f, Theme::PANEL2, Theme::BORDER));
        win.draw(makePanel(190.f, 68.f, 4.f, 60.f, Theme::SUCCESS, Theme::SUCCESS, 0.f));
        win.draw(makeText(font, "Age:  " + to_string(p->getAge()),
            208.f, 80.f, Theme::TEXT_DIM, 12));
        win.draw(makeText(font, "Gender:  " + p->getGender(),
            208.f, 100.f, Theme::TEXT_DIM, 12));
        win.draw(makeText(font, "Balance:  PKR " + oss.str(),
            480.f, 90.f, Theme::SUCCESS, 13));
    }

    drawSectionHeader(win, font, 190.f, 138.f, 1080.f, "PATIENT ACTIONS");

    for (int i = 0;i < PAT_BTN_COUNT;i++) {
        patBtns[i].normalColor = (i == PAT_BTN_COUNT - 1) ? Theme::DANGER_DK : Theme::PANEL2;
        patBtns[i].draw(win);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// REPORT SCREEN
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::drawReportScreen(const string&)
{
    win.draw(makePanel(180.f, 64.f, 1080.f, 686.f, Theme::PANEL, Theme::BORDER));
    // Left accent bar for report panel
    win.draw(makePanel(180.f, 64.f, 4.f, 686.f, Theme::ACCENT, Theme::ACCENT, 0.f));

    float ry = 82.f, lineH = 21.f, maxY = 736.f;
    for (auto& line : reportLines) {
        if (ry > maxY) break;
        sf::Color c = Theme::TEXT; unsigned int sz = 13;
        if (line.find("═══") != string::npos || line.find("───") != string::npos)
        {
            c = Theme::BORDER; sz = 11;
        }
        else if (line.find("REPORT") != string::npos || line.find("LOG") != string::npos
            || line.find("PROFILE") != string::npos)
        {
            c = Theme::ACCENT; sz = 16;
        }
        else if (line.find("Rs.") != string::npos || line.find("PKR") != string::npos)
            c = Theme::SUCCESS;
        else if (line.find("OVERDUE") != string::npos || line.find("FAIL") != string::npos)
            c = Theme::DANGER;
        win.draw(makeText(font, line, 200.f, ry, c, sz));
        ry += lineH;
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// SCROLL SCREEN
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::drawScrollScreen(const string& title)
{
    drawTopBar(title);
    mainList.draw(win);
}

// ═══════════════════════════════════════════════════════════════════════════
// MAIN DRAW DISPATCHER
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::draw()
{
    win.clear(Theme::BG);

    switch (state.current)
    {
        // ── LOGIN ───────────────────────────────────────────────────────────
    case Screen::MAIN_LOGIN:
        drawLogin();
        break;

        // ── ADMIN ───────────────────────────────────────────────────────────
    case Screen::ADMIN_MENU:
        drawAdminMenu();
        break;

    case Screen::ADMIN_ADD_DOCTOR:
        drawBgGrid(win);
        drawTopBar("Add New Doctor");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "REGISTER NEW DOCTOR");
        for (auto& [lbl, tb] : vector<pair<string, TextBox*>>{
            {"Doctor Name",             &tbDocName},
            {"Password  (min 6 chars)", &tbDocPass},
            {"Contact  (0000-0000000)", &tbDocContact},
            {"Specialization",          &tbDocSpec},
            {"Consultation Fee  (Rs)",  &tbDocFee} })
            drawField(win, font, lbl, *tb);
        btnDocSubmit.draw(win);
        break;

    case Screen::ADMIN_ADD_PATIENT:
        drawBgGrid(win);
        drawTopBar("Add New Patient");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "REGISTER NEW PATIENT");
        for (auto& [lbl, tb] : vector<pair<string, TextBox*>>{
            {"Patient Name",            &tbPatName},
            {"Password  (min 6 chars)", &tbPatPass},
            {"Contact  (0000-0000000)", &tbPatContact},
            {"Age",                     &tbPatAge},
            {"Gender  (M / F)",         &tbPatGender} })
            drawField(win, font, lbl, *tb);
        btnPatSubmit.draw(win);
        break;

    case Screen::ADMIN_REMOVE_DOCTOR:
        drawBgGrid(win);
        drawTopBar("Remove Doctor");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "REMOVE DOCTOR");
        mainList.draw(win);
        win.draw(makeText(font, "Enter Doctor ID to Remove:",
            tbTargetID.bx, tbTargetID.by - 18.f, Theme::TEXT_DIM, 11));
        tbTargetID.draw(win); btnConfirmAction.draw(win);
        break;

    case Screen::ADMIN_DISCHARGE:
        drawBgGrid(win);
        drawTopBar("Discharge Patient");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "DISCHARGE PATIENT");
        mainList.draw(win);
        win.draw(makeText(font, "Enter Patient ID to Discharge:",
            tbTargetID.bx, tbTargetID.by - 18.f, Theme::TEXT_DIM, 11));
        tbTargetID.draw(win); btnConfirmAction.draw(win);
        break;

    case Screen::ADMIN_VIEW_DOCTORS:
        drawBgGrid(win);
        drawTopBar("All Registered Doctors");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "DOCTORS");
        mainList.draw(win); break;

    case Screen::ADMIN_VIEW_PATIENTS:
        drawBgGrid(win);
        drawTopBar("All Registered Patients");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "PATIENTS");
        mainList.draw(win); break;

    case Screen::ADMIN_VIEW_APPOINTMENTS:
        drawBgGrid(win);
        drawTopBar("All Appointments");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "APPOINTMENTS");
        mainList.draw(win); break;

    case Screen::ADMIN_VIEW_BILLS:
        drawBgGrid(win);
        drawTopBar("Unpaid Bills");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "UNPAID BILLS");
        mainList.draw(win); break;

    case Screen::ADMIN_DAILY_REPORT:
        drawBgGrid(win);
        drawTopBar("Daily Report");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawReportScreen("Daily Report"); break;

    case Screen::ADMIN_SECURITY_LOG:
        drawBgGrid(win);
        drawTopBar("Security Log");
        drawSidebar("Admin", currentAdmin() ? currentAdmin()->getName() : "");
        drawReportScreen("Security Log"); break;

        // ── DOCTOR ──────────────────────────────────────────────────────────
    case Screen::DOCTOR_MENU:
        drawDoctorMenu(); break;

    case Screen::DOCTOR_VIEW_TODAY:
        drawBgGrid(win);
        drawTopBar("Today's Appointments");
        drawSidebar("Doctor", currentDoctor() ? currentDoctor()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "TODAY'S SCHEDULE");
        mainList.draw(win); break;

    case Screen::DOCTOR_MARK_COMPLETE:
        drawBgGrid(win);
        drawTopBar("Mark Appointment Complete");
        drawSidebar("Doctor", currentDoctor() ? currentDoctor()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "MARK COMPLETE");
        mainList.draw(win);
        win.draw(makeText(font, "Enter Appointment ID:",
            tbTargetID.bx, tbTargetID.by - 18.f, Theme::TEXT_DIM, 11));
        tbTargetID.draw(win); btnConfirmAction.draw(win); break;

    case Screen::DOCTOR_MARK_NOSHOW:
        drawBgGrid(win);
        drawTopBar("Mark Appointment No-Show");
        drawSidebar("Doctor", currentDoctor() ? currentDoctor()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "MARK NO-SHOW");
        mainList.draw(win);
        win.draw(makeText(font, "Enter Appointment ID:",
            tbTargetID.bx, tbTargetID.by - 18.f, Theme::TEXT_DIM, 11));
        tbTargetID.draw(win); btnConfirmAction.draw(win); break;

    case Screen::DOCTOR_WRITE_PRESCRIPTION:
        drawBgGrid(win);
        drawTopBar("Write Prescription");
        drawSidebar("Doctor", currentDoctor() ? currentDoctor()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "NEW PRESCRIPTION");
        for (auto& [lbl, tb] : vector<pair<string, TextBox*>>{
            {"Appointment ID  (must be completed)", &tbPresAppID},
            {"Medicines  (use ; to separate)",       &tbPresMeds},
            {"Notes / Advice",                       &tbPresNotes} })
            drawField(win, font, lbl, *tb);
        btnPresSubmit.draw(win); break;

    case Screen::DOCTOR_PATIENT_HISTORY:
        drawBgGrid(win);
        drawTopBar("Patient History");
        drawSidebar("Doctor", currentDoctor() ? currentDoctor()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "PATIENT HISTORY");
        win.draw(makeText(font, "Patient ID:",
            tbHistPatID.bx, tbHistPatID.by - 18.f, Theme::TEXT_DIM, 11));
        tbHistPatID.draw(win); btnHistSearch.draw(win);
        mainList.draw(win); break;

        // ── PATIENT ─────────────────────────────────────────────────────────
    case Screen::PATIENT_MENU:
        drawPatientMenu(); break;

    case Screen::PATIENT_BOOK_APPOINTMENT:
    {
        drawBgGrid(win);
        drawTopBar("Book Appointment");
        drawSidebar("Patient", currentPatient() ? currentPatient()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "BOOK APPOINTMENT");

        // Doctor hint panel (right side)
        string sf = tbBookSpec.value;
        if (!sf.empty()) {
            vector<string> hints;
            for (int i = 0;i < doctors.getCount();i++) {
                Doctor* d = doctors.getAT(i);
                string sl = d->getSpec(), fl = sf;
                for (char& c : sl) c = (char)tolower((unsigned char)c);
                for (char& c : fl) c = (char)tolower((unsigned char)c);
                if (sl.find(fl) != string::npos)
                    hints.push_back("  ID:" + d->getID() + "  Dr." + d->getName() +
                        "  (" + d->getSpec() + ")  Rs." + to_string((int)d->getfee()));
            }
            float hx = 760.f, hy = 100.f;
            win.draw(makeText(font, "MATCHING DOCTORS", hx, hy, Theme::ACCENT, 11));
            int show = min((int)hints.size(), 6);
            win.draw(makePanel(hx - 4.f, hy + 16.f, 500.f,
                (float)show * 22.f + 8.f, Theme::PANEL2, Theme::BORDER));
            for (int i = 0;i < show;i++)
                win.draw(makeText(font, hints[i], hx + 4.f, hy + 20.f + i * 22.f,
                    Theme::TEXT, 12));
        }

        Patient* pat = currentPatient();
        if (pat) {
            ostringstream oss; oss << fixed << setprecision(2) << pat->getBalance();
            win.draw(makeText(font, "Balance:  PKR " + oss.str(),
                760.f, 420.f, Theme::SUCCESS, 13));
        }

        for (auto& [lbl, tb] : vector<pair<string, TextBox*>>{
            {"Specialization  (filter doctors)",     &tbBookSpec},
            {"Doctor ID",                            &tbBookDocID},
            {"Date  (DD-MM-YYYY)",                   &tbBookDate},
            {"Time Slot  (09:00 to 16:00, on hour)",&tbBookSlot} })
            drawField(win, font, lbl, *tb);
        btnBookSubmit.draw(win);
        break;
    }

    case Screen::PATIENT_CANCEL_APPOINTMENT:
        drawBgGrid(win);
        drawTopBar("Cancel Appointment");
        drawSidebar("Patient", currentPatient() ? currentPatient()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "MY APPOINTMENTS");
        mainList.draw(win);
        win.draw(makeText(font, "Appointment ID to Cancel:",
            tbCancelAppID.bx, tbCancelAppID.by - 18.f, Theme::TEXT_DIM, 11));
        tbCancelAppID.draw(win); btnCancelSubmit.draw(win); break;

    case Screen::PATIENT_VIEW_APPOINTMENTS:
        drawBgGrid(win);
        drawTopBar("My Appointments");
        drawSidebar("Patient", currentPatient() ? currentPatient()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "MY APPOINTMENTS");
        mainList.draw(win); break;

    case Screen::PATIENT_MEDICAL_RECORDS:
        drawBgGrid(win);
        drawTopBar("Medical Records");
        drawSidebar("Patient", currentPatient() ? currentPatient()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "MEDICAL RECORDS");
        mainList.draw(win); break;

    case Screen::PATIENT_VIEW_BILLS:
        drawBgGrid(win);
        drawTopBar("My Bills");
        drawSidebar("Patient", currentPatient() ? currentPatient()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "BILLING HISTORY");
        mainList.draw(win); break;

    case Screen::PATIENT_PAY_BILLS:
        drawBgGrid(win);
        drawTopBar("Pay Bills");
        drawSidebar("Patient", currentPatient() ? currentPatient()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "PAY BILL");
        mainList.draw(win);
        win.draw(makeText(font, "Bill ID to Pay:",
            tbBillID.bx, tbBillID.by - 18.f, Theme::TEXT_DIM, 11));
        tbBillID.draw(win); btnBillPay.draw(win); break;

    case Screen::PATIENT_DEPOSIT:
    {
        drawBgGrid(win);
        drawTopBar("Deposit Balance");
        drawSidebar("Patient", currentPatient() ? currentPatient()->getName() : "");
        drawSectionHeader(win, font, 190.f, 64.f, 1080.f, "DEPOSIT FUNDS");
        Patient* pat = currentPatient();
        if (pat) {
            ostringstream oss; oss << fixed << setprecision(2) << pat->getBalance();
            win.draw(makePanel(tbDepositAmt.bx, tbDepositAmt.by - 56.f,
                tbDepositAmt.bw, 38.f, Theme::PANEL2, Theme::BORDER));
            win.draw(makeText(font, "Current Balance:  PKR " + oss.str(),
                tbDepositAmt.bx + 12.f, tbDepositAmt.by - 44.f,
                Theme::SUCCESS, 13));
        }
        win.draw(makeText(font, "Amount to Deposit  (PKR):",
            tbDepositAmt.bx, tbDepositAmt.by - 18.f, Theme::TEXT_DIM, 11));
        tbDepositAmt.draw(win); btnDepositSubmit.draw(win);
        break;
    }

    default: break;
    }

    // ── Top-bar mini back pill on all sub-screens ─────────────────────────
    bool isMenu = (state.current == Screen::MAIN_LOGIN ||
        state.current == Screen::ADMIN_MENU ||
        state.current == Screen::DOCTOR_MENU ||
        state.current == Screen::PATIENT_MENU);
    if (!isMenu) {
        // Sidebar already drew btnBack — also draw a small top-bar pill
        sf::RectangleShape pill({ 88.f,26.f });
        pill.setPosition({ 134.f,15.f });
        pill.setFillColor(Theme::ACCENT_DARK);
        pill.setOutlineThickness(1.f);
        pill.setOutlineColor(Theme::ACCENT_DIM);
        win.draw(pill);
        win.draw(makeCentered(font, "<- Menu", 178.f, 28.f, Theme::ACCENT, 11));
    }

    toast.draw(win);
    win.display();
}