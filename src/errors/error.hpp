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
            redefiningMOC,
            redefiningBOC,
            startDirectiveWasExpected,
            incorrectFormatOfStartRow,
            thereAreNotEnoughStartAndEndDirectives,
            labelOrMOCOrDirectiveWasExpected,
            MOCOrDirectiveWasExprcted,
            MOCWasExpected,
            oneByteCommandWasExpected,
            addressCounterOverflow
        };

        static inline const std::map<error, const QString> errorMessages {
            { error::fileCannotBeOpened, "Файл не может быть открыт" },
            { error::redefiningMOC, "Переопределение МКОП" },
            { error::redefiningBOC, "Переопределение ДКОП" },
            { error::labelOrMOCOrDirectiveWasExpected, "Ожидается: метка, или МКОП, или директива" },
            { error::MOCWasExpected, "Ожидается: МКОП" },
            { error::MOCOrDirectiveWasExprcted, "Ожидается: МКОП или директива" },
            { error::startDirectiveWasExpected, "Ожидается: start" },
            { error::incorrectFormatOfStartRow, "Неверный формат первой строки" },
            { error::thereAreNotEnoughStartAndEndDirectives, "Не хватает: start или end" },
            { error::oneByteCommandWasExpected, "Ожидается: однобайтная команда" },
            { error::addressCounterOverflow, "Переполнение счетчика адреса" }
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