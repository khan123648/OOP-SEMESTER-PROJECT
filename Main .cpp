#define _CRT_SECURE_NO_WARNINGS

/*
 * ============================================================
 *   MediCore Hospital Management System
 *   SFML GUI Entry Point — main.cpp
 *
 *   Compile (MSVC example):
 *     cl main.cpp HospitalApp*.cpp FileHandler.cpp Validation.cpp
 *        /I<SFML_INCLUDE> /link <SFML_LIB>\sfml-graphics.lib
 *        <SFML_LIB>\sfml-window.lib <SFML_LIB>\sfml-system.lib
 *        <SFML_LIB>\sfml-audio.lib
 *
 *   Compile (g++ / MinGW example):
 *     g++ main.cpp HospitalApp*.cpp FileHandler.cpp Validation.cpp
 *         -I<SFML>/include -L<SFML>/lib
 *         -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
 *         -o MediCore.exe
 *
 *   Required files at runtime (same folder as .exe):
 *     arial.ttf          — font
 *     admin.txt          — auto-created if missing
 *     doctors.txt        — can be empty on first run
 *     patients.txt       — can be empty on first run
 *     appointments.txt   — can be empty on first run
 *     bills.txt          — can be empty on first run
 *     prescriptions.txt  — can be empty on first run
 * ============================================================
 */

#include "HospitalApp.h"   // pulls in SFML, GUI, AppState, all logic headers

 // ── Windows subsystem: hide console window in release builds ───────────────
#if defined(_WIN32) && defined(NDEBUG)
#   pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif
// ─────────────────────────────────────────────────────────────────────────────

int main()
{
    // HospitalApp constructor:
    //   • Creates the 1280×780 SFML window
    //   • Loads arial.ttf (falls back to C:/Windows/Fonts/arial.ttf)
    //   • Calls FileHandler::initAdminFile — creates admin.txt with a
    //     default admin (ID:1, pass:admin123) if the file does not exist
    //   • Loads all .txt data files into Storage<T> containers
    //   • Initialises every Button, TextBox, ScrollList, and Toast widget
    HospitalApp app;

    // run() contains the main game-loop:
    //   while (window.isOpen())  { handleEvents();  draw(); }
    //   syncAndSave() is called once more after the loop exits
    //   (it is also called on every Logout and on window Close)
    app.run();

    return 0;
}