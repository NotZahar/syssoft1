#ifndef MAINWINDOWC_H
#define MAINWINDOWC_H

#include <QObject>
#include <QDebug>

#include <regex>

#include "../algorithm/translator.hpp"
#include "../UI/mainWindow.hpp"
#include "../errors/error.hpp"
#include "../middleware/validate.hpp"

namespace syssoft1 {
    class MainWindowC : public QObject {
        Q_OBJECT

    private:
        MainWindow mainWindow;
        Translator translator;

    private:
        void fillOutTheWindowWithInitialData();
        void addDataToUIAfterFirstPass();
        void clearUI();

    private slots:
       void firstPassWasBegun();
       void secondPassWasBegun();

    public:
        MainWindowC();
        ~MainWindowC();
    }; 
}

#endif // MAINWINDOWC_H