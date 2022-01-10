#include "sbdd.h"

#include <iostream>
#include <algorithm>

#define UINT_MAX 4294967295

bool compare_nodes(const BinaryNode& first_node, const BinaryNode& secont_node)
{
    if (
            first_node.level == secont_node.level &&
            first_node.function_indexes == secont_node.function_indexes &&
            first_node.GetChildren() == secont_node.GetChildren() &&
            first_node.GetVariableName() == secont_node.GetVariableName()
            )
    {
        return true;
    }
    return false;
}

bool SBDDStructureUnit::operator==(const SBDDStructureUnit &other) const
{
    return this->variable_name == other.variable_name &&
           this->index == other.index &&
           this->level == other.level &&
           this->children == other.children &&
           this->function_indexes == other.function_indexes;
}

bool SatUnit::operator==(const SatUnit &other) const
{
    return this->variable_name == other.variable_name && this->value == other.value;
}

bool Sat::operator==(const Sat &other) const
{
    return this->units == other.units;
}

SBDD::SBDD()
{
    this->nodes.push_back(std::shared_ptr < BinaryNode > (new BinaryNode(
                                                              "0",
                                                              {nullptr, nullptr},
                                                              0,
                                                              0,
                                                              {}
                                                          )));
    this->nodes.push_back(std::shared_ptr < BinaryNode > (new BinaryNode(
                                                              "1",
                                                              {nullptr, nullptr},
                                                              1,
                                                              0,
                                                              {}
                                                              )));
}

SBDD::SBDD(const SBDD &other)
{
    this->nodes.push_back(std::shared_ptr < BinaryNode > (new BinaryNode(
                                                              "0",
                                                              {nullptr, nullptr},
                                                              0,
                                                              0,
                                                              {}
                                                          )));
    this->nodes.push_back(std::shared_ptr < BinaryNode > (new BinaryNode(
                                                              "1",
                                                              {nullptr, nullptr},
                                                              1,
                                                              0,
                                                              {}
                                                              )));

    this->functions_root_nodes.reserve(other.functions_root_nodes.size());
    this->functions_nodes.resize(other.functions_nodes.size());

    for(const auto &root : other.functions_root_nodes)
    {
        this->functions_root_nodes.push_back(deepCopy(root));
    }
}

unsigned int SBDD::GetFunctionsCount() const
{
    return this->functions_root_nodes.size();
}

SBDDStructure SBDD::GetStructure() const
{
    SBDDStructure structure;
    for (const std::shared_ptr < BinaryNode > &node : this->functions_root_nodes)
    {
        SBDDStructure function_sbdd_structure = this->getStructureRecirsively(node);
        for (SBDDStructureUnit &unit : function_sbdd_structure.units)
        {
            if (std::find(
                        structure.units.begin(),
                        structure.units.end(),
                        unit
                        ) == structure.units.end()
               )
            {
                structure.units.push_back(unit);
            }
        }
    }
    structure.units.push_back({
                                  0,
                                  0,
                                  {},
                                  "0",
                                  {
                                      (unsigned int)UINT_MAX,
                                      (unsigned int)UINT_MAX
                                  }
                              });
    structure.units.push_back({
                                  1,
                                  0,
                                  {},
                                  "1",
                                  {
                                      (unsigned int)UINT_MAX,
                                      (unsigned int)UINT_MAX
                                  }
                              });
    return structure;
}

std::vector < std::string > SBDD::GetVariablesNames() const
{
    std::vector < std::string > variables;
    for (const std::shared_ptr < BinaryNode > &root_node : this->functions_root_nodes)
    {
        std::vector < std::string > variables_from_function = this->getVariablesNamesRecuresively(
                    root_node
                    );
        for (std::string &variable : variables_from_function)
        {
            if (std::find(variables.begin(), variables.end(), variable) == variables.end())
            {
                variables.push_back(variable);
            }
        }
    }
    std::sort(variables.begin(), variables.end());
    return variables;
}

