#include "textGraphics.h"

TextGraphics::TextGraphics()
{
    //enable ansii escape sequences
    #ifdef _WIN32
        HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;

        // Get the current console mode
        if (!GetConsoleMode(hOutput, &dwMode)) {
            std::cerr << "GetConsoleMode failed!" << std::endl;
            return;
        }

        // Enable virtual terminal processing
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        // Set the new console mode
        if (!SetConsoleMode(hOutput, dwMode)) {
            std::cerr << "SetConsoleMode failed!" << std::endl;
            return;
        }
    #endif // _WIN32

    clearScreen();
    hideCursor();
}

void TextGraphics::draw()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        std::string row;
        for (int j = 0; j < WIDTH; j++)
        {
            row += getAnsiColor(colors[i][j]) + pixels[i][j];
        }
        // Use ANSI escape sequence to set cursor position
        std::cout << "\033[" << i + 1 << ";" << 0 + 1 << "H";
        std::cout << row << "\033[0m";
    }
    std::cout << std::flush;
}

// Define ANSI color codes
std::string TextGraphics::getAnsiColor(Color color) {
    switch (color) {
    case Color::Black:   return "\033[30m";
    case Color::Red:     return "\033[31m";
    case Color::Green:   return "\033[32m";
    case Color::Yellow:  return "\033[33m";
    case Color::Blue:    return "\033[34m";
    case Color::Magenta: return "\033[35m";
    case Color::Cyan:    return "\033[36m";
    case Color::White:   return "\033[37m";
    case Color::Default: return "\033[39m"; // Reset to default foreground color
    default:             return "\033[39m";
    }
}

// Clears the console using ANSI escape sequences
void TextGraphics::clearScreen() {
    // Moves the cursor to the home position (1,1) and then clears the screen.
    std::cout << "\033[H\033[2J" << std::flush;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            pixels[i][j] = ' ';
        }
    }
}

// ---- Non-blocking key read ----
// Returns -1 if no key available; otherwise returns the byte value (0–255).
int TextGraphics::readKey() {
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

// Outputs a character at a specific console position with a given color
void TextGraphics::putCharAt(int x, int y, char ch, Color color)
{
    pixels[y][x] = ch;
    colors[y][x] = color;
}

// Outputs a string at a specific console position with a given color
void TextGraphics::putStringAt(int x, int y, const std::string& text, Color color)
{
    for (int n = 0; n < static_cast<int>(text.size()) && x + n < WIDTH; n++)
    {
        pixels[y][x + n] = text.at(n);
        colors[y][x + n] = color;
    }
}

// Pauses the program for a given number of milliseconds
void TextGraphics::sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Hides the blinking console cursor using ANSI escape sequences
void TextGraphics::hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

// Shows the console cursor using ANSI escape sequences
void TextGraphics::showCursor() {
    std::cout << "\033[?25h" << std::flush;
}
