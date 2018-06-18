#ifndef _FILE_TREE_H
#define _FILE_TREE_H

#include <string>
#include <map>

struct TreeNode{
    std::string value_;
    bool isFile;
    TreeNode *parent;
    TreeNode *firstSon;
    TreeNode *nextSibling;

    TreeNode(const std::string &_value, const bool &_isFile): value_(_value), isFile(_isFile), parent(nullptr), firstSon(nullptr), nextSibling(nullptr){};
};

class FileTree{
private:
    TreeNode *_root;
public:
    FileTree();
    bool insert_node(const std::string &path, const bool isFile);
    bool find_node(const std::string &path, TreeNode **parent)const;
    void list(TreeNode *node, std::map<std::string, std::pair<int, int>>& meta);
    void list(std::map<std::string, std::pair<int, int>>& meta);
};

#endif
