#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <tuple>

#include "../errors/error.hpp"

namespace syssoft1 {
    // for an unambiguous description of the command (2 args - max):
    // 1b: 0 arg(s) (none)
    // 2b: 2 arg(s) (reg1, reg2)
    // 3b: 1 arg(s) (number or string (<= 0xffff))
    // 4b: 1 arg(s) (label)

    // "word" doesn't work with strings

    class Translator {
    private:
        std::map<QString, std::tuple<int, int>> OCT;
        std::map<QString, int> SNT;
        const std::set<QString> directivesList;
        const std::set<QString> registersList;
        std::vector<std::vector<QString>> intermediateRepresentation;
        const int maximumNumberOfCommandArgs;
        const int maximumAddress;
        const int maximumRecordLength;
        const int maximumNumberByte;
        const int maximumNumberWord;
        const int maximumNumberIn3bCommand;
        const int maximumNumberOfHexadecimalDigitsForAddress;
        const int maximumNumberOfHexadecimalDigitsForCommand;
        const int impossibleNonnegativeIntegerValue;
        const QString impossibleProgramName;
        int addressCounter;
        int loadAddress;
        int firstNonEmptyRowNumber;
        QString programName;

        const QRegularExpression whitespacesSplitRegex;
        const QRegularExpression labelRegex;
        const QRegularExpression MOCRegex;
        const QRegularExpression directiveRegex;
        const QRegularExpression operandRegex;
        const QRegularExpression programNameRegex;
        const QRegularExpression startDirectiveRegex;
        const QRegularExpression loadAddressRegex;
        const QRegularExpression endDirectiveRegex;
        const QRegularExpression numberRegex;
        const QRegularExpression stringRegex;
        const QRegularExpression xStringRegex;
        const QRegularExpression cStringRegex;
        const QRegularExpression xString3bRegex;
        const QRegularExpression cString3bRegex;

        enum class addressingType : int {
            none = -1,
            immediate,
            direct
        };

    private:
        QStringList deleteEmptyTokens(const QStringList& _tokens);
        bool isLabel(const QString& _token);
        bool isMOC(const QString& _token);
        bool isDirective(const QString& _token);
        bool isOperand(const QString& _token);
        bool isArgNumber(const QString& _token);
        bool isArgString(const QString& _token);
        bool isString(const QString& _token);
        bool isNumber(const QString& _token);
        bool isByteNumber(const QString& _token);
        bool isWordNumber(const QString& _token);
        int numberOfBytesInString(const QString& _str);
        bool hasEndDirective(const QStringList& _tokens);
        std::tuple<QString, int> processFirstNonEmptyRow(const QStringList& _tokens);
        QString decToHexStr(int _dec, int _numberOfDigits);
        void increaseAddressCounter(int _inc, const QString& _sourceRow);
        void processLabel(const QString& _label, const QString& _sourceRow);
        void addCommandToIntermediateRepresentation(int _BOC);
        void addCommandToIntermediateRepresentation(int _BOC, const QString _body);
        void addDirectiveToIntermediateRepresentation(const QString _directive, const QString _body);
        void Acase(const QString& _sourceRow, const QStringList& _tokens);
        void ABcase(const QString& _sourceRow, const QStringList& _tokens);
        void ABCcase();
        void ABCDcase();

    public:
        Translator();
        ~Translator();

    public:
        void firstPass(QString _source, std::map<QString, std::tuple<int, int>> _OCT);
        void secondPass();
    };
}

#endif // TRANSLATOR_H