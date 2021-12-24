#ifndef EXPORTWIDGET_H
#define EXPORTWIDGET_H

#include <memory>

#include <QDialog>

#include "SBDDExporters/DotSBDDExporter/dotsbddexporter.h"
#include "SBDDExporters/TxtSBDDExporter/txtsbddexporter.h"

namespace Ui {
class ExportWidget;
}

class ExportWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ExportWidget(QWidget *parent = nullptr);
    std::shared_ptr < AbstractSBDDExporter > GetExporter() const;
    ~ExportWidget();

private:
    Ui::ExportWidget *ui;
};

#endif // EXPORTWIDGET_H