void SBDD::Build(const std::vector < BinaryFunction > &functions)
{
    std::vector < std::pair < BinaryFunction, std::shared_ptr < BinaryNode > > > cash;

    this->functions_nodes.resize(functions.size());
    for (unsigned int i = 0; i < functions.size(); i++)
    {
        this->functions_root_nodes.push_back(this->buildRecursively(cash, functions[i], i, 1));
    }
}

void SBDD::Apply(const std::shared_ptr < AbstractBinaryOperation > &operation, const SBDD &other_sbdd)
{
    if (this->GetFunctionsCount() != other_sbdd.GetFunctionsCount())
    {
        std::cerr << "SBDD: Trying to Apply SBDD with different counts of functions!\n";
        return;
    }
    std::map <
        std::pair < unsigned int, unsigned int >,
        std::shared_ptr < BinaryNode >
    > operation_results;

    for (unsigned int i = 0; i < this->functions_root_nodes.size(); i++)
    {
        this->functions_root_nodes[i] = this->applyRecursively(
                    operation,
                    this->functions_root_nodes[i],
                    other_sbdd.functions_root_nodes[i],
                    operation_results
                    );
    }
}

void SBDD::Restrict(const std::string &variable, const bool &switch_node)
{
    if (variable == "0" || variable == "1")
    {
        std::cerr << "SBDD: Trying restrict terminal nodes!\n";
        return;
    }
    bool variable_exists = false;
    unsigned int variable_level = 0;
    for (std::shared_ptr < BinaryNode > &node : this->nodes)
    {
        if (node->GetVariableName() == variable)
        {
            variable_exists = true;
            variable_level = node->level;
            break;
        }
    }
    if (!variable_exists)
    {
        std::cerr << "SBDD: The variable isn't used!\n";
        return;
    }
    for (std::shared_ptr < BinaryNode > &root_node : this->functions_root_nodes)
    {
        root_node = this->restrictRecursively(root_node, variable_level, switch_node);
    }
}

unsigned int SBDD::SatCount() const
{
    unsigned int sat_count = 0;
    unsigned int variables_count = this->GetVariablesNames().size();
    std::vector < Sat > sats = this->AllSat();
    for (const Sat &sat : sats)
    {
        sat_count += 1 << (variables_count - sat.units.size());
    }
    /*for (const std::shared_ptr < BinaryNode > &root_node : this->functions_root_nodes)
    {
        sat_count += this->satCountRecursively(root_node);
    }*/
    return sat_count;
}

Sat SBDD::AnySat() const
{
    std::vector < Sat > any_sats = {};
    for (const std::shared_ptr < BinaryNode > &root_node : this->functions_root_nodes)
    {
        any_sats.push_back(this->anySatRecursively(root_node));
    }
    if (any_sats.size() > 0)
    {
        return any_sats[0];
    }
    return Sat();
}

std::vector < Sat > SBDD::AllSat() const
{
    std::vector < Sat > all_sats = {};
    for (const std::shared_ptr < BinaryNode > &root_node : this->functions_root_nodes)
    {
        std::vector < Sat > all_sats_for_function = this->allSatRecursively(root_node);
        all_sats.insert(all_sats.end(), all_sats_for_function.begin(), all_sats_for_function.end());
    }

    for(int64_t i = 0;  i < int64_t(all_sats.size()) - 1; ++i)
    {
        for(int64_t j = i + 1; j < int64_t(all_sats.size());)
        {
            if(all_sats[i] == all_sats[j])
            {
                all_sats.erase(all_sats.begin() + j);
            }
            else
            {
                ++j;
            }
        }
    }

    return all_sats;
}

void SBDD::Simplify()
{
    for (std::shared_ptr < BinaryNode > &root_node : this->functions_root_nodes)
    {
        root_node = this->simplifyRecursively(root_node, root_node);
    }
}

std::shared_ptr < BinaryNode > SBDD::deepCopy(const std::shared_ptr < BinaryNode > &node)
{
    if(node->node_index == terminal_0_node_index || node->node_index == terminal_1_node_index)
    {
        return this->nodes[node->node_index];
    }

    auto low_child = this->deepCopy(node->GetLowChild());
    auto high_child = this->deepCopy(node->GetHighChild());

    auto node_iterator = std::find(this->nodes.begin(), this->nodes.end(), node);
    if (node_iterator != this->nodes.end())
    {
        return *node_iterator;
    }

    auto new_node = std::shared_ptr < BinaryNode >(new BinaryNode(node->GetVariableName(),
                                                   {low_child, high_child},
                                                   node->node_index,
                                                   node->level,
                                                   node->function_indexes));

    this->nodes.push_back(new_node);
    return new_node;
}

