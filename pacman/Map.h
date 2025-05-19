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
        {1,2},{1,11},{2,3},{3,4},{3,13},{4,5},{5,15},{6,16},{6,7},{7,8},{8,9},{8,18}, {9,10},{10,20},
        {11,12},{11,21},{12,13},{13,23},{13,14},{14,15},{14,24},{15,16},{16,17},{17,18},{17,27},{18,19},{18,28},{19,20},{20,30},
        {21,22},{22,23},{23,31}, {24,25},{25,33},{26,35},{26,27},{28,29},{28,37},{29,30},
        {31,38},{32,33},{32,39},{33,34},{34,35},{34,41},{35,36},{36,43},{37,44},
        {38,39},{38,45},{39,46}, {40,41},{41,42},{43,44},{43,49}, {44,50},
        {45,53},{46,47},{46,54},{47,48},{48,49},{49,57},{50,58},
        {51,52},{51,61},{52,53},{53,63},{53,54},{54,55},{55,65},{56,66},{56,57},{57,58},{58,68},{58,59},{59,60},{60,70},
        {61,62},{62,72},{63,73},{63,64},{64,74},{64,65},{65,66},{66,67},{67,77},{67,68},{68,78},{69,70},{69,79},
        {71,72},{71,81},{72,73},{74,75},{75,85},{76,86},{76,77},{78,79},{79,80},{80,90},
        {81,82},{82,83},{83,84},{84,85},{85,86},{86,87},{87,88},{88,89},{89,90}, {91,92}, {38,91}, {44,92},
    };
    float offset = 20;
    vector<pair<int, int>> pos = {
        {0,0},
        {469 + offset, 42 + offset},{540 + offset, 42 + offset},{650 + offset, 42 + offset},{765 + offset, 42 + offset},{880 + offset, 42 + offset},{995 + offset, 42 + offset},{1110 + offset, 42 + offset},{1225 + offset, 42 + offset},{1335 + offset, 42 + offset},{1410 + offset, 42 + offset},
        {469 + offset, 175 + offset},{540 + offset, 175 + offset},{650 + offset, 175 + offset},{765 + offset, 175 + offset},{880 + offset, 175 + offset},{995 + offset, 175 + offset},{1110 + offset, 175 + offset},{1225 + offset, 175 + offset},{1335 + offset, 175 + offset},{1410 + offset, 175 + offset},
        {469 + offset, 280 + offset},{540 + offset, 280 + offset},{650 + offset, 280 + offset},{765 + offset, 280 + offset},{880 + offset, 280 + offset},{995 + offset, 280 + offset},{1110 + offset, 280 + offset},{1225 + offset, 280 + offset},{1335 + offset, 280 + offset},{1410 + offset, 280 + offset},
        {650 + offset, 385 + offset},{765 + offset, 385 + offset},{880 + offset, 385 + offset},{(880 - 0.75 * offset + 995 + 2.5 * offset) / 2, 385 + offset }, { 995 + offset, 385 + offset },{1110 + offset, 385 + offset},{1225 + offset, 385 + offset},
        {650 + offset, 485 + offset},{765 + offset, 485 + offset},{880 - 0.75 * offset, 485 + offset},{(880 - 0.75 * offset + 995 + 2.5 * offset)/2,485 + offset}, { 995 + 2.5 * offset, 485 + offset },{1110 + offset, 485 + offset},{1225 + offset, 485 + offset},
        {650 + offset, 590 + offset},{765 + offset, 590 + offset},{880 + offset, 590 + offset},{995 + offset, 590 + offset},{1110 + offset, 590 + offset},{1225 + offset, 590 + offset},
        {469 + offset, 695 + offset},{540 + offset, 695 + offset},{650 + offset, 695 + offset},{765 + offset, 695 + offset},{880 + offset, 695 + offset},{995 + offset, 695 + offset},{1110 + offset, 695 + offset},{1225 + offset, 695 + offset},{1335 + offset, 695 + offset},{1410 + offset, 695 + offset},
        {469 + offset, 789 + offset},{540 + offset, 789 + offset},{650 + offset, 789 + offset},{765 + offset, 789 + offset},{880 + offset, 789 + offset},{995 + offset, 789 + offset},{1110 + offset, 789 + offset},{1225 + offset, 789 + offset},{1335 + offset, 789 + offset},{1410 + offset, 789 + offset},
        {469 + offset, 895 + offset},{540 + offset, 895 + offset},{650 + offset, 895 + offset},{765 + offset, 895 + offset},{880 + offset, 895 + offset},{995 + offset, 895 + offset},{1110 + offset, 895 + offset},{1225 + offset, 895 + offset},{1335 + offset, 895 + offset},{1410 + offset, 895 + offset},
        {469 + offset, 995 + offset},{540 + offset, 995 + offset},{650 + offset, 995 + offset},{765 + offset, 995 + offset},{880 + offset, 995 + offset},{995 + offset, 995 + offset},{1110 + offset, 995 + offset},{1225 + offset, 995 + offset},{1335 + offset, 995 + offset},{1410 + offset, 995 + offset},
        {540 + offset, 485 + offset},{1335 + offset, 485 + offset}

    };
    pair<int, int> getPos(int &u);
    void addEdge(int u, int v);
    void printAdjList();
    void init();
};
