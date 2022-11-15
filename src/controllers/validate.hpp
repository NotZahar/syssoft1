#include <regex>
#include <string>

namespace syssoft1 {
    class Validate {
    public:
        static inline const std::string decNumberRegexStr = "[0-9]{1,9}";
        static inline const std::string hexNumberRegexStr = "0x[0-9a-f]{1,7}";
        static inline const std::string numberRegexStr = "(" + hexNumberRegexStr + "|" + decNumberRegexStr + ")";
        static inline const std::string MOCRegexStr = "[a-z]+[0-9]*";
        static inline const std::string BOCRegexStr = numberRegexStr;
        static inline const std::string lengthRegexStr = numberRegexStr;

    public:
        Validate() = default;
        ~Validate() = default;

        static bool containsNumber(const std::string& numberCandidate);
        static bool containsMOC(const std::string& MOCCandidate);
        static bool containsBOC(const std::string& BOCCandidate);
        static bool containsLength(const std::string& lengthCandidate);
    };
}
