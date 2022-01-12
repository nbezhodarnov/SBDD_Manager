#ifndef PLAFILEBINARYFUNCTIONSIMPORTER_H
#define PLAFILEBINARYFUNCTIONSIMPORTER_H

#include <QFileDialog>

#include "BinaryFunctionsImporters/abstractbinaryfunctionsimporter.h"

class PlaFileBinaryFunctionsImporter : public AbstractBinaryFunctionsImporter
{
public:
    PlaFileBinaryFunctionsImporter();
    std::vector < BinaryFunction > ImportBinaryFunctions() const override;
};

#endif // PLAFILEBINARYFUNCTIONSIMPORTER_H
