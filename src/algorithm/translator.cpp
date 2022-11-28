#include "translator.hpp"

namespace syssoft1 {
    Translator::Translator() : 
        OCT(),
        SNT(),
        directivesList{
            "start",
            "end",
            "byte",
            "word",
            "resb",
            "resw"
        },
        registersList{
            "r0",
            "r1",
            "r2",
            "r3",
            "r4",
            "r5",
            "r6",
            "r7",
            "r8",
            "r9",
            "r10",
            "r11",
            "r12",
            "r13",
            "r14",
            "r15",
        },
        intermediateRepresentation(),
        maximumNumberOfCommandArgs(2),
        maximumAddress(0xffffff),
        maximumRecordLength(0xff),
        maximumNumberByte(0xff),
        maximumNumberWord(0xffffff),
        maximumNumberIn3bCommand(0xffff),
        maximumNumberIn2bCommand(0xff),
        maximumNumberOfHexadecimalDigitsForAddress(6),
        maximumNumberOfHexadecimalDigitsForCommand(2),
        impossibleNonnegativeIntegerValue(-1),
        impossibleProgramName(""),
        addressCounter(impossibleNonnegativeIntegerValue),
        loadAddress(impossibleNonnegativeIntegerValue),
        firstNonEmptyRowNumber(impossibleNonnegativeIntegerValue),
        programName(impossibleProgramName),
        whitespacesSplitRegex(" +"),
        labelRegex("^[a-z\\?\\.@\\_\\$]+[0-9]*$"),
        MOCRegex("^[a-z]+[0-9]*$"),
        directiveRegex("^[a-z]+$"),
        operandRegex("^(0x[0-9a-f]{1,7}|[0-9]{1,9}|x'[0-9a-f]{1,255}'|c'[a-zA-Z0-9\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)\\-\\=\\+\\_\\№\\;\\:\\?]{1,127}'|[a-z\\?\\.@\\_\\$]+[0-9]*),?$"),
        programNameRegex("^[a-zA-Z\\_]+[0-9]*$"),
        startDirectiveRegex("^start$"),
        loadAddressRegex("^(0x[0-9a-f]{1,6}|[0-9]{1,7})$"),
        endDirectiveRegex("^end$"),
        numberRegex("^(0x[0-9a-f]{1,7}|[0-9]{1,9})$"),
        stringRegex("^(x'[0-9a-f]{1,255}'|c'[a-zA-Z0-9\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)\\-\\=\\+\\_\\№\\;\\:\\?]{1,127}')$"),
        xStringRegex("^x'[0-9a-f]{1,255}'$"),
        cStringRegex("^c'[a-zA-Z0-9\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)\\-\\=\\+\\_\\№\\;\\:\\?]{1,127}'$"),
        xString3bRegex("^x'[0-9a-f]{4}'$"),
        cString3bRegex("^c'[a-zA-Z0-9\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)\\-\\=\\+\\_\\№\\;\\:\\?]{2}'$")
    {

    }

    Translator::~Translator() {

    }

    void Translator::firstPass(QString _source, std::map<QString, std::tuple<int, int>> _OCT) {
        OCT = _OCT;
        const QStringList sourceRows = _source.split("\n");
        const int numberOfRows = sourceRows.size();

        if (numberOfRows == 0) throw Error::error::startDirectiveWasExpected;
        if (numberOfRows == 1) throw Error::error::thereAreNotEnoughStartAndEndDirectives;

        for (int i = 0; i < numberOfRows; ++i) {
            QString sourceRow = sourceRows[i];
            QStringList allTokens = sourceRow.split(whitespacesSplitRegex);
            QStringList tokens = deleteEmptyTokens(allTokens);

            if (tokens.empty()) continue;

            if (firstNonEmptyRowNumber == impossibleNonnegativeIntegerValue) { 
                firstNonEmptyRowNumber = i; // set number of first non-empty row
            }

            if (i == firstNonEmptyRowNumber) { // we are on the first non-empty row
                const auto [_programName, _loadAddress] = processFirstNonEmptyRow(tokens);
                programName = _programName;
                loadAddress = _loadAddress;
                addressCounter = loadAddress;
                continue;
            }

            const int numberOftokens = tokens.size();
            switch (numberOftokens) {
            case 1:
                Acase(sourceRow, tokens);
                break;
            case 2:
                ABcase(sourceRow, tokens);
                break;
            case 3:
                ABCcase();
                break;
            case 4:
                ABCDcase();
                break;
            default:
                throw ErrorData<QString>(sourceRow, Error::error::impossibleNumberOfTokens);
                break;
            }
        }

        for (const auto& v : intermediateRepresentation) {
            for (const auto& s : v) {
                std::cout << s.toStdString() << " ";
            }
            std::cout << std::endl;
        }
    }

