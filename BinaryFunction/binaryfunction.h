#ifndef BINARYFUNCTION_H
#define BINARYFUNCTION_H

#include <string>
#include <vector>

enum BinaryFunctionValue : unsigned char
{
    False = '0',
    True = '1',
    AnyValue = '-'
};

struct IntervalUnit
{
    std::string variable_name;
    BinaryFunctionValue value;

    bool operator==(const IntervalUnit &other) const;
};

class Interval
{
public:
    Interval() = default;
    void AppendUnit(const IntervalUnit &new_unit);
    void RemoveUnitAt(const unsigned int &index);
    void RemoveUnitWithVariable(const std::string &variable);
    std::vector < IntervalUnit > GetUnits() const;
    IntervalUnit GetUnitWithVariable(const std::string &variable) const;
    bool operator==(const Interval &other) const;

private:
    std::vector < IntervalUnit > units;
};


class BinaryFunction
{
public:
    BinaryFunction();
    void AddInterval(const Interval &new_interval);
    void SetVariables(const std::vector < std::string > &new_variables);
    bool Empty() const;
    BinaryFunctionValue GetValue() const;
    std::vector < std::string > GetVariables() const;
    BinaryFunction FixVariable(const std::string &variable, const BinaryFunctionValue &value) const;
    bool operator==(const BinaryFunction &other) const;

private:
    std::vector < Interval > intervals;
    std::vector < std::string > variables;
};

#endif // BINARYFUNCTION_H
