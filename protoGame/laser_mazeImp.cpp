/*
    File containing class method implementation for laser_maze.h
    Also includes some standalone function implementation
*/
#include "laser_maze.h"
#include "menu.h"
using namespace std;

// Class functions

// Helper functions to check if a cell contains a token 
bool GridObject::isObstacle(char cell) {
    return (cell == '#');
}

bool GridObject::isMirrorForward(char cell) {
    return (cell == '/');
}

bool GridObject::isMirrorBack(char cell) {
    return (cell == '\\');
}

bool GridObject::isVertSplit(char cell) {
    return (cell == '|');
}

bool GridObject::isHoriSplit(char cell) {
    return (cell == '_');
}

bool GridObject::isTarget(char cell) {
    return (cell == 'o');
}

// Autosave function to write the current grid to a file
void GridManager::autoSaveGrid(char grid[7][7], const string& filename) {
    ofstream output(filename);

    if (!output) {
        cerr << "ERROR: COULD NOT OPEN AUTOSAVE FILE." << endl;
        return;
    }

    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            output << grid[row][col]; // Write the cell value
        }
        output << endl; // End of the row
    }

    output.close();
    cout << "Grid autosaved to " << filename << "." << endl;
}

// Function to print the grid
void GridManager::printGrid(char grid[7][7]) {
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
}

// Function to spawn the beam rightwards
void Beam::spawnBeamRight(char grid[7][7], int& bRow, int& bCol, int& targetsFound) {
    for (int i = bCol + 1; i < 7; ++i) {
        if (GridObject::isObstacle(grid[bRow][i])) break;
        if (GridObject::isTarget(grid[bRow][i])) {
            targetsFound++;
        }
        if (GridObject::isMirrorForward(grid[bRow][i])) {
            Beam::spawnBeamUp(grid, bRow, i, targetsFound);
            break;
        }
        if (GridObject::isMirrorBack(grid[bRow][i])) {
            Beam::spawnBeamDown(grid, bRow, i, targetsFound);
            break;
        }
        if (GridObject::isHoriSplit(grid[bRow][i])) {
            Beam::spawnBeamRight(grid, bRow, i, targetsFound);
        }
        if (GridObject::isVertSplit(grid[bRow][i])) {
            Beam::spawnBeamUp(grid, bRow, i, targetsFound);
            Beam::spawnBeamDown(grid, bRow, i, targetsFound);
        }
        if (grid[bRow][i] == '.') {
            grid[bRow][i] = '>';
        } else break;
    }
}

// Function to spawn the beam leftwards
void Beam::spawnBeamLeft(char grid[7][7], int& bRow, int& bCol, int& targetsFound) {
    for (int i = bCol - 1; i >= 0; --i) {
        if (GridObject::isObstacle(grid[bRow][i])) break;
        if (GridObject::isTarget(grid[bRow][i])) {
            targetsFound++;
        }
        if (GridObject::isMirrorForward(grid[bRow][i])) {
            Beam::spawnBeamDown(grid, bRow, i, targetsFound);
            break;
        }
        if (GridObject::isMirrorBack(grid[bRow][i])) {
            Beam::spawnBeamUp(grid, bRow, i, targetsFound);
            break;
        }
        if (GridObject::isHoriSplit(grid[bRow][i])) {
            Beam::spawnBeamLeft(grid, bRow, i, targetsFound);
        }
        if (GridObject::isVertSplit(grid[bRow][i])) {
            Beam::spawnBeamUp(grid, bRow, i, targetsFound);
            Beam::spawnBeamDown(grid, bRow, i, targetsFound);
        }
        if (grid[bRow][i] == '.') {
            grid[bRow][i] = '<';
        } else break;
    }
}

