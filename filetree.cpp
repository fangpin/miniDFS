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
    TreeNode* match = _root;
    TreeNode* cur = _root->firstSon;
    for (size_t i = 0; i < path_folders.size(); ++i) {
      // Try to get the node
      while (cur && cur->value_ != path_folders[i]) {
        cur = cur->nextSibling;
      }

      // Found the node, try next layer
      if (cur) {
        match = cur;
        cur = cur->firstSon;
        continue;
      }

      // If the last layer, add node directly
      if (i + 1 == path_folders.size()) {
        // create this node of file
        auto node = new TreeNode(path_folders[i], true);
        node->parent = match;
        if (!match->firstSon) {
          match->firstSon = node;
        } else {
          auto sibling = match->firstSon;
          while (sibling->nextSibling) {
            sibling = sibling->nextSibling;
          }
          sibling->nextSibling = node;
        }

        return true;
      }

      // If not the last layer, add missing dir node
      auto dir = new TreeNode(path_folders[i], false);
      dir->parent = match;
      if (!match->firstSon) {
        match->firstSon = dir;
      } else {
        auto sibling = match->firstSon;
        while (sibling->nextSibling) {
          sibling = sibling->nextSibling;
        }
        sibling->nextSibling = dir;
      }

      match = dir;
      cur = match->firstSon;
    }

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
    return (*last_node)->isFile;
}

void FileTree::list(TreeNode *node, std::map<std::string, std::pair<int, int>>& meta){
    static int chunkSize = 2 * 1024 * 1024;
    if(node){
        if (node->isFile) {
          auto full_path_of_file = list_node_path(node);
          std::cout << full_path_of_file << "\t" << meta[full_path_of_file].first << "\t" << (int)ceil(1.0 * meta[full_path_of_file].second/chunkSize) << std::endl;
        }
        // std::cout << node->value_ << "\t" << std::endl;
        list(node->firstSon, meta);
        list(node->nextSibling, meta);
    }
}

void FileTree::list(std::map<std::string, std::pair<int, int>>& meta){
    list(_root, meta);
}

const std::string FileTree::list_node_path(TreeNode* node) {
  while (node->parent) {
    return list_node_path(node->parent) + "/" + node->value_;
  }

  // Don't return the root value, which is '/'
  return (node->value_ != "/") ? node->value_ : "";
}