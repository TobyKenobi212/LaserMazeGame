/*
    Header file that contains classes for the game logic.
    Function prototypes only.
    Standalone non-class functions also included
    TODO: Some members could be private


*/

#ifndef LASER_MAZE_H
#define LASER_MAZE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <sstream>

// Utility functions to check the properties of grid cells
class GridObject {
public:
    // Checks if the cell is an obstacle
    static bool isObstacle(char cell);

    // Checks if the cell contains a forward-slash mirror ('/')
    static bool isMirrorForward(char cell);

    // Checks if the cell contains a backslash mirror ('\')
    static bool isMirrorBack(char cell);

    // Checks if the cell is a target ('T')
    static bool isTarget(char cell);

    // Checks if the cell contains a vertical split ('|')
    static bool isVertSplit(char cell);

    // Checks if the cell contains a horizontal split ('_')
    static bool isHoriSplit(char cell);
};

// Functions to handle scanning the grid and token inventory
class GridScanner {
public:
    // Reads grid configuration and initializes variables
    static void scanGrid(
        std::ifstream& input, 
        char grid[7][7], 
        int& bRow, 
        int& bCol, 
        int& totalTargets, 
        std::map<char, int>& tokenInventory
    );

    // Processes a line containing token information and updates the inventory
    static void scanTokens(
        const std::string& line, 
        std::map<char, int>& tokenInventory
    );
};

// Functions to handle beam movement in different directions
class Beam {
public:
    // Spawns a beam moving to the right
    static void spawnBeamRight(
        char grid[7][7], 
        int& bRow, 
        int& bCol, 
        int& targetsFound
    );

    // Spawns a beam moving to the left
    static void spawnBeamLeft(
        char grid[7][7], 
        int& bRow, 
        int& bCol, 
        int& targetsFound
    );

    // Spawns a beam moving upward
    static void spawnBeamUp(
        char grid[7][7], 
        int& bRow, 
        int& bCol, 
        int& targetsFound
    );

    // Spawns a beam moving downward
    static void spawnBeamDown(
        char grid[7][7], 
        int& bRow, 
        int& bCol, 
        int& targetsFound
    );
};

// Functions to handle grid management
class GridManager {
public:
    // Prints the current state of the grid
    static void printGrid(char grid[7][7]);

    // Saves the current state of the grid to a file
    void autoSaveGrid(
        char grid[7][7], 
        const std::string& filename
    );
};


// Standalone functions

// Function for placing tokens on the grid
void placeToken(char grid[7][7], int bRow, int bCol, std::map<char, int>& tokenInventory, int& targetsFound);

// Function for parsing user input into grid coordinates
bool extractCoordinates(const std::string& input, int& x, int& y);

// Function to check if the token inventory is empty
bool isInventoryEmpty(const std::map<char, int>& tokenInventory);


// Play level function
bool playGame(const std::string& difficulty, char choice);



#endif // LASER_MAZE_H
