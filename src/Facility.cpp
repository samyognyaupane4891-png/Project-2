#include "Facility.h"
#include <iostream>

Facility::Facility(std::string id, std::string name, std::string building,
    std::string category)
    : id(id), name(name), building(building), category(category) {}

std::string Facility::getId() const { return id; }
std::string Facility::getName() const { return name; }
std::string Facility::getBuilding() const { return building; }
std::string Facility::getCategory() const { return category; }

void Facility::display() const {
    std::cout << id << " | " << name << " | " << building << " | " << category << "\n";
}
