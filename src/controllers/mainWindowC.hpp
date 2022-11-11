#ifndef MAINWINDOWC_H
#define MAINWINDOWC_H

#include <QObject>

#include <string_view>

#include "../algorithm/translator.hpp"
#include "../UI/mainWindow.hpp"

namespace syssoft1 {
    class MainWindowC : public QObject {
        Q_OBJECT

    private:
        MainWindow mainWindow;

        std::string_view sourceCode;
        Translator translator;

    public slots:
        void sourceCodeWasGiven(const std::string_view _sourceCode);

    public:
        MainWindowC();
        ~MainWindowC();
    };
}

#endif // MAINWINDOWC_H