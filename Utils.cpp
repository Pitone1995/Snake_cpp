#include "Utils.h"

void Utils::setColor(unsigned short int code) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, code);
}

void Utils::resetColor() {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, DEFAULT_TXT);
}

void Utils::showConsoleCursor(bool showFlag) {

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);

    // Set the cursor visibility
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void Utils::clearScreen() {

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD coord = { 0, 0 };
    DWORD count;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {

        DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

        // Reset text and background color to default
        WORD defaultAttributes = csbi.wAttributes;
        FillConsoleOutputAttribute(hStdOut, defaultAttributes, consoleSize, coord, &count);

        // Clear screen buffer with white blank spaces
        FillConsoleOutputCharacter(hStdOut, ' ', consoleSize, coord, &count);

        // Reset cursor on initial position
        SetConsoleCursorPosition(hStdOut, coord);
    }
}

void Utils::drawElement(char element) {
    std::cout << element;
}

void Utils::drawElement(char element, int color) {

    setColor(color);
    std::cout << element;
    setColor(DEFAULT_TXT);
}

int Utils::genRandomInt(int min, int max) {

    // Obtain a random number from hardware
    std::random_device rd;

    // Seed the generator
    std::mt19937 gen(rd());

    // Define the range
    std::uniform_int_distribution<> distr(min, max);

    // Generate number
    return distr(gen);
}