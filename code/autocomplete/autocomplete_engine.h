#ifndef AUTOCOMPLETE_ENGINE_H
#define AUTOCOMPLETE_ENGINE_H

#include <string>
#include <vector>
#include "../tries/tries.h"

using namespace std;

class AutocompleteEngine {
public:
    AutocompleteEngine(Tries* trie) : trie(trie) {};
    vector<string> suggest(string& prefix);
    void suggestHelper(TrieNode* node, string& prefix, string currentWord, vector<string>& suggestions);
    void collectWords(TrieNode* node, string& prefix, string currentWord, vector<string>& suggestions);
    void insert(string& key);
    void remove(string& key);

private:
    Tries* trie;
};

#endif  // AUTOCOMPLETE_ENGINE_H