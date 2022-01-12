#ifndef SATWIDGET_H
#define SATWIDGET_H

#include <vector>

#include <QDialog>

#include "SBDD/sbdd.h"

namespace Ui {
class SatWidget;
}

class SatWidget : public QDialog
{
    Q_OBJECT

public:
    explicit SatWidget(
            const std::vector < Sat > &new_sats,
            const std::vector < std::string > new_variables,
            QWidget *parent = nullptr
            );
    ~SatWidget();

private:
    Ui::SatWidget *ui;
    std::vector < Sat > sats;
    std::vector < std::string > variables;
};

#endif // SATWIDGET_H
