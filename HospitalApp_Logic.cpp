#define _CRT_SECURE_NO_WARNINGS
#include "HospitalApp.h"
#include "InsufficientFundsException.h"
#include "SlotUnavailableException .h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>

// ═══════════════════════════════════════════════════════════════════════════
// LOGIN
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doLogin()
{
    string id = tbLoginID.value;
    string pass = tbLoginPass.value;

    
        if (id.empty() || pass.empty()) {
            toast.show("Please enter ID and Password!", Theme::WARNING);
            return;
        }

    // ── ADMIN (no lockout) ────────────────────────────────────────────────
    if (loginRole == 1) {
        Admin* a = admins.findById(id);
        if (a && a->getPassword() == pass) {
            state.loggedInID = id;
            state.loggedInRole = "admin";
            FileHandler::writeSecurityLog("Admin", id, "SUCCESS");
            toast.show("Welcome, " + a->getName() + "!", Theme::SUCCESS);
            tbLoginID.clear(); tbLoginPass.clear();
            goTo(Screen::ADMIN_MENU);
        }
        else {
            FileHandler::writeSecurityLog("Admin", id, "FAILED");
            toast.show("Invalid Admin credentials!", Theme::DANGER);
        }
        return;
    }

    // ── DOCTOR (lockout enabled) ──────────────────────────────────────────
    if (loginRole == 2) {
        Doctor* d = doctors.findById(id);
        if (!d) {
            toast.show("Doctor ID not found!", Theme::DANGER);
            FileHandler::writeSecurityLog("Doctor", id, "FAILED-NOTFOUND");
            return;
        }

        // Check if currently locked
        if (d->checkLocked()) {
            int rem = d->getRemainingLockSecs();
            toast.show("Account locked! Try again in " +
                to_string(rem) + " seconds.", Theme::DANGER);
            toast.duration = 5.f;
            FileHandler::writeSecurityLog("Doctor", id, "BLOCKED-LOCKED");
            return;
        }

        if (d->getPassword() == pass) {
            d->resetAttempts();
            state.loggedInID = id;
            state.loggedInRole = "doctor";
            FileHandler::writeSecurityLog("Doctor", id, "SUCCESS");
            toast.show("Welcome Dr. " + d->getName() + "!", Theme::SUCCESS);
            tbLoginID.clear(); tbLoginPass.clear();
            goTo(Screen::DOCTOR_MENU);
        }
        else {
            bool nowLocked = d->registerFailedAttempt();
            FileHandler::writeSecurityLog("Doctor", id, "FAILED");
            if (nowLocked) {
                toast.show("Too many attempts! Account locked for 60 seconds.",
                    Theme::DANGER);
                toast.duration = 5.f;
                FileHandler::writeSecurityLog("Doctor", id, "LOCKED");
            }
            else {
                int remaining = Doctor::MAX_ATTEMPTS - d->getFailedAttempts();
                toast.show("Wrong password! " + to_string(remaining) +
                    " attempt(s) remaining.", Theme::DANGER);
            }
        }
        return;
    }

    // ── PATIENT (lockout enabled) ─────────────────────────────────────────
    if (loginRole == 3) {
        Patient* p = patients.findById(id);
        if (!p) {
            toast.show("Patient ID not found!", Theme::DANGER);
            FileHandler::writeSecurityLog("Patient", id, "FAILED-NOTFOUND");
            return;
        }

        if (p->checkLocked()) {
            int rem = p->getRemainingLockSecs();
            toast.show("Account locked! Try again in " +
                to_string(rem) + " seconds.", Theme::DANGER);
            toast.duration = 5.f;
            FileHandler::writeSecurityLog("Patient", id, "BLOCKED-LOCKED");
            return;
        }

        if (p->getPassword() == pass) {
            p->resetAttempts();
            state.loggedInID = id;
            state.loggedInRole = "patient";
            FileHandler::writeSecurityLog("Patient", id, "SUCCESS");
            toast.show("Welcome, " + p->getName() + "!", Theme::SUCCESS);
            tbLoginID.clear(); tbLoginPass.clear();
            goTo(Screen::PATIENT_MENU);
        }
        else {
            bool nowLocked = p->registerFailedAttempt();
            FileHandler::writeSecurityLog("Patient", id, "FAILED");
            if (nowLocked) {
                toast.show("Too many attempts! Account locked for 60 seconds.",
                    Theme::DANGER);
                toast.duration = 5.f;
                FileHandler::writeSecurityLog("Patient", id, "LOCKED");
            }
            else {
                int remaining = Patient::MAX_ATTEMPTS - p->getFailedAttempts();
                toast.show("Wrong password! " + to_string(remaining) +
                    " attempt(s) remaining.", Theme::DANGER);
            }
        }
        return;
    }

    toast.show("Please select a role first!", Theme::WARNING);

}

