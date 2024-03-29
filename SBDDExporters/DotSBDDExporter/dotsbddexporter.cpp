#include "dotsbddexporter.h"

#include <set>
#include <iostream>

DotSBDDExporter::DotSBDDExporter()
{

}

bool DotSBDDExporter::Export(const SBDD &sbdd) const
{
    QString file_name = QFileDialog::getSaveFileName(
                nullptr,
                "Сохранить SBDD",
                QDir::homePath(),
                "DOT файл (*.dot)"
                );
    SBDDStructure sbdd_structure = sbdd.GetStructure();
    QFile file(file_name);
    if (!file.isOpen() && !file.open(QFileDevice::WriteOnly))
    {
        std::cerr << "TxtSBDDExporter: Error while creating file!\n";
        return false;
    }

    file.write("digraph sbdd {\n");

    std::set <unsigned int> children_indexes;
    for (SBDDStructureUnit &unit : sbdd_structure.units)
    {
        for (unsigned int &index : unit.children)
        {
            children_indexes.insert(index);
        }
    }
    for (SBDDStructureUnit &unit : sbdd_structure.units)
    {
        if (std::find(
                    children_indexes.begin(),
                    children_indexes.end(),
                    unit.index
                    ) == children_indexes.end())
        {
            for (unsigned int &function_index : unit.function_indexes)
            {
                QString line = "\tf";
                line.append(QString::number(function_index + 1));
                line.append(" [shape=plaintext]\n");
                file.write(line.toUtf8());
                line = "\tf";
                line.append(QString::number(function_index + 1));
                line.append(" -> ");
                if (unit.variable_name != "0" && unit.variable_name != "1")
                {
                    line.append(
                                QString::fromStdString(unit.variable_name) +
                                "_" +
                                QString::number(unit.index)
                                );
                }
                else
                {
                    line.append(QString::fromStdString(unit.variable_name));
                }
                line.append(";\n");
                file.write(line.toUtf8());
            }
        }
    }

    for (SBDDStructureUnit &unit : sbdd_structure.units)
    {
        QString line;
        if (unit.children[0] != UINT_MAX && unit.children[1] != UINT_MAX)
        {
            if (unit.children[0] != UINT_MAX)
            {
                line.append("\t");
                line.append(
                            QString::fromStdString(unit.variable_name) +
                            "_" +
                            QString::number(unit.index)
                            );
                line.append(" -> ");
                SBDDStructureUnit low_child_unit = *std::find_if(
                            sbdd_structure.units.begin(),
                            sbdd_structure.units.end(),
                            [&](const SBDDStructureUnit &sbdd_unit)
                            {return sbdd_unit.index == unit.children[0] ? true : false;}
                            );
                if (low_child_unit.variable_name != "0" && low_child_unit.variable_name != "1")
                {
                    line.append(
                                QString::fromStdString(low_child_unit.variable_name) +
                                "_" +
                                QString::number(low_child_unit.index)
                                );
                }
                else
                {
                    line.append(QString::fromStdString(low_child_unit.variable_name));
                }
                line.append(" [style=\"dashed\"];\n");
            }
            if (unit.children[1] != UINT_MAX)
            {
                line.append("\t");
                line.append(
                            QString::fromStdString(unit.variable_name) +
                            "_" +
                            QString::number(unit.index)
                            );
                line.append(" -> ");
                SBDDStructureUnit high_child_unit = *std::find_if(
                            sbdd_structure.units.begin(),
                            sbdd_structure.units.end(),
                            [&](const SBDDStructureUnit &sbdd_unit)
                            {return sbdd_unit.index == unit.children[1] ? true : false;}
                            );
                if (high_child_unit.variable_name != "0" && high_child_unit.variable_name != "1")
                {
                    line.append(
                                QString::fromStdString(high_child_unit.variable_name) +
                                "_" +
                                QString::number(high_child_unit.index)
                                );
                }
                else
                {
                    line.append(QString::fromStdString(high_child_unit.variable_name));
                }
                line.append(";\n");
            }
        }
        file.write(line.toUtf8());
    }
    file.write("\t1 [shape=box]\n");
    file.write("\t0 [shape=box]\n");
    file.write("}\n");
    file.close();
    return true;
}
