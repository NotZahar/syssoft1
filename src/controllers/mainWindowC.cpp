#include "mainWindowC.hpp"

namespace syssoft1 {
    MainWindowC::MainWindowC() : 
        mainWindow(nullptr),
        translator()
    {
        QObject::connect(&mainWindow, &MainWindow::firstPassIsBegun, this, &MainWindowC::firstPassWasBegun);

        fillOutTheWindowWithInitialData();
        mainWindow.show();
    }

    MainWindowC::~MainWindowC() {

    }

    void MainWindowC::fillOutTheWindowWithInitialData() {
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
            const std::regex rowOCTRegex("^ *\\." + Validate::MOCRegexStr + " +\\.\\." + Validate::BOCRegexStr + " +\\.\\.\\." + Validate::lengthRegexStr + " *$");
            const std::regex MOCRegex("\\." + Validate::MOCRegexStr);
            const std::regex BOCRegex("\\.\\." + Validate::BOCRegexStr);
            const std::regex lengthRegex("\\.\\.\\." + Validate::lengthRegexStr);
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
        std::map<QString, std::tuple<int, int>> OCT;
        QStandardItemModel* OCTTableModel = mainWindow.getOCTTableModel();
        QTextEdit* sourceEdit = mainWindow.getSourceEdit();

        int rowCount = OCTTableModel->rowCount();
        for (int i = 0; i < rowCount; ++i) {
            std::string MOC = OCTTableModel->item(i, 0)->text().toStdString();
            std::string BOC = OCTTableModel->item(i, 1)->text().toStdString(); 
            std::string length = OCTTableModel->item(i, 2)->text().toStdString();

            if (!Validate::containsMOC(MOC) 
                || !Validate::containsBOC(BOC)
                || !Validate::containsLength(length)) {
                continue;
            }

            std::smatch MOCMatch;
            std::smatch BOCMatch;
            std::smatch lengthMatch;
            std::regex_search(MOC, MOCMatch, std::regex(Validate::MOCRegexStr));
            std::regex_search(BOC, BOCMatch, std::regex(Validate::BOCRegexStr));
            std::regex_search(length, lengthMatch, std::regex(Validate::lengthRegexStr));
            
            QString MOCStr = QString::fromStdString(MOCMatch.str());
            QString BOCStr = QString::fromStdString(BOCMatch.str());
            QString lengthStr = QString::fromStdString(lengthMatch.str());

            bool ok;
            int BOCNum = BOCStr.toInt(&ok, 0);
            int lengthNum = lengthStr.toInt(&ok, 0);

            if (BOCNum > 0x3f) {
                continue;
            }
 
            for (const auto& OCTRow : OCT) {
                if (MOCStr == OCTRow.first) {
                    QMessageBox::critical(nullptr, "Ошибка", Error::errorMessages.at(Error::error::redefiningMOC));
                    return;
                }

                const auto& [BOCP, lengthP] = OCTRow.second;
                if (BOCNum == BOCP) {
                    QMessageBox::critical(nullptr, "Ошибка", Error::errorMessages.at(Error::error::redefiningBOC));
                    return;
                }
            }

            OCT.insert({MOCStr, {BOCNum, lengthNum}});
        }

        translator.firstPass(sourceEdit->toPlainText(), OCT); // TODO: add try
    }
}
