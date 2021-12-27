#include "satwidget.h"
#include "ui_satwidget.h"

SatWidget::SatWidget(
        const std::vector < Sat > &new_sats,
        const std::vector<std::string> new_variables,
        QWidget *parent
        ) :
    QDialog(parent),
    ui(new Ui::SatWidget)
{
    ui->setupUi(this);
    sats = new_sats;
    if (this->sats.size() == 0)
    {
        return;
    }
    this->variables = new_variables;
    this->ui->satTableWidget->setColumnCount(this->variables.size());
    this->ui->satTableWidget->setRowCount(this->sats.size() + 1);
    for (unsigned int i = 0; i < this->variables.size(); i++)
    {
        QTableWidgetItem *table_cell = this->ui->satTableWidget->item(0, i);
        if(!table_cell)
        {
            table_cell = new QTableWidgetItem();
            this->ui->satTableWidget->setItem(0, i, table_cell);
        }
        table_cell->setText(QString::fromStdString(this->variables[i]));
    }
    for (unsigned int i = 0; i < this->sats.size(); i++)
    {
        for (unsigned int j = 0; j < this->variables.size(); j++)
        {
            QTableWidgetItem *table_cell = this->ui->satTableWidget->item(i + 1, j);
            if(!table_cell)
            {
                table_cell = new QTableWidgetItem();
                this->ui->satTableWidget->setItem(i + 1, j, table_cell);
            }
            auto unit_iterator = std::find_if(
                        this->sats[i].units.begin(),
                        this->sats[i].units.end(),
                        [&](const SatUnit &sat_unit)
                        {return sat_unit.variable_name == this->variables[j];}
                        );
            if (unit_iterator == this->sats[i].units.end())
            {
                table_cell->setText("-");
            }
            else
            {
                SatUnit unit = *unit_iterator;
                table_cell->setText(unit.value ? "1" : "0");
            }
        }
    }
}

SatWidget::~SatWidget()
{
    delete ui;
}
