#ifndef ADMIN_DRIVER_HPP
#define ADMIN_DRIVER_HPP

#include "UserDriver.hpp"

class AdminDriver : public UserDriver {
public:
    AdminDriver();
    void addDomain(const char* domain, UserRole role = UserRole::ADMIN);
    void removeDomain(const char* domain, UserRole role = UserRole::ADMIN);
    void printAllowedDomains(UserRole role = UserRole::ADMIN);
private:
    void saveAllowedDomainsToFile(UserRole role);
};

#endif