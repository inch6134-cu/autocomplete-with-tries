#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include "../code/tries/tries.h"
#include "../code/autocomplete/autocomplete_engine.h"

using namespace std;

atomic<bool> running(true);
string currentInput = "";
AutocompleteEngine* engine;

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
    cout << "\033[2J\033[H" << flush;
}

void moveCursor(int x, int y) {
    cout << "\033[" << y << ";" << x << "H" << flush;
}

void printCurrentStateAndSuggestions() {
    clearScreen();
    cout << "Current input: " << currentInput;
    moveCursor(15 + currentInput.length(), 1);  // Move cursor to end of input
    cout << flush;
    
    moveCursor(1, 3);  // Move to third line for suggestions
    auto suggestions = engine->suggest(currentInput);
    cout << "Suggestions:" << endl;
    for (const auto& suggestion : suggestions) {
        cout << suggestion.word << " (" << suggestion.frequency << ")" << endl;
    }
    cout << endl << "Type to search, 'ESC' to quit" << endl;
    
    moveCursor(15 + currentInput.length(), 1);  // Move cursor back to end of input
    cout << flush;
}

void inputThread() {
    enableRawMode();
    printCurrentStateAndSuggestions();  // Initial print
    while (running) {
        char c = cin.get();
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
    Tries* trie = new Tries();
    engine = new AutocompleteEngine(trie);

    cout << "Loading dictionary..." << endl;
    engine->loadDictionaryFromFile("dictionary.txt");
    cout << "Dictionary loaded. Press enter to start" << endl;
    cin.get();  // Wait for user input before clearing screen

    clearScreen();
    thread input(inputThread);
    input.join();

    delete engine;
    delete trie;
    return 0;
}