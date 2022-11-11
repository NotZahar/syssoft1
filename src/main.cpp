#include <QApplication>

#include "controllers/mainWindowC.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    syssoft1::MainWindowC mainWindowC;

    return a.exec();
}
