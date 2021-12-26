#include "txtsbddexporter.h"

#include <iostream>

TxtSBDDExporter::TxtSBDDExporter()
{

}

bool TxtSBDDExporter::Export(const SBDD &sbdd) const
{
    QString file_name = QFileDialog::getSaveFileName(
                nullptr,
                "Сохранить SBDD",
                QDir::homePath(),
                "SBDD файл (*.sbdd)"
                );
    SBDDStructure sbdd_structure = sbdd.GetStructure();

    QFile file(file_name);
    if (!file.isOpen() && !file.open(QFileDevice::WriteOnly))
    {
        std::cerr << "TxtSBDDExporter: Error while creating file!\n";
        return false;
    }

    for (SBDDStructureUnit &unit : sbdd_structure.units)
    {
        QString line;
        line.append(QString::number(unit.index));
        line.append(" ");
        line.append(QString::fromStdString(unit.variable_name));
        line.append(" ");
        if (!unit.function_indexes.empty())
        {
            line.append(QString::number(unit.function_indexes[0]));
            for (unsigned int i = 1; i < unit.function_indexes.size(); i++)
            {
                line.append("," + QString::number(unit.function_indexes[i]));
            }
        }
        if (unit.children[0] != UINT_MAX && unit.children[1] != UINT_MAX)
        {
            line.append(" ");
            if (unit.children[0] != UINT_MAX)
            {
                line.append(QString::number(unit.children[0]));
            }
            if (unit.children[1] != UINT_MAX)
            {
                line.append("," + QString::number(unit.children[1]));
            }
        }
        line.append("\n");
        file.write(line.toUtf8());
    }
    file.close();
    return true;
}
