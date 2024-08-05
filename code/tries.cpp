#include "tries.h"

using namespace std;

Tries::Tries() {
    root = new TrieNode;
}

Tries::~Tries() {}

// Initialize Trie Node
TrieNode* make_node() {
    TrieNode* node = new TrieNode;
    for (int i = 0; i < N; i++) {
        node->children[i] = nullptr;
    }
    node->word_count = 0;
    return node;
}

// Free Trie Node from memory
void free_node (TrieNode* node) {
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
void insert_node(TrieNode* root, string key) {
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
bool delete_node(TrieNode* root, string key) {
    TrieNode* cursor = root;
    TrieNode* leaf = nullptr;
    char leaf_char = 'a';

    for(auto c : key) {
        if(cursor->children[c - 'a'] == nullptr) return false;
        else {
            int count = 0;
            for (int i = 0; i < N; i++) {
                if(cursor->children[i] != nullptr) count++;
            }

            if(count > 1) {
                leaf = cursor;
                leaf_char = c;
            }

            cursor = cursor->children[c - 'a'];
        }
    }

    int count = 0;
    for (int i = 0; i < N; i++) {
        if(cursor->children[i] != nullptr) count++;
    }

    if(count > 0) {
        cursor->word_count--;
        return true;
    }

    if(leaf != nullptr) {
        free_node(leaf->children[leaf_char]);
        return true;
    }

    else {
        free_node(root->children[key[0]]);
        return true;
    }
}

// Search function
bool search(TrieNode*root, string key) {
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

bool pre_search(TrieNode* root, string key) {
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