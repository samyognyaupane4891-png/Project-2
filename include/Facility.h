#ifndef FACILITY_H
#define FACILITY_H

#include <string>

class Facility {
private:
    std::string id;
    std::string name;
    std::string building;
    std::string category;

public:
    Facility(std::string id, std::string name, std::string building,
        std::string category);

    std::string getId() const;
    std::string getName() const;
    std::string getBuilding() const;
    std::string getCategory() const;

    void display() const;
};

#endif