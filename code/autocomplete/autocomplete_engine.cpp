#include "autocomplete_engine.h"
#include <algorithm>
#include <queue>
#include <cmath>
#include <iostream>

using namespace std;


vector<SuggestedWord> AutocompleteEngine::suggest(string& prefix, int limit = 10, int maxDistance = 1) {
    if (prefix.empty()) {
        return {};  // Return an empty vector for empty prefix
    }
    
    vector<SuggestedWord> suggestions;
    string lowercasePrefix = prefix;
    transform(lowercasePrefix.begin(), lowercasePrefix.end(), lowercasePrefix.begin(), ::tolower);
    
    queue<pair<TrieNode*, string>> nodeQueue;
    nodeQueue.push({trie->root, ""});

    while (!nodeQueue.empty() && suggestions.size() < limit * 2) { // collect more suggestions initially
        auto [currentNode, currentWord] = nodeQueue.front();
        nodeQueue.pop();

        if (currentNode->is_end_of_word) {
            int distance = levenshteinDistance(currentWord.substr(0, min(currentWord.length(), lowercasePrefix.length())), lowercasePrefix);
            if (distance <= maxDistance) {
                suggestions.push_back({currentWord, currentNode->frequency});
            }
        }

        for (int i = 0; i < 26; i++) {
            if (currentNode->children[i]) {
                char nextChar = static_cast<char>(i + 'a');
                string newWord = currentWord + nextChar;
                
                // Prune branches that exceed the maximum allowed distance
                if (newWord.length() <= lowercasePrefix.length() || 
                    levenshteinDistance(newWord.substr(0, lowercasePrefix.length()), lowercasePrefix) <= maxDistance) {
                    nodeQueue.push({currentNode->children[i], newWord});
                }
            }
        }
    }

    // Sort suggestions based on our custom scoring function
    sort(suggestions.begin(), suggestions.end(), 
         [&](const SuggestedWord& a, const SuggestedWord& b) {
             double scoreA = scoreWord(a.word, lowercasePrefix, a.frequency);
             double scoreB = scoreWord(b.word, lowercasePrefix, b.frequency);
             return scoreA > scoreB;
         });
    
    // Limit the number of suggestions after sorting
    if (suggestions.size() > limit) {
        suggestions.resize(limit);
    }

    return suggestions;
}


void AutocompleteEngine::insert(string& key, int freq) {
    string lowercaseKey = key;
    transform(lowercaseKey.begin(), lowercaseKey.end(), lowercaseKey.begin(), ::tolower);
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

// helper function to calculate Levenshtein distance for fuzzy match
int AutocompleteEngine::levenshteinDistance(const string& s1, const string& s2) {
    vector<vector<int>> dp(s1.length() + 1, vector<int>(s2.length() + 1, 0));

    for (int i = 0; i <= s1.length(); i++) {
        for (int j = 0; j <= s2.length(); j++) {
            if (i == 0) {
                dp[i][j] = j;
            } else if (j == 0) {
                dp[i][j] = i;
            } else if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }

    return dp[s1.length()][s2.length()];
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
    double score = 1000 * matchLength - 10 * distance + log(frequency + 1);
    return score;
}
