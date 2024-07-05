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

void Utils::clear() {

    // system("cls");

    /* This works far better than cls
    It takes the cursor to first word of top line and starts to overwrite the text
    so the flickering just stops */
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
}

void Utils::clearScreen() {

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD coord = { 0, 0 };
    DWORD count;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {

        DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

        // Reset text and background color to default
        // WORD defaultAttributes = csbi.wAttributes;
        // FillConsoleOutputAttribute(hStdOut, defaultAttributes, consoleSize, coord, &count);

        // Clear screen buffer with white blank spaces
        FillConsoleOutputCharacter(hStdOut, ' ', consoleSize, coord, &count);

        // Reset cursor on initial position
        SetConsoleCursorPosition(hStdOut, coord);
    }
}

void Utils::drawElement(const char &element) {
    std::cout << element;
}

void Utils::drawElement(const std::string &element) {
    std::cout << element << std::endl;
}

void Utils::drawElement(const char &element, int color) {

    setColor(color);
    std::cout << element;
    setColor(DEFAULT_TXT);
}

void Utils::drawElement(const std::string &element, int color) {

    setColor(color);
    std::cout << element << std::endl;
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

bool Utils::pauseRoutine(const std::string &diagMsg, const std::string &question, const std::string &opt1, const std::string &opt2) {

    /* Returns true if user choose opt1, false if choose opt2 */

    bool ret = true;

    showConsoleCursor(true);
    std::cout << diagMsg << "\n";

    std::string choice = "";
    while (choice != opt1 && choice != opt2) {

        std::cout << question << " (" << opt1 << "/" << opt2 << ") ";
        std::cin >> choice;

        if (choice == opt2)
            ret = false;
    }

    return ret;
}