// ═══════════════════════════════════════════════════════════════════════════
// ADD DOCTOR
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doAddDoctor()
{
    string name = tbDocName.value;
    string pass = tbDocPass.value;
    string cont = tbDocContact.value;
    string spec = tbDocSpec.value;
    string feeS = tbDocFee.value;

    if (name.empty() || pass.empty() || cont.empty() || spec.empty() || feeS.empty()) {
        toast.show("All fields are required!", Theme::WARNING); return;
    }
    if (!Validation::validatePassword(pass)) {
        toast.show("Password must be at least 6 characters!", Theme::DANGER); return;
    }
    if (!Validation::validateContact(cont)) {
        toast.show("Contact format: 0000-0000000", Theme::DANGER); return;
    }
    float fee = 0.f;
    try { fee = stof(feeS); }
    catch (...) {
        toast.show("Invalid fee amount!", Theme::DANGER); return;
    }
    if (!Validation::validateFee(fee)) {
        toast.show("Fee must be greater than 0!", Theme::DANGER); return;
    }

    int newNum = FileHandler::getMaxDoctorID("doctors.txt") + 1;
    string id = to_string(newNum);

    Doctor newDoc(id, name, pass, cont, spec, fee);
    doctors.add(newDoc);
    string line = id + "," + name + "," + spec + "," + cont + "," + pass + "," + to_string(fee);
    FileHandler::appendLine("doctors.txt", line);

    toast.show("Doctor " + name + " added! ID: " + id, Theme::SUCCESS);
    tbDocName.clear(); tbDocPass.clear(); tbDocContact.clear();
    tbDocSpec.clear(); tbDocFee.clear();
}

// ═══════════════════════════════════════════════════════════════════════════
// ADD PATIENT
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doAddPatient()
{
    string name = tbPatName.value;
    string pass = tbPatPass.value;
    string cont = tbPatContact.value;
    string ageS = tbPatAge.value;
    string gender = tbPatGender.value;

    if (name.empty() || pass.empty() || cont.empty() || ageS.empty() || gender.empty()) {
        toast.show("All fields are required!", Theme::WARNING); return;
    }
    if (!Validation::validatePassword(pass)) {
        toast.show("Password must be at least 6 characters!", Theme::DANGER); return;
    }
    if (!Validation::validateContact(cont)) {
        toast.show("Contact format: 0000-0000000", Theme::DANGER); return;
    }
    int age = 0;
    try { age = stoi(ageS); }
    catch (...) {
        toast.show("Invalid age!", Theme::DANGER); return;
    }

    // Auto-generate patient ID
    int newNum = 1;
    for (int i = 0; i < patients.getCount(); i++) {
        try {
            int v = stoi(patients.getAT(i)->getID());
            if (v >= newNum) newNum = v + 1;
        }
        catch (...) {}
    }
    string id = to_string(newNum);

    if (patients.findById(id) != nullptr) {
        toast.show("ID conflict, try again!", Theme::DANGER); return;
    }

    Patient newP(id, name, pass, cont, age, gender, 0.0f);
    patients.add(newP);
    string line = id + "," + name + "," + ageS + "," + gender + "," + cont + "," + pass + ",0.00";
    FileHandler::appendLine("patients.txt", line);

    toast.show("Patient " + name + " added! ID: " + id, Theme::SUCCESS);
    tbPatName.clear(); tbPatPass.clear(); tbPatContact.clear();
    tbPatAge.clear(); tbPatGender.clear();
}