    void Translator::secondPass() {

    }

    void Translator::clear() {
        OCT.clear();
        SNT.clear();
        intermediateRepresentation.clear();
        addressCounter = impossibleNonnegativeIntegerValue;
        loadAddress = impossibleNonnegativeIntegerValue;
        firstNonEmptyRowNumber = impossibleNonnegativeIntegerValue;
        programName = impossibleProgramName;
    }

    QStringList Translator::deleteEmptyTokens(const QStringList& _tokens) {
        QStringList withoutEmptyTokensList;
        for (const auto& token : _tokens) {
            if (!token.isEmpty()) {
                withoutEmptyTokensList.append(token);
            }
        }

        return withoutEmptyTokensList;
    }

    bool Translator::isLabel(const QString& _token) {
        QRegularExpressionMatch labelMatch = labelRegex.match(_token);
        
        if (!labelMatch.hasMatch()) {
            return false;
        }

        if (OCT.find(_token) != OCT.end()) {
            return false; 
        }

        if (directivesList.find(_token) != directivesList.end()) {
            return false;
        }

        if (registersList.find(_token) != registersList.end()) {
            return false;
        }

        return true;
    }
    
    bool Translator::isMOC(const QString& _token) {
        QRegularExpressionMatch MOCMatch = MOCRegex.match(_token);

        if (!MOCMatch.hasMatch()) {
            return false;
        }

        return OCT.find(_token) != OCT.end();
    }

    bool Translator::isDirective(const QString& _token) {
        QRegularExpressionMatch directiveMatch = directiveRegex.match(_token);

        if (!directiveMatch.hasMatch()) {
            return false;
        }

        return directivesList.find(_token) != directivesList.end();
    }

    bool Translator::isOperand(const QString& _token) {
        QRegularExpressionMatch operandMatch = operandRegex.match(_token);

        return operandMatch.hasMatch() && !isDirective(_token) && !isMOC(_token);
    }

    bool Translator::is2bNumber(const QString& _token) {
        bool ok;
        int candidate = _token.toInt(&ok, 0);
        
        return ok && candidate <= maximumNumberIn2bCommand;
    }

    bool Translator::is3bNumber(const QString& _token) {
        bool ok;
        int candidate = _token.toInt(&ok, 0);
        
        return ok && candidate <= maximumNumberIn3bCommand;
    }

    bool Translator::is3bString(const QString& _token) {
        QRegularExpressionMatch xStringMatch = xString3bRegex.match(_token);
        QRegularExpressionMatch cStringMatch = cString3bRegex.match(_token);

        bool IS_XSTRING = xStringMatch.hasMatch();
        bool IS_CSTRING = cStringMatch.hasMatch();
        
        return IS_XSTRING || IS_CSTRING;
    }

    bool Translator::isString(const QString& _token) {
        QRegularExpressionMatch stringMatch = stringRegex.match(_token);
        
        return stringMatch.hasMatch();
    }

    bool Translator::isNumber(const QString& _token) {
        QRegularExpressionMatch numberMatch = numberRegex.match(_token);
        
        return numberMatch.hasMatch();
    }

    bool Translator::isByteNumber(const QString& _token) {
        if (!isNumber(_token)) {
            return false;
        }

        bool ok;
        int number = _token.toInt(&ok, 0);
        
        return ok && number <= maximumNumberByte;
    }

     bool Translator::isWordNumber(const QString& _token) {
        if (!isNumber(_token)) {
            return false;
        }

        bool ok;
        int number = _token.toInt(&ok, 0);
        
        return ok && number <= maximumNumberWord;
    }

    int Translator::numberOfBytesInString(const QString& _str) {
        QRegularExpressionMatch xStrMatch = xStringRegex.match(_str);
        QRegularExpressionMatch cStrMatch = cStringRegex.match(_str);
        
        int strSize = _str.size();
        if (xStrMatch.hasMatch()) {
            if (strSize % 2 == 0) {
                return -1;
            }

            return (strSize - 3) / 2;
        } else if (cStrMatch.hasMatch()) {
            return strSize - 3;
        } else {
            return -1;
        }        
    }

