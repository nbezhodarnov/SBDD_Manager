#ifndef FORMULASBINARYFUNCTIONSIMPORTER_H
#define FORMULASBINARYFUNCTIONSIMPORTER_H

#include "BinaryFunctionsImporters/abstractbinaryfunctionsimporter.h"
#include "BinaryFunctionsImporters/FormulasBinaryFunctionsImporter/FormulasBinaryFunctionsImporterWidget/formulasbinaryfunctionsimporterwidget.h"

class BinaryOperand
{
public:
    BinaryOperand() = default;
    void SetValue(const bool &new_value);
    void UnSet();
    bool IsSet() const;
    bool GetValue() const;

private:
    bool is_set = false;
    bool value;
};

class FormulasBinaryFunctionsImporter : public AbstractBinaryFunctionsImporter
{
public:
    FormulasBinaryFunctionsImporter();
    std::vector < BinaryFunction > ImportBinaryFunctions() const override;

private:
    std::vector < std::string > infixToPostfix(const std::string &formula_infix) const;
    BinaryFunction postfixToBinaryFunction(const std::vector < std::string > &formula_postfix) const;
};

#endif // FORMULASBINARYFUNCTIONSIMPORTER_H
