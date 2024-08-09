#include <iostream>

#include "../code/tries/tries.h"
#include "../code/autocomplete/autocomplete_engine.h"

using namespace std;

int main() {
    Tries* trie = new Tries();
    AutocompleteEngine engine(trie);

    // Preload some words
    vector<pair<string, int>> words = {
        {"apple", 5}, {"application", 10}, {"banana", 3}, {"book", 7},
        {"cat", 4}, {"dog", 8}, {"elephant", 2}, {"function", 6}
    };

    for (auto& [word, freq] : words) {
        engine.insert(word, freq);
    }

    while (true) {
        string input;
        cout << "Enter a prefix (or 'quit' to exit): ";
        cin >> input;

        if (input == "quit") break;

        auto suggestions = engine.suggest(input);

        cout << "Suggestions:" << endl;
        for (const auto& suggestion : suggestions) {
            cout << suggestion.word << " (freq: " << suggestion.frequency << ")" << endl;
        }
        cout << endl;
    }

    delete trie;
    return 0;
}