SBDDStructure SBDD::getStructureRecirsively(const std::shared_ptr < BinaryNode > &node) const
{
    if (node == nullptr || node->GetVariableName() == "0" || node->GetVariableName() == "1")
    {
        return SBDDStructure();
    }
    SBDDStructure structure;
    SBDDStructureUnit structure_unit = {
        node->node_index,
        node->level,
        node->function_indexes,
        node->GetVariableName(),
        {
            node->GetLowChild() != nullptr ?
                node->GetLowChild()->node_index : (unsigned int)UINT_MAX,
            node->GetHighChild() != nullptr ?
                node->GetHighChild()->node_index : (unsigned int)UINT_MAX
        }
    };
    structure.units.push_back(structure_unit);
    SBDDStructure low_node_structure = this->getStructureRecirsively(node->GetLowChild());
    SBDDStructure high_node_structure = this->getStructureRecirsively(node->GetHighChild());
    for (SBDDStructureUnit &unit : low_node_structure.units)
    {
        if (std::find(
                    structure.units.begin(),
                    structure.units.end(),
                    unit
                    ) == structure.units.end()
           )
        {
            structure.units.push_back(unit);
        }
    }
    for (SBDDStructureUnit &unit : high_node_structure.units)
    {
        if (std::find(
                    structure.units.begin(),
                    structure.units.end(),
                    unit
                    ) == structure.units.end()
           )
        {
            structure.units.push_back(unit);
        }
    }
    return structure;
}

std::vector < std::string > SBDD::getVariablesNamesRecuresively(
        const std::shared_ptr < BinaryNode > &node
        ) const
{
    if (node == nullptr || node->GetVariableName() == "0" || node->GetVariableName() == "1")
    {
        return {};
    }
    std::vector < std::string > variables;
    std::vector < std::string > variables_from_low_node = this->getVariablesNamesRecuresively(
                node->GetLowChild()
                );
    std::vector < std::string > variables_from_high_node = this->getVariablesNamesRecuresively(
                node->GetHighChild()
                );
    std::vector < std::string > variables_from_children;
    variables_from_children.insert(
                variables_from_children.end(),
                variables_from_low_node.begin(),
                variables_from_low_node.end()
                );
    variables_from_children.insert(
                variables_from_children.end(),
                variables_from_high_node.begin(),
                variables_from_high_node.end()
                );
    for (std::string &variable : variables_from_children)
    {
        if (std::find(variables.begin(), variables.end(), variable) == variables.end())
        {
            variables.push_back(variable);
        }
    }
    if (std::find(variables.begin(), variables.end(), node->GetVariableName()) == variables.end())
    {
        variables.push_back(node->GetVariableName());
    }
    std::sort(variables.begin(), variables.end());
    return variables;
}

bool SBDD::member(const unsigned int &level,
        const std::vector<unsigned int> &function_indexes,
        const std::shared_ptr<BinaryNode> &low_node,
        const std::shared_ptr<BinaryNode> &high_node
        ) const
{
    for (unsigned int i = 0; i < this->nodes.size(); i++)
    {
        if (compare_nodes(
                    *(this->nodes[i].get()),
                    BinaryNode("", {low_node, high_node}, 0, level, function_indexes))
                )
        {
            return true;
        }
    }
    return false;
}

std::shared_ptr<BinaryNode> SBDD::lookup(
        const unsigned int &level,
        const std::vector<unsigned int> &function_indexes,
        const std::shared_ptr<BinaryNode> &low_node,
        const std::shared_ptr<BinaryNode> &high_node
        ) const
{
    for (unsigned int i = 0; i < this->nodes.size(); i++)
    {
        if (compare_nodes(
                    *(this->nodes[i].get()),
                    BinaryNode("", {low_node, high_node}, 0, level, function_indexes))
                )
        {
            return this->nodes[i];
        }
    }
    return std::shared_ptr<BinaryNode>(nullptr);
}

