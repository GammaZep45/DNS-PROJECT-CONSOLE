#include "HashMap.hpp"
#include <cstring>

int HashMap::hash(const char* domain) {
    int sum = 0;
    for (int i = 0; domain[i] != '\0'; ++i)
        sum = (sum * 31 + domain[i]) % TABLE_SIZE;
    return sum;
}

void HashMap::insert(const char* domain) {
    int index = hash(domain);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int pos = (index + i) % TABLE_SIZE;
        if (!table[pos].occupied) {
            strcpy(table[pos].domain, domain);
            table[pos].occupied = true;
            return;
        }
    }
}

bool HashMap::exists(const char* domain) {
    int index = hash(domain);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int pos = (index + i) % TABLE_SIZE;
        if (table[pos].occupied && strcmp(table[pos].domain, domain) == 0)
            return true;
    }
    return false;
}
