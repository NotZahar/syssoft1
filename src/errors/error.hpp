#ifndef ERROR_H
#define ERROR_H

#include <QString>

#include <map>
#include <string>

namespace syssoft1 {
    class Error {
    private:

    public:
        Error() = delete;
        Error(const Error&) = delete;
        Error(Error&&) = delete;

        ~Error() = default;

        enum class error : int {
            fileCannotBeOpened
        };

        static inline const std::map<error, const QString> errorMessages {
            {error::fileCannotBeOpened, "Файл не может быть открыт"}
        };
    };
}

#endif // ERROR_H