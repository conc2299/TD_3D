#pragma once

#include <vector>
#include <string>
#include <functional>
#include <optional>


template <typename I>
class TreeNode
{
public:
    TreeNode(std::string name);
    void setParent(TreeNode<I>* parent) {parentNode = parent;};
    void addChild(TreeNode<I>* child) {children.push_back(child);};
    TreeNode<I>* getParent() {return  parentNode;};
    std::vector<TreeNode<I>*>& getChildren() {return children;};
    bool hasChild(std::string name) const ;
    std::optional<TreeNode<I>*> getChild(std::string name);
    bool isLeaf() const {return children.empty();};
    std::string name() const {return nodeName;};
    void setNodeInfo(I info) {nodeInfo = info;}
    void setNodeInfo(I&& info) {nodeInfo = info;}
    I& getNodeInfo() {return nodeInfo;}
private:
    std::string nodeName;
    I nodeInfo;
    TreeNode<I>* parentNode;
    std::vector<TreeNode<I>*> children; 
};

template <typename I>
TreeNode<I>::TreeNode(std::string name):nodeName(name)
{
    parentNode = nullptr;
}

template <typename I>
bool TreeNode<I>::hasChild(std::string name) const
{
    for(auto child : children)
    {
        if (child->name() == name)
        {
            return true;
        }
        
    }
    return false;
}

template <typename I>
std::optional<TreeNode<I>*> TreeNode<I>::getChild(std::string name)
{
    for(auto child : children)
    {
        if (child->name() == name)
        {
            return std::make_optional(child);
        }
        
    }
    return std::nullopt;
}

template <typename I>
class HierarchyTree
{
public:
    HierarchyTree(I&& info);
    ~HierarchyTree();
    void addHierarchyNode(std::vector<std::string> hierarchyName,I nodeInfo);
    void dfsForeach(std::function<void(I&,size_t)> f);
private:
    void recursiveFree(TreeNode<I>* node);
    TreeNode<I>* topNode;
};

template <typename I>
HierarchyTree<I>::HierarchyTree(I&& info)
{
    topNode = new TreeNode<I>("top");
    topNode->setNodeInfo(info);
}

template <typename I>
void HierarchyTree<I>::recursiveFree(TreeNode<I>* node)
{
    for(auto child : node->getChildren())
    {
        recursiveFree(child);
    }
    delete node;
}

template <typename I>
HierarchyTree<I>::~HierarchyTree()
{
    for(auto child : topNode->getChildren())
    {
        recursiveFree(child);
    }
    delete topNode;
}

template <typename I>
void HierarchyTree<I>::addHierarchyNode(std::vector<std::string> hierarchyName, I nodeInfo)
{
    TreeNode<I>* curNode = topNode;
    for(auto curHierName : hierarchyName)
    {
        std::optional<TreeNode<I>*> nextNode = curNode->getChild(curHierName);
        if(nextNode.has_value())
        {
            curNode = nextNode.value();
            continue;
        }
        else
        {
            // if node does not exist, allocate and add one to the tree
            TreeNode<I>* node = new TreeNode<I>(curHierName);
            node->setParent(curNode);
            node->setNodeInfo(nodeInfo);
            curNode->addChild(node);
            curNode = node;
        }
    }   
}

template <typename I>
void dfsRecursive(TreeNode<I>* node, std::function<void(I&,size_t)> f, size_t depth)
{
    f(node->getNodeInfo(),depth);
    for(TreeNode<I>* child : node->getChildren()){
        dfsRecursive(child,f,depth + 1);
    }
}

template <typename I>
void HierarchyTree<I>::dfsForeach(std::function<void(I&,size_t)> f)
{
    dfsRecursive(topNode,f,0);
}