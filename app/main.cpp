#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <iomanip>

#include "../code/autocomplete/autocomplete_engine.h"

std::atomic<bool> running(true);
std::string currentInput = "";
std::unique_ptr<AutocompleteEngine> engine;

termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void moveCursor(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H" << std::flush;
}

void printCurrentStateAndSuggestions() {
    clearScreen();
    std::cout << "Current input: " << currentInput;
    moveCursor(15 + currentInput.length(), 1);  // Move cursor to end of input
    std::cout << std::flush;
    
    moveCursor(1, 3);  // Move to third line for suggestions
    auto suggestions = engine->suggest(currentInput);
    std::cout << "Suggestions:" << std::endl;
    for (const auto& suggestion : suggestions) {
        std::cout << suggestion.word << " (";
        if (suggestion.frequency >= 1e9) {
            std::cout << std::fixed << std::setprecision(3) << suggestion.frequency / 1e9 << "B";
        } else if (suggestion.frequency >= 1e6) {
            std::cout << std::fixed << std::setprecision(3) << suggestion.frequency / 1e6 << "M";
        } else if (suggestion.frequency >= 1e3) {
            std::cout << std::fixed << std::setprecision(3) << suggestion.frequency / 1e3 << "K";
        } else {
            std::cout << std::fixed << std::setprecision(0) << suggestion.frequency;
        }
        std::cout << ")" << std::endl;
    }
    std::cout << std::endl << "Type to search, 'ESC' to quit" << std::endl;
    
    moveCursor(15 + currentInput.length(), 1);  // Move cursor back to end of input
    std::cout << std::flush;
}

void inputThread() {
    enableRawMode();
    printCurrentStateAndSuggestions();  // Initial print
    while (running) {
        char c = std::cin.get();
        if (c == 27) { // ASCII 27 is the escape key
            running = false;
        } else if (c == 127) {  // Backspace
            if (!currentInput.empty()) {
                currentInput.pop_back();
                printCurrentStateAndSuggestions();
            }
        } else if (c >= 32 && c < 127) {  // Printable characters
            currentInput += c;
            printCurrentStateAndSuggestions();
        }
    }
    disableRawMode();
}

int main() {
    engine = std::make_unique<AutocompleteEngine>();

    std::cout << "Loading dictionary..." << std::endl;
    engine->loadDictionaryFromFile("dictionary.txt");
    std::cout << "Dictionary loaded. Press enter to start" << std::endl;
    std::cin.get();  // Wait for user input before clearing screen

    clearScreen();
    std::thread input(inputThread);
    input.join();

    return 0;
}