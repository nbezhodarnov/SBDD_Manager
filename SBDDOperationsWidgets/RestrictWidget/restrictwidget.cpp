#include "restrictwidget.h"
#include "ui_restrictwidget.h"

#include <iostream>

RestrictWidget::RestrictWidget(const std::vector<std::string> &new_variables, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestrictWidget)
{
    ui->setupUi(this);
    this->variables = new_variables;
    for (std::string &variable : this->variables)
    {
        this->ui->variablesListWidget->addItem(QString::fromStdString(variable));
    }
}

std::pair < std::string, bool > RestrictWidget::GetVariableAndValue() const
{
    int selected_variable_index = 0;
    if (this->ui->variablesListWidget->selectedItems().size() == 0)
    {
        if (this->variables.size() == 0)
        {
            std::cerr << "RestrictWidget: No variables to choose!\n";
            return {"", false};
        }
        std::cerr << "SBDDSelectWidget: No choosen variable! First one will be selected!\n";
    }
    else
    {
        selected_variable_index = this->ui->variablesListWidget->row(
                    this->ui->variablesListWidget->selectedItems()[0]
                );
    }
    return {this->variables[selected_variable_index], (bool)this->ui->valueButtonGroup->checkedId()};
}

RestrictWidget::~RestrictWidget()
{
    delete ui;
}
