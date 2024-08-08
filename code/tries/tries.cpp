#include "tries.h"

using namespace std;

Tries::Tries() {
    root = make_node();
}

Tries::~Tries() {
    if(root) {
        free_node(root);
        root = nullptr;
    }
}

// Initialize Trie Node
TrieNode* Tries::make_node() {
    TrieNode* node = new TrieNode;
    for (int i = 0; i < N; i++) {
        node->children[i] = nullptr;
    }
    node->word_count = 0;
    return node;
}

// Free Trie Node from memory
void Tries::free_node (TrieNode* node) {
    if(node == nullptr) return;

    for (int i = 0; i < N; i++)
    {
        if(node->children[i] != nullptr) {
            free_node(node->children[i]);
        }
        else continue;
    }
    delete node;
}

// Insert function
void Tries::insert_node(TrieNode* root, string key) {
    // initialize pointer to root
    TrieNode* cursor = root;

    // iterate over key
    for(auto c : key) {
        // check if a node exists for current character
        if(cursor->children[c - 'a'] == nullptr) {
            // create new node if character doesn't exist
            cursor->children[c - 'a'] = make_node();
        }
        // move pointer to new node
        cursor = cursor->children[c - 'a'];
    }

    // once loop terminates this implies a string ends at the current node
    cursor->word_count++;
}

// Delete function
bool Tries::delete_node(TrieNode* root, string key) {
    if(key.empty()) return false;
    
    // Initialize pointer to root
    TrieNode* cursor = root;
    
    // Initialize array to save path
    TrieNode* path[key.length() + 1];
    int path_index = 0;
    
    path[path_index++] = cursor;

    // iterate over key
    for(auto c : key) {
        // if path does not exist return false
        if(cursor->children[c - 'a'] == nullptr) return false;

        // else navigate to next child       
        cursor = cursor->children[c - 'a'];

        // save node to path array
        path[path_index++] = cursor;
    }

    // Key was not found
    if(cursor->word_count == 0) return false;
    
    // If key was found decrement word count
    cursor->word_count--;

    // If key is no longer the end of a word, evaluate condition for deletion
    if(cursor->word_count == 0) {
        // Iterate over path array
        for(int i = path_index - 1; i > 0; i--) {
            TrieNode* current = path[i];
            TrieNode* parent = path[i-1];
            char c = key[i-1];
            
            bool hasOtherChildren = false;
            
            // Evaluate path nodes one by one for child nodes
            for(int j = 0; j < N; j++) {
                if(current->children[j] != nullptr) {
                    hasOtherChildren = true;
                    break;
                }
            }
            
            // Delete nodes only if path is not a prefix for other words and
            // is not the end of a keyword in the Trie
            if(!hasOtherChildren && current->word_count == 0) {
                delete current;
                parent->children[c - 'a'] = nullptr;
            } else {
                break;
            }
        }
    }

    return true;
}

// Search function
bool Tries::search(TrieNode*root, string key) {
    // initialize pointer to root
    TrieNode* cursor = root;

    // iterate over key
    for(auto c : key) {
        // check if a node exists for current character
        if(cursor->children[c - 'a'] == nullptr) {
            // return false if character doesn't exist
            return false;
        }
        // move pointer to next existing node in Trie
        cursor = cursor->children[c - 'a'];
    }

    // returns true if word exists and is marked as a complete word
    // returns false if complete word is not found in Trie
    return (cursor->word_count > 0);
}

bool Tries::pre_search(TrieNode* root, string key) {
    // initialize pointer to root
    TrieNode* cursor = root;

    // iterate over key
    for(auto c : key) {
        // check if a node exists for current character
        if(cursor->children[c - 'a'] == nullptr) {
            // return false if character doesn't exist
            return false;
        }
        // move pointer to next existing node in Trie
        cursor = cursor->children[c - 'a'];
    }

    // prefix exists in tree, return true
    return true;
}