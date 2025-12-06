#ifndef DOMAIN_TRIE_HPP
#define DOMAIN_TRIE_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <string>

struct TrieNode {
    bool isEnd;
    TrieNode* children[128];
    TrieNode();
};

class DomainTrie {
public:
    DomainTrie();
    ~DomainTrie();
    void insertReversed(const char* domain);
    bool searchReversed(const char* domain);
    void removeReversed(const char* domain);
    void loadAllowedFromFile(const char* filename);
    void saveAllowedToFile(const char* filename);
    void printAll(TrieNode* node, std::string current);
    TrieNode* getRoot();
    std::vector<std::string> getAllAllowedDomains();
    void getAllAllowedDomains(std::vector<std::string>& domains, TrieNode* node, std::string current);
private:
    TrieNode* root;
    void reverseStr(char* str);
    void deleteTrie(TrieNode* node);
};

#endif
