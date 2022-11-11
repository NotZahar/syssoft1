#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QObject>

#include <string_view>

#include "../algorithm/translator.hpp"

namespace syssoft1 {
    class UIController : public QObject {
        Q_OBJECT

    private:
        std::string_view sourceCode;
        Translator translator;

    public slots:
        void sourceCodeWasGiven(const std::string_view _sourceCode);

    public:
        UIController();
        ~UIController();
    };
}

#endif // UICONTROLLER_H