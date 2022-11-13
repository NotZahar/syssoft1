#include "mainWindowC.hpp"

namespace syssoft1 {
    MainWindowC::MainWindowC() : 
        mainWindow(nullptr) 
    {
        QObject::connect(&mainWindow, &MainWindow::sourceCodeIsGiven, this, &MainWindowC::sourceCodeWasGiven);

        mainWindow.show();
    }

    MainWindowC::~MainWindowC() {

    }

    void MainWindowC::sourceCodeWasGiven([[maybe_unused]] const std::string_view _sourceCode) {

    }
}