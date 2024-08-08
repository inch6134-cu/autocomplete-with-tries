#include "trie_autocomplete_engine.h"

using namespace std;

vector<string> TrieAutocompleteEngine::suggest(const string& prefix) {
    // Implement the autocomplete logic using the Trie data structure
    // and return the suggested words as a vector of strings
}

void TrieAutocompleteEngine::insert(const string& word) {
    // Use the Trie's insert_node() function to add the word to the Trie
    trie.insert_node(trie.root, word);
}

void TrieAutocompleteEngine::remove(const string& word) {
    // Use the Trie's delete_node() function to remove the word from the Trie
    trie.delete_node(trie.root, word);
}