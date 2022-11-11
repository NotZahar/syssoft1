#include "mainWindow.hpp"

namespace syssoft1 {
    MainWindow::MainWindow(QWidget* _parent) :
        QWidget(_parent)
    {
        QObject::connect(this, &MainWindow::sourceCodeIsGiven, &uiController, &UIController::sourceCodeWasGiven);
        
        QFile file("source.syssoft1");
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            std::cout << line.toStdString();
        }   

        // emit sourceCodeIsGiven(std::string_view(""));
    }

    MainWindow::~MainWindow() {
    }
}
