#include "formulasbinaryfunctionsimporter.h"

#include <map>
#include <stack>
#include <iostream>
#include <algorithm>
#include <functional>

#include <QMessageBox>

#define BINARY_FUNCTION_ERROR_VALUE BinaryFunction();

#define UNUSED(expr) do { (void)(expr); } while (0)

void BinaryOperand::SetValue(const bool &new_value)
{
    this->is_set = true;
    this->value = new_value;
}

void BinaryOperand::UnSet()
{
    this->is_set = false;
}

bool BinaryOperand::IsSet() const
{
    return this->is_set;
}

bool BinaryOperand::GetValue() const
{
    if (!this->is_set)
    {
        std::cerr << "BinaryOperand: Getting undefined value!\n";
    }
    return this->value;
}


FormulasBinaryFunctionsImporter::FormulasBinaryFunctionsImporter()
{

}

std::vector < BinaryFunction > FormulasBinaryFunctionsImporter::ImportBinaryFunctions() const
{
    std::vector < BinaryFunction > functions;

    FormulasBinaryFunctionsImporterWidget dialog_window;

    if (dialog_window.exec() != QMessageBox::Ok) {
        return IMPORTER_ERROR_VALUE;
    }

    std::vector < std::string > formulas = dialog_window.GetFormulas();

    for (std::string &formula : formulas)
    {
        BinaryFunction function = this->postfixToBinaryFunction(this->infixToPostfix(formula));
        if (!function.GetVariables().empty())
        {
            functions.push_back(function);
        }
    }

    if (functions.empty())
    {
        QMessageBox::warning(
                    &dialog_window,
                    "Мэнеджер SBDD",
                    "Не удалось импортировать формулы!",
                    QMessageBox::Ok,
                    QMessageBox::Ok
                    );
    }

    return functions;
}

std::vector < std::string > FormulasBinaryFunctionsImporter::infixToPostfix(const std::string &formula_infix) const {
    const unsigned int open_bracket_index = 0;
    std::map < char, unsigned int > operators_priorities = {{'!', 1}, {'&', 2}, {'|', 3}};

    std::vector < std::string > formula_postfix_sliced;
    std::stack < unsigned int > formula_stack;

    unsigned int infix_index = 0;

    bool started_writing_variable_name = false;

    while (infix_index < formula_infix.size()) {
        char symbol = formula_infix[infix_index];
        unsigned int operator_priority = operators_priorities[symbol];

        if (operators_priorities.find(symbol) != operators_priorities.end())
        {
            started_writing_variable_name = false;
            while (!formula_stack.empty())
            {
                int prec2 = formula_stack.top() ;
                int prec1 = operator_priority;
                if (prec2 > prec1 || (prec2 == prec1 && symbol != '^')) {
                    formula_postfix_sliced.push_back(
                                std::string(1, std::find_if(
                                    operators_priorities.begin(),
                                    operators_priorities.end(),
                                    [&](const std::pair < char, unsigned int > &pair)
                                    {return pair.second == formula_stack.top();}
                                )->first
                                ));
                    formula_stack.pop();
                } else break;
            }
            formula_stack.push(operator_priority);
        }
        else if (symbol == '(')
        {
            started_writing_variable_name = false;
            formula_stack.push(open_bracket_index);
        }
        else if (symbol == ')')
        {
            started_writing_variable_name = false;
            while (formula_stack.top() != open_bracket_index)
            {
                formula_postfix_sliced.push_back(
                            std::string(1, operators_priorities[formula_stack.top()])
                        );
                formula_stack.pop();
            }
            formula_stack.pop();
        }
        else
        {
            if (started_writing_variable_name)
            {
                formula_postfix_sliced.push_back(std::string(1, symbol));
                started_writing_variable_name = true;
            }
            else
            {
                formula_postfix_sliced.back().append(std::string(1, symbol));
            }

        }
        infix_index++;
    }

    while (!formula_stack.empty()) {
        formula_postfix_sliced.push_back(
                    std::string(1, std::find_if(
                        operators_priorities.begin(),
                        operators_priorities.end(),
                        [&](const std::pair < char, unsigned int > &pair)
                        {return pair.second == formula_stack.top();}
                    )->first
                    ));
        formula_stack.pop();
    }

    return formula_postfix_sliced;
}

