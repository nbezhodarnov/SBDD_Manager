#include "formulasbinaryfunctionsimporterwidget.h"
#include "ui_formulasbinaryfunctionsimporterwidget.h"

FormulasBinaryFunctionsImporterWidget::FormulasBinaryFunctionsImporterWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormulasBinaryFunctionsImporterWidget)
{
    ui->setupUi(this);
    connect(this->ui->formulasList, &QListWidget::currentRowChanged, this, &FormulasBinaryFunctionsImporterWidget::formulasList_currentRowChanged);
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
    disconnect(this->ui->formulasList, &QListWidget::currentRowChanged, this, &FormulasBinaryFunctionsImporterWidget::formulasList_currentRowChanged);
    delete ui;
}

void FormulasBinaryFunctionsImporterWidget::formulasList_currentRowChanged(const int &currentRow)
{
    if (currentRow == this->ui->formulasList->count() - 1)
    {
        if (this->ui->formulasList->item(currentRow)->text() == "")
        {
            delete this->ui->formulasList->takeItem(this->ui->formulasList->count() - 1);
        }
        else
        {
            this->ui->formulasList->addItem("");
        }
    }
}

