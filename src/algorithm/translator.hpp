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

#include "../errors/error.hpp"

namespace syssoft1 {
    class Translator {
    private:
        std::map<QString, std::tuple<int, int>> OCT;
        std::map<QString, int> SNT;
        const std::set<QString> directivesList;
        const int impossibleNonnegativeIntegerValue;
        int addressCounter;
        int loadAddress;
        int rowNumberThatHasStartDirective;
        int rowNumberThatHasEndDirective;
        int firstNonEmptyRowNumber;

        const QRegularExpression whitespacesSplitRegex;
        const QRegularExpression labelRegex;
        const QRegularExpression MOCRegex;
        const QRegularExpression directiveRegex;
        const QRegularExpression operandsRegex;
        const QRegularExpression startDirectiveRegex;
        const QRegularExpression endDirectiveRegex;

    private:
        QStringList deleteEmptyTokens(const QStringList& _tokens);
        bool isLabel(const QString& _token);
        bool isMOC(const QString& _token);
        bool isDirective(const QString& _token);
        bool hasStartDirective(const QStringList& _tokens);
        bool hasEndDirective(const QStringList& _tokens);

    public:
        Translator();
        ~Translator();

    public:
        void firstPass(QString _source, std::map<QString, std::tuple<int, int>> _OCT);
        void secondPass();
    };
}

#endif // TRANSLATOR_H