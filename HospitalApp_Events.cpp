#define _CRT_SECURE_NO_WARNINGS
#include "HospitalApp.h"

// ═══════════════════════════════════════════════════════════════════════════
// MAIN RUN LOOP
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::run()
{
    while (win.isOpen()) {
        handleEvents();
        draw();
    }
    syncAndSave();
}

// ═══════════════════════════════════════════════════════════════════════════
// HANDLE EVENTS — DISPATCHER
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::handleEvents()
{
    while (auto event = win.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            syncAndSave();
            win.close();
            return;
        }

        sf::Vector2f mp = win.mapPixelToCoords(
            sf::Mouse::getPosition(win));

        // Global back button (non-menu screens)
        bool isMenu = (state.current == Screen::MAIN_LOGIN ||
            state.current == Screen::ADMIN_MENU ||
            state.current == Screen::DOCTOR_MENU ||
            state.current == Screen::PATIENT_MENU);

        if (!isMenu) {
            btnBack.handleHover(mp);
            if (const auto* me = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (me->button == sf::Mouse::Button::Left && btnBack.contains(mp)) {
                    // Go back to the appropriate menu
                    if (state.loggedInRole == "admin")   goTo(Screen::ADMIN_MENU);
                    else if (state.loggedInRole == "doctor")  goTo(Screen::DOCTOR_MENU);
                    else if (state.loggedInRole == "patient") goTo(Screen::PATIENT_MENU);
                    else goTo(Screen::MAIN_LOGIN);
                    syncAndSave();
                    return;
                }
            }
        }

        // Mouse wheel for scroll lists
        if (const auto* mw = event->getIf<sf::Event::MouseWheelScrolled>()) {
            mainList.handleScroll(mw->delta);
            subList.handleScroll(mw->delta);
        }

        switch (state.current) {
        case Screen::MAIN_LOGIN:
            handleLoginEvents(*event, mp); break;

        case Screen::ADMIN_MENU:
            handleAdminMenuEvents(*event, mp); break;

        case Screen::DOCTOR_MENU:
            handleDoctorMenuEvents(*event, mp); break;

        case Screen::PATIENT_MENU:
            handlePatientMenuEvents(*event, mp); break;

        default:
            handleFormEvents(*event, mp); break;
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// LOGIN EVENTS
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::handleLoginEvents(const sf::Event& e, sf::Vector2f mp)
{
    // Hover
    btnLoginAdmin.handleHover(mp);
    btnLoginDoctor.handleHover(mp);
    btnLoginPatient.handleHover(mp);
    btnLoginGo.handleHover(mp);
    btnExit.handleHover(mp);

    // Text input
    tbLoginID.handleEvent(e);
    tbLoginPass.handleEvent(e);

    if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
        if (me->button != sf::Mouse::Button::Left) return;

        tbLoginID.handleClick(mp);
        tbLoginPass.handleClick(mp);

        if (btnLoginAdmin.contains(mp))   loginRole = 1;
        if (btnLoginDoctor.contains(mp))  loginRole = 2;
        if (btnLoginPatient.contains(mp)) loginRole = 3;
        if (btnLoginGo.contains(mp))      doLogin();
        if (btnExit.contains(mp)) { syncAndSave(); win.close(); }
    }

    if (const auto* ke = e.getIf<sf::Event::KeyPressed>()) {
        if (ke->code == sf::Keyboard::Key::Enter) doLogin();
        if (ke->code == sf::Keyboard::Key::Tab) {
            if (tbLoginID.focused) {
                tbLoginID.focused = false;
                tbLoginID.shape.setOutlineColor(Theme::BORDER);
                tbLoginPass.focused = true;
                tbLoginPass.shape.setOutlineColor(Theme::ACCENT);
            }
            else {
                tbLoginPass.focused = false;
                tbLoginPass.shape.setOutlineColor(Theme::BORDER);
                tbLoginID.focused = true;
                tbLoginID.shape.setOutlineColor(Theme::ACCENT);
            }
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// ADMIN MENU EVENTS
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::handleAdminMenuEvents(const sf::Event& e, sf::Vector2f mp)
{
    for (auto& b : adminBtns) b.handleHover(mp);
    btnBack.handleHover(mp);

    if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
        if (me->button != sf::Mouse::Button::Left) return;

        // Logout
        if (btnBack.contains(mp)) {
            syncAndSave();
            state.loggedInID = "";
            state.loggedInRole = "";
            loginRole = 0;
            goTo(Screen::MAIN_LOGIN);
            toast.show("Logged out successfully.", Theme::SUCCESS);
            return;
        }

        if (adminBtns[0].contains(mp)) {
            // Profile — show in report screen
            Admin* a = currentAdmin();
            if (a) {
                reportLines.clear();
                reportLines.push_back("ADMIN PROFILE");
                reportLines.push_back("════════════════════");
                reportLines.push_back("ID:      " + a->getID());
                reportLines.push_back("Name:    " + a->getName());
                reportLines.push_back("Contact: " + a->getContact());
            }
            goTo(Screen::ADMIN_SECURITY_LOG); // reuse report screen
        }
        else if (adminBtns[1].contains(mp)) {
            tbDocName.clear(); tbDocPass.clear(); tbDocContact.clear();
            tbDocSpec.clear(); tbDocFee.clear();
            goTo(Screen::ADMIN_ADD_DOCTOR);
        }
        else if (adminBtns[2].contains(mp)) {
            tbPatName.clear(); tbPatPass.clear(); tbPatContact.clear();
            tbPatAge.clear(); tbPatGender.clear();
            goTo(Screen::ADMIN_ADD_PATIENT);
        }
        else if (adminBtns[3].contains(mp)) {
            tbTargetID.clear();
            doViewDoctors();
            goTo(Screen::ADMIN_REMOVE_DOCTOR);
        }
        else if (adminBtns[4].contains(mp)) { doViewDoctors();      goTo(Screen::ADMIN_VIEW_DOCTORS); }
        else if (adminBtns[5].contains(mp)) { doViewPatients();     goTo(Screen::ADMIN_VIEW_PATIENTS); }
        else if (adminBtns[6].contains(mp)) { doViewAppointments(); goTo(Screen::ADMIN_VIEW_APPOINTMENTS); }
        else if (adminBtns[7].contains(mp)) { doViewUnpaidBills();  goTo(Screen::ADMIN_VIEW_BILLS); }
        else if (adminBtns[8].contains(mp)) { tbTargetID.clear();   goTo(Screen::ADMIN_DISCHARGE); }
        else if (adminBtns[9].contains(mp)) { doDailyReport();      goTo(Screen::ADMIN_DAILY_REPORT); }
        else if (adminBtns[10].contains(mp)) { doSecurityLog();      goTo(Screen::ADMIN_SECURITY_LOG); }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// DOCTOR MENU EVENTS
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::handleDoctorMenuEvents(const sf::Event& e, sf::Vector2f mp)
{
    for (auto& b : docBtns) b.handleHover(mp);
    btnBack.handleHover(mp);

    if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
        if (me->button != sf::Mouse::Button::Left) return;

        if (btnBack.contains(mp) || docBtns[DOC_BTN_COUNT - 1].contains(mp)) {
            syncAndSave();
            state.loggedInID = "";
            state.loggedInRole = "";
            loginRole = 0;
            goTo(Screen::MAIN_LOGIN);
            toast.show("Logged out.", Theme::SUCCESS);
            return;
        }

        if (docBtns[0].contains(mp)) {
            Doctor* d = currentDoctor();
            if (d) {
                reportLines.clear();
                reportLines.push_back("DOCTOR PROFILE");
                reportLines.push_back("════════════════════");
                reportLines.push_back("ID:             " + d->getID());
                reportLines.push_back("Name:           " + d->getName());
                reportLines.push_back("Specialization: " + d->getSpec());
                reportLines.push_back("Contact:        " + d->getContact());
                reportLines.push_back("Fee:            Rs." + to_string((int)d->getfee()));
            }
            goTo(Screen::ADMIN_DAILY_REPORT); // reuse report screen
        }
        else if (docBtns[1].contains(mp)) { doViewTodayAppointments(); goTo(Screen::DOCTOR_VIEW_TODAY); }
        else if (docBtns[2].contains(mp)) { tbTargetID.clear();         goTo(Screen::DOCTOR_MARK_COMPLETE); }
        else if (docBtns[3].contains(mp)) { tbTargetID.clear();         goTo(Screen::DOCTOR_MARK_NOSHOW); }
        else if (docBtns[4].contains(mp)) {
            tbPresAppID.clear(); tbPresMeds.clear(); tbPresNotes.clear();
            goTo(Screen::DOCTOR_WRITE_PRESCRIPTION);
        }
        else if (docBtns[5].contains(mp)) { tbHistPatID.clear(); mainList.setItems({}); goTo(Screen::DOCTOR_PATIENT_HISTORY); }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// PATIENT MENU EVENTS
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::handlePatientMenuEvents(const sf::Event& e, sf::Vector2f mp)
{
    for (auto& b : patBtns) b.handleHover(mp);

    if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
        if (me->button != sf::Mouse::Button::Left) return;

        if (patBtns[PAT_BTN_COUNT - 1].contains(mp)) {
            syncAndSave();
            state.loggedInID = "";
            state.loggedInRole = "";
            loginRole = 0;
            goTo(Screen::MAIN_LOGIN);
            toast.show("Logged out.", Theme::SUCCESS);
            return;
        }

        if (patBtns[0].contains(mp)) {
            Patient* p = currentPatient();
            if (p) {
                reportLines.clear();
                reportLines.push_back("PATIENT PROFILE");
                reportLines.push_back("════════════════════");
                reportLines.push_back("ID:      " + p->getID());
                reportLines.push_back("Name:    " + p->getName());
                reportLines.push_back("Age:     " + to_string(p->getAge()));
                reportLines.push_back("Gender:  " + p->getGender());
                reportLines.push_back("Contact: " + p->getContact());
                ostringstream oss;
                oss << fixed << setprecision(2) << p->getBalance();
                reportLines.push_back("Balance: PKR " + oss.str());
            }
            goTo(Screen::ADMIN_DAILY_REPORT);
        }
        else if (patBtns[1].contains(mp)) {
            tbBookSpec.clear(); tbBookDocID.clear(); tbBookDate.clear(); tbBookSlot.clear();
            goTo(Screen::PATIENT_BOOK_APPOINTMENT);
        }
        else if (patBtns[2].contains(mp)) {
            doViewMyAppointments();
            tbCancelAppID.clear();
            // Reposition cancel widgets
            tbCancelAppID.bx = 400.f; tbCancelAppID.by = 680.f;
            tbCancelAppID.shape.setPosition({ 400.f, 680.f });
            tbCancelAppID.display.setPosition({ 412.f, 690.f });
            btnCancelSubmit.shape.setPosition({ 400.f, 735.f });
            sf::FloatRect lb = btnCancelSubmit.label.getLocalBounds();
            btnCancelSubmit.label.setPosition({
                400.f + (480.f - lb.size.x) / 2.f - lb.position.x,
                735.f + (46.f - lb.size.y) / 2.f - lb.position.y
                });
            // Shrink list to fit
            mainList.lh = 560.f;
            goTo(Screen::PATIENT_CANCEL_APPOINTMENT);
        }
        else if (patBtns[3].contains(mp)) { doViewMyAppointments(); mainList.lh = 670.f; goTo(Screen::PATIENT_VIEW_APPOINTMENTS); }
        else if (patBtns[4].contains(mp)) { doViewMedicalRecords(); mainList.lh = 670.f; goTo(Screen::PATIENT_MEDICAL_RECORDS); }
        else if (patBtns[5].contains(mp)) { doViewMyBills();        mainList.lh = 560.f; goTo(Screen::PATIENT_VIEW_BILLS); }
        else if (patBtns[6].contains(mp)) {
            doViewMyBills();
            tbBillID.clear();
            tbBillID.shape.setPosition({ 400.f, 680.f });
            tbBillID.display.setPosition({ 412.f, 690.f });
            tbBillID.bx = 400.f; tbBillID.by = 680.f;
            btnBillPay.shape.setPosition({ 400.f, 735.f });
            sf::FloatRect lb = btnBillPay.label.getLocalBounds();
            btnBillPay.label.setPosition({
                400.f + (480.f - lb.size.x) / 2.f - lb.position.x,
                735.f + (46.f - lb.size.y) / 2.f - lb.position.y
                });
            mainList.lh = 560.f;
            goTo(Screen::PATIENT_PAY_BILLS);
        }
        else if (patBtns[7].contains(mp)) { tbDepositAmt.clear(); goTo(Screen::PATIENT_DEPOSIT); }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// FORM EVENTS (all sub-screens with text boxes)
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::handleFormEvents(const sf::Event& e, sf::Vector2f mp)
{
    // Route text input to the right text boxes
    auto routeText = [&](vector<TextBox*> boxes) {
        for (auto* tb : boxes) tb->handleEvent(e);
        };

    // Hover buttons
    btnDocSubmit.handleHover(mp);
    btnPatSubmit.handleHover(mp);
    btnConfirmAction.handleHover(mp);
    btnBookSubmit.handleHover(mp);
    btnPresSubmit.handleHover(mp);
    btnBillPay.handleHover(mp);
    btnDepositSubmit.handleHover(mp);
    btnHistSearch.handleHover(mp);
    btnCancelSubmit.handleHover(mp);

    switch (state.current) {

        // ── Add Doctor ────────────────────────────────────────────────────────
    case Screen::ADMIN_ADD_DOCTOR:
        routeText({ &tbDocName, &tbDocPass, &tbDocContact, &tbDocSpec, &tbDocFee });
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbDocName.handleClick(mp);    tbDocPass.handleClick(mp);
                tbDocContact.handleClick(mp); tbDocSpec.handleClick(mp);
                tbDocFee.handleClick(mp);
                if (btnDocSubmit.contains(mp)) doAddDoctor();
            }
        }
        break;

        // ── Add Patient ───────────────────────────────────────────────────────
    case Screen::ADMIN_ADD_PATIENT:
        routeText({ &tbPatName, &tbPatPass, &tbPatContact, &tbPatAge, &tbPatGender });
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbPatName.handleClick(mp);    tbPatPass.handleClick(mp);
                tbPatContact.handleClick(mp); tbPatAge.handleClick(mp);
                tbPatGender.handleClick(mp);
                if (btnPatSubmit.contains(mp)) doAddPatient();
            }
        }
        break;

        // ── Remove Doctor ─────────────────────────────────────────────────────
    case Screen::ADMIN_REMOVE_DOCTOR:
        tbTargetID.handleEvent(e);
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbTargetID.handleClick(mp);
                if (btnConfirmAction.contains(mp)) doRemoveDoctor();
            }
        }
        break;

        // ── Discharge Patient ─────────────────────────────────────────────────
    case Screen::ADMIN_DISCHARGE:
        tbTargetID.handleEvent(e);
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbTargetID.handleClick(mp);
                if (btnConfirmAction.contains(mp)) doDischargePatient();
            }
        }
        break;

        // ── View screens (scroll only) ────────────────────────────────────────
    case Screen::ADMIN_VIEW_DOCTORS:
    case Screen::ADMIN_VIEW_PATIENTS:
    case Screen::ADMIN_VIEW_APPOINTMENTS:
    case Screen::ADMIN_VIEW_BILLS:
    case Screen::DOCTOR_VIEW_TODAY:
    case Screen::PATIENT_VIEW_APPOINTMENTS:
    case Screen::PATIENT_MEDICAL_RECORDS:
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>())
            if (me->button == sf::Mouse::Button::Left)
                mainList.handleClick(mp);
        break;

        // ── Mark Complete ─────────────────────────────────────────────────────
    case Screen::DOCTOR_MARK_COMPLETE:
        tbTargetID.handleEvent(e);
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbTargetID.handleClick(mp);
                if (btnConfirmAction.contains(mp)) doMarkComplete();
            }
        }
        break;

        // ── Mark No-Show ──────────────────────────────────────────────────────
    case Screen::DOCTOR_MARK_NOSHOW:
        tbTargetID.handleEvent(e);
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbTargetID.handleClick(mp);
                if (btnConfirmAction.contains(mp)) doMarkNoShow();
            }
        }
        break;

        // ── Write Prescription ────────────────────────────────────────────────
    case Screen::DOCTOR_WRITE_PRESCRIPTION:
        routeText({ &tbPresAppID, &tbPresMeds, &tbPresNotes });
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbPresAppID.handleClick(mp);
                tbPresMeds.handleClick(mp);
                tbPresNotes.handleClick(mp);
                if (btnPresSubmit.contains(mp)) doWritePrescription();
            }
        }
        break;

        // ── Patient History ───────────────────────────────────────────────────
    case Screen::DOCTOR_PATIENT_HISTORY:
        tbHistPatID.handleEvent(e);
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbHistPatID.handleClick(mp);
                mainList.handleClick(mp);
                if (btnHistSearch.contains(mp)) doPatientHistory();
            }
        }
        break;

        // ── Book Appointment ──────────────────────────────────────────────────
    case Screen::PATIENT_BOOK_APPOINTMENT:
        routeText({ &tbBookSpec, &tbBookDocID, &tbBookDate, &tbBookSlot });
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbBookSpec.handleClick(mp);  tbBookDocID.handleClick(mp);
                tbBookDate.handleClick(mp);  tbBookSlot.handleClick(mp);
                if (btnBookSubmit.contains(mp)) doBookAppointment();
            }
        }
        break;

        // ── Cancel Appointment ────────────────────────────────────────────────
    case Screen::PATIENT_CANCEL_APPOINTMENT:
        tbCancelAppID.handleEvent(e);
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbCancelAppID.handleClick(mp);
                mainList.handleClick(mp);
                if (btnCancelSubmit.contains(mp)) doCancelAppointment();
            }
        }
        break;

        // ── Pay Bills ─────────────────────────────────────────────────────────
    case Screen::PATIENT_PAY_BILLS:
        tbBillID.handleEvent(e);
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbBillID.handleClick(mp);
                mainList.handleClick(mp);
                if (btnBillPay.contains(mp)) doPayBill();
            }
        }
        break;

        // ── Deposit ───────────────────────────────────────────────────────────
    case Screen::PATIENT_DEPOSIT:
        tbDepositAmt.handleEvent(e);
        if (const auto* me = e.getIf<sf::Event::MouseButtonReleased>()) {
            if (me->button == sf::Mouse::Button::Left) {
                tbDepositAmt.handleClick(mp);
                if (btnDepositSubmit.contains(mp)) doDeposit();
            }
        }
        break;

        // ── Report/Log screens (read only) ────────────────────────────────────
    case Screen::ADMIN_DAILY_REPORT:
    case Screen::ADMIN_SECURITY_LOG:
        break;

    default: break;
    }
}