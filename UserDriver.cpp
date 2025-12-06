#include "UserDriver.hpp"
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

UserDriver::UserDriver(UserRole userRole) : role(userRole) {
    std::string filename = getAllowedDomainsFilename();
    trie.loadAllowedFromFile(filename.c_str());
}

std::string UserDriver::getAllowedDomainsFilename() const {
    switch(role) {
        case UserRole::STUDENT:
            return "student_domains.txt";
        case UserRole::INSTRUCTOR:
            return "instructor_domains.txt";
        case UserRole::TEACHING_ASSISTANT:
            return "ta_domains.txt";
        case UserRole::ADMIN:
            return "instructor_domains.txt";
        default:
            return "default_domains.txt";
    }
}

bool isTypo(const std::string& a, const std::string& b) {
    int n = a.size(), m = b.size();
    if (std::abs(n - m) > 1) return false;

    int i = 0, j = 0, mistakes = 0;

    while (i < n && j < m) {
        if (a[i] != b[j]) {
            mistakes++;
            if (mistakes > 1) return false;
            if (n > m) i++;
            else if (m > n) j++;
            else { i++; j++; }
        } else {
            i++; j++;
        }
    }
    return true;
}

std::vector<std::string> findSimilarDomains(const std::string& domain, const std::vector<std::string>& allowedList) {
    std::vector<std::string> similar;
    for (const auto& allowed : allowedList) {
        if (isTypo(domain, allowed)) {
            similar.push_back(allowed);
        }
    }
    return similar;
}

void UserDriver::searchDomain(const char* domain) {
    std::string domainStr(domain);
    bool isAllowed = trie.searchReversed(domain);

    if (cache.exists(domain)) {
        if (isAllowed) {
            std::cout << domain << " -> ALLOWED (Cached)\n";
            std::cout << "You may now visit " << domain << "\n";
        } else {
            std::cout << domain << " -> BLOCKED (Cached)\n";
            std::cout << "Access denied. This domain is not in your allowed list.\n";
        }
        return;
    }

    cache.insert(domain);
    log.insert(domain, isAllowed);

    if (isAllowed) {
        std::cout << domain << " -> ALLOWED\n";
        std::cout << "You may now visit " << domain << "\n";
    } else {
        std::cout << domain << " -> BLOCKED\n";
        std::cout << "Access denied. This domain is not in your allowed list.\n";

        auto allowedDomains = trie.getAllAllowedDomains();
        auto similar = findSimilarDomains(domainStr, allowedDomains);

        if (!similar.empty()) {
            std::cout << "Did you mean one of these?\n";
            for (const auto& suggestion : similar) {
                std::cout << "  • " << suggestion << "\n";
            }
        }
    }
}