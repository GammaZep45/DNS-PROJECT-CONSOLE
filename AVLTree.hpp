#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <fstream>

struct AVLNode {
    char domain[256];
    bool allowed;
    AVLNode *left, *right;
    int height;
    AVLNode(const char* d, bool a);
};

class AVLTree {
private:
    AVLNode* root;
    int height(AVLNode* n);
    int getBalance(AVLNode* n);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* insert(AVLNode* node, const char* domain, bool allowed);
    AVLNode* search(AVLNode* node, const char* domain);
    void inorderPrint(AVLNode* node);
    void printAllowed(AVLNode* node);
    void writeAllowedToFile(AVLNode* node, std::ofstream& file);
public:
    AVLTree();
    void insert(const char* domain, bool allowed);
    bool search(const char* domain, bool& allowed);
    void printAll();
    void printAllowed();
    void writeAllowedToFile(const char* filename);
};

#endif
