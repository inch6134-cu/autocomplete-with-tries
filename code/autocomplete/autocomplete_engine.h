#ifndef AUTOCOMPLETE_ENGINE_H
#define AUTOCOMPLETE_ENGINE_H

#include <string>
#include <vector>

using namespace std;

class AutocompleteEngine {
public:
    virtual vector<string> suggest(const string& prefix) = 0;
    virtual void insert(const string& word) = 0;
    virtual void remove(const string& word) = 0;
};

#endif  // AUTOCOMPLETE_ENGINE_H