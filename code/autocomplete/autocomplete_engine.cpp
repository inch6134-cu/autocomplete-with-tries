#include "autocomplete_engine.h"
#include <algorithm>
#include <queue>

using namespace std;

// vector<SuggestedWord> AutocompleteEngine::suggest(string& prefix, int limit) {
//     // Implement the autocomplete logic using the Trie data structure
//     // and return the suggested words as a vector of strings
//     vector<SuggestedWord> suggestions;
//     TrieNode* node = trie->root;
//     for (char c : prefix) {
//         int index;
//         if (isupper(c)) {
//             index = c - 'A' + 26;
//         } else {
//             index = c - 'a';
//         }
//         if (node->children[index] == nullptr) {
//             return suggestions;  // Prefix not found, return empty vector
//         }
//         node = node->children[index];
//     }

//     suggestHelper(trie->root, prefix, prefix, suggestions);

//      // Sort suggestions based on frequency
//     sort(suggestions.begin(), suggestions.end(), 
//          [](const SuggestedWord& a, const SuggestedWord& b) {
//              return a.frequency > b.frequency;
//          });
    
//     // Limit the number of suggestions
//     if (suggestions.size() > limit) {
//         suggestions.resize(limit);
//     }

//     return suggestions;
// }

// Iterative approach with stack, no helper functions

vector<SuggestedWord> AutocompleteEngine::suggest(string& prefix, int limit = 10) {
    vector<SuggestedWord> suggestions;
    string lowercasePrefix = prefix;
    transform(lowercasePrefix.begin(), lowercasePrefix.end(), lowercasePrefix.begin(), ::tolower);
    
    // Check if the prefix exists in the Trie
    TrieNode* node = trie->root;
    for (char c : lowercasePrefix) {
        int index = c - 'a';
        if (node->children[index] == nullptr) {
            return suggestions;  // Prefix not found, return empty vector
        }
        node = node->children[index];
    }
    
    // Use a queue for depth-first search
    queue<pair<TrieNode*, string>> nodeQueue;
    nodeQueue.push({node, lowercasePrefix});

    while (!nodeQueue.empty() && suggestions.size() < limit) {
        auto [currentNode, currentWord] = nodeQueue.front();
        nodeQueue.pop();

        if (currentNode->is_end_of_word) {
            auto it = find_if(suggestions.begin(), suggestions.end(),
                [&currentWord](const SuggestedWord& sw) { return sw.word == currentWord; });
            if (it != suggestions.end()) {
                it->frequency = max(it->frequency, currentNode->frequency);
            } else {
                suggestions.push_back({currentWord, currentNode->frequency});
            }        }

        for (int i = 0; i < 26; i++) {
            if (currentNode->children[i]) {
                char nextChar = static_cast<char>(i + 'a');
                nodeQueue.push({currentNode->children[i], currentWord + nextChar});
            }
        }
    }

    
    // Sort suggestions based on frequency
    sort(suggestions.begin(), suggestions.end(), 
         [](const SuggestedWord& a, const SuggestedWord& b) {
             return a.frequency > b.frequency;
         });
    
    return suggestions;
}

void AutocompleteEngine::suggestHelper(TrieNode* node, string& prefix, string currentWord, vector<SuggestedWord>& suggestions) {
  // Check if the prefix exists in the Trie
    if (!trie->pre_search(node, prefix)) {
        return;
    }

    TrieNode* prefixNode = node;
    for (char c : prefix) {
        prefixNode = prefixNode->children[c - 'a'];
    }

    // Traverse the Trie and collect all words that start with the given prefix
    collectWords(node, prefix, currentWord, suggestions);
}

void AutocompleteEngine::collectWords(TrieNode* node, string& prefix, string currentWord, vector<SuggestedWord>& suggestions) {
    if (node->is_end_of_word) {
        suggestions.push_back({currentWord, node->frequency});
    }
    for (int i = 0; i < N; i++) {
        if (node->children[i] != nullptr) {
            char nextChar;
            if (i < 26) {
                nextChar = static_cast<char>(i + 'a');
            } else {
                nextChar = static_cast<char>(i - 26 + 'A');
            }
            collectWords(node->children[i], prefix, currentWord + nextChar, suggestions);
        }
    }
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