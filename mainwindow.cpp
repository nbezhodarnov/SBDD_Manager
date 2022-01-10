#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->sbddListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(
                this->ui->sbddListWidget,
                SIGNAL(customContextMenuRequested(QPoint)),
                this,
                SLOT(showSBDDContextMenu(QPoint))
                );
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_import_pla_triggered()
{
    this->binary_functions_importer = std::make_shared < PlaFileBinaryFunctionsImporter >();
    importSBDDFromBinaryFunctions();
}


void MainWindow::on_import_formulas_triggered()
{
    this->binary_functions_importer = std::make_shared < FormulasBinaryFunctionsImporter >();
    importSBDDFromBinaryFunctions();
}

void MainWindow::showSBDDContextMenu(const QPoint &mouse_position)
{
    // Handle global position
    QPoint globalPos = this->ui->sbddListWidget->mapToGlobal(mouse_position);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Apply", this, SLOT(applySBDD()));
    myMenu.addAction("Restrict",  this, SLOT(restrictSBDD()));
    myMenu.addAction("SatCount",  this, SLOT(getSatCountFromSBDD()));
    myMenu.addAction("AnySat",  this, SLOT(getAnySatFromSBDD()));
    myMenu.addAction("AllSat",  this, SLOT(getAllSatFromSBDD()));
    //myMenu.addAction("Simplify",  this, SLOT(simplifySBDD()));
    myMenu.addAction("Export",  this, SLOT(exportSBDD()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void MainWindow::applySBDD()
{
    if (this->ui->sbddListWidget->selectedItems().size() == 0 || this->sbdd_vector.size() <= 1)
    {
        return;
    }
    unsigned int choosen_sbdd_index = this->ui->sbddListWidget->row(
                *this->ui->sbddListWidget->selectedItems().begin()
                );
    std::vector < SBDD > sbdd_vector_copy = this->sbdd_vector;
    sbdd_vector_copy.erase(sbdd_vector_copy.begin() + choosen_sbdd_index);
    SBDDSelectWidget sbdd_select_window(sbdd_vector_copy, choosen_sbdd_index, this);
    if (sbdd_select_window.exec() != QMessageBox::Accepted)
    {
        return;
    }
    SBDD operand = sbdd_select_window.GetSelectedSBDD();
    this->sbdd_vector[choosen_sbdd_index].Apply(sbdd_select_window.GetOperation(), operand);
    QMessageBox::information(this, "Apply", "Операция завершена!");
}

void MainWindow::restrictSBDD()
{
    if (this->ui->sbddListWidget->selectedItems().size() == 0)
    {
        return;
    }
    unsigned int choosen_sbdd_index = this->ui->sbddListWidget->row(
                *this->ui->sbddListWidget->selectedItems().begin()
                );
    std::vector < std::string > variables = this->sbdd_vector[choosen_sbdd_index].GetVariablesNames();
    RestrictWidget restrict_window(variables, this);
    if (restrict_window.exec() != QMessageBox::Accepted)
    {
        return;
    }
    std::pair < std::string, bool > variable_and_value = restrict_window.GetVariableAndValue();
    this->sbdd_vector[choosen_sbdd_index].Restrict(
                variable_and_value.first,
                variable_and_value.second
                );
    QMessageBox::information(this, "Restrict", "Операция завершена!");
}

void MainWindow::getSatCountFromSBDD()
{
    if (this->ui->sbddListWidget->selectedItems().size() == 0)
    {
        return;
    }
    unsigned int choosen_sbdd_index = this->ui->sbddListWidget->row(
                *this->ui->sbddListWidget->selectedItems().begin()
                );
    unsigned int sat_count = this->sbdd_vector[choosen_sbdd_index].SatCount();
    QMessageBox::information(
                this,
                "SatCount",
                "SatCount: " + QString::number(sat_count) + "!"
                );
}

void MainWindow::getAnySatFromSBDD()
{
    if (this->ui->sbddListWidget->selectedItems().size() == 0)
    {
        return;
    }
    unsigned int choosen_sbdd_index = this->ui->sbddListWidget->row(
                *this->ui->sbddListWidget->selectedItems().begin()
                );
    SatWidget sat_window(
                {this->sbdd_vector[choosen_sbdd_index].AnySat()},
                this->sbdd_vector[choosen_sbdd_index].GetVariablesNames(),
                this
                );
    sat_window.exec();
}

void MainWindow::getAllSatFromSBDD()
{
    if (this->ui->sbddListWidget->selectedItems().size() == 0)
    {
        return;
    }
    unsigned int choosen_sbdd_index = this->ui->sbddListWidget->row(
                *this->ui->sbddListWidget->selectedItems().begin()
                );
    SatWidget sat_window(
                this->sbdd_vector[choosen_sbdd_index].AllSat(),
                this->sbdd_vector[choosen_sbdd_index].GetVariablesNames(),
                this
                );
    sat_window.exec();
}

void MainWindow::simplifySBDD()
{
    if (this->ui->sbddListWidget->selectedItems().size() == 0)
    {
        return;
    }
    unsigned int choosen_sbdd_index = this->ui->sbddListWidget->row(
                *this->ui->sbddListWidget->selectedItems().begin()
                );
    this->sbdd_vector[choosen_sbdd_index].Simplify();
}

void MainWindow::exportSBDD()
{
    if (this->ui->sbddListWidget->selectedItems().size() == 0)
    {
        return;
    }
    ExportWidget export_window(this);
    if (export_window.exec() != QMessageBox::Accepted)
    {
        return;
    }
    unsigned int choosen_sbdd_index = this->ui->sbddListWidget->row(
                *this->ui->sbddListWidget->selectedItems().begin()
                );
    export_window.GetExporter()->Export(this->sbdd_vector[choosen_sbdd_index]);
    QMessageBox::information(this, "Export", "Операция завершена!");
}

void MainWindow::importSBDDFromBinaryFunctions()
{
    std::vector < BinaryFunction > functions = this->binary_functions_importer->ImportBinaryFunctions();
    if (functions.empty())
    {
        std::cerr << "MainWindow: No binary functions imported!\n";
        return;
    }
    bool functions_invalid = false;
    for (BinaryFunction &function : functions)
    {
        if (function.Empty())
        {
            functions_invalid = true;
        }
    }
    if (functions_invalid)
    {
        std::cerr << "MainWindow: Functions are invalid!\n";
        return;
    }
    SBDD new_sbdd;
    new_sbdd.Build(functions);
    this->sbdd_vector.push_back(new_sbdd);
    this->ui->sbddListWidget->addItem("SBDD " + QString::number(this->ui->sbddListWidget->count() + 1));
}