std::shared_ptr<BinaryNode> SBDD::insert(const std::string &variable_name,
        const unsigned int &level,
        const std::vector<unsigned int> &function_indexes,
        const std::shared_ptr<BinaryNode> &low_node,
        const std::shared_ptr<BinaryNode> &high_node
        )
{
    std::shared_ptr < BinaryNode > new_node(
        new BinaryNode(
            variable_name,
            {low_node, high_node},
            this->nodes.size(),
            level,
            function_indexes)
        );
    this->nodes.push_back(new_node);
    for (const unsigned int &function_index : function_indexes)
    {
        this->functions_nodes[function_index].push_back(new_node);
    }
    return this->nodes.back();
}

std::shared_ptr < BinaryNode > SBDD::buildRecursively(
        std::vector < std::pair < BinaryFunction,  std::shared_ptr < BinaryNode > > >& cash,
        const BinaryFunction &function,
        const unsigned int &function_index,
        const unsigned int &level_index
        )
{
    auto it = std::find_if(
                cash.begin(),
                cash.end(),
                [&function](const std::pair < BinaryFunction,  std::shared_ptr < BinaryNode > >& data){
                    return data.first == function;
                }
    );

    if (it != cash.end())
    {
        if (it->second->node_index != terminal_0_node_index &&
            it->second->node_index != terminal_1_node_index &&
            std::find(
                    it->second->function_indexes.begin(),
                    it->second->function_indexes.end(),
                    function_index
            ) == it->second->function_indexes.end())
        {
            it->second->function_indexes.push_back(function_index);
            std::sort(
                        it->second->function_indexes.begin(),
                        it->second->function_indexes.end()
                     );
        }
        return it->second;
    }

    BinaryFunctionValue function_value = function.GetValue();
    if (function_value != AnyValue)
    {
        return this->nodes[function_value == False ? terminal_0_node_index : terminal_1_node_index];
    }
    else
    {
        std::string variable_name = function.GetVariables().front();

        auto low_child_function = function.FixVariable(variable_name, False);
        auto high_child_function = function.FixVariable(variable_name, True);

        std::shared_ptr < BinaryNode > low_child =
                this->buildRecursively(
                    cash,
                    low_child_function,
                    function_index,
                    level_index + 1
                    );
        std::shared_ptr < BinaryNode > high_child =
                this->buildRecursively(
                    cash,
                    high_child_function,
                    function_index,
                    level_index + 1
                    );

        if(std::find_if(
                    cash.begin(),
                    cash.end(),
                    [&low_child_function]
                    (const std::pair < BinaryFunction,  std::shared_ptr < BinaryNode > >& data){
                                return data.first == low_child_function;
                    }
                    ) == cash.end())
        {
            cash.push_back({low_child_function, low_child});
        }

        if(std::find_if(
                    cash.begin(),
                    cash.end(),
                    [&high_child_function]
                    (const std::pair < BinaryFunction,  std::shared_ptr < BinaryNode > >& data){
                        return data.first == high_child_function;
                    }
                    ) == cash.end())
        {
            cash.push_back({high_child_function, high_child});
        }

        return MakeNode(
                    variable_name,
                    level_index,
                    {function_index},
                    low_child,
                    high_child
                    );
    }
    return nullptr;
}

