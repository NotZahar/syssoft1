#include "mainWindow.hpp"
// TODO: перенести в контроллер подгрузку из файла
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
        firstPassErrorsEdit(new QTextEdit),
        secondPassErrorsEdit(new QTextEdit),
        OBMEdit(new QTextEdit),
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
        initialWindowGeometryHeight(700)
    {
        this->setGeometry(0, 0, initialWindowGeometryWidth, initialWindowGeometryHeight);
        this->setWindowTitle("Двухпросмотровый ассемблер в абсолютном формате");
        this->setWindowFlags(this->windowFlags()
                            & ~Qt::WindowMaximizeButtonHint);
        
        palette.setColor(QPalette::Window, Qt::black);
        palette.setColor(QPalette::WindowText, Qt::white);
        this->setAutoFillBackground(true); 
        this->setPalette(palette);

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

        sourceEdit->setMinimumHeight((2.7 / 5.0) * initialWindowGeometryHeight);
        auxiliaryEdit->setMinimumHeight((2.2 / 5.0) * initialWindowGeometryHeight);
        firstPassErrorsEdit->setMinimumHeight((0.8 / 5.0) * initialWindowGeometryHeight);
        OBMEdit->setMinimumHeight((2.7 / 5.0) * initialWindowGeometryHeight);

        const QStringList OCTLabels = {"МКОП", "ДКОП", "Длина"};
        OCTTableModel->setHorizontalHeaderLabels(OCTLabels);
        OCTTableView->setModel(OCTTableModel);
        for (int i = 0; i < OCTLabels.size(); ++i) OCTTableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        
        QFile sourceFile("/home/zahar/Documents/syssoft/syssoft1/src/sourceExample/source.syssoft1");
        QFile OCTFile("/home/zahar/Documents/syssoft/syssoft1/src/sourceExample/oct.syssoft1");
        
        if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString startSource = "";
            while (!sourceFile.atEnd()) {
                startSource += sourceFile.readLine();
            }

            sourceEdit->setText(startSource);   
        } else {
            QMessageBox::critical(nullptr, "Ошибка", Error::errorMessages.at(Error::error::fileCannotBeOpened));
        }
        
        for (int row = 0; row < 45; ++row) { // maximum number of commands
            for (int column = 0; column < OCTLabels.size(); ++column) {
                QStandardItem* cell = new QStandardItem("");
                cell->setEditable(true);
                OCTTableModel->setItem(row, column, cell);
            }
        }

        if (OCTFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            const std::regex rowOCTRegex("^ *\\.[a-z]+[0-9]* +\\.\\.(0x[0-9]{1,9}|[0-9]{1,9}) +\\.\\.\\.[0-9]{1,9} *$");
            const std::regex MOCRegex("\\.[a-z]+[0-9]*");
            const std::regex BOCRegex("\\.\\.(0x[0-9]{1,9}|[0-9]{1,9})");
            const std::regex lengthRegex("\\.\\.\\.[0-9]{1,9}");
            std::smatch rawMOC;
            std::smatch rawBOC;
            std::smatch rawLength;
            int i = 0;
            while (!OCTFile.atEnd()) {
                std::string row = OCTFile.readLine().toStdString();
                if (row.empty()) {
                    continue;
                }

                if (row[row.size() - 1] == '\n') {
                    row = row.substr(0, row.size() - 1);
                }

                if (!std::regex_match(row, rowOCTRegex)) {
                    continue;
                }

                std::regex_search(row, rawMOC, MOCRegex);
                std::regex_search(row, rawBOC, BOCRegex);
                std::regex_search(row, rawLength, lengthRegex);

                OCTTableModel->item(i, 0)->setText(QString::fromStdString(rawMOC.str().substr(1)));
                OCTTableModel->item(i, 1)->setText(QString::fromStdString(rawBOC.str().substr(2)));
                OCTTableModel->item(i, 2)->setText(QString::fromStdString(rawLength.str().substr(3)));
                ++i;
            }   
        } else {
            QMessageBox::critical(nullptr, "Ошибка", Error::errorMessages.at(Error::error::fileCannotBeOpened));
        }

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
}
