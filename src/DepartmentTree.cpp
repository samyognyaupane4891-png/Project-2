#include "DepartmentTree.h"
#include <iostream>
#include <cctype>

DepartmentTree::DepartmentTree() : root(nullptr) {}

DepartmentTree::~DepartmentTree() {
    clearTree(root);
}

void DepartmentTree::clearTree(DepartmentNode* node) {
    if (node == nullptr) return;
    for (DepartmentNode* child : node->children) {
        clearTree(child);
    }
    delete node;
}

std::string DepartmentTree::toUpper(const std::string& str) const {
    std::string upperStr = str;
    for (char& c : upperStr) {
        c = std::toupper(c);
    }
    return upperStr;
}

bool DepartmentTree::isEmpty() const {
    return root == nullptr;
}

// Recursively searches the general tree for a matching name
DepartmentNode* DepartmentTree::findNode(DepartmentNode* current, const std::string& targetName) const {
    if (current == nullptr) return nullptr;
    
    if (toUpper(current->name) == toUpper(targetName)) {
        return current;
    }
    
    for (DepartmentNode* child : current->children) {
        DepartmentNode* found = findNode(child, targetName);
        if (found != nullptr) return found;
    }
    return nullptr;
}

bool DepartmentTree::addDepartment(const std::string& name, const std::string& parentName) {
    // Reject duplicate names case-insensitively
    if (findNode(root, name) != nullptr) {
        return false; 
    }

    if (root == nullptr) {
        // First valid record becomes the root
        root = new DepartmentNode(name, nullptr, 0);
        return true;
    }

    // Require an existing parent for non-root nodes
    DepartmentNode* parentNode = findNode(root, parentName);
    if (parentNode == nullptr) {
        return false; // Unknown parent
    }

    DepartmentNode* newNode = new DepartmentNode(name, parentNode, parentNode->depth + 1);
    parentNode->children.push_back(newNode);
    return true;
}

bool DepartmentTree::searchDepartment(const std::string& name) const {
    DepartmentNode* found = findNode(root, name);
    if (found == nullptr) {
        return false;
    }

    std::cout << "Name: " << found->name << "\n";
    std::cout << "Parent: " << (found->parent ? found->parent->name : "None") << "\n";
    std::cout << "Direct Children: " << found->children.size() << "\n";
    std::cout << "Depth: " << found->depth << "\n";
    return true;
}

void DepartmentTree::printHierarchy(DepartmentNode* node) const {
    if (node == nullptr) return;
    
    // Indent 4 spaces per depth level
    std::string indent(node->depth * 4, ' ');
    std::cout << indent << node->name << "\n";
    
    for (DepartmentNode* child : node->children) {
        printHierarchy(child);
    }
}

void DepartmentTree::displayHierarchy() const {
    if (isEmpty()) {
        std::cout << "No departments are loaded.\n";
        return;
    }
    printHierarchy(root);
}

void DepartmentTree::printPreorder(DepartmentNode* node) const {
    if (node == nullptr) return;
    
    std::cout << node->name << "\n"; // No indentation for this display
    
    for (DepartmentNode* child : node->children) {
        printPreorder(child);
    }
}

void DepartmentTree::displayPreorder() const {
    if (isEmpty()) {
        std::cout << "No departments are loaded.\n";
        return;
    }
    std::cout << "\nPreorder Traversal Sequence:\n";
    printPreorder(root);
}