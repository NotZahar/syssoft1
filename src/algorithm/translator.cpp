#include "translator.hpp"

namespace syssoft1 {
    Translator::Translator() : 
        directivesList{
            "start",
            "end",
            "byte",
            "word",
            "resb",
            "resw"
        },
        whitespacesSplitRegex(" +"),
        labelRegex("[a-z\\?\\.@\\_\\$]+[0-9]*"), // TODO: доделать регекспы
        MOCRegex("[a-z]+[0-9]*"),
        operandsRegex("")
    {

    }

    Translator::~Translator() {

    }

    void Translator::firstPass(QString _source, std::map<QString, std::tuple<int, int>> _OCT) {
        _OCT = {}; // TODO: delete
        const QStringList sourceRows = _source.split("\n");
        
        for (const auto& sourceRow : sourceRows) {
            QStringList allTokens = sourceRow.split(whitespacesSplitRegex);
            QStringList tokens = deleteEmptyTokens(allTokens);

            if (tokens.empty()) {
                continue;
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
}
