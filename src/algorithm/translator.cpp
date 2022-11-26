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
        intermediateRepresentation(),
        maximumNumberOfCommandArgs(2),
        maximumAddress(0xffffff),
        maximumRecordLength(0xff),
        maximumNumberOfHexadecimalDigitsForAddress(6),
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
        operandsRegex("^(0x[0-9a-f]{1,7}|[0-9]{1,9}|x'[0-9a-f]{1,255}'|c'[a-zA-Z0-9\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)\\-\\=\\+\\_\\№\\;\\:\\?]{1,127}'|[a-z\\?\\.@\\_\\$]+[0-9]*),?$"),
        programNameRegex("^[a-zA-Z\\_]+[0-9]*$"),
        startDirectiveRegex("^start$"),
        loadAddressRegex("^(0x[0-9a-f]{1,6}|[0-9]{1,7})$"),
        endDirectiveRegex("^end$")
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
            case 1: {
                QString token = tokens[0];
                bool IS_MOC = isMOC(token);
                bool IS_DIRECTIVE = isDirective(token);
                
                if (!IS_MOC && !IS_DIRECTIVE) {
                    throw ErrorData<QString>(sourceRow, Error::error::MOCOrDirectiveWasExprcted);
                }

                if (IS_MOC) {
                    // TODO: МКОП -> ДКОП
                    
                }

                if (IS_DIRECTIVE) {
                    
                }

                break;
            }
            case 2: {
                break;
            }
            case 3: {
                break;
            } 
            default:
                break;
            }
        }
    }

    void Translator::secondPass() {

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
}
