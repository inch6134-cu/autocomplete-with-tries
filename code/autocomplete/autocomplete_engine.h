#ifndef AUTOCOMPLETE_ENGINE_H
#define AUTOCOMPLETE_ENGINE_H

#include <string>
#include <vector>
#include "../tries/tries.h"

using namespace std;

struct SuggestedWord {
    string word;
    int frequency;
};

class AutocompleteEngine {
public:
    AutocompleteEngine(Tries* trie) : trie(trie) {};
    vector<SuggestedWord> suggest(string& prefix, int limit = 10, int maxDistance = 1);
    void insert(string& key, int freq = 1);
    void remove(string& key);

private:
    Tries* trie;
    int levenshteinDistance(const string& s1, const string& s2);
};

#endif  // AUTOCOMPLETE_ENGINE_H