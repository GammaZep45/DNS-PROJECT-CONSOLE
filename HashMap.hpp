#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <iostream>

#define TABLE_SIZE 211

struct Entry {
    char domain[256];
    bool occupied;
    Entry() { occupied = false; domain[0] = '\0'; }
};

class HashMap {
private:
    Entry table[TABLE_SIZE];
    int hash(const char* domain);
public:
    void insert(const char* domain);
    bool exists(const char* domain);
};

#endif
