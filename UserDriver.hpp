#ifndef USER_DRIVER_HPP
#define USER_DRIVER_HPP

#include "DomainTrie.hpp"
#include "HashMap.hpp"
#include "AVLTree.hpp"
#include "UserType.hpp"
#include <string>

class UserDriver {
protected:
    DomainTrie trie;
    HashMap cache;
    AVLTree log;
    UserRole role;

    std::string getAllowedDomainsFilename() const;

public:
    UserDriver(UserRole userRole);
    void searchDomain(const char* domain);
    UserRole getRole() const { return role; }
};

#endif
