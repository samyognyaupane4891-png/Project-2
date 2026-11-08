#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>

// Represents a single department in the general tree
struct DepartmentNode {
    std::string name;
    DepartmentNode* parent;
    std::vector<DepartmentNode*> children;
    int depth;

    DepartmentNode(std::string n, DepartmentNode* p, int d)
        : name(n), parent(p), depth(d) {}
};

class DepartmentTree {
private:
    DepartmentNode* root;

    std::string toUpper(const std::string& str) const;
    DepartmentNode* findNode(DepartmentNode* current, const std::string& targetName) const;
    
    void printHierarchy(DepartmentNode* node) const;
    void printPreorder(DepartmentNode* node) const;
    void clearTree(DepartmentNode* node); // For memory management (NFR-11)

public:
    DepartmentTree();
    ~DepartmentTree();

    bool isEmpty() const;
    bool addDepartment(const std::string& name, const std::string& parentName);
    bool searchDepartment(const std::string& name) const;
    
    void displayHierarchy() const;
    void displayPreorder() const;
};

#endif