#include "CampusGraph.h"
#include <iostream>
#include <queue>
#include <cctype>
#include <algorithm>

CampusGraph::CampusGraph() {}

std::string CampusGraph::toUpper(const std::string& str) const {
    std::string upperStr = str;
    for (char& c : upperStr) {
        c = std::toupper(c);
    }
    return upperStr;
}

int CampusGraph::getBuildingIndex(const std::string& name) const {
    std::string upperName = toUpper(name);
    for (size_t i = 0; i < buildings.size(); ++i) {
        if (toUpper(buildings[i]) == upperName) {
            return i;
        }
    }
    return -1;
}

bool CampusGraph::hasBuilding(const std::string& name) const {
    return getBuildingIndex(name) != -1;
}

bool CampusGraph::addBuilding(const std::string& name) {
    if (hasBuilding(name)) {
        return false; // Reject duplicate building
    }
    buildings.push_back(name);
    adjList.push_back(std::vector<std::string>());
    return true;
}

bool CampusGraph::addConnection(const std::string& b1, const std::string& b2) {
    int index1 = getBuildingIndex(b1);
    int index2 = getBuildingIndex(b2);

    // Both buildings must exist
    if (index1 == -1 || index2 == -1) return false;
    // Reject self-connections
    if (index1 == index2) return false;

    // Checking for existing duplicate connection
    std::string upperB2 = toUpper(b2);
    for (const std::string& neighbor : adjList[index1]) {
        if (toUpper(neighbor) == upperB2) {
            return false;
        }
    }

    // Preserve original formatting of neighbors from buildings.txt
    adjList[index1].push_back(buildings[index2]);
    adjList[index2].push_back(buildings[index1]);
    return true;
}

void CampusGraph::displayGraph() const {
    if (buildings.empty()) {
        std::cout << "No buildings available.\n";
        return;
    }
    for (size_t i = 0; i < buildings.size(); ++i) {
        std::cout << buildings[i] << ": ";
        if (adjList[i].empty()) {
            std::cout << "(no connections)\n";
        } else {
            for (size_t j = 0; j < adjList[i].size(); ++j) {
                std::cout << adjList[i][j];
                if (j < adjList[i].size() - 1) std::cout << ", ";
            }
            std::cout << "\n";
        }
    }
}

void CampusGraph::displayCountsAndDegrees() const {
    int totalConnections = 0;
    std::cout << "\nBuilding Degrees:\n";
    for (size_t i = 0; i < buildings.size(); ++i) {
        std::cout << buildings[i] << ": " << adjList[i].size() << "\n";
        totalConnections += adjList[i].size();
    }
    // Dividing by 2 because undirected edges are stored twice
    std::cout << "Total buildings: " << buildings.size() << "\n";
    std::cout << "Total connections: " << totalConnections / 2 << "\n";
}

void CampusGraph::bfsReachability(const std::string& startBuilding) const {
    int startIndex = getBuildingIndex(startBuilding);
    if (startIndex == -1) {
        std::cout << "Error: Unknown building '" << startBuilding << "'.\n";
        return;
    }


    std::queue<std::pair<int, int>> q;
    std::vector<bool> visited(buildings.size(), false);

    //  Mark visited when added to queue
    q.push({startIndex, 0});
    visited[startIndex] = true;

    int reachableCount = 0;
    std::cout << "\nReachable from " << buildings[startIndex] << ":\n";

    while (!q.empty()) {
        int currIndex = q.front().first;
        int distance = q.front().second;
        q.pop();

        std::cout << buildings[currIndex] << " (distance " << distance << ")\n";
        reachableCount++;

        for (const std::string& neighbor : adjList[currIndex]) {
            int neighborIndex = getBuildingIndex(neighbor);
            if (!visited[neighborIndex]) {
                visited[neighborIndex] = true; // Mark when enqueued
                q.push({neighborIndex, distance + 1});
            }
        }
    }
    std::cout << "Reachable: " << reachableCount << " of " << buildings.size() << " buildings\n";
}