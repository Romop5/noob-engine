#ifndef _TREENODE_H
#define _TREENODE_H
#include <vector>
template <class T>
class TreeNode
{
    private:
        std::vector<TreeNode<T>> children;
    public:
    std::vector<TreeNode<T>> getChildren() const { return this->children; }
    
    void addChild(const TreeNode<T> child)
    {
        this->children.push_back(child);
    }
    void detachChild(const size_t position)
    {
        this->children.erase(children.begin()+position);
    }
};
#endif
