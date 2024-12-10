/*
    File containing class method implementation for laser_maze.h
    TODO: Find uses for object instances(?)

*/

#include "laser_maze.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <map>
#include <string>

// Class functions

// Helper functions to check if a cell contains a token 
bool GridObject::isObstacle(char cell)
{
    return (cell == '#');
}
bool GridObject::isMirrorForward(char cell)
{
    return (cell == '/');
}
bool GridObject::isMirrorBack(char cell)
{
    return (cell == '\\');
}
bool GridObject::isVertSplit(char cell)
{
    return (cell == '|');
}
bool GridObject::isHoriSplit(char cell)
{
    return (cell == '_');
}
bool GridObject::isTarget(char cell)
{
    return (cell == 'o');
}

// Autosave function to write the current grid to a file
void GridManager::autoSaveGrid(char grid[7][7], const std::string& filename)
{
    std::ofstream output(filename);

    if (!output)
    {
        std::cerr << "ERROR: COULD NOT OPEN AUTOSAVE FILE." << std::endl;
        return;
    }

    for (int row = 0; row < 7; ++row)
    {
        for (int col = 0; col < 7; ++col)
        {
            output << grid[row][col]; // Write the cell value
        }
        output << std::endl; // End of the row
    }

    output.close();
    std::cout << "Grid autosaved to " << filename << "." << std::endl;
}

// Function to print the grid
void GridManager::printGrid(char grid[7][7])
{
    for (int row = 0; row < 7; ++row)
    {
        for (int col = 0; col < 7; ++col)
        {
            std::cout << grid[row][col] << " ";
        }
        std::cout << std::endl;
    }
}


// Function to spawn the beam rightwards
void Beam::spawnBeamRight(char grid[7][7], int& bRow, int& bCol, int& targetsFound)
{
    for (int i = bCol + 1; i < 7; ++i)
    {
        if (GridObject::isObstacle(grid[bRow][i])) break;
        if (GridObject::isTarget(grid[bRow][i])) 
        {
            targetsFound++;
        }
        if (GridObject::isMirrorForward(grid[bRow][i]))
        {
          Beam::spawnBeamUp(grid, bRow, i, targetsFound);
            break;
        }
        if (GridObject::isMirrorBack(grid[bRow][i]))
        {
          Beam::spawnBeamDown(grid, bRow, i, targetsFound);
            break;
        }
        if (GridObject::isHoriSplit(grid[bRow][i]))
        {
          Beam::spawnBeamRight(grid, bRow, i, targetsFound);
        }
        if (GridObject::isVertSplit(grid[bRow][i]))
        {
          Beam::spawnBeamUp(grid, bRow, i, targetsFound);
          Beam::spawnBeamDown(grid, bRow, i, targetsFound);
        }
        if (grid[bRow][i] == '.')
        {
            grid[bRow][i] = '>';
        }
        else break;
    }
}

// Function to spawn the beam leftwards
void  Beam::spawnBeamLeft(char grid[7][7], int& bRow, int& bCol, int& targetsFound)
{
    for (int i = bCol - 1; i >= 0; --i)
    {
        if (GridObject::isObstacle(grid[bRow][i])) break;
        if (GridObject::isTarget(grid[bRow][i])) 
        {
            targetsFound++;
        }
        if (GridObject::isMirrorForward(grid[bRow][i]))
        {
          Beam::spawnBeamDown(grid, bRow, i, targetsFound);
            break;
        }
        if (GridObject::isMirrorBack(grid[bRow][i]))
        {
          Beam::spawnBeamUp(grid, bRow, i, targetsFound);
            break;
        }
        if (GridObject::isHoriSplit(grid[bRow][i]))
        {
          Beam::spawnBeamLeft(grid, bRow, i, targetsFound);
        }
        if (GridObject::isVertSplit(grid[bRow][i]))
        {
          Beam::spawnBeamUp(grid, bRow, i, targetsFound);
          Beam::spawnBeamDown(grid, bRow, i, targetsFound);
        }
        if (grid[bRow][i] == '.')
        {
            grid[bRow][i] = '<';
        }
        else break;
    }
}

// Function to spawn the beam upwards
void  Beam::spawnBeamUp(char grid[7][7], int& bRow, int& bCol, int& targetsFound)
{
    for (int j = bRow - 1; j >= 0; --j)
    {
        if (GridObject::isObstacle(grid[j][bCol])) break;
        if (GridObject::isTarget(grid[j][bCol])) 
        {
            targetsFound++;
        }
        if (GridObject::isMirrorForward(grid[j][bCol]))
        {
            Beam::spawnBeamRight(grid, j, bCol, targetsFound);
            break;
        }
        if (GridObject::isMirrorBack(grid[j][bCol]))
        {
          Beam::spawnBeamLeft(grid, j, bCol, targetsFound);
            break;
        }
        if (GridObject::isHoriSplit(grid[j][bCol]))
        {
          Beam::spawnBeamLeft(grid, j, bCol, targetsFound);
          Beam::spawnBeamRight(grid, j, bCol, targetsFound);        
        }
        if (GridObject::isVertSplit(grid[j][bCol]))
        {
          Beam::spawnBeamUp(grid, j, bCol, targetsFound);
        }
        if (grid[j][bCol] == '.')
        {
            grid[j][bCol] = '^';
        }
        else break;
    }
}

