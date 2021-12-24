#include "satwidget.h"
#include "ui_satwidget.h"

SatWidget::SatWidget(const std::vector < Sat > &new_sats, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SatWidget)
{
    ui->setupUi(this);
    sats = new_sats;
    if (this->sats.size() == 0)
    {
        return;
    }
    std::vector < std::string > variables;
    for (SatUnit &unit : this->sats[0].units)
    {
        variables.push_back(unit.variable_name);
    }
    this->ui->satTableWidget->setColumnCount(variables.size());
    this->ui->satTableWidget->setRowCount(this->sats.size() + 1);
    for (unsigned int i = 0; i < variables.size(); i++)
    {
        QTableWidgetItem *table_cell = this->ui->satTableWidget->item(0, i);
        if(!table_cell)
        {
            table_cell = new QTableWidgetItem();
            this->ui->satTableWidget->setItem(0, i, table_cell);
        }
        table_cell->setText(QString::fromStdString(variables[i]));
    }
    for (unsigned int i = 0; i < this->sats.size(); i++)
    {
        for (unsigned int j = 0; j < variables.size(); j++)
        {
            QTableWidgetItem *table_cell = this->ui->satTableWidget->item(i + 1, j);
            if(!table_cell)
            {
                table_cell = new QTableWidgetItem();
                this->ui->satTableWidget->setItem(i + 1, j, table_cell);
            }
            SatUnit unit = *std::find_if(
                        this->sats[i].units.begin(),
                        this->sats[i].units.end(),
                        [&](const SatUnit &sat_unit)
                        {return sat_unit.variable_name == variables[j];}
                        );
            table_cell->setText(unit.value ? "1" : "0");
        }
    }
}

SatWidget::~SatWidget()
{
    delete ui;
}
