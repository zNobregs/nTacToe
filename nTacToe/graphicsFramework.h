#ifndef GRAPHICSFRAMEWORK_H_INCLUDED
#define GRAPHICSFRAMEWORK_H_INCLUDED
#include <iostream>

#include <thread>
#include <chrono>

#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>   // _getch()
#else
  #include <termios.h>
  #include <unistd.h>
  #include <fcntl.h>
#endif

// ---- Simple color enum (foreground only) ----
enum class Color {
    Default,
    Black, Red, Green, Yellow, Blue, Magenta, Cyan, White,
    BrightBlack, BrightRed, BrightGreen, BrightYellow,
    BrightBlue, BrightMagenta, BrightCyan, BrightWhite
};

// ---- Cross-platform sleep (milliseconds) ----
void sleepMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}

// ---- Clear console ----
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H";
    std::cout.flush();
#endif
}

// ---- Put single character at (x, y) with optional color ----
void putCharAt(int x, int y, unsigned char ch, Color color = Color::Default) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD pos{ static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD oldAttr = csbi.wAttributes;

    WORD attr = oldAttr;
    switch (color) {
        case Color::Black:         attr = 0;  break;
        case Color::Blue:          attr = 1;  break;
        case Color::Green:         attr = 2;  break;
        case Color::Cyan:          attr = 3;  break;
        case Color::Red:           attr = 4;  break;
        case Color::Magenta:       attr = 5;  break;
        case Color::Yellow:        attr = 6;  break;
        case Color::White:         attr = 7;  break;
        case Color::BrightBlack:   attr = 8;  break;
        case Color::BrightBlue:    attr = 9;  break;
        case Color::BrightGreen:   attr = 10; break;
        case Color::BrightCyan:    attr = 11; break;
        case Color::BrightRed:     attr = 12; break;
        case Color::BrightMagenta: attr = 13; break;
        case Color::BrightYellow:  attr = 14; break;
        case Color::BrightWhite:   attr = 15; break;
        default: break;
    }

    if (color != Color::Default) SetConsoleTextAttribute(hConsole, attr);
    std::cout << ch << std::flush;
    if (color != Color::Default) SetConsoleTextAttribute(hConsole, oldAttr);

#else
    int code = -1;
    switch (color) {
        case Color::Black:         code = 30; break;
        case Color::Red:           code = 31; break;
        case Color::Green:         code = 32; break;
        case Color::Yellow:        code = 33; break;
        case Color::Blue:          code = 34; break;
        case Color::Magenta:       code = 35; break;
        case Color::Cyan:          code = 36; break;
        case Color::White:         code = 37; break;
        case Color::BrightBlack:   code = 90; break;
        case Color::BrightRed:     code = 91; break;
        case Color::BrightGreen:   code = 92; break;
        case Color::BrightYellow:  code = 93; break;
        case Color::BrightBlue:    code = 94; break;
        case Color::BrightMagenta: code = 95; break;
        case Color::BrightCyan:    code = 96; break;
        case Color::BrightWhite:   code = 97; break;
        default: break;
    }

    if (code == -1) {
        std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << ch;
    } else {
        std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H"
                  << "\033[" << code << "m" << ch << "\033[0m";
    }
    std::cout.flush();
#endif
}

