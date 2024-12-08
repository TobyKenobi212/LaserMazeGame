/*
    To use with map.txt
    Still needs save/reset implementation



*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>  // To track token inventory
#include <limits>
#include <sstream>

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
void scanGrid(ifstream& input, char grid[7][7], int& bRow, int& bCol, int& totalTargets, map<char, int>& tokenInventory);
void scanTokens(const string& line, map<char, int>& tokenInventory);


// Token placement prototype
void placeToken(char grid[7][7], int bRow, int bCol, map<char, int>& tokenInventory);

// Coordinate extraction prototype
void extractCoordinates(const string& input, int& x, int& y);



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

// Function to prompt the user for a token and coordinates, then place the token
void placeToken(char grid[7][7], int bRow, int bCol, map<char, int>& tokenInventory) {
    char token;
    int x, y;

    // Display available tokens
    cout << "Available tokens: ";
    for (auto& item : tokenInventory) {
        for (int i = 0; i < item.second; ++i) {
            cout << item.first << " ";  // Print each token based on its count
        }
    }
    cout << endl;

    while (true) {
        cout << "\nEnter the token you want to place: ";
        cin >> token;

        // Clear the input buffer to remove any leftover characters
        if (cin.peek() != '\n') {
            cout << "Invalid token type. Please enter a single character only." << endl;
            cin.clear();                   // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            continue;
        }

        // Validate the token type
        if (!(token == '/' || token == '\\' || token == '_' || token == '|')) {
            cout << "Invalid token type. Please choose /, \\ , _, or |." << endl;
            continue;
        } 
        else if (tokenInventory[token] <= 0) {
            cout << "You don't have this token in your inventory." << endl;
            continue;
        } 
        else {
            // Token is valid and available
            break;
        }
    }

    // Validate the coordinates
    while (true) {
        string input;
        cout << "Enter the row,column coordinates (0-6) where you want to place the token: ";
        cin >> input;  // Read the full line of input

        extractCoordinates(input, x, y);

        // Validate the range of coordinates
        if (x < 0 || x > 6 || y < 0 || y > 6) {
            cout << "Invalid input. Coordinates must be in the range 0-6." << endl;
            continue;
        } 
        else if (x == bRow && y == bCol) {
            cout << "Cannot place a token at the beam's current position ('b')." << endl;
        }
        else if (grid[x][y] == '#') {
            cout << "Cannot place a token on a blocked position ('#')." << endl;
        }
        else if (grid[x][y] == '/' || grid[x][y] == '\\' || grid[x][y] == '_' || grid[x][y] == '|') {
            cout << "Cannot place a token where another token already exists." << endl;
        }
        else {
            // If all conditions pass, valid coordinates
            break;  // Exit the loop
        }

        // If any condition fails, prompt again for input
        cin.clear();  // Clear error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
    }

    // Place the token and update the inventory
    grid[x][y] = token;
    tokenInventory[token]--;
}

// Function to extract user inputted coordinates
void extractCoordinates(const string& input, int& x, int& y) {
    stringstream ss(input);
    char comma;
    // Check for comma-separated coordinates
    if (ss >> x >> comma >> y) {
        return;  // Successfully extracted coordinates
    } 
    // Check for space-separated coordinates (for / and _ mirrors)
    else {
        stringstream ss2(input);
        if (ss2 >> x >> y) {
            return;
        }
    }
    cerr << "Error: Invalid coordinate format!" << endl;
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

// Function to scan the first line for tokens and update the inventory
void scanTokens(const string& line, map<char, int>& tokenInventory)
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
void scanGrid(ifstream& input, char grid[7][7], int& bRow, int& bCol, int& totalTargets, map<char, int>& tokenInventory)
{
    string firstLine;
    getline(input, firstLine); // Read the first line (Add on Tokens line)
    
    // Call scanTokens to count tokens in the first line
    scanTokens(firstLine, tokenInventory);

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

    // Token inventory setup
    map<char, int> tokenInventory;

    // Open the map.txt file
    input.open("map.txt");
    if (!input)
    {
        cerr << "ERROR: COULD NOT OPEN FILE." << endl;
        return 1;
    }

    // Scan the grid and initialize the variables
    scanGrid(input, grid, bRow, bCol, totalTargets, tokenInventory);
    input.close(); // Close the input file

    // Print the initial grid
    cout << "The map from the text file:" << endl;
    printGrid(grid);

    // Allow the user to place tokens from inventory
    while (true)
    {
        placeToken(grid, bRow, bCol, tokenInventory);

        // Check if inventory is empty
        bool allUsed = true;
        for (auto& item : tokenInventory)
        {
            if (item.second > 0)
            {
                allUsed = false;
                break;
            }
        }

        if (allUsed)
            break;
            printGrid(grid);

    }

    // Spawn the beams in all directions
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
