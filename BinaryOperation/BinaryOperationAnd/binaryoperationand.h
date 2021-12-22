#ifndef BINARYOPERATIONAND_H
#define BINARYOPERATIONAND_H

#include "BinaryOperation/abstractbinaryoperation.h"

class BinaryOperationAnd : public AbstractBinaryOperation
{
public:
    BinaryOperationAnd();
    bool GetValue(const bool &first_variable, const bool &second_variable) const;
};

#endif // BINARYOPERATIONAND_H
