#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <string>
#include <map>
#include <tuple>

namespace syssoft1 {
    class Translator {
    private:
        std::map<std::string, std::tuple<int, int>> OCT;
        std::map<std::string, int> SNT;
        [[maybe_unused]] int addressCounter;
        [[maybe_unused]] int loadAddress;

    public:
        Translator();
        ~Translator();

        void firstPass();
        void secondPass();
    };
}

#endif // TRANSLATOR_H