// Function to spawn the beam upwards
void Beam::spawnBeamUp(char grid[7][7], int& bRow, int& bCol, int& targetsFound) {
    for (int j = bRow - 1; j >= 0; --j) {
        if (GridObject::isObstacle(grid[j][bCol])) break;
        if (GridObject::isTarget(grid[j][bCol])) {
            targetsFound++;
        }
        if (GridObject::isMirrorForward(grid[j][bCol])) {
            Beam::spawnBeamRight(grid, j, bCol, targetsFound);
            break;
        }
        if (GridObject::isMirrorBack(grid[j][bCol])) {
            Beam::spawnBeamLeft(grid, j, bCol, targetsFound);
            break;
        }
        if (GridObject::isHoriSplit(grid[j][bCol])) {
            Beam::spawnBeamLeft(grid, j, bCol, targetsFound);
            Beam::spawnBeamRight(grid, j, bCol, targetsFound);        
        }
        if (GridObject::isVertSplit(grid[j][bCol])) {
            Beam::spawnBeamUp(grid, j, bCol, targetsFound);
        }
        if (grid[j][bCol] == '.') {
            grid[j][bCol] = '^';
        } else break;
    }
}

// Function to spawn the beam downwards
void Beam::spawnBeamDown(char grid[7][7], int& bRow, int& bCol, int& targetsFound) {
    for (int j = bRow + 1; j < 7; ++j) {
        if (GridObject::isObstacle(grid[j][bCol])) break;
        if (GridObject::isTarget(grid[j][bCol])) {
            targetsFound++;
        }
        if (GridObject::isMirrorForward(grid[j][bCol])) {
            Beam::spawnBeamLeft(grid, j, bCol, targetsFound);
            break;
        }
        if (GridObject::isMirrorBack(grid[j][bCol])) {
            Beam::spawnBeamRight(grid, j, bCol, targetsFound);
            break;
        }
        if (GridObject::isHoriSplit(grid[j][bCol])) {
            Beam::spawnBeamLeft(grid, j, bCol, targetsFound);
            Beam::spawnBeamRight(grid, j, bCol, targetsFound);        
        }
        if (GridObject::isVertSplit(grid[j][bCol])) {
            Beam::spawnBeamDown(grid, j, bCol, targetsFound);
        }
        if (grid[j][bCol] == '.') {
            grid[j][bCol] = 'v';
        } else break;
    }
}

// Function to scan the first line for tokens and update the inventory
void GridScanner::scanTokens(const string& line, map<char, int>& tokenInventory) {
    // Token counting: iterate over each character in the line
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '/' || c == '\\' || c == '|' || c == '_') {
            tokenInventory[c]++; // Increment the count of the token
        }
    }
}

// Function to scan the grid from the file and initialize the beam position
void GridScanner::scanGrid(ifstream& input, char grid[7][7], int& bRow, int& bCol, int& totalTargets, map<char, int>& tokenInventory) {
    string firstLine;
    getline(input, firstLine); // Read the first line (Add on Tokens line)
    
    // Call scanTokens to count tokens in the first line
    scanTokens(firstLine, tokenInventory);

    // Read the map into the grid and find the beam position
    for (int row = 0; row < 7; ++row) {
        string line;
        getline(input, line); // Read a line from the file
        for (int col = 0; col < 7; ++col) {
            grid[row][col] = line[col]; // Populate the grid

            // Find the beam position
            if (grid[row][col] == 'b') {
                bRow = row;
                bCol = col;
            }

            // Count the total targets
            if (GridObject::isTarget(grid[row][col])) {
                totalTargets++;
            }
        }
    }
}

// Standalone functions

