#ifndef AUTOCOMPLETE_ENGINE_H
#define AUTOCOMPLETE_ENGINE_H

#include <string>
#include <vector>
#include <fstream>

#include "../tries/tries.h"

using namespace std;

struct SuggestedWord {
    string word;
    int frequency;
};

class AutocompleteEngine {
public:
    AutocompleteEngine(Tries* trie) : trie(trie) {};
    vector<SuggestedWord> suggest(const string& prefix, int limit = 10, int maxDistance = 2);
    void insert(string& key, int freq = 1);
    void remove(string& key);
    void loadDictionaryFromFile(const string& filename);

private:
    Tries* trie;
    void dfs(TrieNode* node, string currentWord, const string& prefix, int maxDistance, int limit, 
             vector<SuggestedWord>& suggestions, unordered_map<string, int>& seenSuggestions);

    int levenshteinDistance(const string& s1, const string& s2);
    int matchingPrefixLength(const string& s1, const string& s2);
    double scoreWord(const string& word, const string& prefix, int frequency);
    string toLower(const string& s);
};

#endif  // AUTOCOMPLETE_ENGINE_H