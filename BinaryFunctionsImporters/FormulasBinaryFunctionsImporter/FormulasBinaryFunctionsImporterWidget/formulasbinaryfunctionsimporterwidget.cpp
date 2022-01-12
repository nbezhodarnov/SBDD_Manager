#include "formulasbinaryfunctionsimporterwidget.h"
#include "ui_formulasbinaryfunctionsimporterwidget.h"

FormulasBinaryFunctionsImporterWidget::FormulasBinaryFunctionsImporterWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormulasBinaryFunctionsImporterWidget)
{
    ui->setupUi(this);
    connect(
                this->ui->formulasList,
                &QListWidget::itemChanged,
                this,
                &FormulasBinaryFunctionsImporterWidget::formulasList_formulasList_itemChanged
                );
    this->deleting_allowed = true;
}

std::vector < std::string > FormulasBinaryFunctionsImporterWidget::GetFormulas() const
{
    std::vector < std::string > formulas;
    for (unsigned int i = 0; i < (unsigned int)this->ui->formulasList->count() - 1; i++)
    {
        formulas.push_back(this->ui->formulasList->item(i)->text().toStdString());
    }
    return formulas;
}

FormulasBinaryFunctionsImporterWidget::~FormulasBinaryFunctionsImporterWidget()
{
    disconnect(
                this->ui->formulasList,
                &QListWidget::itemChanged,
                this,
                &FormulasBinaryFunctionsImporterWidget::formulasList_formulasList_itemChanged
                );
    delete ui;
}

void FormulasBinaryFunctionsImporterWidget::formulasList_formulasList_itemChanged(QListWidgetItem *item)
{
    int current_row = this->ui->formulasList->row(item);
    if (current_row == this->ui->formulasList->count() - 1)
    {
        if (this->ui->formulasList->item(current_row)->text() == "")
        {
            if (this->ui->formulasList->count() != 1 && this->deleting_allowed)
            {
                delete this->ui->formulasList->takeItem(this->ui->formulasList->count() - 1);
            }
        }
        else
        {
            this->deleting_allowed = false;
            this->ui->formulasList->addItem("");
            this->ui->formulasList->item(this->ui->formulasList->count() - 1)->setFlags(
                        Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled
                        );
            this->deleting_allowed = true;
        }
    }
}
