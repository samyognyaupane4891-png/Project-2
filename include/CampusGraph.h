#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>

class CampusGraph {
private:
    std::vector<std::string> buildings;

    std::vector<std::vector<std::string>> adjList; 

    std::string toUpper(const std::string& str) const;
    int getBuildingIndex(const std::string& name) const;

public:
    CampusGraph();

    bool addBuilding(const std::string& name);
    bool addConnection(const std::string& b1, const std::string& b2);

    void displayGraph() const;
    void displayCountsAndDegrees() const;
    void bfsReachability(const std::string& startBuilding) const;
    
    // Will be used later to validate facilities
    bool hasBuilding(const std::string& name) const; 
};

#endif
