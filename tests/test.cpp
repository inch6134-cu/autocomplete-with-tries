#include "../code/tries/tries.h"
#include "../code/autocomplete/trie_autocomplete_engine.h"
#include <cassert>
#include <iostream>
#include <algorithm>

void test_insert_and_search() {
    Tries trie;
    trie.insert_node(trie.root, "hello");
    trie.insert_node(trie.root, "world");
    trie.insert_node(trie.root, "hell");

    assert(trie.search(trie.root, "hello") == true);
    assert(trie.search(trie.root, "world") == true);
    assert(trie.search(trie.root, "hell") == true);
    assert(trie.search(trie.root, "hel") == false);
    assert(trie.search(trie.root, "helloo") == false);
    
    std::cout << "Insert and search tests passed." << std::endl;
}

void test_prefix_search() {
    Tries trie;
    trie.insert_node(trie.root, "hello");
    trie.insert_node(trie.root, "hell");
    trie.insert_node(trie.root, "heaven");

    assert(trie.pre_search(trie.root, "hel") == true);
    assert(trie.pre_search(trie.root, "hell") == true);
    assert(trie.pre_search(trie.root, "hello") == true);
    assert(trie.pre_search(trie.root, "hea") == true);
    assert(trie.pre_search(trie.root, "world") == false);
    
    std::cout << "Prefix search tests passed." << std::endl;
}

void test_delete() {
    Tries trie;
    trie.insert_node(trie.root, "hello");
    trie.insert_node(trie.root, "hell");
    trie.insert_node(trie.root, "heaven");

    assert(trie.delete_node(trie.root, "hello") == true);
    assert(trie.search(trie.root, "hello") == false);
    assert(trie.search(trie.root, "hell") == true);
    assert(trie.pre_search(trie.root, "hel") == true);

    assert(trie.delete_node(trie.root, "hell") == true);
    assert(trie.search(trie.root, "hell") == false);
    assert(trie.pre_search(trie.root, "hel") == false);

    assert(trie.delete_node(trie.root, "heaven") == true);
    assert(trie.search(trie.root, "heaven") == false);
    assert(trie.pre_search(trie.root, "h") == false);

    assert(trie.delete_node(trie.root, "nonexistent") == false);
    
    std::cout << "Delete tests passed." << std::endl;
}

void test_autocomplete_suggest() {
    Tries trie;
    TrieAutocompleteEngine engine(&trie);

    engine.insert("hello");
    engine.insert("hell");
    engine.insert("hello world");
    engine.insert("heaven");

    std::vector<std::string> suggestions = engine.suggest("he");
    assert(suggestions.size() == 3);
    std::vector<std::string> expected_suggestions = {"hello", "hell", "heaven"};
    assert(std::is_permutation(suggestions.begin(), suggestions.end(), expected_suggestions.begin()));

    suggestions = engine.suggest("hell");
    assert(suggestions.size() == 2);
    expected_suggestions = {"hello", "hell"};
    assert(std::is_permutation(suggestions.begin(), suggestions.end(), expected_suggestions.begin()));

    suggestions = engine.suggest("hello");
    assert(suggestions.size() == 2);
    expected_suggestions = {"hello", "hello world"};
    assert(std::is_permutation(suggestions.begin(), suggestions.end(), expected_suggestions.begin()));

    std::cout << "Autocomplete suggest tests passed." << std::endl;
}

void test_autocomplete_insert_and_remove() {
    Tries trie;
    TrieAutocompleteEngine engine(&trie);

    engine.insert("hello");
    engine.insert("hell");
    engine.insert("heaven");

    std::vector<std::string> suggestions = engine.suggest("he");
    assert(suggestions.size() == 3);

    engine.remove("hello");
    suggestions = engine.suggest("he");
    assert(suggestions.size() == 2);
    std::vector<std::string> expected_suggestions = {"hell", "heaven"};
    assert(std::is_permutation(suggestions.begin(), suggestions.end(), expected_suggestions.begin()));

    engine.remove("hell");
    suggestions = engine.suggest("he");
    assert(suggestions.size() == 1);
    expected_suggestions = {"heaven"};
    assert(std::is_permutation(suggestions.begin(), suggestions.end(), expected_suggestions.begin()));

    engine.insert("hello");
    suggestions = engine.suggest("he");
    assert(suggestions.size() == 2);
    expected_suggestions = {"hello", "heaven"};
    assert(std::is_permutation(suggestions.begin(), suggestions.end(), expected_suggestions.begin()));

    std::cout << "Autocomplete insert and remove tests passed." << std::endl;
}

int main() {
    test_insert_and_search();
    test_prefix_search();
    test_delete();

    test_autocomplete_suggest();
    test_autocomplete_insert_and_remove();

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}