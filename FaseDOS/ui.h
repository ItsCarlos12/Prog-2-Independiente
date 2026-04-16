#ifndef UI_H
#define UI_H

// Colores para la consola de Windows
enum ConsoleColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};

void setColor(ConsoleColor color);
void resetColor();
void printSuccess(const std::string& message);
void printError(const std::string& message);
void printWarning(const std::string& message);
void printInfo(const std::string& message);
void printTitle(const std::string& title);

#endif // UI_H
