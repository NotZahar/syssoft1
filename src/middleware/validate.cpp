#include "validate.hpp"

namespace syssoft1 {
    bool Validate::containsNumber(const std::string& _numberCandidate) {
        const std::regex numberRegex("^ *" + numberRegexStr + " *$");

        return std::regex_match(_numberCandidate, numberRegex);
    }

    bool Validate::containsMOC(const std::string& _MOCCandidate) {
        const std::regex MOCRegex("^ *" + MOCRegexStr + " *$");
        
        return std::regex_match(_MOCCandidate, MOCRegex);
    }

    bool Validate::containsBOC(const std::string& _BOCCandidate) {
        const std::regex BOCRegex("^ *" + BOCRegexStr + " *$");

        return std::regex_match(_BOCCandidate, BOCRegex);
    }

    bool Validate::containsLength(const std::string& _lengthCandidate) {
        const std::regex lengthRegex("^ *" + lengthRegexStr + " *$"); 

        return std::regex_match(_lengthCandidate, lengthRegex);
    }
}