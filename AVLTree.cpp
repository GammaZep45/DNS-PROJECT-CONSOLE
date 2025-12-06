#include "AVLTree.hpp"
#include <cstring>
#include <algorithm>

AVLNode::AVLNode(const char* d, bool a) {
    strcpy(domain, d);
    allowed = a;
    left = right = nullptr;
    height = 1;
}

AVLTree::AVLTree() { root = nullptr; }

int AVLTree::height(AVLNode* n) { return n ? n->height : 0; }

int AVLTree::getBalance(AVLNode* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + std::max(height(y->left), height(y->right));
    x->height = 1 + std::max(height(x->left), height(x->right));
    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));
    return y;
}

AVLNode* AVLTree::insert(AVLNode* node, const char* domain, bool allowed) {
    if (!node) return new AVLNode(domain, allowed);
    int cmp = strcmp(domain, node->domain);
    if (cmp < 0)
        node->left = insert(node->left, domain, allowed);
    else if (cmp > 0)
        node->right = insert(node->right, domain, allowed);
    else
        node->allowed = allowed;
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);
    if (balance > 1 && strcmp(domain, node->left->domain) < 0) return rotateRight(node);
    if (balance < -1 && strcmp(domain, node->right->domain) > 0) return rotateLeft(node);
    if (balance > 1 && strcmp(domain, node->left->domain) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && strcmp(domain, node->right->domain) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

void AVLTree::insert(const char* domain, bool allowed) { root = insert(root, domain, allowed); }

AVLNode* AVLTree::search(AVLNode* node, const char* domain) {
    if (!node) return nullptr;
    int cmp = strcmp(domain, node->domain);
    if (cmp == 0) return node;
    if (cmp < 0) return search(node->left, domain);
    return search(node->right, domain);
}

bool AVLTree::search(const char* domain, bool& allowed) {
    AVLNode* n = search(root, domain);
    if (n) { allowed = n->allowed; return true; }
    return false;
}

void AVLTree::inorderPrint(AVLNode* node) {
    if (!node) return;
    inorderPrint(node->left);
    std::cout << node->domain << " -> " << (node->allowed ? "Allowed" : "Blocked") << std::endl;
    inorderPrint(node->right);
}

void AVLTree::printAllowed(AVLNode* node) {
    if (!node) return;
    printAllowed(node->left);
    if (node->allowed) std::cout << node->domain << std::endl;
    printAllowed(node->right);
}

void AVLTree::printAll() { inorderPrint(root); }
void AVLTree::printAllowed() { printAllowed(root); }

void AVLTree::writeAllowedToFile(AVLNode* node, std::ofstream& file) {
    if (!node) return;
    writeAllowedToFile(node->left, file);
    if (node->allowed) file << node->domain << std::endl;
    writeAllowedToFile(node->right, file);
}

void AVLTree::writeAllowedToFile(const char* filename) {
    std::ofstream file(filename, std::ios::trunc);
    if (file.is_open()) {
        writeAllowedToFile(root, file);
        file.close();
    }
}