// ═══════════════════════════════════════════════════════════════════════════
// REMOVE DOCTOR
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doRemoveDoctor()
{
    string id = tbTargetID.value;
    if (id.empty()) { toast.show("Enter Doctor ID!", Theme::WARNING); return; }

    Doctor* d = doctors.findById(id);
    if (!d) { toast.show("Doctor not found!", Theme::DANGER); return; }

    for (int i = 0; i < appointments.getCount(); i++) {
        if (appointments.getAT(i)->getDocID() == id &&
            appointments.getAT(i)->getStatus() == "pending") {
            toast.show("Cannot remove! Doctor has pending appointments.", Theme::DANGER);
            return;
        }
    }

    doctors.removeById(id);
    FileHandler::deleteRecordByID("doctors.txt", id);
    toast.show("Doctor ID " + id + " removed successfully.", Theme::SUCCESS);
    tbTargetID.clear();
    doViewDoctors();
}

// ═══════════════════════════════════════════════════════════════════════════
// DISCHARGE PATIENT
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doDischargePatient()
{
    string id = tbTargetID.value;
    if (id.empty()) { toast.show("Enter Patient ID!", Theme::WARNING); return; }

    for (int i = 0; i < bills.getCount(); i++) {
        if (bills.getAT(i)->getPatientID() == id && bills.getAT(i)->getStatus() == "unpaid") {
            toast.show("Cannot discharge! Patient has unpaid bills.", Theme::DANGER); return;
        }
    }
    for (int i = 0; i < appointments.getCount(); i++) {
        if (appointments.getAT(i)->getPatientID() == id &&
            appointments.getAT(i)->getStatus() == "pending") {
            toast.show("Cannot discharge! Patient has pending appointments.", Theme::DANGER); return;
        }
    }

    FileHandler::archivePatient(id);
    FileHandler::deleteRecordByID("patients.txt", id);
    FileHandler::deleteRecordsByField("appointments.txt", 1, id);
    FileHandler::deleteRecordsByField("bills.txt", 1, id);
    FileHandler::deleteRecordsByField("prescriptions.txt", 2, id);
    patients.removeById(id);
    toast.show("Patient " + id + " discharged and archived.", Theme::SUCCESS);
    tbTargetID.clear();
}

// ═══════════════════════════════════════════════════════════════════════════
// MARK APPOINTMENT COMPLETE
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doMarkComplete()
{
    string appid = tbTargetID.value;
    if (appid.empty()) { toast.show("Enter Appointment ID!", Theme::WARNING); return; }

    Appointment* ap = appointments.findById(appid);
    if (!ap || ap->getDocID() != state.loggedInID) {
        toast.show("Appointment not found or unauthorized!", Theme::DANGER); return;
    }
    if (ap->getStatus() != "pending") {
        toast.show("Appointment is not pending!", Theme::DANGER); return;
    }
    if (ap->getDate() != getTodayStr()) {
        toast.show("Appointment is not scheduled for today!", Theme::DANGER); return;
    }

    ap->setStatus("completed");
    FileHandler::updateAppointmentStatus("appointments.txt", appid, "completed");

    for (int i = 0; i < bills.getCount(); i++) {
        if (bills.getAT(i)->getAppID() == appid) {
            bills.getAT(i)->setStatus("paid");
            FileHandler::updateBillStatusByAppID("bills.txt", appid, "paid");
            break;
        }
    }
    toast.show("Appointment " + appid + " marked as completed!", Theme::SUCCESS);
    tbTargetID.clear();
}

