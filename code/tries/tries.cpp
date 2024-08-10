#include "tries.h"

using namespace std;

const TrieNode *Tries::pre_search(const string &prefix) const
{
    // initialize pointer to root
    auto cursor = root.get();

    // iterate over key
    for (char c : prefix)
    {
        int index = tolower(c) - 'a';
        // check if a node exists for current character
        if (index < 0 || index >= 26 || !cursor->children[index])
        {
            // return false if character doesn't exist
            return nullptr;
        }
        // move pointer to next existing node in Trie
        cursor = cursor->children[index].get();
    }

    // prefix exists in tree, return true
    return cursor;
}

// Insert function
void Tries::insert_node(const string &key, double freq)
{
    // initialize pointer to root
    auto cursor = root.get();

    // iterate over key
    for (char c : key)
    {
        int index = tolower(c) - 'a';
        if (index < 0 || index >= 26)
            continue; // Skip non-alphabetic characters
        // check if a node exists for current character
        if (!cursor->children[index])
        {
            // create new node if character doesn't exist
            cursor->children[index] = make_unique<TrieNode>();
        }
        // move pointer to new node
        cursor = cursor->children[index].get();
    }

    // once loop terminates this implies a string ends at the current node
    cursor->is_end_of_word = true;
    cursor->frequency = freq;
}

// Search function
bool Tries::search(const string &key) const
{
    // call prefix search
    auto cursor = pre_search(key);
    // return if word exists and is end of word
    return cursor && cursor->is_end_of_word;
}

bool Tries::starts_with(const string &prefix) const
{
    // return if prefix exists
    return pre_search(prefix) != nullptr;
}

const TrieNode *Tries::get_node(const string &prefix) const
{
    return pre_search(prefix);
}
