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
    // 1b: 0 args (none)
    // 2b: 2 args (reg1, reg2) or 1 arg (number <= 0xff)
    // 3b: 1 arg (number or string (<= 0xffff))
    // 4b: 1 arg (label)

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
        const int maximumNumberIn2bCommand;
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
            none = 0,
            immediate = 0,
            reg = 0,
            direct
        };

    private:
        QStringList deleteEmptyTokens(const QStringList& _tokens);
        bool isLabel(const QString& _token);
        bool isMOC(const QString& _token);
        bool isDirective(const QString& _token);
        bool isOperand(const QString& _token);
        bool isRegister(const QString& _token);
        bool is2bNumber(const QString& _token);
        bool is3bNumber(const QString& _token);
        bool is3bString(const QString& _token);
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
        void labelMOC(const QString& _label, const QString& _MOC, const QString& _sourceRow);
        void labelDirective(const QString& _label, const QString& _directive, const QString& _sourceRow);
        void MOCOperand(const QString& _MOC, const QString& _operand, const QString& _sourceRow);
        void directiveOperand(const QString& _directive, const QString& _operand, const QString& _sourceRow);
        void labelMOCOperand(const QString& _label, const QString& _MOC, const QString& _operand, const QString& _sourceRow);
        void labelDirectiveOperand(const QString& _label, const QString& _directive, const QString& _operand, const QString& _sourceRow);
        void MOCOperandOperand(const QString& _MOC, const QString& _operand1, const QString& _operand2, const QString& _sourceRow);
        void directiveOperandOperand(const QString& _directive, const QString& _operand1, const QString& _operand2, const QString& _sourceRow);
        
        void Acase(const QString& _sourceRow, const QStringList& _tokens);
        void ABcase(const QString& _sourceRow, const QStringList& _tokens);
        void ABCcase(const QString& _sourceRow, const QStringList& _tokens);
        void ABCDcase();

    public:
        Translator();
        ~Translator();

    public:
        void firstPass(QString _source, std::map<QString, std::tuple<int, int>> _OCT);
        void secondPass();
        void clear();
    };
}

#endif // TRANSLATOR_H