// ═══════════════════════════════════════════════════════════════════════════
// MARK NO-SHOW
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doMarkNoShow()
{
    string appid = tbTargetID.value;
    if (appid.empty()) { toast.show("Enter Appointment ID!", Theme::WARNING); return; }

    Appointment* ap = appointments.findById(appid);
    if (!ap || ap->getDocID() != state.loggedInID) {
        toast.show("Appointment not found or unauthorized!", Theme::DANGER); return;
    }
    if (ap->getStatus() != "pending") {
        toast.show("Appointment is not pending!", Theme::DANGER); return;
    }
    if (ap->getDate() != getTodayStr()) {
        toast.show("Appointment not scheduled for today!", Theme::DANGER); return;
    }

    ap->setStatus("noshow");
    FileHandler::updateAppointmentStatus("appointments.txt", appid, "noshow");

    for (int i = 0; i < bills.getCount(); i++) {
        if (bills.getAT(i)->getAppID() == appid) {
            bills.getAT(i)->setStatus("cancelled");
            FileHandler::updateBillStatusByAppID("bills.txt", appid, "cancelled");
            break;
        }
    }
    toast.show("Appointment " + appid + " marked as no-show.", Theme::WARNING);
    tbTargetID.clear();
}

// ═══════════════════════════════════════════════════════════════════════════
// WRITE PRESCRIPTION
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doWritePrescription()
{
    string appid = tbPresAppID.value;
    string meds = tbPresMeds.value;
    string notes = tbPresNotes.value;

    if (appid.empty() || meds.empty()) {
        toast.show("Appointment ID and Medicines are required!", Theme::WARNING); return;
    }

    Appointment* ap = appointments.findById(appid);
    if (!ap || ap->getDocID() != state.loggedInID) {
        toast.show("Appointment not found or unauthorized!", Theme::DANGER); return;
    }
    if (ap->getStatus() != "completed") {
        toast.show("Appointment must be completed first!", Theme::DANGER); return;
    }
    for (int i = 0; i < prescriptions.getCount(); i++) {
        if (prescriptions.getAT(i)->getAppointmentID() == appid) {
            toast.show("Prescription already written for this appointment!", Theme::DANGER); return;
        }
    }

    int newNum = FileHandler::getMaxPrescriptionID("prescriptions.txt") + 1;
    string presID = to_string(newNum);

    Prescription p(appid, presID, ap->getPatientID(), state.loggedInID,
        ap->getDate(), meds, notes);
    prescriptions.add(p);
    string line = presID + "," + appid + "," + ap->getPatientID() + "," +
        state.loggedInID + "," + ap->getDate() + "," + meds + "," + notes;
    FileHandler::appendLine("prescriptions.txt", line);

    toast.show("Prescription written! ID: " + presID, Theme::SUCCESS);
    tbPresAppID.clear(); tbPresMeds.clear(); tbPresNotes.clear();
}

// ═══════════════════════════════════════════════════════════════════════════
// CANCEL APPOINTMENT (patient)
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doCancelAppointment()
{
    string appid = tbCancelAppID.value;
    if (appid.empty()) { toast.show("Enter Appointment ID!", Theme::WARNING); return; }

    Appointment* ap = appointments.findById(appid);
    if (!ap || ap->getPatientID() != state.loggedInID || ap->getStatus() != "pending") {
        toast.show("Invalid appointment or not yours!", Theme::DANGER); return;
    }

    float fee = 0.f;
    Doctor* d = doctors.findById(ap->getDocID());
    if (d) fee = d->getfee();

    ap->setStatus("cancelled");
    FileHandler::updateAppointmentStatus("appointments.txt", appid, "cancelled");

    for (int i = 0; i < bills.getCount(); i++) {
        if (bills.getAT(i)->getAppID() == appid) {
            bills.getAT(i)->setStatus("cancelled");
            FileHandler::updateBillStatusByAppID("bills.txt", appid, "cancelled");
            break;
        }
    }

    Patient* pat = currentPatient();
    if (pat) {
        *pat += fee;
        FileHandler::updatePatientBalance("patients.txt", state.loggedInID, pat->getBalance());
    }

    toast.show("Appointment cancelled. PKR " + to_string((int)fee) + " refunded.", Theme::SUCCESS);
    tbCancelAppID.clear();
    doViewMyAppointments();
}