// Function to place a token and trigger the autosave
bool placeToken(char grid[7][7], int bRow, int bCol, map<char, int>& tokenInventory, int& targetsFound, Player& player) {
    char token;
    int x, y;

    // Display available tokens
    cout << "\nAvailable tokens: ";
    for (auto& item : tokenInventory) {
        for (int i = 0; i < item.second; ++i) {
            cout << item.first << " ";  // Print each token based on its count
        }
    }
    cout << endl;

    while (true) {
        cout << "\nEnter the token you want to place, or type E to exit: ";
        // Read the input
        string input;
        getline(cin, input);

        // Trim leading and trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        // Check if input is exactly 'e' or 'E'
        if (input == "e" || input == "E") {
            cout << "Exiting to main menu...\n";
            return true; // Indicate that the player chose to exit
        }

        // If input is empty, prompt again
        if (input.empty()) {
            cout << "Invalid input. Please try again.\n";
            continue;
        }

        // Use the first character as the token
        char token = input[0];
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
        cout << "Enter the row,column coordinates (1-7) where you want to place the token: ";
        cin >> input;  // Read the full line of input

        if (!extractCoordinates(input, x, y)) {
            cout << "Error: Invalid coordinate format!" << endl;
            continue;
        }

        // Validate the range of coordinates
        if (x <= 0 || y <= 0 || x > 7 || y > 7) {
            cout << "Invalid input. Coordinates must be in the range 1-7." << endl;
            continue;
        } 
        else if (x == bRow && y == bCol) {
            cout << "Cannot place a token at the beam's current position ('b')." << endl;
        }
        else if (grid[x-1][y-1] == '#') {
            cout << "Cannot place a token on a blocked position ('#')." << endl;
        }
        else if (grid[x-1][y-1] == 'o') {
            cout << "Cannot place a token on a target ('o')." << endl;
        }
        else if (grid[x-1][y-1] == 'b') {
            cout << "Cannot place a token on laser position ('b')." << endl;
        }
        else if (grid[x-1][y-1] == '/' || grid[x-1][y-1] == '\\' || grid[x-1][y-1] == '_' || grid[x-1][y-1] == '|') {
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
    grid[x-1][y-1] = token;
    tokenInventory[token]--;

    return false; // Indicate that the player did not choose to exit
}

// Function to extract user inputted coordinates
bool extractCoordinates(const string& input, int& x, int& y) {
    stringstream ss(input);
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
bool isInventoryEmpty(const map<char, int>& tokenInventory) {
    for (const auto& item : tokenInventory) {
        if (item.second > 0) {
            return false; // Found at least one token still in inventory
        }
    }
    return true; // All tokens used
}

bool playGame(Player& player, const string& difficulty, char choice) {
    int difficultyIndex = (difficulty == "Easy") ? 0 : (difficulty == "Medium") ? 1 : 2;
    int levelIndex = (choice == '1') ? 0 : 1;
    while (true) {
        // Game logic setup
        ifstream input;
        int bRow = 0, bCol = 0;
        int targetsFound = 0;
        int totalTargets = 0;
        char grid[7][7]; // 7x7 grid
        string username = player.getUsername();
        // Token inventory setup
        map<char, int> tokenInventory;
        // Open the map.txt file
        input.open(difficulty + choice + ".txt");
        if (!input) {
            cerr << "ERROR: COULD NOT OPEN FILE." << endl;
            return false;
        }
        // Scan the grid and initialize the variables using the GridScanner class
        GridScanner::scanGrid(input, grid, bRow, bCol, totalTargets, tokenInventory);
        input.close(); // Close the input file
        // Print the initial grid using the GridManager class
        cout << "The map from the text file:" << endl;
        GridManager::printGrid(grid);
        player.printLives(); // Print lives
        // Allow the user to place tokens from inventory using the TokenPlacer class
        while (!isInventoryEmpty(tokenInventory)) {
            if (placeToken(grid, bRow, bCol, tokenInventory, targetsFound, player)) {
                return false; // Exit to main menu if the player chose to exit
            }
            // Print the grid after placing tokens
            GridManager::printGrid(grid);
        }
        // Spawn the beams in all directions using Beam class
        Beam::spawnBeamRight(grid, bRow, bCol, targetsFound);
        Beam::spawnBeamLeft(grid, bRow, bCol, targetsFound);
        Beam::spawnBeamUp(grid, bRow, bCol, targetsFound);
        Beam::spawnBeamDown(grid, bRow, bCol, targetsFound);
        // Print the final grid
        cout << "\nThe result:" << endl;
        GridManager::printGrid(grid);
        // Print success message
        if (targetsFound == totalTargets) {
            cout << "Success!" << endl;
            remove(("autosaves/" + username + "_autosave.txt").c_str());
            int score = player.getLives() * 100;
            cout << "Score: " << score << " pts" << endl;
            player.updateHighestScore(difficultyIndex, levelIndex, score);
            cout << "Highest Score Earned: " << player.getHighestScore(difficultyIndex, levelIndex) << " pts" << endl;
            return true;
        } else {
            cout << "Not all targets were hit!" << endl;
            player.loseLife();
            player.autosave();
            if (player.isOutOfLives()) {
                cout << "You have lost all your lives. Returning to the main menu..." << endl;
                remove(("autosaves/" + username + "_autosave.txt").c_str());
                cout << "Highest Score Earned: " << player.getHighestScore(difficultyIndex, levelIndex) << " pts" << endl;
                return false;
            }
            cout << "Try again!\n";
        }
    }
}
