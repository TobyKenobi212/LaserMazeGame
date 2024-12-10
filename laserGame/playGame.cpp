/*
    File to test laser classes.
    To be used in conjunction with laser_maze.h and laser_mazeImp.cpp
    Also use map.txt (temporary)
    TODO: Integrate main menu files 

*/

#include "laser_maze.h"
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main()
{
    // Game logic setup
    std::ifstream input;
    int bRow = 0, bCol = 0;
    int targetsFound = 0;
    int totalTargets = 0;
    char grid[7][7]; // 7x7 grid

    // Token inventory setup
    std::map<char, int> tokenInventory;

    // Open the map.txt file
    input.open("map.txt");
    if (!input)
    {
        std::cerr << "ERROR: COULD NOT OPEN FILE." << std::endl;
        return 1;
    }

    // Scan the grid and initialize the variables using the GridScanner class
    GridScanner::scanGrid(input, grid, bRow, bCol, totalTargets, tokenInventory);
    input.close(); // Close the input file

    // Print the initial grid using the GridManager class
    std::cout << "The map from the text file:" << std::endl;
    GridManager::printGrid(grid);

    // Allow the user to place tokens from inventory using the TokenPlacer class
    while (!isInventoryEmpty(tokenInventory))
    {
        placeToken(grid, bRow, bCol, tokenInventory);

        // Print the grid after placing tokens
        GridManager::printGrid(grid);
    }

    // Spawn the beams in all directions using Beam class
    Beam::spawnBeamRight(grid, bRow, bCol, targetsFound);
    Beam::spawnBeamLeft(grid, bRow, bCol, targetsFound);
    Beam::spawnBeamUp(grid, bRow, bCol, targetsFound);
    Beam::spawnBeamDown(grid, bRow, bCol, targetsFound);

    // Print the final grid
    std::cout << "\nThe result:" << std::endl;
    GridManager::printGrid(grid);

    // Print success message
    if (targetsFound == totalTargets)
    {
        std::cout << "Success!" << std::endl;
    }
    else
    {
        std::cout << "Not all targets were hit!" << std::endl;
    }

    return 0;
}