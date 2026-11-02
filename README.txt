Smart Campus Navigation and Facility Analytics System

Purpose: Load campus data to locate facilities, find building routes via BFS,
and navigate academic departments.

Directory Layout:
/include  - Header files (.h)
/src      - Implementation files (.cpp)
/data     - Default instructor data files
/testdata - Custom edge-case testing files

Build and Run Instructions (UNT CSE CELL):
1. Compile: g++ -std=c++17 src/*.cpp -I include -o campus
2. Run default: ./campus
3. Run with custom data: ./campus testdata/