// Function to spawn the beam downwards
void  Beam::spawnBeamDown(char grid[7][7], int& bRow, int& bCol, int& targetsFound)
{
    for (int j = bRow + 1; j < 7; ++j)
    {
        if (GridObject::isObstacle(grid[j][bCol])) break;
        if (GridObject::isTarget(grid[j][bCol])) 
        {
            targetsFound++;
        }
        if (GridObject::isMirrorForward(grid[j][bCol]))
        {
          Beam::spawnBeamLeft(grid, j, bCol, targetsFound);
            break;
        }
        if (GridObject::isMirrorBack(grid[j][bCol]))
        {
          Beam:  Beam::spawnBeamRight(grid, j, bCol, targetsFound);
            break;
        }
        if (GridObject::isHoriSplit(grid[j][bCol]))
        {
          Beam::spawnBeamLeft(grid, j, bCol, targetsFound);
          Beam::spawnBeamRight(grid, j, bCol, targetsFound);        
        }
        if (GridObject::isVertSplit(grid[j][bCol]))
        {
          Beam::spawnBeamDown(grid, j, bCol, targetsFound);
        }
        if (grid[j][bCol] == '.')
        {
            grid[j][bCol] = 'v';
        }
        else break;
    }
}



// Function to scan the first line for tokens and update the inventory
void GridScanner::scanTokens(const std::string& line, std::map<char, int>& tokenInventory)
{
    // Token counting: iterate over each character in the line
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '/' || c == '\\' || c == '|' || c == '_') {
            tokenInventory[c]++; // Increment the count of the token
        }
    }
}



// Function to scan the grid from the file and initialize the beam position
void GridScanner::scanGrid(std::ifstream& input, char grid[7][7], int& bRow, int& bCol, int& totalTargets, std::map<char, int>& tokenInventory)
{
    std::string firstLine;
    getline(input, firstLine); // Read the first line (Add on Tokens line)
    
    // Call scanTokens to count tokens in the first line
    scanTokens(firstLine, tokenInventory);

    // Read the map into the grid and find the beam position
    for (int row = 0; row < 7; ++row)
    {
        std::string line;
        getline(input, line); // Read a line from the file
        for (int col = 0; col < 7; ++col)
        {
            grid[row][col] = line[col]; // Populate the grid

            // Find the beam position
            if (grid[row][col] == 'b')
            {
                bRow = row;
                bCol = col;
            }

            // Count the total targets
            if (GridObject::isTarget(grid[row][col]))
            {
                totalTargets++;
            }
        }
    }
}

// Standalone functions

// Function to place a token and trigger the autosave
void placeToken(char grid[7][7], int bRow, int bCol, std::map<char, int>& tokenInventory) {
    char token;
    int x, y;

    // Display available tokens
    std::cout << "Available tokens: ";
    for (auto& item : tokenInventory) {
        for (int i = 0; i < item.second; ++i) {
            std::cout << item.first << " ";  // Print each token based on its count
        }
    }
    std::cout << std::endl;

    while (true) {
        std::cout << "\nEnter the token you want to place: ";
        std::cin >> token;

        // Clear the input buffer to remove any leftover characters
        if (std::cin.peek() != '\n') {
            std::cout << "Invalid token type. Please enter a single character only." << std::endl;
            std::cin.clear();                   // Clear error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            continue;
        }

        // Validate the token type
        if (!(token == '/' || token == '\\' || token == '_' || token == '|')) {
            std::cout << "Invalid token type. Please choose /, \\ , _, or |." << std::endl;
            continue;
        } 
        else if (tokenInventory[token] <= 0) {
            std::cout << "You don't have this token in your inventory." << std::endl;
            continue;
        } 
        else {
            // Token is valid and available
            break;
        }
    }

    // Validate the coordinates
    while (true) {
        std::string input;
        std::cout << "Enter the row,column coordinates (0-6) where you want to place the token: ";
        std::cin >> input;  // Read the full line of input

        if (!extractCoordinates(input, x, y))
        {
            std::cout << "Error: Invalid coordinate format!" << std::endl;
            continue;
        }

        // Validate the range of coordinates
        if (x < 0 || x > 6 || y < 0 || y > 6) {
            std::cout << "Invalid input. Coordinates must be in the range 0-6." << std::endl;
            continue;
        } 
        else if (x == bRow && y == bCol) {
            std::cout << "Cannot place a token at the beam's current position ('b')." << std::endl;
        }
        else if (grid[x][y] == '#') {
            std::cout << "Cannot place a token on a blocked position ('#')." << std::endl;
        }
        else if (grid[x][y] == '/' || grid[x][y] == '\\' || grid[x][y] == '_' || grid[x][y] == '|') {
            std::cout << "Cannot place a token where another token already exists." << std::endl;
        }
        else {
            // If all conditions pass, valid coordinates
            break;  // Exit the loop
        }

        // If any condition fails, prompt again for input
        std::cin.clear();  // Clear error state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
    }

    // Place the token and update the inventory
    grid[x][y] = token;
    tokenInventory[token]--;

    // Autosave the grid
    GridManager manager; // Create an instance of GridManager
    manager.autoSaveGrid(grid, "autosave.txt");
}

// Function to extract user inputted coordinates
bool extractCoordinates(const std::string& input, int& x, int& y) 
{
    std::stringstream ss(input);
    char comma;
    // Check for comma-separated coordinates
    if (ss >> x >> comma >> y) {
        return true;  // Successfully extracted coordinates
    } 
    // Check for space-separated coordinates (for / and _ mirrors)
    else {
            return false;
        }
}

// Function to check inventory
bool isInventoryEmpty(const std::map<char, int>& tokenInventory)
{
    for (const auto& item : tokenInventory)
    {
        if (item.second > 0)
        {
            return false; // Found at least one token still in inventory
        }
    }
    return true; // All tokens used
}
