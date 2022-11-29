#include "mainWindow.hpp"

namespace syssoft1 {
    MainWindow::MainWindow(QWidget* _parent) :
        QWidget(_parent),
        palette(),
        mainLayout(new QVBoxLayout()),
        topLayout(new QHBoxLayout()),
        bottomLayout(new QHBoxLayout()),
        leftLayout(new QVBoxLayout()),
        middleLayout(new QVBoxLayout()),
        rightLayout(new QVBoxLayout()),
        OCTTableModel(new QStandardItemModel()),
        OCTTableView(new QTableView()),
        SNTTableModel(new QStandardItemModel()),
        SNTTableView(new QTableView()),
        sourceEdit(new QTextEdit()),
        auxiliaryEdit(new QTextEdit()),
        firstPassErrorsEdit(new QTextEdit()),
        secondPassErrorsEdit(new QTextEdit()),
        OBMEdit(new QTextEdit()),
        firstPassBtn(new QPushButton("Первый проход")),
        secondPassBtn(new QPushButton("Второй проход")),
        sourceLabel(new QLabel("Исходный код")),
        OCTLabel(new QLabel("Таблица кодов операций")),
        auxiliaryLabel(new QLabel("Вспомогательная таблица")),
        SNTLabel(new QLabel("Таблица символических имен")),
        firstPassErrorsLabel(new QLabel("Ошибки первого прохода")),
        OBMLabel(new QLabel("Выходной объектный модуль")),
        secondPassErrorsLabel(new QLabel("Ошибки второго прохода")),
        initialWindowGeometryWidth(1000),
        initialWindowGeometryHeight(600)
    {
        QObject::connect(firstPassBtn, &QPushButton::clicked, this, &MainWindow::firstPassBtnWasPushed);
        QObject::connect(secondPassBtn, &QPushButton::clicked, this, &MainWindow::secondPassBtnWasPushed);

        this->setGeometry(0, 0, initialWindowGeometryWidth, initialWindowGeometryHeight);
        this->setWindowTitle("Двухпросмотровый ассемблер в абсолютном формате");
        this->setWindowFlags(this->windowFlags()
                            & ~Qt::WindowMaximizeButtonHint);
        
        palette.setColor(QPalette::Window, Qt::black);
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Button, Qt::lightGray);
        this->setAutoFillBackground(true); 
        this->setPalette(palette);

        secondPassBtn->setEnabled(false);

        const QFont generalFont("Ubuntu Mono", 14);
        const QFont labelFont("Ubuntu Mono", 12);
        const QFont btnFont("Ubuntu Mono", 14, QFont::Bold);
        sourceEdit->setFont(generalFont);
        auxiliaryEdit->setFont(generalFont);
        firstPassErrorsEdit->setFont(generalFont);
        secondPassErrorsEdit->setFont(generalFont);
        OBMEdit->setFont(generalFont);
        OCTTableView->setFont(generalFont);
        SNTTableView->setFont(generalFont);
        firstPassBtn->setFont(btnFont);
        secondPassBtn->setFont(btnFont);
        sourceLabel->setFont(labelFont);
        sourceLabel->setFont(labelFont);
        OCTLabel->setFont(labelFont);
        auxiliaryLabel->setFont(labelFont);
        SNTLabel->setFont(labelFont);
        firstPassErrorsLabel->setFont(labelFont);
        OBMLabel->setFont(labelFont);
        secondPassErrorsLabel->setFont(labelFont);

        auxiliaryEdit->setReadOnly(true);
        firstPassErrorsEdit->setReadOnly(true);
        secondPassErrorsEdit->setReadOnly(true);
        OBMEdit->setReadOnly(true);
        OCTTableView->setShowGrid(true);
        OCTTableView->verticalHeader()->setVisible(false);
        SNTTableView->setShowGrid(true);
        SNTTableView->verticalHeader()->setVisible(false);

        sourceEdit->setMinimumHeight((2.7 / 5.0) * initialWindowGeometryHeight);
        auxiliaryEdit->setMinimumHeight((2.2 / 5.0) * initialWindowGeometryHeight);
        firstPassErrorsEdit->setMaximumHeight((0.4 / 5.0) * initialWindowGeometryHeight);
        secondPassErrorsEdit->setMaximumHeight((0.4 / 5.0) * initialWindowGeometryHeight);
        OBMEdit->setMinimumHeight((2.7 / 5.0) * initialWindowGeometryHeight);

        leftLayout->addWidget(sourceLabel);
        leftLayout->addWidget(sourceEdit);
        leftLayout->addWidget(OCTLabel);
        leftLayout->addWidget(OCTTableView);
        middleLayout->addWidget(auxiliaryLabel);
        middleLayout->addWidget(auxiliaryEdit);
        middleLayout->addWidget(SNTLabel);
        middleLayout->addWidget(SNTTableView);
        middleLayout->addWidget(firstPassErrorsLabel);
        middleLayout->addWidget(firstPassErrorsEdit);
        rightLayout->addWidget(OBMLabel);
        rightLayout->addWidget(OBMEdit);
        rightLayout->addWidget(secondPassErrorsLabel);
        rightLayout->addWidget(secondPassErrorsEdit);
        topLayout->addLayout(leftLayout);
        topLayout->addLayout(middleLayout);
        topLayout->addLayout(rightLayout);
        bottomLayout->addWidget(firstPassBtn);
        bottomLayout->addWidget(secondPassBtn);
        mainLayout->addLayout(topLayout);
        mainLayout->addLayout(bottomLayout);
        this->setLayout(mainLayout);
    }

    MainWindow::~MainWindow() {

    }

    QStandardItemModel* MainWindow::getOCTTableModel() {
        return OCTTableModel;
    }

    QTableView* MainWindow::getOCTTableView() {
        return OCTTableView;
    }

    QStandardItemModel* MainWindow::getSNTTableModel() {
        return SNTTableModel;
    }
    
    QTableView* MainWindow::getSNTTableView() {
        return SNTTableView;
    }

    QTextEdit* MainWindow::getSourceEdit() {
        return sourceEdit;
    }

    QTextEdit* MainWindow::getOBMEdit() {
        return OBMEdit;
    }

    QTextEdit* MainWindow::getFirstPassErrorsEdit() {
        return firstPassErrorsEdit;
    }
    
    QTextEdit* MainWindow::getSecondPassErrorsEdit() {
        return secondPassErrorsEdit;
    }

    QTextEdit* MainWindow::getAuxiliaryEdit() {
        return auxiliaryEdit;
    }

    QPushButton* MainWindow::getFirstPassBtn() {
        return firstPassBtn;
    }
    
    QPushButton* MainWindow::getSecondPassBtn() {
        return secondPassBtn;
    }

    void MainWindow::firstPassBtnWasPushed(bool) {
        emit firstPassIsBegun();
    }

    void MainWindow::secondPassBtnWasPushed(bool) {
        emit secondPassIsBegun();
    }
}