    bool Translator::hasEndDirective(const QStringList& _tokens) {
        for (const auto& token : _tokens) {
            if (endDirectiveRegex.match(token).hasMatch()) {
                return true;
            }
        }

        return false;
    }

    std::tuple<QString, int> Translator::processFirstNonEmptyRow(const QStringList& _tokens) {
        if (_tokens.size() != 3) throw Error::error::incorrectFormatOfStartRow;

        QRegularExpressionMatch programNameMatch = programNameRegex.match(_tokens[0]);
        QRegularExpressionMatch startDirectiveMatch = startDirectiveRegex.match(_tokens[1]);
        QRegularExpressionMatch loadAddressMatch = loadAddressRegex.match(_tokens[2]);
        
        if (!programNameMatch.hasMatch() 
            || !startDirectiveMatch.hasMatch()
            || !loadAddressMatch.hasMatch()) {
            throw Error::error::incorrectFormatOfStartRow;
        }
        
        bool ok;
        return std::make_tuple(programNameMatch.captured(), loadAddressMatch.captured().toInt(&ok, 0));
    }

    QString Translator::decToHexStr(int _dec, int _numberOfDigits) {
        QString resultHexStr;

        QString hexStr = QString::number(_dec, 16);
        const int lackOfZeros = _numberOfDigits - hexStr.size();
        for (int i = 0; i < lackOfZeros; ++i) {
            resultHexStr += "0";
        }
        resultHexStr += hexStr;

        return resultHexStr;
    }

    void Translator::increaseAddressCounter(int _inc, const QString& _sourceRow) {
        if (addressCounter + _inc > maximumAddress) {
            throw ErrorData<QString>(_sourceRow, Error::error::addressCounterOverflow);
        }

        addressCounter += _inc;
    }

    void Translator::processLabel(const QString& _label, const QString& _sourceRow) {
        if (SNT.find(_label) == SNT.end()) {
            SNT.insert({_label, addressCounter});
        } else {
            throw ErrorData<QString>(_sourceRow, Error::error::symbolicNameAlreadyExists);
        }
    }

    void Translator::addCommandToIntermediateRepresentation(int _BOC) {
        intermediateRepresentation.push_back({
            "0x" + decToHexStr(addressCounter, maximumNumberOfHexadecimalDigitsForAddress),
            "0x" + decToHexStr(_BOC, maximumNumberOfHexadecimalDigitsForCommand)
        });
    }

    void Translator::addCommandToIntermediateRepresentation(int _BOC, const QString _body) {
        intermediateRepresentation.push_back({
            "0x" + decToHexStr(addressCounter, maximumNumberOfHexadecimalDigitsForAddress),
            "0x" + decToHexStr(_BOC, maximumNumberOfHexadecimalDigitsForCommand),
            _body
        });
    }

    void Translator::addDirectiveToIntermediateRepresentation(const QString _directive, const QString _body) {
        intermediateRepresentation.push_back({
            "0x" + decToHexStr(addressCounter, maximumNumberOfHexadecimalDigitsForAddress),
            _directive,
            _body
        });
    }

