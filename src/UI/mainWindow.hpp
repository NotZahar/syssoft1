#include <QWidget>
#include <QFile>

#include <string_view>
#include <iostream>

#include "../controllers/UIController.hpp"

namespace syssoft1 {
    class MainWindow : public QWidget {
        Q_OBJECT

    private:
        UIController uiController;

    signals:
        void sourceCodeIsGiven(const std::string_view _sourceCode);

    public:
        MainWindow() = delete;
        explicit MainWindow(QWidget* _parent = nullptr);

        ~MainWindow();
    };
}
