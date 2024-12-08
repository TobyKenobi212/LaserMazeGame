/*    
    To test the file you would need to create "easy.txt" and "easyTemp.txt" and input both ONLY the coordinates
    from the "Easy Layout and Directions.txt"
    
    laser beam: done 
    reflected laser: only for mirrors 
    user menu: only 4 options for now
              1. Place Mirror (/ or \)
              2. Place Beam Splitter (_ or |)
              3. Reset Level
              4. Exit and save maze 
    read/write maze grid : done (need to read the Easy Layout and Directions .txt to know specific lines are being read/write)
    save/reset maze grid: default maze grid is at easy.txt, every modification of the maze like adding mirors/splitters is at easyTemp.txt,
                          easy.txt will never change! When the user reset the maze grid, the value of easy.txt would replaced the easyTemp.txt
    mirrors (/) and (\) : done
    beam splitter (_) and (|) : done
    target (o): not done
    Need to integrate into the Classes files: still on it
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <queue>

using namespace std;

// Function to print the Laser Maze grid to the terminal
void printGrid(const vector<vector<char>>& grid) {
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;  // Add a newline after the grid display
}

// Function to write the updated maze grid to a temporary file (easyTemp.txt)
void writeGridToTempFile(const vector<vector<char>>& grid, const string& filename, 
                         const vector<pair<int, int>>& mirrors, const vector<pair<int, int>>& beamSplitters) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }
    vector<string> lines;
    string line;

    // Read all lines from the file into the vector
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    // Update the beam splitters and mirrors section in the file
    int beamSplitterIndex = 9;  // Beam splitters are at lines 10 and 11
    int mirrorIndex = 11;       // Mirrors are at lines 12 to 17

    // Write the updated grid size and laser position
    lines[0] = to_string(grid.size()) + "x" + to_string(grid[0].size());
    bool laserFound = false;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == 'b') {
                lines[1] = to_string(i + 1) + "," + to_string(j + 1);
                laserFound = true;
                break;
            }
        }
        if (laserFound) break;
    }

    // Write the beam splitters section (up to 2 beam splitters)
    for (int i = 0; i < beamSplitters.size(); ++i) {
        // If the beam splitter is '_', write with a space between coordinates
        if (grid[beamSplitters[i].first - 1][beamSplitters[i].second - 1] == '_') {
            lines[beamSplitterIndex + i] = to_string(beamSplitters[i].first) + " " + to_string(beamSplitters[i].second);
        } 
        // If the beam splitter is '|', write with a comma between coordinates
        else if (grid[beamSplitters[i].first - 1][beamSplitters[i].second - 1] == '|') {
            lines[beamSplitterIndex + i] = to_string(beamSplitters[i].first) + "," + to_string(beamSplitters[i].second);
        }
    }

    // Write the mirrors section (up to 6 mirrors)
    for (int i = 0; i < mirrors.size(); ++i) {
        // If the mirror is a forward slash '/', write with a space between coordinates
        if (grid[mirrors[i].first - 1][mirrors[i].second - 1] == '/') {
            lines[mirrorIndex + i] = to_string(mirrors[i].first) + " " + to_string(mirrors[i].second);
        } 
        // If the mirror is a backslash '\', write with a comma between coordinates
        else if (grid[mirrors[i].first - 1][mirrors[i].second - 1] == '\\') {
            lines[mirrorIndex + i] = to_string(mirrors[i].first) + "," + to_string(mirrors[i].second);
        }
    }

    // Write all the modified lines back to the file
    ofstream outputFile(filename);
    for (const string& modifiedLine : lines) {
        outputFile << modifiedLine << endl;
    }

    outputFile.close();
}

void shootLaser(vector<vector<char>>& grid, int laserX, int laserY) {
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false)); // Track visited cells
    queue<pair<pair<int, int>, pair<int, int>>> q; // ((x, y), direction)

    // Laser starts shooting in all four directions (0: right, 1: down, 2: left, 3: up)
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    vector<char> directionSymbols = {'>', 'V', '<', '^'}; // Corresponding symbols for directions

    for (int i = 0; i < 4; ++i) {
        q.push({{laserX, laserY}, directions[i]});
    }

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        int x = current.first.first;
        int y = current.first.second;
        int dx = current.second.first;
        int dy = current.second.second;

        // Move the laser
        x += dx;
        y += dy;

        // Stop if out of bounds
        if (x < 0 || x >= rows || y < 0 || y >= cols) continue;

        // Stop if the cell has been visited in this direction
        if (visited[x][y]) continue;
        visited[x][y] = true; // Mark the cell as visited

        // Handle different grid elements
        if (grid[x][y] == '#' || grid[x][y] == 'b') {
            // Blocker stops the laser (laser doesn't move forward in this direction)
            continue;
        } else if (grid[x][y] == 'o') {
            // Target accepts the laser (but laser continues in the same direction)
        } else if (grid[x][y] == '/') {
            // Forward slash mirror reflects the laser
            if (dx == -1 && dy == 0) {  // Laser is moving upward '^' (dx = -1)
                // Clear the laser path moving upward
                int clearX = x - 1, clearY = y;
                while (clearX >= 0 && grid[clearX][clearY] == '^') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearX--;
                }
                // Reflect the laser to the right '>'
                q.push({{x, y}, {0, 1}}); // Reflect to the right
            } else if (dx == 0 && dy == 1) { // Laser is moving right '>'
                int clearX = x, clearY = y + 1;
                while (clearY < grid[0].size() && grid[clearX][clearY] == '>') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearY++;
                }
                // Handle rightward laser reflection 
                q.push({{x, y}, {-1, 0}});  // Reflect to upward '^'
            } else if (dx == 1 && dy == 0) { // Laser is moving downward 'V'
                int clearX = x + 1, clearY = y;
                while (clearX < grid.size() && grid[clearX][clearY] == 'V') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearX++;
                }
                // Handle downward laser reflection 
                q.push({{x, y}, {0, -1}});  // Reflect to left '<'
            } else if (dx == 0 && dy == -1) { // Laser is moving left '<'
                int clearX = x, clearY = y - 1;
                while (clearY >= 0 && grid[clearX][clearY] == '<') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearY--;
                }
                // Handle leftward laser reflection 
                q.push({{x, y}, {1, 0}});   // Reflect to downward 'V'
            }
            continue;
        } else if (grid[x][y] == '\\') {
            // Backslash mirror reflects the laser
            if (dx == 0 && dy == 1) { // Laser is moving right '>'
                int clearX = x, clearY = y + 1;
                while (clearY < grid[0].size() && grid[clearX][clearY] == '>') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearY++;
                }
                q.push({{x, y}, {1, 0}});   // Reflect to downward 'V'
            } else if (dx == 0 && dy == -1) { // Laser is moving left '<'
                int clearX = x, clearY = y - 1;
                while (clearY >= 0 && grid[clearX][clearY] == '<') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearY--;
                }
                q.push({{x, y}, {-1, 0}});  // Reflect to upward '^'
            } else if (dx == -1 && dy == 0) {  // Laser is moving upward '^'
                int clearX = x - 1, clearY = y;
                while (clearX >= 0 && grid[clearX][clearY] == '^') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearX--;
                }
                q.push({{x, y}, {0, -1}});  // Reflect to left '<'
            } else if (dx == 1 && dy == 0) { // Laser is moving downward 'V'
                int clearX = x + 1, clearY = y;
                while (clearX < grid.size() && grid[clearX][clearY] == 'V') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearX++;
                }
                q.push({{x, y}, {0, 1}}); // Reflect to the right
            }
            continue;
        } else if (grid[x][y] == '_') {
            if (dx == 1 && dy == 0) { // Laser is moving downward 'V'
                int clearX = x + 1, clearY = y;
                while (clearX < grid.size() && grid[clearX][clearY] == 'V') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearX++;
                }
                // Horizontal beam splitter splits horizontally
                q.push({{x, y}, {dx, dy}}); // Keep the current direction
                q.push({{x, y}, {0, 1}});    // Split laser to the right
                q.push({{x, y}, {0, -1}});   // Split laser to the left
            } else if (dx == -1 && dy == 0) {  // Laser is moving upward '^'
                int clearX = x - 1, clearY = y;
                while (clearX >= 0 && grid[clearX][clearY] == '^') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearX--;
                }
                // Horizontal beam splitter splits horizontally
                q.push({{x, y}, {dx, dy}}); // Keep the current direction
                q.push({{x, y}, {0, 1}});    // Split laser to the right
                q.push({{x, y}, {0, -1}});   // Split laser to the left
            }

            continue; // No more laser propagation in the current direction
        } else if (grid[x][y] == '|') {
            // Vertical beam splitter splits vertically
            q.push({{x, y}, {dx, dy}}); // Keep the current direction
            q.push({{x, y}, {1, 0}});    // Split laser downward
            q.push({{x, y}, {-1, 0}});   // Split laser upward
            continue; // No more laser propagation in the current direction
        }

        // Mark the cell with the direction of the laser
        else if (grid[x][y] == '.') {
            grid[x][y] = directionSymbols[dx == 0 && dy == 1 ? 0 : (dx == 1 && dy == 0 ? 1 : (dx == 0 && dy == -1 ? 2 : 3))];
        }

        // Continue the laser in the current direction
        q.push({{x, y}, {dx, dy}});
    }
}



// Function to extract coordinates from a string (e.g., "1,2" to x=1, y=2)
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

// Function to read the grid size from the file
void readGridSize(ifstream& inputFile, int& rows, int& cols) {
    string line;
    getline(inputFile, line);
    if (sscanf(line.c_str(), "%dx%d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) {
        cerr << "Error: Invalid grid size!" << endl;
    }
}

// Function to read the laser position from the file and place it on the grid
void readLaserPosition(ifstream& inputFile, vector<vector<char>>& grid, int rows, int cols) {
    string line;
    getline(inputFile, line);
    int laserX, laserY;
    extractCoordinates(line, laserX, laserY);
    if (laserX <= 0 || laserY <= 0 || laserX > rows || laserY > cols) {
        cerr << "Error: Invalid laser position!" << endl;
        return;
    }
    grid[laserX - 1][laserY - 1] = 'b'; // Place the laser beam ('b')
}

// Function to read targets from the file and place them on the grid
void readTargets(ifstream& inputFile, vector<vector<char>>& grid) {
    string line;
    for (int i = 0; i < 3; ++i) {
        getline(inputFile, line);
        int x, y;
        extractCoordinates(line, x, y);
        if (x > 0 && y > 0) {
            grid[x - 1][y - 1] = 'o';  // Place target ('o')
        }
    }
}

// Function to read blockers from the file and place them on the grid
void readBlockers(ifstream& inputFile, vector<vector<char>>& grid) {
    string line;
    for (int i = 0; i < 4; ++i) {
        getline(inputFile, line);
        int x, y;
        extractCoordinates(line, x, y);
        if (x > 0 && y > 0) {
            grid[x - 1][y - 1] = '#';  // Place blocker ('#')
        }
    }
}

// Function to read beam splitters from the file and place them on the grid
void readBeamSplitters(ifstream& inputFile, vector<vector<char>>& grid, vector<pair<int, int>>& beamSplitters) {
    string line;
    for (int i = 0; i < 2; ++i) {
        getline(inputFile, line);
        int x, y;
        extractCoordinates(line, x, y);
        if (x > 0 && y > 0) {
            if (line.find(" ") != string::npos) {
                grid[x - 1][y - 1] = '_';  // Horizontal beam splitter ('_')
            } else {
                grid[x - 1][y - 1] = '|';  // Vertical beam splitter ('|')
            }
            beamSplitters.push_back({x, y});  // Store the beam splitter coordinates
        }
    }
}

// Function to read mirrors from the file and place them on the grid
void readMirrors(ifstream& inputFile, vector<vector<char>>& grid, vector<pair<int, int>>& mirrors) {
    string line;
    for (int i = 0; i < 6; ++i) {
        getline(inputFile, line);
        int x, y;
        extractCoordinates(line, x, y);
        if (x > 0 && y > 0) {
            // Check if the coordinates represent a forward slash or backslash
            if (line.find(' ') != string::npos) {
                grid[x - 1][y - 1] = '/';  // Forward slash mirror
            } else if (line.find(',') != string::npos) {
                grid[x - 1][y - 1] = '\\';  // Backslash mirror
            }
            mirrors.push_back({x, y});  // Store the mirror coordinates
        }
    }
}

// Function to place mirrors or beam splitters on the grid manually
void placeToken(vector<vector<char>>& grid, int& mirrorCount, int& beamSplitterCount, int rows, int cols, vector<pair<int, int>>& mirrors, vector<pair<int, int>>& beamSplitters, int laserX, int laserY) {
    string token;
    int x, y;

    cout << "Enter token type (/ or \\ for mirror, _ or | for beam splitter): ";
    cin >> token;

    // Validate the token type
    if (!(token == "/" || token == "\\" || token == "_" || token == "|")) {
        cout << "Invalid token type. Please choose /, \\ , _, or |." << endl;
        return;
    }

    // Get the coordinates for the token
    cout << "Enter coordinates (row,column): ";
    string coords;
    cin >> coords;
    extractCoordinates(coords, x, y);


    // Check if the coordinates are valid
    if (x <= 0 || y <= 0 || x > rows || y > cols) {
        cout << "Invalid coordinates. Please try again." << endl;
        return;
    }

    // Place the token on the grid and enforce the limits
    if (token == "/" || token == "\\") {
        if (mirrorCount >= 6) {
            cout << "Error: Maximum mirrors (6) reached." << endl;
            return;
        }
        grid[x - 1][y - 1] = token[0];  // Place the mirror
        mirrors.push_back({x, y});  // Store the mirror coordinates
        mirrorCount++;  // Increment mirror count
    } else if (token == "_" || token == "|") {
        if (beamSplitterCount >= 2) {
            cout << "Error: Maximum beam splitters (2) reached." << endl;
            return;
        }
        grid[x - 1][y - 1] = token[0];  // Place the beam splitter
        beamSplitters.push_back({x, y});  // Store the beam splitter coordinates
        beamSplitterCount++;  // Increment beam splitter count
    }

    // Re-simulate the laser path
    shootLaser(grid, laserX, laserY);

    // Write the updated grid to the temporary file
    writeGridToTempFile(grid, "easyTemp.txt", mirrors, beamSplitters);

    // Display the updated maze grid
    cout << "Updated Maze Grid with Laser Path:" << endl;
    printGrid(grid);
}


// Function to reset the maze to the default state (from easy.txt to easyTemp.txt)
void resetLevel(vector<vector<char>>& grid, int rows, int cols, 
                vector<pair<int, int>>& mirrors, vector<pair<int, int>>& beamSplitters) {
    // Read the original maze from easy.txt
    ifstream easyFile("easy.txt");
    ofstream tempFile("easyTemp.txt");

    string line;
    while (getline(easyFile, line)) {
        tempFile << line << endl;
    }

    easyFile.close();
    tempFile.close();

    // Reload the grid from easyTemp.txt to reflect the reset state
    ifstream inputFile("easyTemp.txt");
    readGridSize(inputFile, rows, cols);
    grid = vector<vector<char>>(rows, vector<char>(cols, '.'));  // Reset the grid
    readLaserPosition(inputFile, grid, rows, cols);
    readTargets(inputFile, grid);
    readBlockers(inputFile, grid);
    readBeamSplitters(inputFile, grid, beamSplitters);
    readMirrors(inputFile, grid, mirrors);

    inputFile.close();

    cout << "Maze reset to the default state." << endl;
    cout << "Current Maze Grid (reset):" << endl;
    printGrid(grid);

    // After reset, save the current grid state to easyTemp.txt again
    writeGridToTempFile(grid, "easyTemp.txt", mirrors, beamSplitters);
}

// Menu function to allow the user to input tokens
void displayMenuAndPlaceTokens(vector<vector<char>>& grid, int rows, int cols, 
                               vector<pair<int, int>>& mirrors, vector<pair<int, int>>& beamSplitters, int laserX, int laserY) {
    int mirrorCount = mirrors.size();
    int beamSplitterCount = beamSplitters.size();
    string choice;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Place Mirror (/ or \\)\n";
        cout << "2. Place Beam Splitter (_ or |)\n";
        cout << "3. Reset Level\n";
        cout << "4. Exit and save maze\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == "1" || choice == "2") {
            placeToken(grid, mirrorCount, beamSplitterCount, rows, cols, mirrors, beamSplitters, laserX, laserY);
        } else if (choice == "3") {
            beamSplitterCount = 0;
            mirrorCount = 0;
            resetLevel(grid, rows, cols, mirrors, beamSplitters);  // Call the reset function
        } else if (choice == "4") {
            cout << "Saving maze..." << endl;
            writeGridToTempFile(grid, "easyTemp.txt", mirrors, beamSplitters);  // Save the current state
            break;  // Exit the menu loop
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}


int main() {
    // Read the maze size and grid from the file (e.g., "easy.txt")
    ifstream inputFile("easyTemp.txt"); // Open the input file
    if (!inputFile) {
        cerr << "Error: Could not open the file!" << endl;
        return 1;
    }

    int rows, cols;
    readGridSize(inputFile, rows, cols);

    // Initialize the grid with blank spaces ('.')
    vector<vector<char>> grid(rows, vector<char>(cols, '.'));

    // Read the laser position and place it on the grid
    readLaserPosition(inputFile, grid, rows, cols);

    // Read the targets
    readTargets(inputFile, grid);

    // Read the blockers
    readBlockers(inputFile, grid);

    // Read the beam splitters
    vector<pair<int, int>> beamSplitters;
    readBeamSplitters(inputFile, grid, beamSplitters);

    // Read the mirrors
    vector<pair<int, int>> mirrors;
    readMirrors(inputFile, grid, mirrors);

    // Close the input file after reading
    inputFile.close();

    // Find the laser's position
    int laserX = -1, laserY = -1;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 'b') {
                laserX = i;
                laserY = j;
                break;
            }
        }
        if (laserX != -1) break;
    }

    // Simulate the laser path
    if (laserX != -1 && laserY != -1) {
        shootLaser(grid, laserX, laserY);
    }

    // Save the updated grid with the laser path
    writeGridToTempFile(grid, "easyTemp.txt", mirrors, beamSplitters);

    // Display the initial maze grid in the terminal
    cout << "Updated Maze Grid with Laser Path:" << endl;
    printGrid(grid);

    // Allow the user to place mirrors and beam splitters manually
    displayMenuAndPlaceTokens(grid, rows, cols, mirrors, beamSplitters, laserX, laserY);

    return 0;
}



