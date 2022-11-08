#include <QApplication>
#include <QDebug>

#include <iostream>
#include <string>

#include "print.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Print p("Zdarova!");
    p.print();

    return a.exec();
}
