#ifndef MAINWINDOWC_H
#define MAINWINDOWC_H

#include <QObject>
#include <QDebug>

#include <regex>
#include <iostream>

#include "../algorithm/translator.hpp"
#include "../UI/mainWindow.hpp"
#include "../errors/error.hpp"
#include "validate.hpp"

namespace syssoft1 {
    class MainWindowC : public QObject {
        Q_OBJECT

    private:
        MainWindow mainWindow;
        Translator translator;

    public:
        MainWindowC();
        ~MainWindowC();

    private slots:
       void firstPassWasBegun();
    
    private:
        void fillOutTheWindowWithInitialData();
    }; 
}

#endif // MAINWINDOWC_H