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
            // controllers errors
            fileCannotBeOpened,
            redefiningMOC,
            redefiningBOC,

            // translator errors
            startDirectiveWasExpected,
            startWasMet,
            endWasMet,
            incorrectFormatOfStartRow,
            incorrectFormatOfRow,
            thereAreNotEnoughStartAndEndDirectives,
            labelOrMOCOrDirectiveWasExpected,
            MOCOrDirectiveWasExprcted,
            MOCWasExpected,
            labelWasExpected,
            oneByteCommandWasExpected,
            numberWasExpected,
            addressCounterOverflow,
            symbolicNameAlreadyExists,
            wrongCommandLength,
            impossibleNumberOfTokens,
            wrongAddressingType,
            impossibleOperand,
            directiveMustHaveOperand,
            directiveMustHaveOnlyOneOperand,
            numberDoesNotFitInByte,
            numberDoesNotFitInWord,
            thereShouldBeNoLabelInThisLine
        };

        static inline const std::map<error, const QString> errorMessages {
            // controllers errors
            { error::fileCannotBeOpened, "Файл не может быть открыт" },
            { error::redefiningMOC, "Переопределение МКОП" },
            { error::redefiningBOC, "Переопределение ДКОП" },

            // translator errors
            { error::labelOrMOCOrDirectiveWasExpected, "Ожидается: метка, или МКОП, или директива" },
            { error::MOCWasExpected, "Ожидается: МКОП" },
            { error::labelWasExpected, "Ожидается: метка" },
            { error::MOCOrDirectiveWasExprcted, "Ожидается: МКОП или директива" },
            { error::startDirectiveWasExpected, "Ожидается: start" },
            { error::startWasMet, "Директива start ранее встречалась" },
            { error::endWasMet, "Директива end ранее встречалась" },
            { error::incorrectFormatOfStartRow, "Неверный формат первой строки" },
            { error::incorrectFormatOfRow, "Неверный формат строки" },
            { error::thereAreNotEnoughStartAndEndDirectives, "Не хватает: start или end" },
            { error::oneByteCommandWasExpected, "Ожидается: однобайтная команда" },
            { error::numberWasExpected, "Ожидается: число" },
            { error::addressCounterOverflow, "Переполнение счетчика адреса" },
            { error::symbolicNameAlreadyExists, "Символическое имя уже существует" },
            { error::wrongCommandLength, "Задана неверная длина команды" },
            { error::impossibleNumberOfTokens, "Превышено количество обрабатываемых токенов" },
            { error::wrongAddressingType, "Указан неправильный тип адресации" },
            { error::impossibleOperand, "Указан некорректный операнд" },
            { error::directiveMustHaveOperand, "Директива должна иметь операнд" },
            { error::directiveMustHaveOnlyOneOperand, "Директива должна иметь только 1 операнд" },
            { error::numberDoesNotFitInByte, "Число не помещается в байт" },
            { error::numberDoesNotFitInWord, "Число не помещается в слово" },
            { error::thereShouldBeNoLabelInThisLine, "В этой строке не должно быть метки" }
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