/*
    To use with map.txt
    Still needs save/reset implementation



*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>  // To track token inventory

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
void placeToken(char grid[7][7], int bRow, int bCol, map<char, int>& tokenInventory)
{
    char token;
    int x, y;

    // List of available tokens
    cout << "Available tokens: / \\ | _" << endl;
    cout << "Your current inventory:" << endl;
    for (auto& item : tokenInventory) {
        cout << item.first << ": " << item.second << endl;
    }

    cout << "Enter the token you want to place: ";
    cin >> token;

// Validate the token input
    while (true) 
    {
        // Check if the token exists in the inventory
       
        // Check if the token is one of the valid options
        if (token != '/' && token != '\\' && token != '|' && token != '_') 
        {
            cout << "Invalid token. Available tokens are: / \\ | _" << endl;    
        }
         else if (tokenInventory[token] <= 0) 
        {
            cout << "You don't have this token in your inventory." << endl;
        }
        else 
        {
            // Token is valid and available in inventory
            break;  // Exit the loop
        }
        
        // Prompt user to re-enter token
        cout << "Enter the token you want to place: ";
        cin >> token;
    }


    // Get the coordinates from the user in x,y format
    cout << "Enter the x,y coordinates (0-6) where you want to place the token: ";
    cin >> x;
    cin.ignore(); // To ignore the ',' separator
    cin >> y;

    // Validate the coordinates input
    while (x < 0 || x > 6 || y < 0 || y > 6 || (x == bRow && y == bCol))
    {
        if (x < 0 || x > 6 || y < 0 || y > 6)
        {
            cout << "Invalid coordinates. Please enter values between 0 and 6." << endl;
        }
        else if (x == bRow && y == bCol)
        {
            cout << "Cannot place a token at the beam's current position ('b')." << endl;
        }
        cout << "Enter the x,y coordinates (0-6) where you want to place the token: ";
        cin >> x;
        cin.ignore(); // To ignore the ',' separator
        cin >> y;
    }

    // Place the token at the specified coordinates
    grid[x][y] = token;
    tokenInventory[token]--; // Decrease token count
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
