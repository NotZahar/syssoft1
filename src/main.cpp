#include <QApplication>

#include "UI/mainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    syssoft1::MainWindow mainWindow(nullptr);

    return a.exec();
}
