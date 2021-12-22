#include "binarynode.h"

#include <iostream>

BinaryNode::BinaryNode(
        const std::string &variable_name_input,
        const std::vector<std::shared_ptr<BinaryNode> > &children_input,
        const unsigned int &node_index_input,
        const unsigned int &level_input,
        const std::vector<unsigned int> &function_indexes_input
        )
{
    this->SetVariableName(variable_name_input);

    this->children.resize(CHILDREN_COUNT);
    this->SetChildren(children_input);

    this->node_index = node_index_input;
    this->level = level_input;
    this->function_indexes = function_indexes_input;
}

void BinaryNode::SetVariableName(const std::string &new_variable_name)
{
    this->variable_name = new_variable_name;
}

void BinaryNode::SetLowChild(const std::shared_ptr<BinaryNode>& new_low_child)
{
    this->children[LowChild] = new_low_child;
}

void BinaryNode::SetHighChild(const std::shared_ptr<BinaryNode>& new_high_child)
{
    this->children[HighChild] = new_high_child;
}

void BinaryNode::SetChildren(const std::vector<std::shared_ptr<BinaryNode> >& new_children)
{
    if (new_children.size() > CHILDREN_COUNT)
    {
        std::cout << "BinaryNode: There are more nodes that " << CHILDREN_COUNT << "! Only first " << CHILDREN_COUNT << " nodes will be used!\n";
        for (unsigned int i = 0; i < CHILDREN_COUNT; i++) {
            this->children[i] = new_children[i];
        }
    }
    else if (new_children.size() < CHILDREN_COUNT)
    {
        std::cerr << "BinaryNode: There are less nodes that " << CHILDREN_COUNT << "! Children will be set to default values!\n";
    }
    else
    {
        this->children = new_children;
    }
}

std::string BinaryNode::GetVariableName() const
{
    return this->variable_name;
}

std::shared_ptr<BinaryNode> BinaryNode::GetLowChild() const
{
    return this->children[LowChild];
}

std::shared_ptr<BinaryNode> BinaryNode::GetHighChild() const
{
    return this->children[HighChild];
}

std::vector<std::shared_ptr<BinaryNode> > BinaryNode::GetChildren() const
{
    return this->children;
}
