#include "../code/tries.h"
#include <cassert>
#include <iostream>

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

int main() {
    test_insert_and_search();
    test_prefix_search();
    test_delete();

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}