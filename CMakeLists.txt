cmake_minimum_required(VERSION 3.27)
project(dns_firewall_console)

set(CMAKE_CXX_STANDARD 17)

add_executable(dns_firewall_console
        main.cpp
        AdminDriver.cpp
        UserDriver.cpp
        DomainTrie.cpp
        CacheTrie.cpp
        AVL_Log.cpp
        allowed_domains.txt
)
