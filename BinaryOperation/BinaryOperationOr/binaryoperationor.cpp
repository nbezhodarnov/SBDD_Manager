#include "binaryoperationor.h"

BinaryOperationOr::BinaryOperationOr()
{

}

bool BinaryOperationOr::GetValue(const bool &first_variable, const bool &second_variable) const
{
    return first_variable || second_variable;
}
