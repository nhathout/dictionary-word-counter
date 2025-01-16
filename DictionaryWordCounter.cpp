#include "DictionaryWordCounter.h"
#include <unordered_set>
#include <fstream>
#include <string>
#include <memory>
#include <unordered_map>

using namespace std;

class TrieNode {
public:
    unordered_map<char, shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    shared_ptr<TrieNode> root;
public:
    Trie() : root(make_shared<TrieNode>()) {}
    void insert(const string& word) {
        auto node = root;
        for (char ch : word) {
            if (!node->children[ch]) node->children[ch] = make_shared<TrieNode>();
            node = node->children[ch];
        }
        node->isEndOfWord = true;
    }
    shared_ptr<TrieNode> getRoot() { return root; }
};

int countDictionaryWords() {
    unordered_set<string> dictionary;
    ifstream dictFile("dictionary.txt");

    if (!dictFile.is_open()) {
        cerr << "Unable to open dictionary file." << endl;
        return -1;
    }

    string word;
    char excludedLetter = 't';

    while (dictFile >> word) {
        if (word.back() != excludedLetter) dictionary.insert(word);
    }
    dictFile.close();

    Trie trie;
    for (const auto& w : dictionary) trie.insert(w);

    ifstream dataFile("BigData.txt");
    if (!dataFile.is_open()) {
        cerr << "Unable to open data file." << endl;
        return -1;
    }

    string line;
    int count = 0;
    while (getline(dataFile, line)) {
        for (size_t i = 0; i < line.length(); ++i) {
            auto node = trie.getRoot();
            for (size_t j = i; j < line.length(); ++j) {
                char ch = line[j];
                if (!node->children[ch]) break;
                node = node->children[ch];
                if (node->isEndOfWord) count++;
            }
        }
    }

    return count;
}
