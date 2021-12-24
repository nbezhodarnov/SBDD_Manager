#ifndef ABSTRACTBINARYOPERATION_H
#define ABSTRACTBINARYOPERATION_H

#define UNUSED(expr) do { (void)(expr); } while (0)

class AbstractBinaryOperation
{
public:
    AbstractBinaryOperation() = default;
    virtual bool GetValue(const bool &first_variable, const bool &second_variable) const = 0;
    virtual ~AbstractBinaryOperation() = default;
};

#endif // ABSTRACTBINARYOPERATION_H
