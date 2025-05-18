#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Map {
private:
public:
    unordered_map<int, vector<int>> adjList;
    
    vector<pair<int, int>> edges = {
       {1, 2}, {1, 7}, {2, 3}, {2, 8}, {3, 10}, {4, 11}, {4, 5}, {5, 13}, {5, 6}, {6, 14},
       {7, 15}, {7, 8}, {8, 9}, {8, 16}, {9, 17}, {9, 10}, {10, 11}, {11, 12}, {12, 20}, {12, 13},
       {13, 21}, {13, 14}, {14, 22}, {15, 16}, {16, 23}, {17, 18}, {18, 26}, {19, 20}, {19, 27},
       {21, 22}, {21, 30}, {23, 31}, {23, 24}, {24, 25}, {24, 32}, {25, 26}, {26, 27},
       {27, 28}, {28, 29}, {29, 30}, {29, 33}, {30, 34}, {31, 35}, {31, 36}, {32, 33}, {32, 36},
       {33, 39}, {34, 39}, {34, 40}, {34, 48}, {35, 41}, {36, 37}, {37, 45}, {38, 46}, {38, 39},
       {40, 50}, {41, 42}, {42, 52}, {43, 53}, {43, 44}, {44, 45}, {44, 54}, {45, 46}, {46, 47},
       {47, 48}, {47, 57}, {48, 58}, {49, 59}, {49, 50}, {51, 52}, {51, 61}, {52, 53}, {54, 55},
       {55, 62}, {56, 57}, {56, 63}, {58, 59}, {59, 60}, {61, 62}, {62, 63}, {63, 64}, {31,43} ,
       {60,64}
    };
    int offset = 20;
    vector<pair<int, int>> pos = {
        {0 , 0 }, //0
        {469 + offset, 42 + offset}, {650 + offset, 42 + offset}, {880 + offset, 42 + offset}, {995 + offset, 42 + offset}, {1225 + offset, 42 + offset}, {1410 + offset, 42 + offset}, //6
        {469 + offset, 175 + offset}, {650 + offset, 175 + offset}, {765 + offset, 175 + offset}, {880 + offset, 175 + offset}, {995 + offset, 175 + offset}, {1110 + offset, 175 + offset}, {1225 + offset, 175 + offset}, {1410 + offset, 175 + offset}, //14
        {469 + offset, 280 + offset}, {650 + offset, 280 + offset}, {765 + offset, 280 + offset}, {880 + offset, 280 + offset}, {995 + offset, 280 + offset}, {1110 + offset, 280 + offset}, {1225 + offset, 280 + offset}, {1410 + offset, 280 + offset}, //22
        {650 + offset, 485 + offset}, {765 + offset, 485 + offset}, {765 + offset, 385 + offset}, {880 + offset, 385 + offset}, {995 + offset, 385 + offset}, {1110 + offset, 385 + offset}, {1110 + offset, 485 + offset}, {1225 + offset, 485 + offset}, //30
        {650 + offset, 695 + offset}, {765 + offset, 590 + offset}, {1110 + offset, 590 + offset}, {1225 + offset, 695 + offset}, //34
        {469 + offset, 695 + offset}, {765 + offset, 695 + offset}, {880 + offset, 695 + offset}, {995 + offset, 695 + offset}, {1110 + offset, 695 + offset}, {1410 + offset, 695 + offset}, //40
        {469 + offset, 789 + offset}, {540 + offset, 789 + offset}, {650 + offset, 789 + offset}, {765 + offset, 789 + offset}, {880 + offset, 789 + offset}, {995 + offset, 789 + offset}, {1110 + offset, 789 + offset}, {1225 + offset, 789 + offset}, {1335 + offset, 789 + offset}, {1410 + offset, 789 + offset}, //50
        {469 + offset, 895 + offset}, {540 + offset, 895 + offset}, {650 + offset, 895 + offset}, {765 + offset, 895 + offset}, {880 + offset, 895 + offset}, {995 + offset, 895 + offset}, {1110 + offset, 895 + offset}, {1225 + offset, 895 + offset}, {1335 + offset, 895 + offset}, {1410 + offset, 895 + offset}, //60
        {469 + offset, 995 + offset}, {880 + offset, 995 + offset}, {995 + offset, 995 + offset}, {1410 + offset, 995 + offset} //64
    };
    pair<int, int> getPos(int &u);
    void addEdge(int u, int v);
    void printAdjList();
    void init();
};