BinaryFunction FormulasBinaryFunctionsImporter::postfixToBinaryFunction(
        const std::vector < std::string > &formula_postfix
        ) const
{
    const std::vector < std::string > operators_and_constants = {"0", "1", "!", "&", "|"};
    std::map < std::string, bool > constatns = {{"0", false}, {"1", true}};
    std::map < std::string, std::function < bool(const BinaryOperand&, const BinaryOperand&) > >
            operators = {
                {"!", [&](const BinaryOperand &first, const BinaryOperand &second)
                      {UNUSED(second); return !first.GetValue();}},
                {"&", [&](const BinaryOperand &first, const BinaryOperand &second)
                      {return first.GetValue() && second.GetValue();}},
                {"|", [&](const BinaryOperand &first, const BinaryOperand &second)
                      {return first.GetValue() || second.GetValue();}}
            };

    std::vector < std::string > variables;
    for (const std::string &formula_element : formula_postfix)
    {
        if (std::find(
                    operators_and_constants.begin(),
                    operators_and_constants.end(),
                    formula_element
                    ) == operators_and_constants.end())
        {
            variables.push_back(formula_element);
        }
    }

    if (variables.size() > sizeof(unsigned long long int) * 8 - 1)
    {
        std::cerr << "FormulasBinaryFunctionsImporter: fomula contains too huge count of variables!\n";
        return BINARY_FUNCTION_ERROR_VALUE;
    }
    std::sort(variables.begin(), variables.end());

    BinaryFunction function;

    unsigned long long int binary_vector = 0;
    unsigned long long int binary_vector_end = 1 << variables.size();

    std::stack < BinaryOperand > operands;

    while (binary_vector < binary_vector_end)
    {
        Interval interval;

        unsigned long long int bool_value_reader = 1;
        for (const std::string &variable : variables)
        {
            interval.AppendUnit({variable, binary_vector & bool_value_reader ? True : False});
            bool_value_reader <<= 1;
        }

        for (const std::string &formula_element : formula_postfix)
        {
            if (std::find(
                        operators_and_constants.begin(),
                        operators_and_constants.end(),
                        formula_element
                        ) != operators_and_constants.end())
            {
                if (constatns.find(formula_element) != constatns.end())
                {
                    BinaryOperand operand;
                    operand.SetValue(constatns[formula_element]);
                    operands.push(operand);
                }
                else
                {
                    BinaryOperand first_operand;
                    BinaryOperand second_operand;
                    BinaryOperand result_operand;
                    if (operands.empty())
                    {
                        std::cerr << "FormulasBinaryFunctionsImporter: Incorrect postfix formula!\n";
                        return BINARY_FUNCTION_ERROR_VALUE;
                    }
                    first_operand = operands.top();
                    operands.pop();
                    if (formula_element == "!")
                    {
                        result_operand.SetValue(
                                    operators[formula_element](first_operand, second_operand)
                                );
                    }
                    else
                    {
                        if (operands.empty())
                        {
                            std::cerr << "FormulasBinaryFunctionsImporter: Incorrect postfix formula!\n";
                            return BINARY_FUNCTION_ERROR_VALUE;
                        }
                        second_operand = operands.top();
                        operands.pop();
                        result_operand.SetValue(
                                    operators[formula_element](first_operand, second_operand)
                                );
                    }
                    operands.push(result_operand);
                }
            }
            else
            {
                BinaryOperand operand;
                operand.SetValue(
                            interval.GetUnitWithVariable(formula_element).value == False ? false : true
                            );
                operands.push(operand);
            }
        }

        if (operands.size() != 1)
        {
            std::cerr << "FormulasBinaryFunctionsImporter: Incorrect postfix formula!\n";
            return BINARY_FUNCTION_ERROR_VALUE;
        }
        BinaryOperand function_result = operands.top();
        operands.pop();

        if (function_result.GetValue())
        {
            function.AddInterval(interval);
        }
    }
    return function;
}
