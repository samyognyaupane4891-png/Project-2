#include "DepartmentTree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include "Facility.h"
#include "HashTable.h"
#include "CampusGraph.h"


std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}


std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(line);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token)); // Trim spaces from each field
    }
    return tokens;
}

void loadBuildings(const std::string& directory, CampusGraph& graph) {
    std::string filepath = directory + "/buildings.txt";
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filepath << "\n";
        return;
    }

    std::string line;
    int lineNumber = 0, accepted = 0, rejected = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine[0] == '#') continue;

        if (graph.addBuilding(trimmedLine)) {
            accepted++;
        } else {
            std::cout << "Warning: buildings.txt line " << lineNumber << " rejected (duplicate name).\n";
            rejected++;
        }
    }
    std::cout << "buildings.txt: " << accepted << " accepted, " << rejected << " rejected\n";
}

// Load Connections
void loadConnections(const std::string& directory, CampusGraph& graph) {
    std::string filepath = directory + "/connections.txt";
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filepath << "\n";
        return;
    }

    std::string line;
    int lineNumber = 0, accepted = 0, rejected = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine[0] == '#') continue;

        std::vector<std::string> fields = split(line, '|');
        if (fields.size() != 2) {
            std::cout << "Warning: connections.txt line " << lineNumber << " rejected (invalid format).\n";
            rejected++;
            continue;
        }

        if (!graph.hasBuilding(fields[0]) || !graph.hasBuilding(fields[1])) {
            std::cout << "Warning: connections.txt line " << lineNumber << " rejected (unknown building).\n";
            rejected++;
            continue;
        }

        if (graph.addConnection(fields[0], fields[1])) {
            accepted++;
        } else {
            std::cout << "Warning: connections.txt line " << lineNumber << " rejected (duplicate or self-connection).\n";
            rejected++;
        }
    }
    std::cout << "connections.txt: " << accepted << " accepted, " << rejected << " rejected\n";
}

// Member A: Load Facilities
void loadFacilities(const std::string& directory, HashTable& hashTable) {
    std::string filepath = directory + "/facilities.txt";
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filepath << "\n";
        return;
    }

    std::string line;
    int lineNumber = 0;
    int accepted = 0;
    int rejected = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        std::string trimmedLine = trim(line);
        
        // Skip empty lines and comments
        if (trimmedLine.empty() || trimmedLine[0] == '#') continue;

        std::vector<std::string> fields = split(line, '|');

        // Validate field count
        if (fields.size() != 4) {
            std::cout << "Warning: facilities.txt line " << lineNumber
            << " rejected (invalid field count).\n";
            rejected++;
            continue;
        }

        std::string id = fields[0];
        std::string name = fields[1];
        std::string building = fields[2];
        std::string category = fields[3];

        // Validate empty fields
        if (id.empty() || name.empty() || building.empty() || category.empty()) {
            std::cout << "Warning: facilities.txt line " << lineNumber
            << " rejected (empty field).\n";
            rejected++;
            continue;
        }

        // Validate ID format (F + exactly 3 digits)
        std::string upperId = HashTable::toUppercase(id);
        if (upperId.length() != 4 || upperId[0] != 'F' || 
            !isdigit(upperId[1]) || !isdigit(upperId[2]) || !isdigit(upperId[3])) {
            std::cout << "Warning: facilities.txt line " << lineNumber
            << " rejected (invalid ID format).\n";
            rejected++;
            continue;
        }


        Facility f(id, name, building, category);
        

        if (hashTable.insert(f)) {
            accepted++;
        } else {
            std::cout << "Warning: facilities.txt line " << lineNumber
            << " rejected (duplicate ID).\n";
            rejected++;
        }
    }

    // LOAD-4 requirement: Facilities Load Summary
    std::cout << "facilities.txt: " << accepted << " accepted, "
    << rejected << " rejected ";
    std::cout << "(hash capacity " << hashTable.getCapacity() 
              << ", entries " << hashTable.getEntries() 
              << ", collisions " << hashTable.getCollisions() << ")\n";
}


// Load Departments
void loadDepartments(const std::string& directory, DepartmentTree& tree) {
    std::string filepath = directory + "/departments.txt";
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filepath << "\n";
        return;
    }

    std::string line;
    int lineNumber = 0, accepted = 0, rejected = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine[0] == '#') continue;

        std::vector<std::string> fields = split(line, '|');
        if (fields.size() != 2) {
            std::cout << "Warning: departments.txt line " << lineNumber << " rejected (invalid format).\n";
            rejected++;
            continue;
        }

        std::string name = fields[0];
        std::string parentName = fields[1];

        if (name.empty() || parentName.empty()) {
            std::cout << "Warning: departments.txt line " << lineNumber << " rejected (empty field).\n";
            rejected++;
            continue;
        }

        if (tree.addDepartment(name, parentName)) {
            accepted++;
        } else {
            std::cout << "Warning: departments.txt line " << lineNumber << " rejected (duplicate name or unknown parent).\n";
            rejected++;
        }
    }
    std::cout << "departments.txt: " << accepted << " accepted, " << rejected << " rejected\n";
}


