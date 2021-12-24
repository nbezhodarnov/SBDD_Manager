#include "plafilebinaryfunctionsimporter.h"

#include <iostream>

PlaFileBinaryFunctionsImporter::PlaFileBinaryFunctionsImporter()
{

}

std::vector < BinaryFunction > PlaFileBinaryFunctionsImporter::ImportBinaryFunctions() const
{
    QString file_name = QFileDialog::getOpenFileName(
                nullptr,
                "Открыть PLA Файл",
                QDir::homePath(),
                "Файл PLA (*.pla)"
                );
    QFile file(file_name);
    if (!file.isOpen() && !file.open(QFile::ReadOnly))
    {
        std::cerr << "PlaFileBinaryFunctionsImporter: Error while accessing the file!\n";
        return IMPORTER_ERROR_VALUE;
    }

    unsigned int input_variables_count = 0;
    unsigned int output_variables_count = 0;

    std::vector < std::string > variables;

    std::vector < BinaryFunction > functions;

    while (!file.atEnd())
    {
        QString file_data = file.readLine();
        QStringList splited_data = file_data.split(" ");

        if (splited_data[0] == ".i")
        {
            if (input_variables_count == 0)
            {
                input_variables_count = splited_data[1].toUInt();
                if (input_variables_count == 0)
                {
                    std::cerr << "PlaFileBinaryFunctionsImporter: No input variables!\n";
                }
                else
                {
                    for (unsigned int j = 0; j < input_variables_count; j++)
                    {
                        std::string variable_name = "x";
                        variable_name += std::to_string(j + 1);
                        variables.push_back(variable_name);
                    }
                    for (BinaryFunction &function : functions)
                    {
                        function.SetVariables(variables);
                    }
                }
            }
        }
        else if (splited_data[0] == ".o")
        {
            if (output_variables_count == 0)
            {
                output_variables_count = splited_data[1].toUInt();
                if (output_variables_count == 0)
                {
                    std::cerr << "PlaFileBinaryFunctionsImporter: No output variables!\n";
                }
                else
                {
                    functions.resize(output_variables_count);
                    for (BinaryFunction &function : functions)
                    {
                        function.SetVariables(variables);
                    }
                }
            }
        }
        else if (splited_data[0] == ".p")
        {
            unsigned int lines_count = splited_data[1].toUInt();
            unsigned int i = 0;
            for (; i < lines_count && !file.atEnd(); i++)
            {
                file_data = file.readLine();
                splited_data = file_data.split(" ");

                if ((unsigned int)splited_data[0].size() != input_variables_count)
                {
                    std::cout << "PlaFileBinaryFunctionsImporter: Info about input variables and actual state are different!\n";
                    variables.clear();
                }
                Interval interval;
                if (variables.empty())
                {
                    for (unsigned int j = 0; j < (unsigned int)splited_data[0].size(); j++)
                    {
                        std::string variable_name = "x";
                        variable_name += std::to_string(j + 1);
                        variables.push_back(variable_name);
                    }
                    for (BinaryFunction &function : functions)
                    {
                        function.SetVariables(variables);
                    }
                }
                for (unsigned int j = 0; j < (unsigned int)splited_data[0].size(); j++)
                {
                    interval.AppendUnit({variables[j], splited_data[0] == '0' ? False : True});
                }
                if ((unsigned int)splited_data[1].size() != output_variables_count)
                {
                    std::cout << "PlaFileBinaryFunctionsImporter: Info about output variables and actual state are different!\n";
                    functions.resize(splited_data[1].size());
                    for (BinaryFunction &function : functions)
                    {
                        function.SetVariables(variables);
                    }
                }
                for (unsigned int j = 0; j < (unsigned int)splited_data[1].size(); j++)
                {
                    if (splited_data[1][j] == '1')
                    {
                        functions[j].AddInterval(interval);
                    }
                }
            }
            if (i < lines_count)
            {
                std::cerr << "PlaFileBinaryFunctionsImporter: Info about intervals and actual state are different! Data may be corrupted!\n";
            }
        }
    }
    file.close();
    return functions;
}