// ═══════════════════════════════════════════════════════════════════════════
// BOOK APPOINTMENT (patient)
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doBookAppointment()
{
    string docid = tbBookDocID.value;
    string date = tbBookDate.value;
    string slot = tbBookSlot.value;

    if (docid.empty() || date.empty() || slot.empty()) {
        toast.show("Doctor ID, Date and Slot are required!", Theme::WARNING); return;
    }
    if (!Validation::validateDate(date)) {
        toast.show("Invalid date! Use DD-MM-YYYY", Theme::DANGER); return;
    }
    if (!Validation::validateTimeSlot(slot)) {
        toast.show("Invalid slot! Valid: 09:00 10:00 11:00 12:00 13:00 14:00 15:00 16:00", Theme::DANGER); return;
    }

    Doctor* doc = doctors.findById(docid);
    if (!doc) { toast.show("Doctor not found!", Theme::DANGER); return; }

    // Check slot conflict
    for (int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.getAT(i);
        if (a->getDocID() == docid && a->getDate() == date &&
            a->getSlot() == slot && a->getStatus() != "cancelled") {
            toast.show("Slot already taken! Pick another.", Theme::DANGER); return;
        }
    }

    Patient* pat = currentPatient();
    if (!pat) { toast.show("Patient not found!", Theme::DANGER); return; }

    if (pat->getBalance() < doc->getfee()) {
        toast.show("Insufficient balance! Fee: PKR " + to_string((int)doc->getfee()), Theme::DANGER);
        return;
    }

    *pat -= doc->getfee();

    int newNum = FileHandler::getMaxAppointmentID("appointments.txt") + 1;
    string appId = to_string(newNum);
    string billId = to_string(newNum + 500);

    Appointment finalApp(appId, state.loggedInID, docid, date, slot, "pending");
    appointments.add(finalApp);

    Bills newBill(billId, state.loggedInID, appId, date,
        to_string((int)doc->getfee()), "unpaid");
    bills.add(newBill);

    FileHandler::appendLine("appointments.txt",
        appId + "," + state.loggedInID + "," + docid + "," + date + "," + slot + ",pending");
    FileHandler::appendLine("bills.txt",
        billId + "," + state.loggedInID + "," + appId + "," +
        to_string((int)doc->getfee()) + ",unpaid," + date);
    FileHandler::updatePatientBalance("patients.txt", state.loggedInID, pat->getBalance());

    toast.show("Appointment booked! ID: " + appId, Theme::SUCCESS);
    tbBookSpec.clear(); tbBookDocID.clear(); tbBookDate.clear(); tbBookSlot.clear();
    doViewMyAppointments();
}

// ═══════════════════════════════════════════════════════════════════════════
// PAY BILL
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doPayBill()
{
    string billID = tbBillID.value;
    if (billID.empty()) { toast.show("Enter Bill ID!", Theme::WARNING); return; }

    Bills* bill = bills.findById(billID);
    if (!bill || bill->getPatientID() != state.loggedInID || bill->getStatus() != "unpaid") {
        toast.show("Invalid bill or already paid!", Theme::DANGER); return;
    }

    float amount = stof(bill->getAmount());
    Patient* pat = currentPatient();
    if (!pat || pat->getBalance() < amount) {
        toast.show("Insufficient balance! Deposit funds first.", Theme::DANGER); return;
    }

    *pat -= amount;
    bill->setStatus("paid");
    FileHandler::updateBillStatusByBillID("bills.txt", billID, "paid");
    FileHandler::updatePatientBalance("patients.txt", state.loggedInID, pat->getBalance());

    ostringstream oss;
    oss << fixed << setprecision(2) << pat->getBalance();
    toast.show("Bill paid! Remaining balance: PKR " + oss.str(), Theme::SUCCESS);
    tbBillID.clear();
    doViewMyBills();
}

