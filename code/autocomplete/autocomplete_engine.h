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
    vector<SuggestedWord> suggest(string& prefix, int limit = 10);
    void suggestHelper(TrieNode* node, string& prefix, string currentWord, vector<SuggestedWord>& suggestions);
    void collectWords(TrieNode* node, string& prefix, string currentWord, vector<SuggestedWord>& suggestions);
    void insert(string& key, int freq = 1);
    void remove(string& key);

private:
    Tries* trie;
};

#endif  // AUTOCOMPLETE_ENGINE_H