#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <vector>

#include <QMainWindow>

#include "SBDD/sbdd.h"
#include "SBDDOperationsWidgets/SatWidget/satwidget.h"
#include "SBDDOperationsWidgets/ExportWidget/exportwidget.h"
#include "SBDDOperationsWidgets/RestrictWidget/restrictwidget.h"
#include "SBDDOperationsWidgets/SBDDSelectWidget/sbddselectwidget.h"
#include "SBDDExporters/DotSBDDExporter/dotsbddexporter.h"
#include "SBDDExporters/TxtSBDDExporter/txtsbddexporter.h"
#include "BinaryFunctionsImporters/PlaFileBinaryFunctionsImporter/plafilebinaryfunctionsimporter.h"
#include "BinaryFunctionsImporters/FormulasBinaryFunctionsImporter/formulasbinaryfunctionsimporter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_import_pla_triggered();

    void on_import_formulas_triggered();

    void showSBDDContextMenu(const QPoint&);

    void applySBDD();

    void restrictSBDD();

    void getSatCountFromSBDD();

    void getAnySatFromSBDD();

    void getAllSatFromSBDD();

    void simplifySBDD();

    void exportSBDD();

private:
    void importSBDDFromBinaryFunctions();

    Ui::MainWindow *ui;
    std::shared_ptr < AbstractBinaryFunctionsImporter > binary_functions_importer;
    std::vector < SBDD > sbdd_vector;
};
#endif // MAINWINDOW_H
