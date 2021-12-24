#ifndef RESTRICTWIDGET_H
#define RESTRICTWIDGET_H

#include <vector>
#include <string>

#include <QDialog>

namespace Ui {
class RestrictWidget;
}

class RestrictWidget : public QDialog
{
    Q_OBJECT

public:
    explicit RestrictWidget(
            const std::vector < std::string > &new_variables,
            QWidget *parent = nullptr
            );
    std::pair < std::string, bool > GetVariableAndValue() const;
    ~RestrictWidget();

private:
    Ui::RestrictWidget *ui;
    std::vector < std::string > variables;
};

#endif // RESTRICTWIDGET_H
