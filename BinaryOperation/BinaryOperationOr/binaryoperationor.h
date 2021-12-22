#ifndef BINARYOPERATIONOR_H
#define BINARYOPERATIONOR_H

#include "BinaryOperation/abstractbinaryoperation.h"

class BinaryOperationOr : public AbstractBinaryOperation
{
public:
    BinaryOperationOr();
    bool GetValue(const bool &first_variable, const bool &second_variable) const;
};

#endif // BINARYOPERATIONOR_H
