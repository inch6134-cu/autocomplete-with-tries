#include "../code/tries/tries.h"
#include "../code/autocomplete/autocomplete_engine.h"
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

void testCaseInsensitiveAutocomplete() {
    Tries* trie = new Tries();
    AutocompleteEngine engine(trie);
    
    // Test cases
    vector<pair<string, int>> words = {
        {"Apple", 5},
        {"application", 10},
        {"Banana", 3},
        {"BOOK", 7},
        {"cat", 4},
        {"CAT", 6},
        {"dog", 8}
    };
    
    // Insert words
    for (auto& [word, freq] : words) {
        engine.insert(word);
    }
    
    // Test cases
    vector<pair<string, int>> testCases = {
        {"a", 3},  // Should return Apple, application, (possibly CAT if it's merged with cat)
        {"A", 3},  // Same as above
        {"app", 2},  // Should return Apple, application
        {"APP", 2},  // Same as above
        {"c", 2},  // Should return CAT (or cat, depending on how you handle duplicates)
        {"C", 2},  // Same as above
        {"b", 2},  // Should return Banana, BOOK
        {"BO", 1},  // Should return BOOK
        {"bo", 1},  // Should return BOOK
    };
    
    // Run tests
    for (auto& [prefix, expectedCount] : testCases) {
        cout << "Testing prefix: " << prefix << endl;
        auto suggestions = engine.suggest(prefix);
        cout << "Got " << suggestions.size() << " suggestions (expected " << expectedCount << "):" << endl;
        for (const auto& suggestion : suggestions) {
            cout << "  " << suggestion.word << " (freq: " << suggestion.frequency << ")" << endl;
        }
        cout << endl;
    }
    
    delete trie;
}

int main() {
    test_insert_and_search();
    test_prefix_search();
    test_delete();

    testCaseInsensitiveAutocomplete();

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}