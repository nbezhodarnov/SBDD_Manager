#ifndef BINARYOPERATIONNOT_H
#define BINARYOPERATIONNOT_H

#include "BinaryOperation/abstractbinaryoperation.h"

class BinaryOperationNot : public AbstractBinaryOperation
{
public:
    BinaryOperationNot();
    bool GetValue(const bool &first_variable, const bool &second_variable) const override;
};

#endif // BINARYOPERATIONNOT_H
