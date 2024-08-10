#ifndef TRIES_H__
#define TRIES_H__

#include <string>
#include <array>
#include <memory>

using namespace std;

 struct TrieNode {
        array<unique_ptr<TrieNode>, 26> children = {};
        bool is_end_of_word = false;
        double frequency = 0.0;
};

class Tries {
    private:
    unique_ptr<TrieNode> root;

    // Prefix Search
    const TrieNode* pre_search(const string& prefix) const;
    
    public:
    Tries() : root(make_unique<TrieNode>()) {}


    // Insert function
    void insert_node(const string& key, double freq);

    // Search function
    bool search(const string& key) const;

    bool starts_with(const string& prefix) const;

    const TrieNode* get_node(const string& prefix) const;
};


#endif // TRIES_H__