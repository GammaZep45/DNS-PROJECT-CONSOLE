#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <limits>
#include <map>
#include "AdminDriver.hpp"
#include "UserDriver.hpp"
#include "UserType.hpp"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::map<std::string, std::pair<std::string, UserRole>> userCredentials = {
    {"student1", {"stu123", UserRole::STUDENT}},
    {"student2", {"stu456", UserRole::STUDENT}},
    {"instructor1", {"prof789", UserRole::INSTRUCTOR}},
    {"instructor2", {"prof012", UserRole::INSTRUCTOR}},
    {"ta1", {"ta345", UserRole::TEACHING_ASSISTANT}},
    {"ta2", {"ta678", UserRole::TEACHING_ASSISTANT}}
};

void adminMenu();
void userLogin();

UserRole authenticateUser(bool& success) {
    std::string username, password;

    std::cout << "\n=== USER LOGIN ===" << std::endl;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (username == "admin" && password == "cracker0015") {
        std::cout << "[SUCCESS] Admin login successful!" << std::endl;
        success = true;
        return UserRole::ADMIN;
    }

    auto it = userCredentials.find(username);
    if (it != userCredentials.end() && it->second.first == password) {
        std::cout << "[SUCCESS] Login successful! Welcome ";
        switch(it->second.second) {
            case UserRole::STUDENT: std::cout << "Student"; break;
            case UserRole::INSTRUCTOR: std::cout << "Instructor"; break;
            case UserRole::TEACHING_ASSISTANT: std::cout << "Teaching Assistant"; break;
            default: std::cout << "User";
        }
        std::cout << "!" << std::endl;
        success = true;
        return it->second.second;
    }

    std::cout << "[ERROR] Invalid username or password!" << std::endl;
    success = false;
    return UserRole::STUDENT;
}

bool authenticateAdmin() {
    std::string password;
    std::cout << "\n=== ADMIN LOGIN ===" << std::endl;
    std::cout << "Enter admin password: ";
    std::cin >> password;

    if (password == "cracker0015") {
        std::cout << "[SUCCESS] Admin access granted!" << std::endl;
        return true;
    }

    std::cout << "[ERROR] Incorrect admin password!" << std::endl;
    return false;
}

void userMenu(UserRole role) {
    UserDriver user(role);
    int choice = -1;

    std::cout << "\n=== ";
    switch(role) {
        case UserRole::STUDENT: std::cout << "STUDENT"; break;
        case UserRole::INSTRUCTOR: std::cout << "INSTRUCTOR"; break;
        case UserRole::TEACHING_ASSISTANT: std::cout << "TEACHING ASSISTANT"; break;
        default: std::cout << "USER";
    }
    std::cout << " MENU ===" << std::endl;

    while (true) {
        std::cout << "\n1. Search domain" << std::endl;
        std::cout << "2. Switch to Admin Mode" << std::endl;
        std::cout << "3. Logout" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter operation: ";

        // Read input as string first
        std::string input;
        std::cin >> input;

        // Check if input is a valid number
        bool isValidNumber = true;
        for (char c : input) {
            if (!std::isdigit(c)) {
                isValidNumber = false;
                break;
            }
        }

        if (!isValidNumber) {
            std::cout << "[ERROR] Invalid input! Please enter 1, 2, 3, or 0." << std::endl;
            clearInputBuffer();
            continue; // Stay in menu
        }

        // Convert to integer
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "[ERROR] Invalid input! Please enter 1, 2, 3, or 0." << std::endl;
            clearInputBuffer();
            continue;
        }

        // Validate choice
        if (choice != 1 && choice != 2 && choice != 3 && choice != 0) {
            std::cout << "[ERROR] Invalid option! Please choose 1, 2, 3, or 0." << std::endl;
            clearInputBuffer();
            continue;
        }

        char domain[256];
        switch(choice) {
            case 1:
                std::cout << "Enter domain to search: ";
                std::cin >> domain;
                user.searchDomain(domain);
                clearInputBuffer();
                break;
            case 2:
                std::cout << "Switching to Admin Mode..." << std::endl;
                clearInputBuffer();
                adminMenu();
                return;
            case 3:
                std::cout << "Logging out..." << std::endl;
                clearInputBuffer();
                return;
            case 0:
                std::cout << "Exiting..." << std::endl;
                exit(0);
        }
    }
}

void userLogin() {
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    while (attempts < MAX_ATTEMPTS) {
        bool loginSuccess = false;
        UserRole role = authenticateUser(loginSuccess);

        if (loginSuccess) {
            if (role == UserRole::ADMIN) {
                adminMenu();
            } else {
                userMenu(role);
            }
            return;
        }

        attempts++;
        if (attempts < MAX_ATTEMPTS) {
            std::cout << "\n[INFO] Attempt " << attempts << " of " << MAX_ATTEMPTS << " failed." << std::endl;
            std::cout << "1. Try login again" << std::endl;
            std::cout << "2. Continue as Guest (Student access)" << std::endl;
            std::cout << "0. Return to main menu" << std::endl;
            std::cout << "Enter choice: ";

            int choice;
            std::cin >> choice;
            clearInputBuffer();

            if (choice == 0) {
                return;
            } else if (choice == 2) {
                std::cout << "[INFO] Continuing as Guest with Student access." << std::endl;
                userMenu(UserRole::STUDENT);
                return;
            }
        }
    }

    std::cout << "\n[ERROR] Maximum login attempts (" << MAX_ATTEMPTS << ") reached!" << std::endl;
    std::cout << "1. Continue as Guest (Student access)" << std::endl;
    std::cout << "0. Return to main menu" << std::endl;
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    clearInputBuffer();

    if (choice == 1) {
        std::cout << "[INFO] Continuing as Guest with Student access." << std::endl;
        userMenu(UserRole::STUDENT);
    }
}

