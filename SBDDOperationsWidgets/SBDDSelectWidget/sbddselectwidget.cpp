#include "sbddselectwidget.h"
#include "ui_sbddselectwidget.h"

#include <iostream>

SBDDSelectWidget::SBDDSelectWidget(
        const std::vector < SBDD > &new_sbdd_vector,
        const unsigned int &choosen_sbdd_index,
        QWidget *parent
        ) :
    QDialog(parent),
    ui(new Ui::SBDDSelectWidget)
{
    ui->setupUi(this);
    this->sbdd_vector = new_sbdd_vector;
    for (unsigned int i = 0; i < choosen_sbdd_index; i++)
    {
        this->ui->sbddListWidget->addItem("SBDD " + QString::number(i + 1));
    }
    for (unsigned int i = choosen_sbdd_index + 1; i < this->sbdd_vector.size() + 1; i++)
    {
        this->ui->sbddListWidget->addItem("SBDD " + QString::number(i + 1));
    }
}

SBDD SBDDSelectWidget::GetSelectedSBDD() const
{
    int selected_sbdd_index = 0;
    if (this->ui->sbddListWidget->selectedItems().size() == 0)
    {
        if (this->sbdd_vector.size() == 0)
        {
            std::cerr << "SBDDSelectWidget: No SBDD to choose!\n";
            return SBDD();
        }
        std::cerr << "SBDDSelectWidget: No choosen SBDD! First one will be selected!\n";
    }
    else
    {
        selected_sbdd_index = this->ui->sbddListWidget->row(
                    this->ui->sbddListWidget->selectedItems()[0]
                );
    }
    return this->sbdd_vector[selected_sbdd_index];
}

std::shared_ptr < AbstractBinaryOperation > SBDDSelectWidget::GetOperation() const
{
    std::shared_ptr < AbstractBinaryOperation > operation;
    switch (this->ui->operationsButtonGroup->checkedId())
    {
    case -2:
        operation = std::make_shared < BinaryOperationNot >();
        break;
    case -3:
        operation = std::make_shared < BinaryOperationAnd >();
        break;
    case -4:
        operation = std::make_shared < BinaryOperationOr >();
        break;
    default:
        operation = std::make_shared < BinaryOperationNot >();
        break;
    }
    return operation;
}

SBDDSelectWidget::~SBDDSelectWidget()
{
    delete ui;
}
