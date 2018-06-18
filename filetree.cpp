#include <iostream>
#include <string>
#include <cmath>
#include "filetree.h"
#include "util.h"

FileTree::FileTree(){
    _root = new TreeNode("/", false);
}

bool FileTree::insert_node(const std::string &path, const bool isFile){
    TreeNode * parent = nullptr;
    bool isFound = find_node(path, &parent);
    if(isFound) return false;

    std::vector<std::string> path_folders = split(path, '/');
    TreeNode *newNode = new TreeNode(path, isFile);
    newNode->parent = parent;
    TreeNode* son = parent->firstSon;
    if(!son)
        parent->firstSon = newNode;
    else{
        while(son->nextSibling)
            son = son->nextSibling;
        son->nextSibling = newNode;
    }
    while(son) son = son->nextSibling;
    son = newNode;
    return true;
}

bool FileTree::find_node(const std::string &path, TreeNode **last_node)const{
    std::vector<std::string> path_folders = split(path, '/');
    TreeNode *node = _root->firstSon;
    *last_node = _root;
    for(const auto &name: path_folders){
        while(node && node->value_ != name)
            node = node->nextSibling;
        if(!node)
            return false;
        *last_node = node;
        node = node->firstSon;
    }
    return true && node->isFile;
}

void FileTree::list(TreeNode *node, std::map<std::string, std::pair<int, int>>& meta){
    static int chunkSize = 2 * 1024 * 1024;
    if(node){
        std::cout << node->value_ << "\t" << meta[node->value_].first << "\t" << (int)ceil(1.0 * meta[node->value_].second/chunkSize) << std::endl;
        // std::cout << node->value_ << "\t" << std::endl;
        list(node->firstSon, meta);
        list(node->nextSibling, meta);
    }
}

void FileTree::list(std::map<std::string, std::pair<int, int>>& meta){
    list(_root, meta);
}
