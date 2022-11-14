#include "mainWindowC.hpp"

namespace syssoft1 {
    MainWindowC::MainWindowC() : 
        mainWindow(nullptr),
        translator()
    {
        QObject::connect(&mainWindow, &MainWindow::firstPassIsBegun, this, &MainWindowC::firstPassWasBegun);

        initMainWindow();
        mainWindow.show();
    }

    MainWindowC::~MainWindowC() {

    }

    void MainWindowC::initMainWindow() {
        QStandardItemModel* OCTTableModel = mainWindow.getOCTTableModel();
        QTableView* OCTTableView = mainWindow.getOCTTableView();
        QTextEdit* sourceEdit = mainWindow.getSourceEdit();

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
    }

    void MainWindowC::firstPassWasBegun() {
        std::map<QString, std::tuple<QString, QString>> OCT;
        QStandardItemModel* OCTTableModel = mainWindow.getOCTTableModel();
        [[maybe_unused]] QTextEdit* sourceEdit = mainWindow.getSourceEdit();

        int rowCount = OCTTableModel->rowCount();
        for (int i = 0; i < rowCount; ++i) {
            QString MOC = OCTTableModel->item(i, 0)->text();
            QString BOC = OCTTableModel->item(i, 1)->text(); 
            QString length = OCTTableModel->item(i, 2)->text();

            if (MOC.isEmpty() || BOC.isEmpty() || length.isEmpty()) {
                continue;
            } else {
                OCT.insert({MOC, {BOC, length}});
            }
        }

        // TODO: начинать здесь 
    }
}