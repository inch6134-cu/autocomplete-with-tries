# Autocomplete Program using Trie Data Structure

## Project Overview

This project implements an autocomplete feature using a Trie data structure in C++. The program suggests words as the user types, providing a smooth and efficient autocomplete experience.

## Data Structure: Trie

### What is a Trie?

A Trie (pronounced "try") is a tree-like data structure used to store and retrieve strings. It's particularly efficient for prefix-based operations, making it ideal for autocomplete functionality.

### Key Features of Our Trie Implementation:

1. Each node represents a character in the alphabet.
2. Paths from the root to leaf nodes represent complete words.
3. Common prefixes are shared, saving space.
4. Each node stores a frequency value for ranking suggestions.

### Why Use a Trie for Autocomplete?

- Fast prefix matching: O(m) time complexity, where m is the length of the prefix.
- Space-efficient for storing words with common prefixes.
- Easily extendable for additional features like fuzzy matching.

## How to Run the Project

1. Ensure you have a C++ compiler installed (e.g., g++).
2. Clone the repository to your local machine.
3. Navigate to the build directory and make sure you have a dictionary saved as 'dictionary.txt'. The dictionary must have one word and one frequency number in each line, space delimited. (You can copy the dictionary.txt in the root folder into the build folder for demo purposes. Source: https://www.kaggle.com/datasets/rtatman/english-word-frequency?resource=download)
4. Compile the project:
   ```
   g++ -std=c++17 ../app/main.cpp ../code/autocomplete/autocomplete_engine.cpp ../code/tries/tries.cpp -o autocomplete
   ```
5. Run the compiled program:
   ```
   ./autocomplete
   ```
6. The program will load the dictionary and prompt you to press Enter to start.
7. Type to see autocomplete suggestions. Press ESC to quit.

## Features

- Real-time suggestions as you type
- Fuzzy matching for typo tolerance
- Ranking of suggestions based on word frequency and relevance
- Simple, intuitive command-line interface

## Project Structure

- `main.cpp`: Contains the user interface and input handling.
- `autocomplete_engine.cpp` and `.h`: Implements the autocomplete logic.
- `tries.cpp` and `.h`: Implements the Trie data structure.
- `dictionary.txt`: Contains words and their frequencies (not provided in the code snippets).

## How It Works

1. The program loads a dictionary of words and their frequencies into the Trie.
2. As the user types, the program searches the Trie for matching prefixes.
3. It performs a depth-first search (DFS) to find words that match the prefix.
4. Fuzzy matching is applied to suggest words even with slight misspellings.
5. Suggestions are scored based on prefix match, edit distance, and word frequency.
6. The top-ranked suggestions are displayed in real-time.

## Future Improvements

- Implement multi-threaded suggestion generation for improved performance.
- Add support for phrase completion and context-aware suggestions.
- Develop a graphical user interface for easier interaction.