void adminMenu() {
    if (!authenticateAdmin()) {
        std::cout << "\n[OPTIONS]" << std::endl;
        std::cout << "1. Try admin login again" << std::endl;
        std::cout << "2. Return to main menu" << std::endl;
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;
        clearInputBuffer();

        if (choice == 1) {
            adminMenu();
        }
        return;
    }

    AdminDriver admin;
    int n = -1;

    std::cout << "\n=== ADMIN MENU ===" << std::endl;

    while (true) {
        std::cout << "\n1. Add domain to Student list" << std::endl;
        std::cout << "2. Add domain to Instructor list" << std::endl;
        std::cout << "3. Add domain to TA list" << std::endl;
        std::cout << "4. Remove domain from list" << std::endl;
        std::cout << "5. Print Student domains" << std::endl;
        std::cout << "6. Print Instructor domains" << std::endl;
        std::cout << "7. Print TA domains" << std::endl;
        std::cout << "8. Switch to User Mode" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter operation: ";

        // Read input as string first
        std::string input;
        std::cin >> input;

        // Check if input is a valid number
        bool isValidNumber = true;
        for (char c : input) {
            if (!std::isdigit(c)) {
                isValidNumber = false;
                break;
            }
        }

        if (!isValidNumber) {
            std::cout << "[ERROR] Invalid input! Please enter a number (1-8 or 0)." << std::endl;
            clearInputBuffer();
            continue;
        }

        // Convert to integer
        try {
            n = std::stoi(input);
        } catch (...) {
            std::cout << "[ERROR] Invalid input! Please enter a number (1-8 or 0)." << std::endl;
            clearInputBuffer();
            continue;
        }

        if (n < 0 || n > 8) {
            std::cout << "[ERROR] Invalid option! Please choose 1-8 or 0." << std::endl;
            clearInputBuffer();
            continue;
        }

        char domain[256];
        switch(n) {
            case 1:
                std::cout << "Domain to add to Student list: ";
                std::cin >> domain;
                admin.addDomain(domain, UserRole::STUDENT);
                clearInputBuffer();
                break;
            case 2:
                std::cout << "Domain to add to Instructor list: ";
                std::cin >> domain;
                admin.addDomain(domain, UserRole::INSTRUCTOR);
                clearInputBuffer();
                break;
            case 3:
                std::cout << "Domain to add to TA list: ";
                std::cin >> domain;
                admin.addDomain(domain, UserRole::TEACHING_ASSISTANT);
                clearInputBuffer();
                break;
            case 4:
                std::cout << "Remove domain from which list? (1=Student, 2=Instructor, 3=TA): ";
                int listChoice;
                std::cin >> listChoice;
                clearInputBuffer();
                UserRole removeRole;
                switch(listChoice) {
                    case 1: removeRole = UserRole::STUDENT; break;
                    case 2: removeRole = UserRole::INSTRUCTOR; break;
                    case 3: removeRole = UserRole::TEACHING_ASSISTANT; break;
                    default: removeRole = UserRole::STUDENT;
                }
                std::cout << "Domain to remove: ";
                std::cin >> domain;
                admin.removeDomain(domain, removeRole);
                clearInputBuffer();
                break;
            case 5:
                admin.printAllowedDomains(UserRole::STUDENT);
                clearInputBuffer();
                break;
            case 6:
                admin.printAllowedDomains(UserRole::INSTRUCTOR);
                clearInputBuffer();
                break;
            case 7:
                admin.printAllowedDomains(UserRole::TEACHING_ASSISTANT);
                clearInputBuffer();
                break;
            case 8:
                std::cout << "Switching to User Mode..." << std::endl;
                clearInputBuffer();
                userLogin();
                return;
            case 0:
                std::cout << "Exiting..." << std::endl;
                exit(0);
        }
    }
}

int main() {
    int mode = -1;

    while (true) {
        std::cout << "\n=== DNS FIREWALL CONSOLE ===" << std::endl;
        std::cout << "1. User Login" << std::endl;
        std::cout << "2. Admin Login" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter mode: ";

        // Read input as string first
        std::string input;
        std::cin >> input;

        // Check if input is a valid number
        bool isValidNumber = true;
        for (char c : input) {
            if (!std::isdigit(c)) {
                isValidNumber = false;
                break;
            }
        }

        if (!isValidNumber) {
            std::cout << "[ERROR] Invalid input! Please enter 1, 2, or 0." << std::endl;
            clearInputBuffer();
            continue;
        }

        // Convert to integer
        try {
            mode = std::stoi(input);
        } catch (...) {
            std::cout << "[ERROR] Invalid input! Please enter 1, 2, or 0." << std::endl;
            clearInputBuffer();
            continue;
        }

        if (mode != 1 && mode != 2 && mode != 0) {
            std::cout << "[ERROR] Invalid mode! Please enter 1, 2, or 0." << std::endl;
            clearInputBuffer();
            continue;
        }

        switch(mode) {
            case 1:
                userLogin();
                break;
            case 2:
                adminMenu();
                break;
            case 0:
                std::cout << "Goodbye!" << std::endl;
                return 0;
        }
    }
}