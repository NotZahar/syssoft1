#ifndef VALIDATE_H
#define VALIDATE_H

#include <regex>
#include <string>

namespace syssoft1 {
    struct Validate {
        static inline const std::string decNumberRegexStr = "[0-9]{1,9}";
        static inline const std::string hexNumberRegexStr = "0x[0-9a-f]{1,7}";
        static inline const std::string numberRegexStr = "(" + hexNumberRegexStr + "|" + decNumberRegexStr + ")";
        static inline const std::string MOCRegexStr = "[a-z]+[0-9]*";
        static inline const std::string BOCRegexStr = numberRegexStr;
        static inline const std::string lengthRegexStr = numberRegexStr;

        Validate() = default;
        ~Validate() = default;

        static bool containsNumber(const std::string& _numberCandidate);
        static bool containsMOC(const std::string& _MOCCandidate);
        static bool containsBOC(const std::string& _BOCCandidate);
        static bool containsLength(const std::string& _lengthCandidate);
    };
}

#endif // VALIDATE_H