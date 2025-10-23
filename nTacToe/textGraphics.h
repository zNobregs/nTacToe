#ifndef TEXTGRAPHICS_H_INCLUDED
#define TEXTGRAPHICS_H_INCLUDED

#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// Conditional compilation for Linux-specific non-blocking input
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else //Mac / Linux
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

const int WIDTH = 80;
const int HEIGHT = 25;
const unsigned char ESCAPE = 27;

enum class Color {
    Default,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
};

class TextGraphics
{
    public:
        TextGraphics();
        int width() { return WIDTH; }
        int height() { return HEIGHT; }
        void draw();
        void clearScreen();
        int readKey();
        void putCharAt(int i, int j, char ch, Color color = Color::Default);
        void putStringAt(int i, int j, const std::string& text, Color color = Color::Default);
        char getCharAt(int i, int j) { return pixels[i][j]; }
        Color getColorAt(int i, int j) { return colors[i][j]; }
        void sleepMs(int ms);
    private:
        void hideCursor();
        void showCursor();
        std::string getAnsiColor(Color color);

        char pixels[HEIGHT][WIDTH];
        Color colors[HEIGHT][WIDTH];
};

#endif // TEXTGRAPHICS_H_INCLUDED
