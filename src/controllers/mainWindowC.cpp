#include "mainWindowC.hpp"

namespace syssoft1 {
    MainWindowC::MainWindowC() : 
        mainWindow(nullptr) 
    {
        QObject::connect(&mainWindow, &MainWindow::sourceCodeIsGiven, this, &MainWindowC::sourceCodeWasGiven);
        
        QFile file("/home/zahar/Документы/syssoft/syssoft1/src/UI/source.syssoft1");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) qDebug() << "!!!";

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            qDebug() << line;
        }

        // emit sourceCodeIsGiven(std::string_view(""));
    }

    MainWindowC::~MainWindowC() {
    }

    void MainWindowC::sourceCodeWasGiven([[maybe_unused]] const std::string_view _sourceCode) {

    }
}