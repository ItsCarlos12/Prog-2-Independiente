#include "ui.h"
#include <iostream>
#include <windows.h>

void setColor(ConsoleColor color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void resetColor() {
    setColor(WHITE);
}

void printSuccess(const std::string& message) {
    setColor(LIGHT_GREEN);
    std::cout << message << std::endl;
    resetColor();
}

void printError(const std::string& message) {
    setColor(LIGHT_RED);
    std::cout << message << std::endl;
    resetColor();
}

void printWarning(const std::string& message) {
    setColor(YELLOW);
    std::cout << message << std::endl;
    resetColor();
}

void printInfo(const std::string& message) {
    setColor(LIGHT_CYAN);
    std::cout << message << std::endl;
    resetColor();
}

void printTitle(const std::string& title) {
    setColor(LIGHT_MAGENTA);
    std::cout << "\n--- " << title << " ---" << std::endl;
    resetColor();
}