// ---- Put a string at (x, y) with optional color ----
void putStringAt(int x, int y, const std::string& text, Color color = Color::Default) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD pos{ static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD oldAttr = csbi.wAttributes;

    WORD attr = oldAttr;
    switch (color) {
        case Color::Black:         attr = 0;  break;
        case Color::Blue:          attr = 1;  break;
        case Color::Green:         attr = 2;  break;
        case Color::Cyan:          attr = 3;  break;
        case Color::Red:           attr = 4;  break;
        case Color::Magenta:       attr = 5;  break;
        case Color::Yellow:        attr = 6;  break;
        case Color::White:         attr = 7;  break;
        case Color::BrightBlack:   attr = 8;  break;
        case Color::BrightBlue:    attr = 9;  break;
        case Color::BrightGreen:   attr = 10; break;
        case Color::BrightCyan:    attr = 11; break;
        case Color::BrightRed:     attr = 12; break;
        case Color::BrightMagenta: attr = 13; break;
        case Color::BrightYellow:  attr = 14; break;
        case Color::BrightWhite:   attr = 15; break;
        default: break;
    }

    if (color != Color::Default) SetConsoleTextAttribute(hConsole, attr);
    std::cout << text << std::flush;
    if (color != Color::Default) SetConsoleTextAttribute(hConsole, oldAttr);

#else
    int code = -1;
    switch (color) {
        case Color::Black:         code = 30; break;
        case Color::Red:           code = 31; break;
        case Color::Green:         code = 32; break;
        case Color::Yellow:        code = 33; break;
        case Color::Blue:          code = 34; break;
        case Color::Magenta:       code = 35; break;
        case Color::Cyan:          code = 36; break;
        case Color::White:         code = 37; break;
        case Color::BrightBlack:   code = 90; break;
        case Color::BrightRed:     code = 91; break;
        case Color::BrightGreen:   code = 92; break;
        case Color::BrightYellow:  code = 93; break;
        case Color::BrightBlue:    code = 94; break;
        case Color::BrightMagenta: code = 95; break;
        case Color::BrightCyan:    code = 96; break;
        case Color::BrightWhite:   code = 97; break;
        default: break;
    }

    if (code == -1) {
        std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << text;
    } else {
        std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H"
                  << "\033[" << code << "m" << text << "\033[0m";
    }
    std::cout.flush();
#endif
}

/*
// ---- Cross-platform: read a single keystroke (blocking) ----
// Returns the byte value of the key pressed (0–255). No echo.
int readKey() {
#ifdef _WIN32
    // _getch() does not echo and returns immediately after a key.
    // Note: function/arrow keys return 0 or 224 first; call _getch() again to get the code if needed.
    return _getch();
#else
    // Save current terminal settings
    termios oldt{};
    tcgetattr(STDIN_FILENO, &oldt);

    // Set raw mode: no echo, no canonical processing
    termios raw = oldt;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN]  = 1;  // block for at least 1 byte
    raw.c_cc[VTIME] = 0;  // no timeout
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    unsigned char ch = 0;
    ssize_t n = read(STDIN_FILENO, &ch, 1);

    // Restore terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return (n == 1) ? static_cast<int>(ch) : -1;
#endif
}
*/

// ---- Non-blocking key read ----
// Returns -1 if no key available; otherwise returns the byte value (0–255).
int readKey() {
#ifdef _WIN32
    if (_kbhit()) {
        // Note: special keys may return 0 or 224 first; you can call readKey() again to get the next byte if needed.
        return _getch();
    }
    return -1;
#else
    // Save current terminal settings
    termios oldt{};
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return -1;

    termios raw = oldt;
    raw.c_lflag &= ~(ICANON | ECHO);   // raw mode: no line buffering, no echo
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    // Set non-blocking on stdin
    int oldfl = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldfl | O_NONBLOCK);

    unsigned char ch = 0;
    ssize_t n = read(STDIN_FILENO, &ch, 1);

    // Restore flags and terminal
    fcntl(STDIN_FILENO, F_SETFL, oldfl);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return (n == 1) ? static_cast<int>(ch) : -1;
#endif
}

// ---- Hide / show cursor (cross-platform) ----
void hideCursor() {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci{};
    if (GetConsoleCursorInfo(h, &ci)) {
        ci.bVisible = FALSE;
        if (ci.dwSize == 0) ci.dwSize = 20; // ensure valid size
        SetConsoleCursorInfo(h, &ci);
    }
#else
    std::cout << "\033[?25l" << std::flush;
#endif
}


#endif // GRAPHICSFRAMEWORK_H_INCLUDED
