#include "AdminDriver.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

AdminDriver::AdminDriver() : UserDriver(UserRole::ADMIN) {
}

std::string getFilenameForRole(UserRole role) {
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
            return "instructor_domains.txt";
    }
}

void AdminDriver::addDomain(const char* domain, UserRole role) {
    std::cout << "Adding domain to " <<
        (role == UserRole::STUDENT ? "Student" :
         role == UserRole::INSTRUCTOR ? "Instructor" :
         role == UserRole::TEACHING_ASSISTANT ? "TA" : "Admin")
         << " list: " << domain << std::endl;

    DomainTrie roleTrie;
    std::string filename = getFilenameForRole(role);
    roleTrie.loadAllowedFromFile(filename.c_str());

    if (roleTrie.searchReversed(domain)) {
        std::cout << "[INFO] Domain " << domain << " is already in the list.\n";
        return;
    }

    roleTrie.insertReversed(domain);

    std::ofstream file(filename);
    if (file.is_open()) {
        auto domains = roleTrie.getAllAllowedDomains();
        for (const auto& d : domains) {
            file << d << std::endl;
        }
        file.close();
        std::cout << "[SUCCESS] Domain " << domain << " added.\n";
    } else {
        std::cout << "[ERROR] Could not open file for writing\n";
    }
}

void AdminDriver::removeDomain(const char* domain, UserRole role) {
    std::string filename = getFilenameForRole(role);
    DomainTrie roleTrie;
    roleTrie.loadAllowedFromFile(filename.c_str());

    if (!roleTrie.searchReversed(domain)) {
        std::cout << "[INFO] Domain " << domain << " is not in the list.\n";
        return;
    }

    roleTrie.removeReversed(domain);

    std::ofstream file(filename);
    if (file.is_open()) {
        auto domains = roleTrie.getAllAllowedDomains();
        for (const auto& d : domains) {
            file << d << std::endl;
        }
        file.close();
        std::cout << "[SUCCESS] Domain " << domain << " removed.\n";
    }
}

void AdminDriver::printAllowedDomains(UserRole role) {
    std::cout << "--- Allowed Domains for ";
    switch(role) {
        case UserRole::STUDENT: std::cout << "Students"; break;
        case UserRole::INSTRUCTOR: std::cout << "Instructors"; break;
        case UserRole::TEACHING_ASSISTANT: std::cout << "TAs"; break;
        case UserRole::ADMIN: std::cout << "Admin"; break;
    }
    std::cout << " ---\n";

    DomainTrie roleTrie;
    std::string filename = getFilenameForRole(role);
    roleTrie.loadAllowedFromFile(filename.c_str());

    std::vector<std::string> domains = roleTrie.getAllAllowedDomains();

    if (domains.empty()) {
        std::cout << "No allowed domains found.\n";
    } else {
        for (const auto& domain : domains) {
            std::cout << "- " << domain << std::endl;
        }
        std::cout << "Total: " << domains.size() << " domains\n";
    }
}

void AdminDriver::saveAllowedDomainsToFile(UserRole role) {
    std::string filename = getFilenameForRole(role);
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "[ERROR] Could not open " << filename << " for writing" << std::endl;
        return;
    }

    DomainTrie roleTrie;
    roleTrie.loadAllowedFromFile(filename.c_str());
    std::vector<std::string> domains = roleTrie.getAllAllowedDomains();

    for (const auto& domain : domains) {
        file << domain << std::endl;
    }

    file.close();
    std::cout << "[SUCCESS] Saved " << domains.size() << " domains to " << filename << std::endl;
}