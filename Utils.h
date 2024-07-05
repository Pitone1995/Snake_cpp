#ifndef Utils_H
#define Utils_H

#include <windows.h>
#include <string>
#include <iostream>
#include <random>

// From color --help
#define HIGHLIGHT_TXT 240 // black text, white background
#define DEFAULT_TXT 7 // grey text, black background (default for cmd)
#define GREEN_TXT 10 // green text (brighter than FOREGROUND_GREEN), black background
#define RED_TXT 12 // red text (brighter than FOREGROUND_RED), black background

#define UP_ARROW 72
#define DWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_A_LOW 97
#define KEY_D_LOW 100
#define KEY_S_LOW 115
#define KEY_W_LOW 119
#define KEY_A_UP 65
#define KEY_D_UP 68
#define KEY_S_UP 83
#define KEY_W_UP 87

class Utils {

public:

    static void setColor(unsigned short int code);
	static void resetColor();
	static void showConsoleCursor(bool showFlag);
    static void clear();
    static void clearScreen();

    static void drawElement(const char &element);
    static void drawElement(const std::string &element);
	static void drawElement(const char &element, int color);
    static void drawElement(const std::string &element, int color);
	
    static int genRandomInt(int min, int max);

    static bool pauseRoutine(const std::string &diagMsg, const std::string &question, const std::string &opt1, const std::string &opt2);
};

#endif
