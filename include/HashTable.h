#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Facility.h"
#include <vector>
#include <string>

class HashTable {
private:
    // The hash table buckets for Separate Chaining
    std::vector<std::vector<Facility>> table;
    
    // Tracks insertion order
    std::vector<Facility> fileOrderList; 

    int capacity;
    int entries;
    int collisions;

    // Custom hash function
    int hashFunction(const std::string& key) const;

public:

    HashTable(int cap = 50); 

    // Helper to normalize strings to uppercase
    static std::string toUppercase(const std::string& str);

    bool insert(const Facility& facility);
    bool searchById(std::string id, Facility& foundFacility) const;

    void displayAllInOrder() const;
    
    int getCapacity() const;
    int getEntries() const;
    int getCollisions() const;
};

#endif