#include "autocomplete_engine.h"
#include <algorithm>
#include <queue>
#include <cmath>
#include <iostream>
#include <unordered_map>

using namespace std;


vector<SuggestedWord> AutocompleteEngine::suggest(const string& prefix, int limit, int maxDistance) {
    if (prefix.empty()) {
        return {};  // Return an empty vector for empty prefix
    }
    
    vector<SuggestedWord> suggestions;
    string lowercasePrefix = toLower(prefix); // convert string to lowercase
    
    queue<pair<TrieNode*, string>> nodeQueue;
    nodeQueue.push({trie->root, ""});

    unordered_map<string, int> seenSuggestions;
    dfs(trie->root, "", lowercasePrefix, maxDistance, limit, suggestions, seenSuggestions);

    // Sort suggestions based on our custom scoring function
    sort(suggestions.begin(), suggestions.end(), 
            [&](const SuggestedWord& a, const SuggestedWord& b) {
                return scoreWord(a.word, lowercasePrefix, a.frequency) >
                    scoreWord(b.word, lowercasePrefix, b.frequency);
            });
    
    // Limit the number of suggestions after sorting
    if (suggestions.size() > limit) {
        suggestions.resize(limit);
    }

    return suggestions;
}


void AutocompleteEngine::insert(string& key, int freq) {
    string lowercaseKey = toLower(key);
    // Use the Trie's insert_node() function to add the word to the Trie
    trie->insert_node(trie->root, lowercaseKey, freq);
}

void AutocompleteEngine::remove(string& key) {
    // Use the Trie's delete_node() function to remove the word from the Trie
    trie->delete_node(trie->root, key);
}

void AutocompleteEngine::loadDictionaryFromFile(const string& filename) {
    ifstream file(filename);
    string word;
    int frequency;

    if (file.is_open()) {
        while (file >> word >> frequency) {
            insert(word, frequency);
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

 void AutocompleteEngine::dfs(TrieNode* node, string currentWord, const string& prefix, int maxDistance, int limit, 
         vector<SuggestedWord>& suggestions, unordered_map<string, int>& seenSuggestions) {
        if (suggestions.size() >= limit) return;
        
        if (node->is_end_of_word) {
            int distance = levenshteinDistance(currentWord.substr(0, min(currentWord.length(), prefix.length())), prefix);
            if (distance <= maxDistance && seenSuggestions.find(currentWord) == seenSuggestions.end()) {
                suggestions.push_back({currentWord, node->frequency});
                seenSuggestions[currentWord] = 1;
            }
        }

        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                char nextChar = static_cast<char>(i + 'a');
                string newWord = currentWord + nextChar;
                
                if (newWord.length() <= prefix.length() || 
                    levenshteinDistance(newWord.substr(0, prefix.length()), prefix) <= maxDistance) {
                    dfs(node->children[i], newWord, prefix, maxDistance, limit, suggestions, seenSuggestions);
                }
            }
        }
    }  


// helper function to calculate Levenshtein distance for fuzzy match
int AutocompleteEngine::levenshteinDistance(const string& s1, const string& s2) {
    vector<int> prev(s2.length() + 1, 0), curr(s2.length() + 1, 0);

        for (int j = 0; j <= s2.length(); j++)
            prev[j] = j;

        for (int i = 1; i <= s1.length(); i++) {
            curr[0] = i;
            for (int j = 1; j <= s2.length(); j++) {
                if (s1[i - 1] == s2[j - 1])
                    curr[j] = prev[j - 1];
                else
                    curr[j] = 1 + min({prev[j], curr[j - 1], prev[j - 1]});
            }
            prev = curr;
        }

        return prev[s2.length()];
}

// helper function to calculate the number of matching characters at the beginning
int AutocompleteEngine::matchingPrefixLength(const string& s1, const string& s2) {
    int i = 0;
    while (i < s1.length() && i < s2.length() && s1[i] == s2[i]) {
        i++;
    }
    return i;
}

// scoring function
double AutocompleteEngine::scoreWord(const string& word, const string& prefix, int frequency) {
    int matchLength = matchingPrefixLength(word, prefix);
    int distance = levenshteinDistance(word.substr(0, prefix.length()), prefix);
    
    // Scoring formula: prioritize matching prefix, then consider edit distance and frequency
    return 1000 * matchLength - 10 * distance + log(frequency + 1);
}


string AutocompleteEngine::toLower(const string& s) {
        string result = s;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
}