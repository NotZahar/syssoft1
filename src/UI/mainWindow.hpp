#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QLabel>
#include <QFont>

#include <string_view>
#include <iostream>
#include <regex>

#include "../errors/error.hpp"

namespace syssoft1 {
    class MainWindow : public QWidget {
        Q_OBJECT

    private:
        QPalette palette;
        QVBoxLayout* mainLayout;
        QHBoxLayout* topLayout;
        QHBoxLayout* bottomLayout;
        QVBoxLayout* leftLayout;
        QVBoxLayout* middleLayout;
        QVBoxLayout* rightLayout;
        QStandardItemModel* OCTTableModel;
        QTableView* OCTTableView;
        QStandardItemModel* SNTTableModel;
        QTableView* SNTTableView;
        QTextEdit* sourceEdit;
        QTextEdit* auxiliaryEdit;
        QTextEdit* firstPassErrorsEdit;
        QTextEdit* secondPassErrorsEdit;
        QTextEdit* OBMEdit;
        QPushButton* firstPassBtn;
        QPushButton* secondPassBtn;
        QLabel* sourceLabel;
        QLabel* OCTLabel;
        QLabel* auxiliaryLabel;
        QLabel* SNTLabel;
        QLabel* firstPassErrorsLabel;
        QLabel* OBMLabel;
        QLabel* secondPassErrorsLabel;

        const int initialWindowGeometryWidth;
        const int initialWindowGeometryHeight;

    signals:
        void sourceCodeIsGiven(const std::string_view _sourceCode);

    public:
        MainWindow() = delete;
        explicit MainWindow(QWidget* _parent = nullptr);

        ~MainWindow();
    };
}

#endif // MAINWINDOW_H