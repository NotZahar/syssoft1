#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QString>
#include <QFile>
#include <QFileInfo>
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
        QPushButton* againBtn;
        QLabel* sourceLabel;
        QLabel* OCTLabel;
        QLabel* auxiliaryLabel;
        QLabel* SNTLabel;
        QLabel* firstPassErrorsLabel;
        QLabel* OBMLabel;
        QLabel* secondPassErrorsLabel;

        const int initialWindowGeometryWidth;
        const int initialWindowGeometryHeight;

    private slots:
        void firstPassBtnWasPushed(bool);
        void secondPassBtnWasPushed(bool);
        void againBtnWasPushed(bool);

    signals:
        void firstPassIsBegun();
        void secondPassIsBegun();
        void againIsBegun();

    public:
        MainWindow() = delete;
        explicit MainWindow(QWidget* _parent = nullptr);

        ~MainWindow();

    public:
        QStandardItemModel* getOCTTableModel();
        QTableView* getOCTTableView();
        QStandardItemModel* getSNTTableModel();
        QTableView* getSNTTableView();
        QTextEdit* getSourceEdit();
        QTextEdit* getOBMEdit();
        QTextEdit* getFirstPassErrorsEdit();
        QTextEdit* getSecondPassErrorsEdit();
        QTextEdit* getAuxiliaryEdit();
        QPushButton* getFirstPassBtn();
        QPushButton* getSecondPassBtn();
        QPushButton* getAgainBtn();
    };
}

#endif // MAINWINDOW_H