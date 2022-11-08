#include "print.hpp"

void Print::print() {
    std::cout << str << std::endl;
}

const std::string Print::getStr() const {
    return str;
}