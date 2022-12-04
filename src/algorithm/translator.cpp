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
        addressOfEntryPoint(impossibleNonnegativeIntegerValue),
        addressCounter(impossibleNonnegativeIntegerValue),
        loadAddress(impossibleNonnegativeIntegerValue),
        firstNonEmptyRowNumber(impossibleNonnegativeIntegerValue),
        programName(impossibleProgramName),
        endWasMet(false),
        startWasMet(false),
        whitespacesSplitRegex(" +"),
        labelRegex("^[a-zA-Z\\_]+[0-9]*$"),
        MOCRegex("^[a-z]+[0-9]*$"),
        directiveRegex("^[a-z]+$"),
        operandRegex("^(0x[0-9a-f]{1,7}|[0-9]{1,9}|x'[0-9a-f]{1,255}'|c'(\\s|\\S){1,127}'|[a-zA-Z\\_]+[0-9]*),?$"),
        programNameRegex("^[a-zA-Z\\_]+[0-9]*$"),
        startDirectiveRegex("^start$"),
        loadAddressRegex("^(0x[0-9a-f]{1,6}|[0-9]{1,7})$"),
        endDirectiveRegex("^end$"),
        numberRegex("^(0x[0-9a-f]{1,7}|[0-9]{1,9})$"),
        stringRegex("^(x'[0-9a-f]{1,255}'|c'(\\s|\\S){1,127}')$"),
        xStringRegex("^x'[0-9a-f]{1,255}'$"),
        cStringRegex("^c'(\\s|\\S){1,127}'$"),
        pureCStringRegex("c'(\\s|\\S){1,127}'"),
        xString3bRegex("^x'[0-9a-f]{4}'$"),
        cString3bRegex("^c'(\\s|\\S){2}'$")
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
            QStringList allTokens = splitIntoTokens(sourceRow);
            QStringList tokens = deleteEmptyTokens(allTokens);

            if (tokens.empty()) continue;

            if (firstNonEmptyRowNumber == impossibleNonnegativeIntegerValue) { 
                firstNonEmptyRowNumber = i; // set number of first non-empty row
            }

            if (i == firstNonEmptyRowNumber) { // we are on the first non-empty row
                const auto [_programName, _loadAddress] = processFirstNonEmptyRow(tokens);
                
                if (_loadAddress == 0) { // this is not an absolute format
                    throw Error::error::programIsNotInAbsoluteFormat;
                }
                
                programName = _programName;
                loadAddress = _loadAddress;
                addressCounter = loadAddress;
                startWasMet = true;
                continue;
            }

            if (endWasMet) break;

            const int numberOftokens = tokens.size();
            switch (numberOftokens) {
            case 1:
                Acase(sourceRow, tokens);
                break;
            case 2:
                ABcase(sourceRow, tokens);
                break;
            case 3:
                ABCcase(sourceRow, tokens);
                break;
            case 4:
                ABCDcase(sourceRow, tokens);
                break;
            default:
                throw ErrorData<QString>(sourceRow, Error::error::impossibleNumberOfTokens);
                break;
            }
        }

        if (!startWasMet || !endWasMet) {
            throw Error::error::thereAreNotEnoughStartAndEndDirectives;
        }

        intermediateRepresentation.push_front({
            programName,
            "0x" + decToHexStr(loadAddress, maximumNumberOfHexadecimalDigitsForAddress),
            "0x" + decToHexStr(addressCounter - loadAddress, maximumNumberOfHexadecimalDigitsForAddress)
        });
    }

    void Translator::secondPass() {
        for (auto& row : intermediateRepresentation) {
            if (row[0] == programName) {
                row.push_front("H");
            } else if (row[1] == "end") {
                row[0] = "E";
                row[1] = row[2];
                row.pop_back();
            } else if (isDirective(row[1])) {
                processDirectiveRowSP(row);
            } else {
                processOPRowSP(row);
            }
        }
    }

    void Translator::clear() {
        OCT.clear();
        SNT.clear();
        intermediateRepresentation.clear();
        addressOfEntryPoint = impossibleNonnegativeIntegerValue;
        addressCounter = impossibleNonnegativeIntegerValue;
        loadAddress = impossibleNonnegativeIntegerValue;
        firstNonEmptyRowNumber = impossibleNonnegativeIntegerValue;
        programName = impossibleProgramName;
        endWasMet = false;
        startWasMet = false;
    }

    const std::deque<std::deque<QString>>& Translator::getIntermediateRepresentation() {
        return intermediateRepresentation;
    }

    const std::map<QString, int>& Translator::getSNT() {
        return SNT;
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

    QStringList Translator::splitIntoTokens(const QString& _sourceRow) {
        QRegularExpressionMatch rowWithCStringCandidateMatch = pureCStringRegex.match(_sourceRow);
        if (rowWithCStringCandidateMatch.hasMatch()) {
            QString cString = rowWithCStringCandidateMatch.captured();
            int startIndex = rowWithCStringCandidateMatch.capturedStart();
            int endIndex = rowWithCStringCandidateMatch.capturedEnd();
            
            QStringList before = _sourceRow.mid(0, startIndex).split(whitespacesSplitRegex);
            QStringList after = _sourceRow.mid(endIndex).split(whitespacesSplitRegex);
            
            before << cString;

            return before + after;
        }

        return _sourceRow.split(whitespacesSplitRegex);
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

    bool Translator::isRegister(const QString& _token) {
        return registersList.find(_token) != registersList.end();
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

    bool Translator::isXString(const QString& _token) {
        QRegularExpressionMatch xStringMatch = xStringRegex.match(_token);
        
        return xStringMatch.hasMatch();
    }
    
    bool Translator::isCString(const QString& _token) {
        QRegularExpressionMatch cStringMatch = cStringRegex.match(_token);
        
        return cStringMatch.hasMatch();
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

    void Translator::processDirectiveRowSP(std::deque<QString>& _row) {
        QString directive = _row.at(1);
        QString body = _row.at(2);
        
        _row.pop_back();
        _row.pop_back();
        _row.push_front("T");

        if (directive == "byte") {
            if (isNumber(body)) {
                _row.push_back("0x02");
                bool ok;
                _row.push_back("0x" + decToHexStr(body.toInt(&ok, 0), 2));
            } else if (isXString(body)) {
                QString processedStr = body.mid(2);
                processedStr.chop(1);
                
                _row.push_back("0x" + decToHexStr(processedStr.size(), 2));
                _row.push_back("0x" + processedStr);
            } else if (isCString(body)) {
                QString processedStr = body.mid(2);
                processedStr.chop(1);
                
                QStringList hexChars;
                for (const auto& ch: processedStr) {
                    hexChars << QString::number(ch.unicode(), 16);
                }
                QString res = hexChars.join("");

                _row.push_back("0x" + decToHexStr(res.size(), 2));
                _row.push_back("0x" + res);
            }
        } else if (directive == "word") {
            _row.push_back("0x06");
            bool ok;
            _row.push_back("0x" + decToHexStr(body.toInt(&ok, 0), 6));
        } else if (directive == "resb") {
            _row.push_back("0x00");
        } else if (directive == "resw") {
            _row.push_back("0x00");
        }
    }
    
    void Translator::processOPRowSP(std::deque<QString>& _row) {
        QString OP = _row.at(1);

        if (_row.size() == 2) { // we have no body
            _row.pop_back();
            _row.push_front("T");

            _row.push_back("0x02");
            _row.push_back(OP);

            return;
        }

        QString body = _row.at(2);
        
        _row.pop_back();
        _row.pop_back();
        _row.push_front("T");

        if (isLabel(body)) {
            if (SNT.find(body) == SNT.end()) {
                throw ErrorData<QString>(body, Error::error::symbolicNameWasNotFound);
            }

            int address = SNT.at(body);
            _row.push_back("0x08");
            _row.push_back(OP + decToHexStr(address, maximumNumberOfHexadecimalDigitsForAddress));
        } else if (isXString(body)) {
            QString processedStr = body.mid(2);
            processedStr.chop(1);
            
            _row.push_back("0x" + decToHexStr(2 + processedStr.size(), 2));
            _row.push_back(OP + processedStr);
        } else if (isCString(body)) {
            QString processedStr = body.mid(2);
            processedStr.chop(1);
            
            QStringList hexChars;
            for (const auto& ch: processedStr) {
                hexChars << QString::number(ch.unicode(), 16);
            }
            QString res = hexChars.join("");

            _row.push_back("0x" + decToHexStr(2 + res.size(), 2));
            _row.push_back(OP + res);
        } else if (isNumber(body)) {
            bool ok;
            int num = body.toInt(&ok, 16);
            QString hexStr = QString::number(num, 16);

            _row.push_back("0x" + decToHexStr(2 + hexStr.size(), 2));
            _row.push_back(OP + hexStr);
        } else { // 2 registers
            QStringList regs = body.split(" ");
            QString reg1 = regs[0];
            QString reg2 = regs[1];
            reg1.chop(1);

            QString reg1NumberStr = reg1.mid(1);
            QString reg2NumberStr = reg2.mid(1);
            int reg1Number = reg1NumberStr.toInt();
            int reg2Number = reg2NumberStr.toInt();

            QString regPair = QString::number(reg1Number, 16) + QString::number(reg2Number, 16);

            _row.push_back("0x04");
            _row.push_back(OP + regPair);
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

    void Translator::addEndDirectiveToIntermediateRepresentation(const QString _directive, int _address) {
        intermediateRepresentation.push_back({
            "0x" + decToHexStr(addressCounter, maximumNumberOfHexadecimalDigitsForAddress),
            _directive,
            "0x" + decToHexStr(_address, maximumNumberOfHexadecimalDigitsForAddress)
        });
    }

    void Translator::labelMOC(const QString& _label, const QString& _MOC, const QString& _sourceRow) {
        processLabel(_label, _sourceRow);

        const auto& [BOC, commandLength] = OCT.at(_MOC);
        if (commandLength != 1) { // [1b]
            throw ErrorData<QString>(_sourceRow, Error::error::wrongCommandLength);
        }

        addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::none);
        increaseAddressCounter(commandLength, _sourceRow);
    }

    void Translator::labelDirective(const QString& _label, const QString& _directive, const QString& _sourceRow) {
        if (_directive != "end") {
            throw ErrorData<QString>(_sourceRow, Error::error::directiveMustHaveOperand);
        }

        if (endWasMet) {
            throw ErrorData<QString>(_sourceRow, Error::error::endWasMet);
        }

        processLabel(_label, _sourceRow);
        endWasMet = true;
        addressOfEntryPoint = loadAddress;
        addEndDirectiveToIntermediateRepresentation(_directive, addressOfEntryPoint);
    }

    void Translator::MOCOperand(const QString& _MOC, const QString& _operand, const QString& _sourceRow) {
        const auto& [BOC, commandLength] = OCT.at(_MOC);
        if (commandLength == 2) { // [2b]
            if (is2bNumber(_operand)) {
                addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::immediate, _operand);
            } else {
                throw ErrorData<QString>(_sourceRow, Error::error::impossibleOperand);
            }
        } else if (commandLength == 3) { // [3b]
            if (is3bNumber(_operand) || is3bString(_operand)) {
                addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::immediate, _operand);
            } else {
                throw ErrorData<QString>(_sourceRow, Error::error::impossibleOperand);
            }
        } else if (commandLength == 4) { // [4b]
            if (isLabel(_operand)) {
                addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::direct, _operand);
            } else {
                throw ErrorData<QString>(_sourceRow, Error::error::impossibleOperand);
            }
        } else {
            throw ErrorData<QString>(_sourceRow, Error::error::wrongCommandLength);
        }

        increaseAddressCounter(commandLength, _sourceRow);
    }

    void Translator::directiveOperand(const QString& _directive, const QString& _operand, const QString& _sourceRow) {
        if (_directive == "start") {
            throw ErrorData<QString>(_sourceRow, Error::error::startWasMet);
        } else if (_directive == "end") {
            if (endWasMet) {
                throw ErrorData<QString>(_sourceRow, Error::error::endWasMet);  
            }

            if (!isNumber(_operand)) {
                throw ErrorData<QString>(_sourceRow, Error::error::numberWasExpected);
            }

            bool ok;
            int addressOfEntryPointCandidate = _operand.toInt(&ok, 0);

            if (!ok || addressOfEntryPointCandidate > maximumAddress) {
                throw ErrorData<QString>(_sourceRow, Error::error::numberWasExpected);
            }

            addressOfEntryPointCandidate = std::clamp(addressOfEntryPointCandidate, loadAddress, addressCounter); 
            endWasMet = true;
            addressOfEntryPoint = addressOfEntryPointCandidate;
            addEndDirectiveToIntermediateRepresentation(_directive, addressOfEntryPoint);
        } else if (_directive == "byte") {
            if (isString(_operand)) {
                addDirectiveToIntermediateRepresentation(_directive, _operand);
                
                int numberofBytes = numberOfBytesInString(_operand);
                if (numberofBytes == -1) {
                    throw ErrorData<QString>(_sourceRow, Error::error::impossibleOperand);
                }

                increaseAddressCounter(numberofBytes, _sourceRow);
            } else if (isByteNumber(_operand)) {
                addDirectiveToIntermediateRepresentation(_directive, _operand);
                increaseAddressCounter(1, _sourceRow);
            } else {
                throw ErrorData<QString>(_sourceRow, Error::error::numberDoesNotFitInByte);
            }
        } else if (_directive == "word") {
            if (isString(_operand) || !isWordNumber(_operand)) {
                throw ErrorData<QString>(_sourceRow, Error::error::numberDoesNotFitInWord);
            }

            addDirectiveToIntermediateRepresentation(_directive, _operand);
            increaseAddressCounter(3, _sourceRow);
        } else if (_directive == "resb") {
            if (isNumber(_operand)) {
                addDirectiveToIntermediateRepresentation(_directive, _operand);
                bool ok;
                increaseAddressCounter(_operand.toInt(&ok, 0), _sourceRow);
            } else {
                throw ErrorData<QString>(_sourceRow, Error::error::numberWasExpected);
            }
        } else if (_directive == "resw") {
            if (isNumber(_operand)) {
                addDirectiveToIntermediateRepresentation(_directive, _operand);
                bool ok;
                increaseAddressCounter(_operand.toInt(&ok, 0) * 3, _sourceRow);
            } else {
                throw ErrorData<QString>(_sourceRow, Error::error::numberWasExpected);
            }
        }
    }

    void Translator::labelMOCOperand(const QString& _label, const QString& _MOC, const QString& _operand, const QString& _sourceRow) {
        processLabel(_label, _sourceRow);
        MOCOperand(_MOC, _operand, _sourceRow);
    }

    void Translator::labelDirectiveOperand(const QString& _label, const QString& _directive, const QString& _operand, const QString& _sourceRow) {
        processLabel(_label, _sourceRow);
        directiveOperand(_directive, _operand, _sourceRow);
    }

    void Translator::MOCOperandOperand(const QString& _MOC, const QString& _operand1, const QString& _operand2, const QString& _sourceRow) {
        const auto& [BOC, commandLength] = OCT.at(_MOC);
        if (commandLength == 2) { // [2b]
            if (!_operand1.endsWith(",") || _operand2.endsWith(",")) {
                throw ErrorData<QString>(_sourceRow, Error::error::incorrectFormatOfRow);
            }

            QString processedRegister = _operand1;
            processedRegister.chop(1);
            if (isRegister(processedRegister) && isRegister(_operand2)) {
                addCommandToIntermediateRepresentation(BOC << 2 | (int)addressingType::reg, _operand1 + " " + _operand2);
            } else {
                throw ErrorData<QString>(_sourceRow, Error::error::impossibleOperand);
            }
        } else {
            throw ErrorData<QString>(_sourceRow, Error::error::wrongCommandLength);
        }

        increaseAddressCounter(commandLength, _sourceRow);
    }

    void Translator::directiveOperandOperand([[maybe_unused]] const QString& _directive, [[maybe_unused]] const QString& _operand1, [[maybe_unused]] const QString& _operand2, const QString& _sourceRow) {
        throw ErrorData<QString>(_sourceRow, Error::error::directiveMustHaveOnlyOneOperand);
    }

    void Translator::labelMOCOperandOperand(const QString& _label, const QString& _MOC, const QString& _operand1, const QString& _operand2, const QString& _sourceRow) {
        processLabel(_label, _sourceRow);
        MOCOperandOperand(_MOC, _operand1, _operand2, _sourceRow);
    }
    
    void Translator::labelDirectiveOperandOperand([[maybe_unused]] const QString& _label, [[maybe_unused]] const QString& _directive, [[maybe_unused]] const QString& _operand1, [[maybe_unused]] const QString& _operand2, const QString& _sourceRow) {
        throw ErrorData<QString>(_sourceRow, Error::error::directiveMustHaveOnlyOneOperand);
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
            if (token != "end") {
                throw ErrorData<QString>(_sourceRow, Error::error::MOCOrEndDirectiveWasExpected);
            }

            if (endWasMet) {
                throw ErrorData<QString>(_sourceRow, Error::error::endWasMet);  
            }

            endWasMet = true;
            addressOfEntryPoint = loadAddress;
            addEndDirectiveToIntermediateRepresentation(token, addressOfEntryPoint);
        }
    }
    
    void Translator::ABcase(const QString& _sourceRow, const QStringList& _tokens) {
        QString firstToken = _tokens[0];
        QString secondToken = _tokens[1];

        bool FIRST_TOKEN_IS_LABEL = isLabel(firstToken);
        bool FIRST_TOKEN_IS_MOC = isMOC(firstToken);
        bool FIRST_TOKEN_IS_DIRECTIVE = isDirective(firstToken);

        bool SECOND_TOKEN_IS_MOC = isMOC(secondToken);
        bool SECOND_TOKEN_IS_DIRECTIVE = isDirective(secondToken);
        bool SECOND_TOKEN_IS_OPERAND = isOperand(secondToken);
        
        if (!FIRST_TOKEN_IS_LABEL && !FIRST_TOKEN_IS_MOC && !FIRST_TOKEN_IS_DIRECTIVE) {
            throw ErrorData<QString>(_sourceRow, Error::error::labelOrMOCOrDirectiveWasExpected);
        }

        if (FIRST_TOKEN_IS_LABEL && SECOND_TOKEN_IS_MOC) {
            labelMOC(firstToken, secondToken, _sourceRow);
        } else if (FIRST_TOKEN_IS_LABEL && SECOND_TOKEN_IS_DIRECTIVE) {
            labelDirective(firstToken, secondToken, _sourceRow);
        } else if (FIRST_TOKEN_IS_MOC && SECOND_TOKEN_IS_OPERAND) {
            MOCOperand(firstToken, secondToken, _sourceRow);
        } else if (FIRST_TOKEN_IS_DIRECTIVE && SECOND_TOKEN_IS_OPERAND) {
            directiveOperand(firstToken, secondToken, _sourceRow);
        } else {
            throw ErrorData<QString>(_sourceRow, Error::error::incorrectFormatOfRow);
        }
    }
    
    void Translator::ABCcase(const QString& _sourceRow, const QStringList& _tokens) {
        QString firstToken = _tokens[0];
        QString secondToken = _tokens[1];
        QString thirdToken = _tokens[2];

        bool FIRST_TOKEN_IS_LABEL = isLabel(firstToken);
        bool FIRST_TOKEN_IS_MOC = isMOC(firstToken);
        bool FIRST_TOKEN_IS_DIRECTIVE = isDirective(firstToken);

        bool SECOND_TOKEN_IS_MOC = isMOC(secondToken);
        bool SECOND_TOKEN_IS_DIRECTIVE = isDirective(secondToken);
        bool SECOND_TOKEN_IS_OPERAND = isOperand(secondToken);

        bool THIRD_TOKEN_IS_OPERAND = isOperand(thirdToken);
        
        if (!FIRST_TOKEN_IS_LABEL && !FIRST_TOKEN_IS_MOC && !FIRST_TOKEN_IS_DIRECTIVE) {
            throw ErrorData<QString>(_sourceRow, Error::error::labelOrMOCOrDirectiveWasExpected);
        }

        if (FIRST_TOKEN_IS_LABEL && SECOND_TOKEN_IS_MOC && THIRD_TOKEN_IS_OPERAND) {
            labelMOCOperand(firstToken, secondToken, thirdToken, _sourceRow);
        } else if (FIRST_TOKEN_IS_LABEL && SECOND_TOKEN_IS_DIRECTIVE && THIRD_TOKEN_IS_OPERAND) {
            labelDirectiveOperand(firstToken, secondToken, thirdToken, _sourceRow);
        } else if (FIRST_TOKEN_IS_MOC && SECOND_TOKEN_IS_OPERAND && THIRD_TOKEN_IS_OPERAND) {
            MOCOperandOperand(firstToken, secondToken, thirdToken, _sourceRow);
        } else if (FIRST_TOKEN_IS_DIRECTIVE && SECOND_TOKEN_IS_OPERAND && THIRD_TOKEN_IS_OPERAND) {
            directiveOperandOperand(firstToken, secondToken, thirdToken, _sourceRow);
        } else {
            throw ErrorData<QString>(_sourceRow, Error::error::incorrectFormatOfRow);
        }
    }

    void Translator::ABCDcase(const QString& _sourceRow, const QStringList& _tokens) {
        QString firstToken = _tokens[0];
        QString secondToken = _tokens[1];
        QString thirdToken = _tokens[2];
        QString fourthToken = _tokens[3];

        bool FIRST_TOKEN_IS_LABEL = isLabel(firstToken);

        bool SECOND_TOKEN_IS_MOC = isMOC(secondToken);
        bool SECOND_TOKEN_IS_DIRECTIVE = isDirective(secondToken);

        bool THIRD_TOKEN_IS_OPERAND = isOperand(thirdToken);
        
        bool FOURTH_TOKEN_IS_OPERAND = isOperand(fourthToken);
        
        if (!FIRST_TOKEN_IS_LABEL) {
            throw ErrorData<QString>(_sourceRow, Error::error::labelWasExpected);
        }

        if (FIRST_TOKEN_IS_LABEL && SECOND_TOKEN_IS_MOC && THIRD_TOKEN_IS_OPERAND && FOURTH_TOKEN_IS_OPERAND) {
            labelMOCOperandOperand(firstToken, secondToken, thirdToken, fourthToken, _sourceRow);
        } else if (FIRST_TOKEN_IS_LABEL && SECOND_TOKEN_IS_DIRECTIVE && THIRD_TOKEN_IS_OPERAND && FOURTH_TOKEN_IS_OPERAND) {
            labelDirectiveOperandOperand(firstToken, secondToken, thirdToken, fourthToken, _sourceRow);
        } else {
            throw ErrorData<QString>(_sourceRow, Error::error::incorrectFormatOfRow);
        }
    }
}