std::shared_ptr < BinaryNode > SBDD::applyRecursively(
        const std::shared_ptr < AbstractBinaryOperation > &operation,
        const std::shared_ptr < BinaryNode > &left_operand,
        const std::shared_ptr < BinaryNode > &right_operand,
        std::map <
            std::pair < unsigned int, unsigned int >,
            std::shared_ptr < BinaryNode >
        > &operation_results
        )
{
    std::shared_ptr < BinaryNode > result(nullptr);
    if (operation_results.find(
            {left_operand->node_index, right_operand->node_index}
                ) != operation_results.end())
    {
        return operation_results[{left_operand->node_index, right_operand->node_index}];
    }
    else if (
             (left_operand->GetVariableName() == "0" ||
              left_operand->GetVariableName() == "1") &&
             (right_operand->GetVariableName() == "0" ||
              right_operand->GetVariableName() == "1")
             )
    {
        bool left_operand_value = true;
        if (left_operand->GetVariableName() == "0")
        {
            left_operand_value = false;
        }
        bool right_operand_value = true;
        if (right_operand->GetVariableName() == "0")
        {
            right_operand_value = false;
        }
        bool operation_value = operation->GetValue(left_operand_value, right_operand_value);
        if (operation_value)
        {
            result = this->nodes[terminal_1_node_index];
        }
        else
        {
            result = this->nodes[terminal_0_node_index];
        }
    }
    else if (left_operand->GetVariableName() == "0" || left_operand->GetVariableName() == "1")
    {
        result = this->MakeNode(
                    right_operand->GetVariableName(),
                    left_operand->level,
                    left_operand->function_indexes,
                    this->applyRecursively(
                        operation,
                        left_operand,
                        right_operand->GetLowChild(),
                        operation_results
                        ),
                    this->applyRecursively(
                        operation,
                        left_operand,
                        right_operand->GetHighChild(),
                        operation_results
                        )
                    );
    }
    else if (right_operand->GetVariableName() == "0" || right_operand->GetVariableName() == "1")
    {
        result = this->MakeNode(
                    left_operand->GetVariableName(),
                    left_operand->level,
                    left_operand->function_indexes,
                    this->applyRecursively(
                        operation,
                        left_operand->GetLowChild(),
                        right_operand,
                        operation_results
                        ),
                    this->applyRecursively(
                        operation,
                        left_operand->GetHighChild(),
                        right_operand,
                        operation_results
                        )
                    );
    }
    else if (left_operand->GetVariableName() == right_operand->GetVariableName())
    {
        result = this->MakeNode(
                    left_operand->GetVariableName(),
                    left_operand->level,
                    left_operand->function_indexes,
                    this->applyRecursively(
                        operation,
                        left_operand->GetLowChild(),
                        right_operand->GetLowChild(),
                        operation_results
                        ),
                    this->applyRecursively(
                        operation,
                        left_operand->GetHighChild(),
                        right_operand->GetHighChild(),
                        operation_results
                        )
                    );
    }
    else if (left_operand->GetVariableName() < right_operand->GetVariableName())
    {
        result = this->MakeNode(
                    left_operand->GetVariableName(),
                    left_operand->level,
                    left_operand->function_indexes,
                    this->applyRecursively(
                        operation,
                        left_operand->GetLowChild(),
                        right_operand,
                        operation_results
                        ),
                    this->applyRecursively(
                        operation,
                        left_operand->GetHighChild(),
                        right_operand,
                        operation_results
                        )
                    );
    }
    else
    {
        result = this->MakeNode(
                    left_operand->GetVariableName(),
                    left_operand->level,
                    left_operand->function_indexes,
                    this->applyRecursively(
                        operation,
                        left_operand,
                        right_operand->GetLowChild(),
                        operation_results
                        ),
                    this->applyRecursively(
                        operation,
                        left_operand,
                        right_operand->GetHighChild(),
                        operation_results
                        )
                    );
    }
    operation_results[{left_operand->node_index, right_operand->node_index}] = result;
    return result;
}

std::shared_ptr<BinaryNode> SBDD::restrictRecursively(const std::shared_ptr<BinaryNode> &node,
        const unsigned int &level_index,
        const bool &switch_node
        )
{
    if (node->GetVariableName() == "0" || node->GetVariableName() == "1")
    {
        return node;
    }
    if (node->level > level_index)
    {
        return node;
    }
    else if (node->level < level_index)
    {
        return this->MakeNode(
                    node->GetVariableName(),
                    node->level,
                    node->function_indexes,
                    this->restrictRecursively(node->GetLowChild(), level_index, switch_node),
                    this->restrictRecursively(node->GetHighChild(), level_index, switch_node)
                    );
    }
    else if (switch_node)
    {
        return this->restrictRecursively(node->GetHighChild(), level_index, switch_node);
    }
    else
    {
        return this->restrictRecursively(node->GetLowChild(), level_index, switch_node);
    }
    return nullptr;
}

