#include "mainWindow.hpp"

namespace syssoft1 {
    MainWindow::MainWindow(QWidget* _parent) :
        QWidget(_parent)
    {
        QObject::connect(this, &MainWindow::sourceCodeIsGiven, &model, &UIModel::sourceCodeWasGiven);

        emit sourceCodeIsGiven(std::string_view(""));
    }

    MainWindow::~MainWindow() {
    }
}
