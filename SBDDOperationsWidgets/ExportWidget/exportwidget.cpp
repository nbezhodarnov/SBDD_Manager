#include "exportwidget.h"
#include "ui_exportwidget.h"

ExportWidget::ExportWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportWidget)
{
    ui->setupUi(this);
}

std::shared_ptr<AbstractSBDDExporter> ExportWidget::GetExporter() const
{
    std::shared_ptr < AbstractSBDDExporter > exporter;
    switch (this->ui->exportButtonGroup->checkedId())
    {
    case -2:
        exporter = std::make_shared < TxtSBDDExporter >();
        break;
    case -3:
        exporter = std::make_shared < DotSBDDExporter >();
        break;
    default:
        exporter = std::make_shared < TxtSBDDExporter >();
        break;
    }
    return exporter;
}

ExportWidget::~ExportWidget()
{
    delete ui;
}
