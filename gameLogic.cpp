/*    
    To test the file you would need to create "easy.txt" and "easyTemp.txt" and input both ONLY the coordinates
    from the "Easy Layout and Directions.txt"
    or you can use your own coordinates, but make sure both coordinates need to be the same in both files
    
    laser beam: done 
    reflected laser: done allowing the reflected laser to interact with another token's type (meaning the reflected
                     laser would continue to reflect upon hitting another token)
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
    target (o): done
                targets only accept laser coming into the target and if at least 1 of the laser hit the target
                then the code returns true, all targets need to return true (condition to clear the level)
                when all the targets got hit, there will be a message under the maze grid in the terminal
                that said "All targets hit! Level passed."
    Messages: Adding messages everytime the users put down a token (motivation maybe)
    Need to integrate into the Classes files: still on it
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <queue>
#include <cstdlib> 
#include <ctime>   

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

string selectLevel() {
    int levelChoice;
    cout << "Choose your level:\n";
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";
    cout << "Enter your choice (1, 2, or 3): ";
    cin >> levelChoice;

    // Return the corresponding level based on the user's choice
    switch (levelChoice) {
        case 1:
            return "easy";   // Return "easy" if user chooses 1
        case 2:
            return "medium"; // Return "medium" if user chooses 2
        case 3:
            return "hard";   // Return "hard" if user chooses 3
        default:
            cout << "Invalid choice. Defaulting to 'easy'." << endl;
            return "easy";   // Default to "easy" if invalid input
    }
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

vector<vector<bool>> shootLaser(vector<vector<char>>& grid, int laserX, int laserY) {
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false)); // Track visited cells
    vector<vector<bool>> targetHit(rows, vector<bool>(cols, false)); // Track if targets are hit
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

        // Determine the laser's symbol
        char directionSymbol = (dx == 0 && dy == 1) ? '>' :  // Right
                               (dx == 1 && dy == 0) ? 'V' :  // Down
                               (dx == 0 && dy == -1) ? '<' : // Left
                               '^';                          // Up

        // Handle different grid elements
        if (grid[x][y] == '#' || grid[x][y] == 'b') {
            // Blocker stops the laser (laser doesn't move forward in this direction)
            continue;
        } else if (grid[x][y] == 'o') {
            // Check if the target accepts the laser from this direction
            if ((directionSymbol == 'V' && dx == 1) ||     // Laser coming from above
                (directionSymbol == '^' && dx == -1) ||    // Laser coming from below
                (directionSymbol == '>' && dy == 1) ||     // Laser coming from left
                (directionSymbol == '<' && dy == -1)) {    // Laser coming from right
                targetHit[x][y] = true; // Mark target as hit
            }
            continue; // Laser does not continue past the target
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
                //q.push({{x, y}, {dx, dy}}); // Keep the current direction
                q.push({{x, y}, {0, 1}});    // Split laser to the right
                q.push({{x, y}, {0, -1}});   // Split laser to the left
            } else if (dx == -1 && dy == 0) {  // Laser is moving upward '^'
                int clearX = x - 1, clearY = y;
                while (clearX >= 0 && grid[clearX][clearY] == '^') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearX--;
                }
                // Horizontal beam splitter splits horizontally
                //q.push({{x, y}, {dx, dy}}); // Keep the current direction
                q.push({{x, y}, {0, 1}});    // Split laser to the right
                q.push({{x, y}, {0, -1}});   // Split laser to the left
            } else if (dx == 0 && dy == 1) { // Laser is moving right '>'
                int clearX = x, clearY = y + 1;
                while (clearY < grid[0].size() && grid[clearX][clearY] == '>') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearY++;
                }
                q.push({{x, y}, {0, 1}}); // Reflect to the right '>'
            } else if (dx == 0 && dy == -1) { // Laser is moving left '<'
                int clearX = x, clearY = y - 1;
                while (clearY >= 0 && grid[clearX][clearY] == '<') {
                    grid[clearX][clearY] = '.';  // Reset to empty cell
                    clearY--;
                }
                q.push({{x, y}, {0, -1}});  // Reflect to left '<'
            }
            continue; // No more laser propagation in the current direction
        } else if (grid[x][y] == '|') {
            if (dx == 1 && dy == 0) { // Laser is moving downward 'V' 
                // Vertical beam splitter splits vertically
                //q.push({{x, y}, {dx, dy}}); // Keep the current direction
                q.push({{x, y}, {1, 0}});    // Keep moving downward
            } else if (dx == -1 && dy == 0) {  // Laser is moving upward '^'
                q.push({{x, y}, {-1, 0}});   // Keep moving upward
            } else if (dx == 0 && dy == 1) { // Laser is moving right '>'
                q.push({{x, y}, {1, 0}});    // Split laser downward
                q.push({{x, y}, {-1, 0}});   // Split laser upward
            } else if (dx == 0 && dy == -1) { // Laser is moving left '<'
                q.push({{x, y}, {1, 0}});    // Split laser downward
                q.push({{x, y}, {-1, 0}});   // Split laser upward   
            }
            continue; // No more laser propagation in the current direction
        }

        // Mark the cell with the direction of the laser
        else if (grid[x][y] == '.') {
            grid[x][y] = directionSymbols[dx == 0 && dy == 1 ? 0 : (dx == 1 && dy == 0 ? 1 : (dx == 0 && dy == -1 ? 2 : 3))];
        }

        // Continue the laser in the current direction
        q.push({{x, y}, {dx, dy}});
    }

    return targetHit;
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
void placeToken(vector<vector<char>>& grid, int& mirrorCount, int& beamSplitterCount, int rows, int cols, 
                vector<pair<int, int>>& mirrors, vector<pair<int, int>>& beamSplitters, int laserX, int laserY, const string& currentLevel) {
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
    if (x <= 0 || y <= 0 || x > rows || y > cols || 
        grid[x-1][y-1] == '#' || grid[x-1][y-1] == 'b' || grid[x-1][y-1] == 'o') {
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
    auto targetHit = shootLaser(grid, laserX, laserY);

    // Write the updated grid to the temporary file
    writeGridToTempFile(grid, currentLevel + "Temp.txt", mirrors, beamSplitters);

    // Display the updated maze grid
    cout << "Updated Maze Grid with Laser Path:" << endl;
    printGrid(grid);

    // Initialize random seed
    srand(static_cast<unsigned>(time(0)));

    // Check if all targets are hit
    bool allTargetsHit = true;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 'o' && !targetHit[i][j]) {
                allTargetsHit = false;
                break;
            }
        }
    }

    // Output the result
    if (allTargetsHit) {
        cout << "All targets hit! Level passed." << endl;
        cout << "You can move to the next level or play again." << endl;
    } else {
        // List of motivational messages
        vector<string> messages = {
            "Are you sure about that?",
            "Oh, I don't know about that...",
            "Everyone makes mistakes, right?",
            "Close, you are getting there.",
            "Hmm, is that place good enough?",
            "Not quite there.",
            "The laser disagrees with your strategy. Try again!",
            "Nice one!",
            "You're good at this!",
            "Fantastic move!",
            "Are you giving up yet?"
        };

        // Select a random message
        int randomIndex = rand() % messages.size();
        cout << messages[randomIndex] << endl;
    }
}

// Function to reset the maze to the default state (from easy.txt to easyTemp.txt)
void resetLevel(vector<vector<char>>& grid, int rows, int cols, 
                vector<pair<int, int>>& mirrors, vector<pair<int, int>>& beamSplitters, const string& currentLevel) {
    string levelFile = currentLevel + ".txt";  // Use the current level file (easy.txt, medium.txt, hard.txt)
    string tempFile = currentLevel + "Temp.txt"; // Corresponding temp file (easyTemp.txt, mediumTemp.txt, hardTemp.txt)

    // Copy the level file to the temp file
    ifstream levelFileStream(levelFile);
    ofstream tempFileStream(tempFile);
    string line;
    while (getline(levelFileStream, line)) {
        tempFileStream << line << endl;
    }
    levelFileStream.close();
    tempFileStream.close();

    // Reload the grid from the temp file
    ifstream inputFile(tempFile);
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
    writeGridToTempFile(grid, tempFile, mirrors, beamSplitters);
}

// Menu function to allow the user to input tokens and go back to selecting a level
void displayMenuAndPlaceTokens(vector<vector<char>>& grid, int rows, int cols, 
                               vector<pair<int, int>>& mirrors, vector<pair<int, int>>& beamSplitters, 
                               int laserX, int laserY, string& currentLevel) {
    int mirrorCount = mirrors.size();
    int beamSplitterCount = beamSplitters.size();
    string choice;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Place Mirror (/ or \\)\n";
        cout << "2. Place Beam Splitter (_ or |)\n";
        cout << "3. Reset Level\n";
        cout << "4. Go back and choose a new level\n";
        cout << "5. Exit and save maze\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == "1" || choice == "2") {
            placeToken(grid, mirrorCount, beamSplitterCount, rows, cols, mirrors, beamSplitters, laserX, laserY, currentLevel);
        } else if (choice == "3") {
            beamSplitterCount = 0;
            mirrorCount = 0;
            resetLevel(grid, rows, cols, mirrors, beamSplitters, currentLevel);  // Call the reset function
        } else if (choice == "4") {
            // Allow the user to select a new level
            cout << "\nYou chose to pick a new level.\n";
            currentLevel = selectLevel();  // Re-select the level
            // Re-load the grid based on the new level
            string tempFile = currentLevel + "Temp.txt";
            ifstream inputFile(tempFile);
            if (!inputFile) {
                cerr << "Error: Could not open the file " << tempFile << endl;
                return;
            }

            readGridSize(inputFile, rows, cols);

            // Initialize the grid with blank spaces ('.')
            grid = vector<vector<char>>(rows, vector<char>(cols, '.'));
            // Read the laser position and place it on the grid
            readLaserPosition(inputFile, grid, rows, cols);
            // Read the targets
            readTargets(inputFile, grid);
            // Read the blockers
            readBlockers(inputFile, grid);
            // Read the beam splitters
            beamSplitters.clear();
            readBeamSplitters(inputFile, grid, beamSplitters);
            // Read the mirrors
            mirrors.clear();
            readMirrors(inputFile, grid, mirrors);
            // Close the input file after reading
            inputFile.close();
            // Find the laser's position
            laserX = -1, laserY = -1;
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
            // Display the new maze grid in the terminal
            cout << "New Maze Grid for " << currentLevel << " level:" << endl;
            printGrid(grid);
        } else if (choice == "5") {
            cout << "Saving maze..." << endl;
            writeGridToTempFile(grid, currentLevel + "Temp.txt", mirrors, beamSplitters);  // Save the current state
            break;  // Exit the menu loop
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

int main() {
    // Select the level (easy, medium, or hard)
    string currentLevel = selectLevel();  // Call it once and store the level

    // Construct the file paths for the current level and its temporary file
    string tempFile = currentLevel + "Temp.txt";

    // Open the corresponding level file (e.g., "easy.txt", "medium.txt", or "hard.txt")
    ifstream inputFile(tempFile);
    if (!inputFile) {
        cerr << "Error: Could not open the file " << tempFile << endl;
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
    writeGridToTempFile(grid, tempFile, mirrors, beamSplitters);

    // Display the initial maze grid in the terminal
    cout << "Updated Maze Grid with Laser Path:" << endl;
    printGrid(grid);

    // Allow the user to place mirrors and beam splitters manually
    displayMenuAndPlaceTokens(grid, rows, cols, mirrors, beamSplitters, laserX, laserY, currentLevel);

    return 0;
}



