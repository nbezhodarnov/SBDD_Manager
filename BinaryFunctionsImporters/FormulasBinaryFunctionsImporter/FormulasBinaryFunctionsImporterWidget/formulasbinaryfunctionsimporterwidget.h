#ifndef FORMULASBINARYFUNCTIONSIMPORTERWIDGET_H
#define FORMULASBINARYFUNCTIONSIMPORTERWIDGET_H

#include <string>
#include <vector>

#include <QDialog>

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
    void formulasList_currentRowChanged(const int &currentRow);

private:
    Ui::FormulasBinaryFunctionsImporterWidget *ui;
};

#endif // FORMULASBINARYFUNCTIONSIMPORTERWIDGET_H
