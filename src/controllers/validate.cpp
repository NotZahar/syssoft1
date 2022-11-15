#include "validate.hpp"

namespace syssoft1 {
    bool Validate::containsNumber(const std::string& numberCandidate) {
        const std::regex numberRegex("^ *" + numberRegexStr + " *$");

        return std::regex_match(numberCandidate, numberRegex);
    }

    bool Validate::containsMOC(const std::string& MOCCandidate) {
        const std::regex MOCRegex("^ *" + MOCRegexStr + " *$");
        
        return std::regex_match(MOCCandidate, MOCRegex);
    }

    bool Validate::containsBOC(const std::string& BOCCandidate) {
        return containsNumber(BOCCandidate);
    }

    bool Validate::containsLength(const std::string& lengthCandidate) {
        return containsNumber(lengthCandidate);
    }
}