// ═══════════════════════════════════════════════════════════════════════════
// DEPOSIT
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doDeposit()
{
    string amtS = tbDepositAmt.value;
    if (amtS.empty()) { toast.show("Enter amount!", Theme::WARNING); return; }

    float amt = 0.f;
    try { amt = stof(amtS); }
    catch (...) {
        toast.show("Invalid amount!", Theme::DANGER); return;
    }
    if (!Validation::validatePositiveFloat(amt)) {
        toast.show("Amount must be greater than 0!", Theme::DANGER); return;
    }

    Patient* pat = currentPatient();
    if (!pat) return;

    *pat += amt;
    FileHandler::updatePatientBalance("patients.txt", state.loggedInID, pat->getBalance());

    ostringstream oss;
    oss << fixed << setprecision(2) << pat->getBalance();
    toast.show("PKR " + amtS + " deposited! New balance: " + oss.str(), Theme::SUCCESS);
    tbDepositAmt.clear();
}

// ═══════════════════════════════════════════════════════════════════════════
// VIEW HELPERS — populate ScrollList / reportLines
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doViewDoctors()
{
    vector<string> rows;
    for (int i = 0; i < doctors.getCount(); i++) {
        Doctor* d = doctors.getAT(i);
        rows.push_back("ID: " + d->getID() + "   Name: " + d->getName() +
            "   Spec: " + d->getSpec() +
            "   Fee: Rs." + to_string((int)d->getfee()));
    }
    if (rows.empty()) rows.push_back("No doctors registered.");
    mainList.setItems(rows);
}

void HospitalApp::doViewPatients()
{
    vector<string> rows;
    for (int i = 0; i < patients.getCount(); i++) {
        Patient* p = patients.getAT(i);
        int unpaid = 0;
        for (int j = 0; j < bills.getCount(); j++) {
            if (bills.getAT(j)->getPatientID() == p->getID() &&
                bills.getAT(j)->getStatus() == "unpaid") unpaid++;
        }
        rows.push_back("ID: " + p->getID() + "   Name: " + p->getName() +
            "   Age: " + to_string(p->getAge()) +
            "   Gender: " + p->getGender() +
            "   Unpaid Bills: " + to_string(unpaid));
    }
    if (rows.empty()) rows.push_back("No patients registered.");
    mainList.setItems(rows);
}

void HospitalApp::doViewAppointments()
{
    vector<string> rows;
    for (int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.getAT(i);
        rows.push_back("AppID: " + a->getAppID() +
            "   PatID: " + a->getPatientID() +
            "   DocID: " + a->getDocID() +
            "   Date: " + a->getDate() +
            "   Slot: " + a->getSlot() +
            "   Status: " + a->getStatus());
    }
    if (rows.empty()) rows.push_back("No appointments found.");
    mainList.setItems(rows);
}

void HospitalApp::doViewUnpaidBills()
{
    vector<string> rows;
    time_t now = time(0);
    for (int i = 0; i < bills.getCount(); i++) {
        Bills* b = bills.getAT(i);
        if (b->getStatus() != "unpaid") continue;
        string tag = "";
        string d = b->getDate();
        if (d.length() == 10) {
            tm bt = {};
            bt.tm_mday = stoi(d.substr(0, 2));
            bt.tm_mon = stoi(d.substr(3, 2)) - 1;
            bt.tm_year = stoi(d.substr(6, 4)) - 1900;
            time_t bt2 = mktime(&bt);
            if (difftime(now, bt2) > 7.0 * 24 * 3600) tag = " [OVERDUE]";
        }
        rows.push_back("BillID: " + b->getBill() +
            "   PatID: " + b->getPatientID() +
            "   Amt: Rs." + b->getAmount() +
            "   Date: " + d + tag);
    }
    if (rows.empty()) rows.push_back("No unpaid bills.");
    mainList.setItems(rows);
}