unsigned int SBDD::satCountRecursively(const std::shared_ptr<BinaryNode> &node) const
{
    if (compare_nodes(*node, *this->nodes[terminal_0_node_index]))
    {
        return 0;
    }
    else if (compare_nodes(*node, *this->nodes[terminal_1_node_index]))
    {
        return 1;
    }
    unsigned int low_node_level =
            node->GetLowChild()->level != 0
            ? node->GetLowChild()->level : this->GetVariablesNames().size() + 1;
    unsigned int high_node_level =
            node->GetHighChild()->level != 0
            ? node->GetHighChild()->level : this->GetVariablesNames().size() + 1;
    return
            (1 << (low_node_level - node->level - 1)) *
            satCountRecursively(node->GetLowChild()) +
            (1 << (high_node_level - node->level - 1)) *
            satCountRecursively(node->GetHighChild());
}

Sat SBDD::anySatRecursively(const std::shared_ptr<BinaryNode> &node) const
{
    if (compare_nodes(*node, *this->nodes[terminal_0_node_index]))
    {
        std::cerr << "SBDD: Getting 0-terminal node for searching sats!\n";
        return SAT_ERROR_VALUE;
    }
    else if (compare_nodes(*node, *this->nodes[terminal_1_node_index]))
    {
        return Sat();
    }
    else if (compare_nodes(*node->GetLowChild(), *this->nodes[terminal_0_node_index]) &&
             compare_nodes(*node->GetHighChild(), *this->nodes[terminal_0_node_index]))
    {
        return SAT_ERROR_VALUE;
    }
    else if (compare_nodes(*node->GetLowChild(), *this->nodes[terminal_0_node_index]))
    {
        Sat sat = anySatRecursively(node->GetHighChild());
        sat.units.push_back({node->GetVariableName(), true});
        return sat;
    }
    else if (compare_nodes(*node->GetHighChild(), *this->nodes[terminal_1_node_index]))
    {
        Sat sat = anySatRecursively(node->GetLowChild());
        sat.units.push_back({node->GetVariableName(), false});
        return sat;
    }

    Sat sat_from_low_node = anySatRecursively(node->GetLowChild());
    Sat sat_from_high_node = anySatRecursively(node->GetHighChild());
    Sat sat = Sat();

    if (sat_from_high_node.units.size() > sat_from_low_node.units.size())
    {
        sat = sat_from_high_node;
        sat.units.push_back({node->GetVariableName(), true});
    }
    else
    {
        sat = sat_from_low_node;
        sat.units.push_back({node->GetVariableName(), false});
    }
    return sat;
}

std::vector < Sat > SBDD::allSatRecursively(const std::shared_ptr < BinaryNode > &node) const
{
    if (compare_nodes(*node, *this->nodes[terminal_0_node_index]))
    {
        return {};
    }
    else if (compare_nodes(*node, *this->nodes[terminal_1_node_index]))
    {
        return {Sat()};
    }
    else if (compare_nodes(*node->GetLowChild(), *this->nodes[terminal_0_node_index]) &&
             compare_nodes(*node->GetHighChild(), *this->nodes[terminal_0_node_index]))
    {
        return {};
    }
    else if (compare_nodes(*node->GetLowChild(), *this->nodes[terminal_0_node_index]))
    {
        std::vector < Sat > all_sats = allSatRecursively(node->GetHighChild());
        for (Sat &sat : all_sats)
        {
            sat.units.push_back({node->GetVariableName(), true});
        }
        return all_sats;
    }
    else if (compare_nodes(*node->GetHighChild(), *this->nodes[terminal_0_node_index]))
    {
        std::vector < Sat > all_sats = allSatRecursively(node->GetLowChild());
        for (Sat &sat : all_sats)
        {
            sat.units.push_back({node->GetVariableName(), false});
        }
        return all_sats;
    }

    std::vector < Sat > all_sats_from_low_node = allSatRecursively(node->GetLowChild());
    std::vector < Sat > all_sats_from_high_node = allSatRecursively(node->GetHighChild());
    std::vector < Sat > all_sats = {};

    for (Sat &sat : all_sats_from_high_node)
    {
        sat.units.push_back({node->GetVariableName(), true});
    }
    for (Sat &sat : all_sats_from_low_node)
    {
        sat.units.push_back({node->GetVariableName(), false});
    }

    all_sats.insert(all_sats.end(), all_sats_from_low_node.begin(), all_sats_from_low_node.end());
    all_sats.insert(all_sats.end(), all_sats_from_high_node.begin(), all_sats_from_high_node.end());

    return all_sats;
}

