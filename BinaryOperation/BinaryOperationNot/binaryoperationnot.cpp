#include "binaryoperationnot.h"

BinaryOperationNot::BinaryOperationNot()
{

}

bool BinaryOperationNot::GetValue(const bool &first_variable, const bool &second_variable) const
{
    UNUSED(second_variable);
    return !first_variable;
}