    void Translator::Acase(const QString& _sourceRow, const QStringList& _tokens) {
        QString token = _tokens[0];
        bool IS_MOC = isMOC(token);
        bool IS_DIRECTIVE = isDirective(token);
        
        if (!IS_MOC && !IS_DIRECTIVE) {
            throw ErrorData<QString>(_sourceRow, Error::error::MOCOrDirectiveWasExprcted);
        }

        if (IS_MOC) {
            const auto& [BOC, commandLength] = OCT.at(token);
            if (commandLength != 1) { // [1b]
                throw ErrorData<QString>(_sourceRow, Error::error::wrongCommandLength);
            }

            addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::none);
            increaseAddressCounter(commandLength, _sourceRow);
        } else if (IS_DIRECTIVE) {
            throw ErrorData<QString>(_sourceRow, Error::error::MOCWasExpected);
        }
    }
    
    void Translator::ABcase(const QString& _sourceRow, const QStringList& _tokens) {
        QString firstToken = _tokens[0];
        QString secondToken = _tokens[1];

        bool FIRST_TOKEN_IS_LABEL = isLabel(firstToken);
        bool FIRST_TOKEN_IS_MOC = isMOC(firstToken);
        bool FIRST_TOKEN_IS_DIRECTIVE = isDirective(firstToken);

        bool SECOND_TOKEN_IS_LABEL = isLabel(secondToken);
        bool SECOND_TOKEN_IS_MOC = isMOC(secondToken);
        bool SECOND_TOKEN_IS_DIRECTIVE = isDirective(secondToken);
        bool SECOND_TOKEN_IS_OPERAND = isOperand(secondToken);
        
        if (!FIRST_TOKEN_IS_LABEL && !FIRST_TOKEN_IS_MOC && !FIRST_TOKEN_IS_DIRECTIVE) {
            throw ErrorData<QString>(_sourceRow, Error::error::labelOrMOCOrDirectiveWasExpected);
        }

        if (FIRST_TOKEN_IS_LABEL && SECOND_TOKEN_IS_MOC) {
            processLabel(firstToken, _sourceRow);

            const auto& [BOC, commandLength] = OCT.at(secondToken);
            if (commandLength != 1) { // [1b]
                throw ErrorData<QString>(_sourceRow, Error::error::wrongCommandLength);
            }

            addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::none);
            increaseAddressCounter(commandLength, _sourceRow);
        } else if (FIRST_TOKEN_IS_LABEL && SECOND_TOKEN_IS_DIRECTIVE) {
            throw ErrorData<QString>(_sourceRow, Error::error::directiveMustHaveOperand);
        } else if (FIRST_TOKEN_IS_MOC && SECOND_TOKEN_IS_OPERAND) {
            const auto& [BOC, commandLength] = OCT.at(firstToken);
            if (commandLength == 2) { // [2b]
                if (is2bNumber(secondToken)) {
                    addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::immediate, secondToken);
                } else {
                    throw ErrorData<QString>(_sourceRow, Error::error::impossibleOperand);
                }
            } else if (commandLength == 3) { // [3b]
                if (is3bNumber(secondToken) || is3bString(secondToken)) {
                    addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::immediate, secondToken);
                } else {
                    throw ErrorData<QString>(_sourceRow, Error::error::impossibleOperand);
                }
            } else if (commandLength == 4) { // [4b]
                if (SECOND_TOKEN_IS_LABEL) {
                    addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::direct, secondToken);
                } else {
                    throw ErrorData<QString>(_sourceRow, Error::error::impossibleOperand);
                }
            } else {
                throw ErrorData<QString>(_sourceRow, Error::error::wrongCommandLength);
            }

            increaseAddressCounter(commandLength, _sourceRow);
        } else if (FIRST_TOKEN_IS_DIRECTIVE && SECOND_TOKEN_IS_OPERAND) {
            if (firstToken == "byte") {
                if (isString(secondToken)) {
                    addDirectiveToIntermediateRepresentation(firstToken, secondToken);
                    
                    int numberofBytes = numberOfBytesInString(secondToken);
                    if (numberofBytes == -1) {
                        throw ErrorData<QString>(_sourceRow, Error::error::impossibleOperand);
                    }

                    increaseAddressCounter(numberofBytes, _sourceRow);
                } else if (isByteNumber(secondToken)) {
                    addDirectiveToIntermediateRepresentation(firstToken, secondToken);
                    increaseAddressCounter(1, _sourceRow);
                } else {
                    throw ErrorData<QString>(_sourceRow, Error::error::numberDoesNotFitInByte);
                }
            } else if (firstToken == "word") {
                if (isString(secondToken) || !isWordNumber(secondToken)) {
                    throw ErrorData<QString>(_sourceRow, Error::error::numberDoesNotFitInWord);
                }

                addDirectiveToIntermediateRepresentation(firstToken, secondToken);
                increaseAddressCounter(3, _sourceRow);
            } else if (firstToken == "resb") {
                if (isNumber(secondToken)) {
                    addDirectiveToIntermediateRepresentation(firstToken, secondToken);
                    bool ok;
                    increaseAddressCounter(secondToken.toInt(&ok, 0), _sourceRow);
                } else {
                    throw ErrorData<QString>(_sourceRow, Error::error::numberWasExpected);
                }
            } else if (firstToken == "resw") {
                if (isNumber(secondToken)) {
                    addDirectiveToIntermediateRepresentation(firstToken, secondToken);
                    bool ok;
                    increaseAddressCounter(secondToken.toInt(&ok, 0) * 3, _sourceRow);
                } else {
                    throw ErrorData<QString>(_sourceRow, Error::error::numberWasExpected);
                }
            }
        } else {
            throw ErrorData<QString>(_sourceRow, Error::error::incorrectFormatOfRow);
        }
    }
    
    void Translator::ABCcase() {
        // TODO:
    }

    void Translator::ABCDcase() {
        // TODO:
    }
}
