#ifndef TRIES_H__
#define TRIES_H__

#include <string>

using namespace std;


// Trie is a N-ary tree that can hold all standard ASCII characters
// excluding 32 non-printing characters
// TODO: Implement full support for ASCII ex. non-printing characters
// #define N 127 - 32

// NOTE: First implementation will only include lowercase alphabet
#define N 26

// define Trie Node struct
typedef struct TrieNode TrieNode;

// Trie Node structure with N children and a counter to check if node is key
struct TrieNode {
    TrieNode* children[N];
    int word_count;
};

class Tries {
    public:
    Tries();
    ~Tries();

    // Initialize Trie Node
    TrieNode* make_node() {};

    // Free Trie Node
    void free_node (TrieNode* node) {};

    // Insert function
    void insert_node(TrieNode* root, string key) {};

    // Delete function
    bool delete_node(TrieNode* root, string key) {};

    // Search function
    bool search(TrieNode*root, string key) {};

    // Prefix Search
    bool pre_search(TrieNode* root, string key) {};

    private:
    TrieNode* root;
};


#endif // TRIES_H__