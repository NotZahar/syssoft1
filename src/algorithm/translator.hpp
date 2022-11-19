#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>

#include <string>
#include <map>
#include <set>
#include <tuple>

#include "../middleware/validate.hpp"
#include "../errors/error.hpp"

namespace syssoft1 {
    class Translator {
    private:
        std::map<QString, std::tuple<int, int>> OCT;
        std::map<QString, int> SNT;
        const std::set<QString> directivesList;
        int addressCounter;
        int loadAddress;

        const QRegularExpression whitespacesSplitRegex;
        const QRegularExpression labelRegex;
        const QRegularExpression MOCRegex;
        const QRegularExpression directiveRegex;
        const QRegularExpression operandsRegex;

    private:
        QStringList deleteEmptyTokens(const QStringList& _tokens);
        bool isLabel(const QString& _token);
        bool isMOC(const QString& _token);
        bool isDirective(const QString& _token);

    public:
        Translator();
        ~Translator();

    public:
        void firstPass(QString _source, std::map<QString, std::tuple<int, int>> _OCT);
        void secondPass();
    };
}

#endif // TRANSLATOR_H