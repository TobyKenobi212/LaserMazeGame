# Laser Maze Game

## Introduction

This project is a console-based game developed entirely in C++ that challenges players to guide a laser beam through a 7x7 grid filled with obstacles and targets. Players must strategically place various tokens on the grid to direct the laser towards all targets, progressing through levels of increasing complexity. The game features three difficulty levels: easy, normal, and hard, each with two levels. Players must complete the first level of each difficulty to unlock the second. With 3 lives per level, the final score is determined by the number of lives remaining upon successful completion. The game includes an autosave function to save progress and a leaderboard to display the highest scores. Users can register, login, and logout as needed.

## Features

- **Three Difficulty Levels**: Easy, normal, and hard, each with two levels.
- **Lives System**: Players have 3 lives per level.
- **Autosave Function**: Automatically saves the progress of the last played level.
- **Leaderboard**: Displays the highest scores for each level.
- **User Management**: Register, login, and logout functionality.

## Classes

- **Player Class**: Manages player data (username, lives, score), tracks progress, and handles autosave and loading levels.
- **Menu Class**: Coordinates navigation and interactions between the Player, Leaderboard, and game logic.
- **GridObject Class**: Identifies cell types within the grid.
- **GridScanner Class**: Initializes the game grid and manages tokens.
- **Beam Class**: Handles laser movement and interactions with grid elements.
- **GridManager Class**: Visualizes the grid and manages autosaving.

## How to Play

1. **Start the Game**: Launch the game from the console.
2. **Register/Login**: Create a new user or login with an existing account.
3. **Select Difficulty**: Choose from easy, normal, or hard.
4. **Place Tokens**: Strategically place tokens on the grid to guide the laser.
5. **Complete Levels**: Hit all targets within the grid using all tokens in your inventory.
6. **Progress**: Unlock new levels by completing the first level of each difficulty.
7. **Save and Load**: The game autosaves your progress.
8. **Check Leaderboard**: View the highest scores for each level.
