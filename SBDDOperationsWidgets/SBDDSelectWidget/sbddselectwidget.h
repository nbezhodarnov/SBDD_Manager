#ifndef SBDDSELECTWIDGET_H
#define SBDDSELECTWIDGET_H

#include <memory>
#include <vector>

#include <QDialog>

#include "SBDD/sbdd.h"
#include "BinaryOperation/BinaryOperationOr/binaryoperationor.h"
#include "BinaryOperation/BinaryOperationAnd/binaryoperationand.h"
#include "BinaryOperation/BinaryOperationNot/binaryoperationnot.h"


namespace Ui {
class SBDDSelectWidget;
}

class SBDDSelectWidget : public QDialog
{
    Q_OBJECT

public:
    explicit SBDDSelectWidget(
            const std::vector < SBDD > &new_sbdd_vector,
            const unsigned int &choosen_sbdd_index,
            QWidget *parent = nullptr
            );
    SBDD GetSelectedSBDD() const;
    std::shared_ptr <AbstractBinaryOperation> GetOperation() const;
    ~SBDDSelectWidget();

private:
    Ui::SBDDSelectWidget *ui;
    std::vector < SBDD > sbdd_vector;
};

#endif // SBDDSELECTWIDGET_H
