#ifndef ABSTRACTBINARYFUNCTIONSIMPORTER_H
#define ABSTRACTBINARYFUNCTIONSIMPORTER_H

#include <vector>

#include "BinaryFunction/binaryfunction.h"

#define IMPORTER_ERROR_VALUE std::vector < BinaryFunction > {}

class AbstractBinaryFunctionsImporter
{
public:
    AbstractBinaryFunctionsImporter();
    virtual std::vector < BinaryFunction > ImportBinaryFunctions() const = 0;
};

#endif // ABSTRACTBINARYFUNCTIONSIMPORTER_H
