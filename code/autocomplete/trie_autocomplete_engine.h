#ifndef TRIE_AUTOCOMPLETE_ENGINE_H
#define TRIE_AUTOCOMPLETE_ENGINE_H

#include "autocomplete_engine.h"
#include "../tries/tries.h"

using namespace std;

class TrieAutocompleteEngine : public AutocompleteEngine {
public:

    vector<string> suggest(const string& prefix) override;
    void insert(const string& word) override;
    void remove(const string& word) override;

private:
    Tries trie;
};

#endif  // TRIE_AUTOCOMPLETE_ENGINE_H