int main(int argc, char* argv[]) {

    std::string dataDir = "data";
    if (argc > 1) {
        dataDir = argv[1];
    }

    HashTable facilityTable(50); // HASH-5: Initial capacity 
    
    // sidd work
    CampusGraph campusGraph;
    loadBuildings(dataDir, campusGraph);
    loadConnections(dataDir, campusGraph);
    loadFacilities(dataDir, facilityTable);
    
    // Aayush's Tree loading work please put here buddy
    DepartmentTree departmentTree;
    loadDepartments(dataDir, departmentTree);

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- Milestone 1 Menu ---\n";
        std::cout << "1. View all facilities\n";
        std::cout << "2. Search facility by ID\n";
        std::cout << "3. View campus graph\n";
        std::cout << "4. Display graph counts and degrees\n";
        std::cout << "5. Find reachable buildings (BFS)\n";

       
        std::cout << "6. Add department\n";
        std::cout << "7. Search department by name\n";
        std::cout << "8. Display department hierarchy\n";
        std::cout << "9. Display departments in preorder\n";
       
        
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        
        // UI-4: Recover from text at numeric prompts
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a valid number.\n";
            choice = -1;
            continue;
        }

        if (choice == 1) {
            facilityTable.displayAllInOrder();
        } else if (choice == 2) {
            std::string searchId;
            std::cout << "Enter Facility ID: ";
            std::cin >> searchId;
            
            // Clean stream in case of leftover whitespace
            std::cin.ignore(10000, '\n'); 
            
            std::string upperSearchId = HashTable::toUppercase(trim(searchId));
            
            // Validate search format (F + 3 digits)
            if (upperSearchId.length() != 4 || upperSearchId[0] != 'F' || 
                !isdigit(upperSearchId[1]) || !isdigit(upperSearchId[2]) ||
                !isdigit(upperSearchId[3])) {
                std::cout << "Invalid format: an ID is F followed by "
                             "exactly three digits.\n";
                continue;
            }

            Facility found("", "", "", "");
            if (facilityTable.searchById(upperSearchId, found)) {
                std::cout << "Found: ";
                found.display();
            } else {
                std::cout << "Not found: " << upperSearchId << "\n";
            }
        } else if (choice == 3) {
            campusGraph.displayGraph();
        } else if (choice == 4) {
            campusGraph.displayCountsAndDegrees();
        } else if (choice == 5) {
            std::string startBuilding;
            std::cout << "Enter start building: ";

            // Clearing out any leftover newline characters from previous input
            std::cin.ignore(10000, '\n');

            // Using getline to allow spaces in building names
            std::getline(std::cin, startBuilding);
            startBuilding = trim(startBuilding);

            if (startBuilding.empty()) {
                std::cout << "Error: Building name cannot be blank.\n";
            } else {
                campusGraph.bfsReachability(startBuilding);
            }
        } else if (choice == 0) {
            std::cout << "Exiting...\n";

           
        } 
        
        //Aayush's options 6-9 below
        else if (choice == 6) {
            std::cin.ignore(10000, '\n'); 
            if (departmentTree.isEmpty()) {
                std::cout << "Tree is empty. Enter root department name: ";
                std::string rootName;
                std::getline(std::cin, rootName);
                rootName = trim(rootName);
                
                if (rootName.empty()) {
                    std::cout << "Error: Name cannot be blank.\n";
                } else if (departmentTree.addDepartment(rootName, "NONE")) {
                    std::cout << "Root department added successfully.\n";
                }
            } else {
                std::cout << "Enter new department name: ";
                std::string name;
                std::getline(std::cin, name);
                
                std::cout << "Enter existing parent department name: ";
                std::string parent;
                std::getline(std::cin, parent);
                
                name = trim(name);
                parent = trim(parent);
                
                if (name.empty() || parent.empty()) {
                    std::cout << "Error: Name and parent cannot be blank.\n";
                } else if (departmentTree.addDepartment(name, parent)) {
                    std::cout << "Department added successfully.\n";
                } else {
                    std::cout << "Error: Duplicate department name or unknown parent.\n";
                }
            }
        } else if (choice == 7) {
            std::string searchName;
            std::cout << "Enter department name to search: ";
            std::cin.ignore(10000, '\n');
            std::getline(std::cin, searchName);
            searchName = trim(searchName);
            
            if (searchName.empty()) {
                std::cout << "Error: Name cannot be blank.\n";
            } else if (!departmentTree.searchDepartment(searchName)) {
                std::cout << "Department not found.\n";
            }
        } else if (choice == 8) {
            departmentTree.displayHierarchy();
        } else if (choice == 9) {
            departmentTree.displayPreorder();
        } else {
            std::cout << "Option not implemented yet or out of range.\n";
        }
    }
    
    return 0;
}
