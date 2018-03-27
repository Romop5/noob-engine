#ifndef _TREENODE_H
#define _TREENODE_H
#include <vector>
template <class T> class TreeNode {
  private:
    std::vector<T> children;

  public:
    std::vector<T> getChildren() const { return this->children; }

    void addChild(const T child) { this->children.push_back(child); }
    void detachChild(const size_t position) {
        this->children.erase(children.begin() + position);
    }
    void clearChildren() { this->children.clear(); }
};
#endif
