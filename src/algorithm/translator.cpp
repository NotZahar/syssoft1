#include "translator.hpp"

namespace syssoft1 {
    Translator::Translator() {
        addressCounter = 1;
        loadAddress = 2;
    }

    Translator::~Translator() {

    }

    void Translator::firstPass(std::string _source, std::map<std::string, std::tuple<int, int>> _OCT) {
        _source = "";
        _OCT = {};
    }

    void Translator::secondPass() {

    }
}
