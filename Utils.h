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
#define ENTER 13
#define KEY_A 97
#define KEY_D 100
#define KEY_S 115
#define KEY_W 119

class Utils {

public:

    static void setColor(unsigned short int code);
	static void resetColor();
	static void showConsoleCursor(bool showFlag);
    static void clearScreen();

    static void drawElement(char element);
	static void drawElement(char element, int color);
	
    static int genRandomInt(int min, int max);
};

#endif
