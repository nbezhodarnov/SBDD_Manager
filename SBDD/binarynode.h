#ifndef BINARYNODE_H
#define BINARYNODE_H

#include <string>
#include <vector>
#include <memory>

class BinaryNode
{
public:
    BinaryNode(
            const std::string& variable_name_input = "",
            const std::vector < std::shared_ptr < BinaryNode > >& children_input = {nullptr, nullptr},
            const unsigned int &node_index_input = 0,
            const unsigned int &level_input = 0,
            const std::vector < unsigned int > &function_indexes_input = std::vector < unsigned int >()
            );

    static const int CHILDREN_COUNT = 2;

    enum ChildrenType {
        LowChild = 0,
        HighChild = 1
    };

    unsigned int node_index;
    unsigned int level;
    std::vector < unsigned int > function_indexes;

    void SetVariableName(const std::string &new_variable_name);

    void SetLowChild(const std::shared_ptr< BinaryNode > &new_low_child);
    void SetHighChild(const std::shared_ptr< BinaryNode > &new_high_child);
    void SetChildren(const std::vector < std::shared_ptr < BinaryNode > > &new_children);

    std::string GetVariableName() const;

    std::shared_ptr< BinaryNode > GetLowChild() const;
    std::shared_ptr< BinaryNode > GetHighChild() const;
    std::vector < std::shared_ptr < BinaryNode > > GetChildren() const;

protected:
    std::string variable_name;
    std::vector < std::shared_ptr < BinaryNode > > children;
};

#endif // BINARYNODE_H
