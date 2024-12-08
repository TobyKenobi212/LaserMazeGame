#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Prototypes
// Token detection prototypes
bool isObstacle(char cell);
bool isMirrorForward(char cell);
bool isMirrorBack(char cell);
bool isTarget(char cell);
bool isVertSplit(char cell);
bool isHoriSplit(char cell);

// Beam spawning prototypes
void spawnBeamRight(char grid[7][7], int& bRow, int& bCol, int& targetsFound);
void spawnBeamLeft(char grid[7][7], int& bRow, int& bCol, int& targetsFound);
void spawnBeamUp(char grid[7][7], int& bRow, int& bCol, int& targetsFound);
void spawnBeamDown(char grid[7][7], int& bRow, int& bCol, int& targetsFound);

// Print grid function prototype
void printGrid(char grid[7][7]);

// Grid scanning function prototype
void scanGrid(ifstream& input, char grid[7][7], int& bRow, int& bCol, int& totalTargets);

// Functions

// Helper functions to check if a cell contains a token 
bool isObstacle(char cell)
{
    return (cell == '#');
}
bool isMirrorForward(char cell)
{
    return (cell == '/');
}
bool isMirrorBack(char cell)
{
    return (cell == '\\');
}
bool isVertSplit(char cell)
{
    return (cell == '|');
}
bool isHoriSplit(char cell)
{
    return (cell == '_');
}
bool isTarget(char cell)
{
    return (cell == 'o');
}

// Function to spawn the beam rightwards
void spawnBeamRight(char grid[7][7], int& bRow, int& bCol, int& targetsFound)
{
    for (int i = bCol + 1; i < 7; ++i)
    {
        if (isObstacle(grid[bRow][i])) break;
        if (isTarget(grid[bRow][i])) 
        {
            targetsFound++;
        }
        if (isMirrorForward(grid[bRow][i]))
        {
            spawnBeamUp(grid, bRow, i, targetsFound);
            break;
        }
        if (isMirrorBack(grid[bRow][i]))
        {
            spawnBeamDown(grid, bRow, i, targetsFound);
            break;
        }
        if (isHoriSplit(grid[bRow][i]))
        {
            spawnBeamRight(grid, bRow, i, targetsFound);
        }
        if (isVertSplit(grid[bRow][i]))
        {
            spawnBeamUp(grid, bRow, i, targetsFound);
            spawnBeamDown(grid, bRow, i, targetsFound);
        }
        if (grid[bRow][i] == '.')
        {
            grid[bRow][i] = '>';
        }
        else break;
    }
}

// Function to spawn the beam leftwards
void spawnBeamLeft(char grid[7][7], int& bRow, int& bCol, int& targetsFound)
{
    for (int i = bCol - 1; i >= 0; --i)
    {
        if (isObstacle(grid[bRow][i])) break;
        if (isTarget(grid[bRow][i])) 
        {
            targetsFound++;
        }
        if (isMirrorForward(grid[bRow][i]))
        {
            spawnBeamDown(grid, bRow, i, targetsFound);
            break;
        }
        if (isMirrorBack(grid[bRow][i]))
        {
            spawnBeamUp(grid, bRow, i, targetsFound);
            break;
        }
        if (isHoriSplit(grid[bRow][i]))
        {
            spawnBeamLeft(grid, bRow, i, targetsFound);
        }
        if (isVertSplit(grid[bRow][i]))
        {
            spawnBeamUp(grid, bRow, i, targetsFound);
            spawnBeamDown(grid, bRow, i, targetsFound);
        }
        if (grid[bRow][i] == '.')
        {
            grid[bRow][i] = '<';
        }
        else break;
    }
}

// Function to spawn the beam upwards
void spawnBeamUp(char grid[7][7], int& bRow, int& bCol, int& targetsFound)
{
    for (int j = bRow - 1; j >= 0; --j)
    {
        if (isObstacle(grid[j][bCol])) break;
        if (isTarget(grid[j][bCol])) 
        {
            targetsFound++;
        }
        if (isMirrorForward(grid[j][bCol]))
        {
            spawnBeamRight(grid, j, bCol, targetsFound);
            break;
        }
        if (isMirrorBack(grid[j][bCol]))
        {
            spawnBeamLeft(grid, j, bCol, targetsFound);
            break;
        }
        if (isHoriSplit(grid[j][bCol]))
        {
            spawnBeamLeft(grid, j, bCol, targetsFound);
            spawnBeamRight(grid, j, bCol, targetsFound);        
        }
        if (isVertSplit(grid[j][bCol]))
        {
            spawnBeamUp(grid, j, bCol, targetsFound);
        }
        if (grid[j][bCol] == '.')
        {
            grid[j][bCol] = '^';
        }
        else break;
    }
}

// Function to spawn the beam downwards
void spawnBeamDown(char grid[7][7], int& bRow, int& bCol, int& targetsFound)
{
    for (int j = bRow + 1; j < 7; ++j)
    {
        if (isObstacle(grid[j][bCol])) break;
        if (isTarget(grid[j][bCol])) 
        {
            targetsFound++;
        }
        if (isMirrorForward(grid[j][bCol]))
        {
            spawnBeamLeft(grid, j, bCol, targetsFound);
            break;
        }
        if (isMirrorBack(grid[j][bCol]))
        {
            spawnBeamRight(grid, j, bCol, targetsFound);
            break;
        }
        if (isHoriSplit(grid[j][bCol]))
        {
            spawnBeamLeft(grid, j, bCol, targetsFound);
            spawnBeamRight(grid, j, bCol, targetsFound);        
        }
        if (isVertSplit(grid[j][bCol]))
        {
            spawnBeamDown(grid, j, bCol, targetsFound);
        }
        if (grid[j][bCol] == '.')
        {
            grid[j][bCol] = 'v';
        }
        else break;
    }
}

// Function to print the grid
void printGrid(char grid[7][7])
{
    for (int row = 0; row < 7; ++row)
    {
        for (int col = 0; col < 7; ++col)
        {
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
}

// Function to scan the grid from the file and initialize the beam position
void scanGrid(ifstream& input, char grid[7][7], int& bRow, int& bCol, int& totalTargets)
{
    // Read the map into the grid and find the beam position
    for (int row = 0; row < 7; ++row)
    {
        string line;
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
            if (isTarget(grid[row][col]))
            {
                totalTargets++;
            }
        }
    }
}

// Main function
int main()
{
    ifstream input;
    int bRow = 0, bCol = 0;
    int targetsFound = 0;
    int totalTargets = 0;
    char grid[7][7]; // 7x7 grid

    // Open the map.txt file
    input.open("map.txt");
    if (!input)
    {
        cerr << "ERROR: COULD NOT OPEN FILE." << endl;
        return 1;
    }

    // Scan the grid and initialize the variables
    scanGrid(input, grid, bRow, bCol, totalTargets);
    input.close(); // Close the input file

    // Print the initial grid
    cout << "The map from the text file:" << endl;
    printGrid(grid);

    // Spawn the beam in all directions
    spawnBeamRight(grid, bRow, bCol, targetsFound);
    spawnBeamLeft(grid, bRow, bCol, targetsFound);
    spawnBeamUp(grid, bRow, bCol, targetsFound);
    spawnBeamDown(grid, bRow, bCol, targetsFound);

    // Print the final grid
    cout << "\nThe result:" << endl;
    printGrid(grid);

    // Print success message
    if (targetsFound == totalTargets)
    {
        cout << "Success!" << endl;
    }
    else
    {
        cout << "Not all targets were hit!" << endl;
    }

    return 0;
}
