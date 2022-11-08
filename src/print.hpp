#include <iostream>
#include <string>

class Print {
private:
    const std::string str;

public:
    Print() = delete;
    Print(const std::string& s) : str(s) {

    }

    ~Print() = default;

    void print();
    const std::string getStr() const;
};