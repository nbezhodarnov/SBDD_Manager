#ifndef FORMULASBINARYFUNCTIONSIMPORTERWIDGET_H
#define FORMULASBINARYFUNCTIONSIMPORTERWIDGET_H

#include <string>
#include <vector>

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class FormulasBinaryFunctionsImporterWidget;
}

class FormulasBinaryFunctionsImporterWidget : public QDialog
{
    Q_OBJECT

public:
    explicit FormulasBinaryFunctionsImporterWidget(QWidget *parent = nullptr);
    std::vector < std::string > GetFormulas() const;
    ~FormulasBinaryFunctionsImporterWidget();

private slots:
    void formulasList_formulasList_itemChanged(QListWidgetItem *item);

private:
    Ui::FormulasBinaryFunctionsImporterWidget *ui;
    bool deleting_allowed = false;
};

#endif // FORMULASBINARYFUNCTIONSIMPORTERWIDGET_H
