#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include <string_view>
#include <iostream>

namespace syssoft1 {
    class MainWindow : public QWidget {
        Q_OBJECT

    private:

    signals:
        void sourceCodeIsGiven(const std::string_view _sourceCode);

    public:
        MainWindow() = delete;
        explicit MainWindow(QWidget* _parent = nullptr);

        ~MainWindow();
    };
}

#endif // MAINWINDOW_H