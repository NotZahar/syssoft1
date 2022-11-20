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
        impossibleNonnegativeIntegerValue(-1),
        addressCounter(impossibleNonnegativeIntegerValue),
        loadAddress(impossibleNonnegativeIntegerValue),
        rowNumberThatHasStartDirective(impossibleNonnegativeIntegerValue),
        rowNumberThatHasEndDirective(impossibleNonnegativeIntegerValue),
        firstNonEmptyRowNumber(impossibleNonnegativeIntegerValue),
        whitespacesSplitRegex(" +"),
        labelRegex("^[a-z\\?\\.@\\_\\$]+[0-9]*$"),
        MOCRegex("^[a-z]+[0-9]*$"),
        directiveRegex("^[a-z]+$"),
        operandsRegex("^(0x[0-9a-f]{1,7}|[0-9]{1,9}|x'[0-9a-f]+'|c'[a-zA-Z0-9\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)\\-\\=\\+\\_\\№\\;\\:\\?]+'|[a-z\\?\\.@\\_\\$]+[0-9]*),?$"),
        startDirectiveRegex("^start$"),
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
            QStringList allTokens = sourceRows[i].split(whitespacesSplitRegex);
            QStringList tokens = deleteEmptyTokens(allTokens);

            if (tokens.empty()) continue;

            if (firstNonEmptyRowNumber == impossibleNonnegativeIntegerValue) { 
                firstNonEmptyRowNumber = i; // set number of first non-empty row
            }

            if (firstNonEmptyRowNumber == i) { // we are on the first row
                if (!hasStartDirective(tokens)) { // TODO: переделать, нужно распарсить прямо здесь и посмотреть есть ли АЗ
                    throw Error::error::startDirectiveWasExpected;
                }

                rowNumberThatHasStartDirective = i;
            }

            const int numberOftokens = tokens.size();
            switch (numberOftokens) {
            case 1: {
                QString token = tokens[0];
                bool IS_MOC = isMOC(token);
                bool IS_DIRECTIVE = isDirective(token);
                
                if (!IS_MOC && !IS_DIRECTIVE) {
                    throw ErrorData<QString>(sourceRows[i], Error::error::MOCOrDirectiveWasExprcted);
                }

                if (IS_MOC) {

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

    bool Translator::hasStartDirective(const QStringList& _tokens) {
        for (const auto& token : _tokens) {
            if (startDirectiveRegex.match(token).hasMatch()) {
                return true;
            }
        }

        return false;
    }

    bool Translator::hasEndDirective(const QStringList& _tokens) {
        for (const auto& token : _tokens) {
            if (endDirectiveRegex.match(token).hasMatch()) {
                return true;
            }
        }

        return false;
    }
}
