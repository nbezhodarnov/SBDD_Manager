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
    explicit SatWidget(const std::vector < Sat > &new_sats, QWidget *parent = nullptr);
    ~SatWidget();

private:
    Ui::SatWidget *ui;
    std::vector < Sat > sats;
};

#endif // SATWIDGET_H