std::shared_ptr < BinaryNode > SBDD::simplifyRecursively(
        const std::shared_ptr < BinaryNode > &node1,
        const std::shared_ptr < BinaryNode > &node2)
{
    if (compare_nodes(*node1, *this->nodes[terminal_0_node_index]))
    {
        return this->nodes[terminal_0_node_index];
    }
    else if (compare_nodes(*node2, *this->nodes[terminal_0_node_index]) ||
             compare_nodes(*node2, *this->nodes[terminal_1_node_index]))
    {
        return node2;
    }
    else if (compare_nodes(*node1, *this->nodes[terminal_1_node_index]))
    {
        return this->MakeNode(
                    node2->GetVariableName(),
                    node2->level,
                    node2->function_indexes,
                    this->simplifyRecursively(node1, node2->GetLowChild()),
                    this->simplifyRecursively(node1, node2->GetHighChild())
                    );
    }
    else if (node1->level == node2->level)
    {
        if (compare_nodes(*node1->GetLowChild(), *this->nodes[terminal_0_node_index]))
        {
            return this->simplifyRecursively(node1->GetHighChild(), node2->GetHighChild());
        }
        else if (compare_nodes(*node1->GetHighChild(), *this->nodes[terminal_0_node_index]))
        {
            return this->simplifyRecursively(node1->GetLowChild(), node2->GetLowChild());
        }
        else
        {
            return this->MakeNode(
                        node2->GetVariableName(),
                        node2->level,
                        node2->function_indexes,
                        this->simplifyRecursively(node1->GetLowChild(), node2->GetLowChild()),
                        this->simplifyRecursively(node1->GetHighChild(), node2->GetHighChild())
                        );
        }
    }
    else if (node1->level < node2->level)
    {
        return this->MakeNode(
                    node2->GetVariableName(),
                    node2->level,
                    node2->function_indexes,
                    this->simplifyRecursively(node1->GetLowChild(), node2),
                    this->simplifyRecursively(node1->GetHighChild(), node2)
                    );
    }
    else
    {
        return this->MakeNode(
                    node2->GetVariableName(),
                    node2->level,
                    node2->function_indexes,
                    this->simplifyRecursively(node1, node2->GetLowChild()),
                    this->simplifyRecursively(node1, node2->GetHighChild())
                    );
    }
    return nullptr;
}

std::shared_ptr < BinaryNode > SBDD::MakeNode(const std::string &variable_name,
        const unsigned int &level,
        const std::vector < unsigned int > &function_indexes,
        const std::shared_ptr < BinaryNode > &low_node,
        const std::shared_ptr < BinaryNode > &high_node
        )
{
    if ((low_node != nullptr || high_node != nullptr) &&
            compare_nodes(*(low_node.get()), *(high_node.get())))
    {
        return low_node;
    }
    else if (this->member(level, function_indexes, low_node, high_node))
    {
        return this->lookup(level, function_indexes, low_node, high_node);
    }
    std::shared_ptr < BinaryNode > low_node_copy = low_node;
    std::shared_ptr < BinaryNode > high_node_copy = high_node;
    if (std::find(this->nodes.begin(), this->nodes.end(), low_node) == this->nodes.end())
    {
        low_node_copy = this->deepCopy(low_node);
    }
    if (std::find(this->nodes.begin(), this->nodes.end(), high_node_copy) == this->nodes.end())
    {
        high_node_copy = this->deepCopy(high_node_copy);
    }
    return this->insert(variable_name, level, function_indexes, low_node_copy, high_node_copy);
}
