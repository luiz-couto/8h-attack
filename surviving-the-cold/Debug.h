#pragma once

#include <iostream>
#include <string>

namespace Debug {
    void print(int value) {
        std::cout << value << std::endl;
    }

    void printBool(bool value) {
        std::string message = "true";
        if (!value) message = "false";
        std::cout << message << std::endl;
    }

    void log(const std::string& message) {
        std::cout << message << std::endl;
    }
}