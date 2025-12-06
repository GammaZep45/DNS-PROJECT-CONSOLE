#include "DomainTrie.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

TrieNode::TrieNode() {
    isEnd = false;
    for (int i = 0; i < 128; i++) children[i] = nullptr;
}

DomainTrie::DomainTrie() {
    root = new TrieNode();
}

DomainTrie::~DomainTrie() {
    deleteTrie(root);
}

void DomainTrie::deleteTrie(TrieNode* node) {
    if (node == nullptr) return;
    for (int i = 0; i < 128; i++) {
        if (node->children[i] != nullptr) {
            deleteTrie(node->children[i]);
        }
    }
    delete node;
}

TrieNode* DomainTrie::getRoot() {
    return root;
}

void DomainTrie::reverseStr(char* str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        std::swap(str[i], str[n - i - 1]);
    }
}

void DomainTrie::getAllAllowedDomains(std::vector<std::string>& domains, TrieNode* node, std::string current) {
    if (node == nullptr) return;
    if (node->isEnd) {
        std::string reversed = current;
        std::reverse(reversed.begin(), reversed.end());
        domains.push_back(reversed);
    }
    for (int i = 0; i < 128; i++) {
        if (node->children[i]) {
            getAllAllowedDomains(domains, node->children[i], current + (char)i);
        }
    }
}

std::vector<std::string> DomainTrie::getAllAllowedDomains() {
    std::vector<std::string> domains;
    getAllAllowedDomains(domains, root, "");
    return domains;
}

void DomainTrie::saveAllowedToFile(const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open " << filename << " for writing.\n";
        return;
    }

    std::vector<std::string> domains = getAllAllowedDomains();
    for (const auto& domain : domains) {
        file << domain << std::endl;
    }
    file.close();
}

void DomainTrie::insertReversed(const char* domain) {
    char buffer[256];
    strcpy(buffer, domain);
    reverseStr(buffer);

    TrieNode* current = root;
    for (int i = 0; buffer[i] != '\0'; ++i) {
        unsigned char index = buffer[i];
        if (current->children[index] == nullptr)
            current->children[index] = new TrieNode();
        current = current->children[index];
    }
    current->isEnd = true;
}

bool DomainTrie::searchReversed(const char* domain) {
    char buffer[256];
    strcpy(buffer, domain);
    reverseStr(buffer);

    TrieNode* current = root;
    for (int i = 0; buffer[i] != '\0'; ++i) {
        unsigned char index = buffer[i];
        if (current->children[index] == nullptr)
            return false;
        current = current->children[index];
    }
    return current->isEnd;
}

void DomainTrie::removeReversed(const char* domain) {
    char buffer[256];
    strcpy(buffer, domain);
    reverseStr(buffer);
    TrieNode* current = root;
    for (int i = 0; buffer[i] != '\0'; ++i) {
        unsigned char index = buffer[i];
        if (current->children[index] == nullptr)
            return;
        current = current->children[index];
    }
    current->isEnd = false;
}

void DomainTrie::loadAllowedFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::ofstream createFile(filename);
        createFile.close();
        return;
    }
    char domain[256];
    while (file >> domain) {
        insertReversed(domain);
    }
    file.close();
}

void DomainTrie::printAll(TrieNode* node, std::string current) {
    if (node == nullptr) return;
    if (node->isEnd) std::cout << current << std::endl;
    for (int i = 0; i < 128; i++) {
        if (node->children[i]) {
            printAll(node->children[i], current + (char)i);
        }
    }
}