void HospitalApp::doViewTodayAppointments()
{
    string today = getTodayStr();
    vector<string> rows;
    for (int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.getAT(i);
        if (a->getDocID() == state.loggedInID && a->getDate() == today) {
            rows.push_back("AppID: " + a->getAppID() +
                "   PatID: " + a->getPatientID() +
                "   Slot: " + a->getSlot() +
                "   Status: " + a->getStatus());
        }
    }
    if (rows.empty()) rows.push_back("No appointments scheduled for today.");
    mainList.setItems(rows);
}

void HospitalApp::doViewMyAppointments()
{
    vector<string> rows;
    for (int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.getAT(i);
        if (a->getPatientID() != state.loggedInID) continue;
        Doctor* d = doctors.findById(a->getDocID());
        string dname = d ? d->getName() : a->getDocID();
        string dspec = d ? d->getSpec() : "";
        rows.push_back("AppID: " + a->getAppID() +
            "   Dr." + dname +
            " (" + dspec + ")" +
            "   " + a->getDate() +
            " @ " + a->getSlot() +
            "   [" + a->getStatus() + "]");
    }
    if (rows.empty()) rows.push_back("No appointments found.");
    mainList.setItems(rows);
}

void HospitalApp::doViewMedicalRecords()
{
    vector<string> rows;
    for (int i = 0; i < prescriptions.getCount(); i++) {
        Prescription* p = prescriptions.getAT(i);
        if (p->getPatientID() != state.loggedInID) continue;
        rows.push_back("PresID: " + p->getPrescriptionID() +
            "   Date: " + p->getDate() +
            "   Meds: " + p->getMedicines());
        rows.push_back("   Notes: " + p->getNotes());
        rows.push_back("─────────────────────────────────────────────────");
    }
    if (rows.empty()) rows.push_back("No medical records found.");
    mainList.setItems(rows);
}

void HospitalApp::doViewMyBills()
{
    vector<string> rows;
    float totalUnpaid = 0.f;
    for (int i = 0; i < bills.getCount(); i++) {
        Bills* b = bills.getAT(i);
        if (b->getPatientID() != state.loggedInID) continue;
        if (b->getStatus() == "unpaid") totalUnpaid += stof(b->getAmount());
        rows.push_back("BillID: " + b->getBill() +
            "   AppID: " + b->getAppID() +
            "   Rs." + b->getAmount() +
            "   " + b->getDate() +
            "   [" + b->getStatus() + "]");
    }
    if (rows.empty()) {
        rows.push_back("No bills found.");
    }
    else {
        ostringstream oss;
        oss << fixed << setprecision(2) << totalUnpaid;
        rows.push_back("─────────────────────────────────────────────────");
        rows.push_back("Total Unpaid: Rs. " + oss.str());
    }
    mainList.setItems(rows);
}

void HospitalApp::doPatientHistory()
{
    string pid = tbHistPatID.value;
    if (pid.empty()) { toast.show("Enter Patient ID!", Theme::WARNING); return; }

    // Access check
    bool allowed = false;
    for (int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.getAT(i);
        if (a->getPatientID() == pid && a->getDocID() == state.loggedInID &&
            a->getStatus() == "completed") {
            allowed = true; break;
        }
    }
    if (!allowed) {
        toast.show("Access denied! No completed appointments with this patient.", Theme::DANGER);
        return;
    }

    vector<string> rows;
    for (int i = 0; i < prescriptions.getCount(); i++) {
        Prescription* p = prescriptions.getAT(i);
        if (p->getPatientID() != pid || p->getDoctorID() != state.loggedInID) continue;
        rows.push_back("PresID: " + p->getPrescriptionID() +
            "   Date: " + p->getDate() +
            "   AppID: " + p->getAppointmentID());
        rows.push_back("   Meds: " + p->getMedicines());
        rows.push_back("   Notes: " + p->getNotes());
        rows.push_back("─────────────────────────────────────────────────");
    }
    if (rows.empty()) rows.push_back("No prescriptions found for this patient.");
    mainList.setItems(rows);
    toast.show("History loaded for Patient ID: " + pid, Theme::SUCCESS);
}

