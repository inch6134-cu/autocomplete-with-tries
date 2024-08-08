#include "autocomplete_engine.h"

using namespace std;

vector<string> AutocompleteEngine::suggest(string& prefix) {
    // Implement the autocomplete logic using the Trie data structure
    // and return the suggested words as a vector of strings
    vector<string> suggestions;
    suggestHelper(trie->root, prefix, "", suggestions);
    return suggestions;
}

void AutocompleteEngine::suggestHelper(TrieNode* node, string& prefix, string currentWord, vector<string>& suggestions) {
  // Check if the prefix exists in the Trie
    if (!trie->pre_search(node, prefix)) {
        return;
    }

    // Traverse the Trie and collect all words that start with the given prefix
    collectWords(node, prefix, "", suggestions);
}

void AutocompleteEngine::collectWords(TrieNode* node, string& prefix, string currentWord, vector<string>& suggestions) {
    currentWord += static_cast<char>(node - trie->root->children[0] + 'a');

    // If the current node is the end of a word, add the current word to the suggestions
    if (trie->search(node, currentWord)) {
        suggestions.push_back(currentWord);
    }

    // Recursively traverse the Trie and collect all words that start with the prefix
    for (int i = 0; i < N; i++) {
        if (node->children[i] != nullptr) {
            collectWords(node->children[i], prefix, currentWord, suggestions);
        }
    }
}

void AutocompleteEngine::insert(string& word) {
    // Use the Trie's insert_node() function to add the word to the Trie
    trie->insert_node(trie->root, word);
}

void AutocompleteEngine::remove(string& word) {
    // Use the Trie's delete_node() function to remove the word from the Trie
    trie->delete_node(trie->root, word);
}