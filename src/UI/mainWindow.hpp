#include <QWidget>

#include <string_view>

#include "../model/UIModel.hpp"

namespace syssoft1 {
    class MainWindow : public QWidget {
        Q_OBJECT

    private:
        UIModel model;

    signals:
        void sourceCodeIsGiven(const std::string_view _sourceCode);

    public:
        MainWindow() = delete;
        explicit MainWindow(QWidget* _parent = nullptr);

        ~MainWindow();
    };
}
