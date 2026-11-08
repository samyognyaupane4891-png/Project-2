#include "HashTable.h"
#include <iostream>
#include <cctype>

HashTable::HashTable(int cap) : capacity(cap), entries(0), collisions(0) {
    table.resize(capacity);
}

// Converts a string to uppercase character by character
std::string HashTable::toUppercase(const std::string& str) {
    std::string upperStr = str;
    for (char& c : upperStr) {
        c = std::toupper(c);
    }
    return upperStr;
}

// Custom polynomial rolling hash function
int HashTable::hashFunction(const std::string& key) const {
    unsigned int hashValue = 0;
    for (char c : key) {
        hashValue = (hashValue * 31) + c; 
    }
    return hashValue % capacity;
}

bool HashTable::insert(const Facility& facility) {
    std::string upperId = toUppercase(facility.getId());
    int index = hashFunction(upperId);

    // Check for duplicates case-insensitively
    for (const Facility& f : table[index]) {
        if (toUppercase(f.getId()) == upperId) {
            return false; // Reject duplicate
        }
    }

    // Define a collision as inserting into a bucket that already has items
    if (!table[index].empty()) {
        collisions++;
    }

    table[index].push_back(facility);
    fileOrderList.push_back(facility);
    entries++;
    return true;
}

bool HashTable::searchById(std::string id, Facility& foundFacility) const {
    std::string upperId = toUppercase(id);
    int index = hashFunction(upperId);

    for (const Facility& f : table[index]) {
        if (toUppercase(f.getId()) == upperId) {
            foundFacility = f;
            return true;
        }
    }
    return false;
}

void HashTable::displayAllInOrder() const {
    if (fileOrderList.empty()) {
        std::cout << "No facilities available.\n";
        return;
    }
    for (const Facility& f : fileOrderList) {
        f.display();
    }
    std::cout << "Total facilities: " << entries << "\n";
}

int HashTable::getCapacity() const { return capacity; }
int HashTable::getEntries() const { return entries; }
int HashTable::getCollisions() const { return collisions; }