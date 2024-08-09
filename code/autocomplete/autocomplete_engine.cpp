#include "autocomplete_engine.h"
#include <algorithm>
#include <queue>

using namespace std;


vector<SuggestedWord> AutocompleteEngine::suggest(string& prefix, int limit = 10, int maxDistance = 1) {
    vector<SuggestedWord> suggestions;
    string lowercasePrefix = prefix;
    transform(lowercasePrefix.begin(), lowercasePrefix.end(), lowercasePrefix.begin(), ::tolower);
    
    queue<pair<TrieNode*, string>> nodeQueue;
    nodeQueue.push({trie->root, ""});

    while (!nodeQueue.empty() && suggestions.size() < limit) {
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

    // Sort suggestions based on edit distance and frequency
    sort(suggestions.begin(), suggestions.end(), 
         [&](const SuggestedWord& a, const SuggestedWord& b) {
             int distA = levenshteinDistance(a.word.substr(0, min(a.word.length(), lowercasePrefix.length())), lowercasePrefix);
             int distB = levenshteinDistance(b.word.substr(0, min(b.word.length(), lowercasePrefix.length())), lowercasePrefix);
             if (distA != distB) return distA < distB;
             return a.frequency > b.frequency;
         });
    
    return suggestions;
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