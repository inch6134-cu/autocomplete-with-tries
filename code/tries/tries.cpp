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
    node->is_end_of_word = false;
    node->frequency = 0;
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
void Tries::insert_node(TrieNode* root, string key, int freq = 1) {
    // initialize pointer to root
    TrieNode* cursor = root;

    // iterate over key
    for(auto c : key) {
        int index = c - 'a';
        // check if a node exists for current character
        if(cursor->children[index] == nullptr) {
            // create new node if character doesn't exist
            cursor->children[index] = make_node();
        }
        // move pointer to new node
        cursor = cursor->children[index];
    }

    // once loop terminates this implies a string ends at the current node
    cursor->is_end_of_word = true;
    cursor->frequency = freq;
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
        int index = tolower(c) - 'a';
        // if path does not exist return false
        if(cursor->children[index] == nullptr) return false;

        // else navigate to next child       
        cursor = cursor->children[index];

        // save node to path array
        path[path_index++] = cursor;
    }

    // If key was found mark as not end of word and reset frequency
    if (cursor->is_end_of_word) {
        cursor->is_end_of_word = false;
    }
    
    // If key is no longer the end of a word, evaluate condition for deletion
    if(!cursor->is_end_of_word) {
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
            if(!hasOtherChildren && !current->is_end_of_word) {
                delete current;
                int index = tolower(c) - 'a';
                parent->children[index] = nullptr;
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
        int index = tolower(c) - 'a';
        // check if a node exists for current character
        if(cursor->children[index] == nullptr) {
            // return false if character doesn't exist
            return false;
        }
        // move pointer to next existing node in Trie
        cursor = cursor->children[index];
    }

    // returns true if word exists
    // returns false if word is not found in Trie
    return cursor->is_end_of_word;
}

bool Tries::pre_search(TrieNode* root, string key) {
    // initialize pointer to root
    TrieNode* cursor = root;

    // iterate over key
    for(auto c : key) {
        int index = tolower(c) - 'a';
        // check if a node exists for current character
        if(cursor->children[index] == nullptr) {
            // return false if character doesn't exist
            return false;
        }
        // move pointer to next existing node in Trie
        cursor = cursor->children[index];
    }

    // prefix exists in tree, return true
    return true;
}