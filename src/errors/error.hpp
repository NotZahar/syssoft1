#ifndef ERROR_H
#define ERROR_H

#include <QString>

#include <map>

namespace syssoft1 {
    struct Error {
        Error() = default;
        Error(const Error&) = delete;
        Error(Error&&) = delete;

        ~Error() = default;

        enum class error : int {
            fileCannotBeOpened,
            labelOrMOCOrDirectiveWasExpected,
            MOCOrDirectiveWasExprcted,
            MOCWasExpected
        };

        static inline const std::map<error, const QString> errorMessages {
            { error::fileCannotBeOpened, "Файл не может быть открыт" },
            { error::labelOrMOCOrDirectiveWasExpected, "Ожидается: метка, или МКОП, или директива" },
            { error::MOCWasExpected, "Ожидается: МКОП" }
        };
    };

    template <class T>
    struct ErrorData {
        T data;
        Error::error err;

        ErrorData() = delete;
        explicit ErrorData(T _data, Error::error _err) : 
            data(_data),
            err(_err) 
        {

        }

        ~ErrorData() = default;
    };
}

#endif // ERROR_H