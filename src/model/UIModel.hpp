#include <QObject>

#include <string_view>

#include "../algorithm/translator.hpp"

namespace syssoft1 {
    class UIModel : public QObject {
        Q_OBJECT

    private:
        std::string_view sourceCode;
        Translator translator;

    public slots:
        void sourceCodeWasGiven(const std::string_view _sourceCode);

    public:
        UIModel();
        ~UIModel();
    };
}