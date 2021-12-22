#include "binaryfunction.h"

#include <algorithm>
#include <iostream>

#define ERROR_VALUE BinaryFunction()

void Interval::AppendUnit(const IntervalUnit &new_unit)
{
    for (unsigned int i = 0; i < this->units.size(); i++)
    {
        if (new_unit.variable_name > this->units[i].variable_name)
        {
            continue;
        }
        if (new_unit.variable_name == this->units[i].variable_name)
        {
            this->units[i].value = new_unit.value;
            return;
        }
        this->units.insert(this->units.begin() + i, new_unit);
        return;
    }
    this->units.push_back(new_unit);
}

void Interval::RemoveUnitAt(const unsigned int &index)
{
    if (index >= this->units.size())
    {
        std::cerr << "Interval: Index is out of array!\n";
    }
}

void Interval::RemoveUnitWithVariable(const std::string &variable)
{
    this->units.erase(std::find(this->units.begin(), this->units.end(), variable));
}

std::vector<IntervalUnit> Interval::GetUnits() const
{
    return this->units;
}

BinaryFunction::BinaryFunction()
{

}

void BinaryFunction::AddInterval(const Interval &new_interval)
{
    for (IntervalUnit& unit : new_interval.GetUnits()) {
        if (std::find(this->variables.begin(), this->variables.end(), unit.variable_name) == this->variables.end())
        {
            std::cerr << "BinaryFunction: This variable doesn't used in this function!\n";
            return;
        }
    }
    Interval new_interval_copy = new_interval;
    for (std::string &variable : this->variables)
    {
        bool variable_missing = true;
        for (IntervalUnit& unit : new_interval.GetUnits()) {
            if (unit.variable_name == variable)
            {
                variable_missing = false;
                break;
            }
        }
        if (variable_missing)
        {
            new_interval_copy.AppendUnit({variable, AnyValue});
        }
    }
    this->intervals.push_back(new_interval_copy);
}

void BinaryFunction::SetVariables(const std::vector<std::string> &new_variables)
{
    std::vector < std::string > new_variables_copy = new_variables;
    std::sort(new_variables_copy.begin(), new_variables_copy.end());
    for (Interval &interval : this->intervals)
    {
        for (std::string variable : new_variables)
        {
            bool variable_missing = true;
            for (IntervalUnit &unit : interval.GetUnits())
            {
                if (unit.variable_name == variable)
                {
                    variable_missing = false;
                    break;
                }
            }
            if (variable_missing)
            {
                interval.AppendUnit(IntervalUnit({variable, AnyValue}));
            }
        }
        for (unsigned int i = 0; i < interval.GetUnits().size(); i++)
        {
            if (std::find(new_variables.begin(), new_variables.end(), interval.GetUnits()[i].variable_name) == new_variables.end())
            {
                interval.RemoveUnitAt(i);
                i--;
            }
        }
    }
    this->variables = new_variables;
}

BinaryFunctionValue BinaryFunction::GetValue() const
{
    if (this->variables.empty())
    {
        if (this->intervals.empty())
        {
            return False;
        }
        return True;
    }
    return AnyValue;
}

std::vector < std::string > BinaryFunction::GetVariables() const
{
    return this->variables;
}

BinaryFunction BinaryFunction::FixVariable(const std::string &variable, const BinaryFunctionValue &value) const
{
    if (std::find(this->variables.begin(), this->variables.end(), variable) == this->variables.end())
    {
        std::cerr << "BinaryFunction: This variable doesn't used in this function!\n";
        return ERROR_VALUE;
    }
    if (value == AnyValue) {
        std::cerr << "BinaryFunction: There is no need to fix the variable at value \"AnyValue\"!\n";
        return ERROR_VALUE;
    }

    std::vector < std::string > variables_result = this->variables;
    variables_result.erase(std::find(this->variables.begin(), this->variables.end(), variable));
    BinaryFunction result;
    result.SetVariables(variables_result);
    for (const Interval &interval : intervals)
    {
        IntervalUnit variable_interval_unit;
        for (IntervalUnit& unit : interval.GetUnits())
        {
            if (unit.variable_name == variable)
            {
                variable_interval_unit = unit;
                break;
            }
        }
        if (variable_interval_unit.value == AnyValue || variable_interval_unit.value == value)
        {
            Interval new_interval = interval;
            new_interval.RemoveUnitWithVariable(variable_interval_unit.variable_name);
            result.AddInterval(new_interval);
        }
    }
    return result;
}