// ═══════════════════════════════════════════════════════════════════════════
// DAILY REPORT
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doDailyReport()
{
    string today = getTodayStr();
    reportLines.clear();
    reportLines.push_back("HOSPITAL DAILY REPORT  —  " + today);
    reportLines.push_back("════════════════════════════════════════");

    int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;
    for (int i = 0; i < appointments.getCount(); i++) {
        if (appointments.getAT(i)->getDate() == today) {
            total++;
            string s = appointments.getAT(i)->getStatus();
            if (s == "pending")   pending++;
            else if (s == "completed") completed++;
            else if (s == "noshow")    noshow++;
            else if (s == "cancelled") cancelled++;
        }
    }
    reportLines.push_back("Appointments Today:   " + to_string(total));
    reportLines.push_back("  Pending:    " + to_string(pending));
    reportLines.push_back("  Completed:  " + to_string(completed));
    reportLines.push_back("  No-Show:    " + to_string(noshow));
    reportLines.push_back("  Cancelled:  " + to_string(cancelled));
    reportLines.push_back("");

    float revenue = 0.f;
    for (int i = 0; i < bills.getCount(); i++) {
        if (bills.getAT(i)->getStatus() == "paid" && bills.getAT(i)->getDate() == today)
            revenue += stof(bills.getAT(i)->getAmount());
    }
    ostringstream oss;
    oss << fixed << setprecision(2) << revenue;
    reportLines.push_back("Revenue Collected:    PKR " + oss.str());
    reportLines.push_back("");
    reportLines.push_back("─── Patients with Unpaid Bills ─────────");

    for (int i = 0; i < patients.getCount(); i++) {
        float owed = 0.f;
        for (int j = 0; j < bills.getCount(); j++) {
            if (bills.getAT(j)->getPatientID() == patients.getAT(i)->getID() &&
                bills.getAT(j)->getStatus() == "unpaid")
                owed += stof(bills.getAT(j)->getAmount());
        }
        if (owed > 0) {
            ostringstream o2;
            o2 << fixed << setprecision(2) << owed;
            reportLines.push_back(patients.getAT(i)->getName() + "  →  Rs. " + o2.str());
        }
    }

    reportLines.push_back("");
    reportLines.push_back("─── Doctor-wise Summary ─────────────────");

    for (int i = 0; i < doctors.getCount(); i++) {
        string did = doctors.getAT(i)->getID();
        int dc = 0, dp = 0, dn = 0;
        for (int j = 0; j < appointments.getCount(); j++) {
            if (appointments.getAT(j)->getDocID() == did &&
                appointments.getAT(j)->getDate() == today) {
                string s = appointments.getAT(j)->getStatus();
                if (s == "completed") dc++;
                else if (s == "pending")  dp++;
                else if (s == "noshow")   dn++;
            }
        }
        if (dc + dp + dn > 0) {
            reportLines.push_back("Dr. " + doctors.getAT(i)->getName() +
                "  Done:" + to_string(dc) +
                "  Pending:" + to_string(dp) +
                "  NoShow:" + to_string(dn));
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// SECURITY LOG
// ═══════════════════════════════════════════════════════════════════════════
void HospitalApp::doSecurityLog()
{
    reportLines.clear();
    reportLines.push_back("SECURITY LOG");
    reportLines.push_back("════════════════════════════════════════");

    ifstream file("security_log.txt");
    if (!file.is_open()) {
        reportLines.push_back("No security events logged.");
        return;
    }
    string line;
    while (getline(file, line)) {
        if (!line.empty()) reportLines.push_back(line);
    }
    if (reportLines.size() == 2) reportLines.push_back("No events yet.");
}