#ifndef TRIES_H__
#define TRIES_H__

#include <string>

using namespace std;

#define N 52 // 26 uppercase + 26 lowercase

// define Trie Node struct
typedef struct TrieNode TrieNode;

// Trie Node structure with N children and a counter to check if node is key
struct TrieNode {
    TrieNode* children[N];
    bool is_end_of_word;
    int frequency;
};

class Tries {
    public:
    Tries();
    ~Tries();


    // Insert function
    void insert_node(TrieNode* root, string key, int freq = 1);

    // Delete function
    bool delete_node(TrieNode* root, string key);

    // Search function
    bool search(TrieNode* root, string key);

    // Prefix Search
    bool pre_search(TrieNode* root, string key);

    TrieNode* root;

    // Initialize Trie Node
    TrieNode* make_node();

    // Free Trie Node
    void free_node (TrieNode* node);
};


#endif // TRIES_H__