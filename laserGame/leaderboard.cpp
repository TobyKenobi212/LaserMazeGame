/*
    File containing class method implementation for leaderboard.h
    Author(s): Toby Pham, Tri Nguyen, Benjamin Thai
    Last updated 12/15/2024
    Version 1.03
*/
#include "leaderboard.h"
using namespace std;

void Leaderboard::addPlayer(const Player& player) {
    // Check if the player already exists in the leaderboard
    for (auto& p : players) {
        if (p.getUsername() == player.getUsername()) {
            p = player; // Update existing player data
            return;
        }
    }
    players.push_back(player); // Add new player
}

void Leaderboard::displayLeaderboard() const {
    struct ScoreEntry {
        string username;
        int score;
    };

    vector<ScoreEntry> scores;

    for (const auto& player : players) {
        if (player.getUsername() != "dev") { // Exclude the "dev" account
            scores.push_back({player.getUsername(), player.getTotalHighestScore()});
        }
    }

    // Bubble sort to sort the scores in descending order
    for (size_t i = 0; i < scores.size(); ++i) {
        for (size_t j = 0; j < scores.size() - 1; ++j) {
            if (scores[j].score < scores[j + 1].score) {
                ScoreEntry temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    if (scores.empty()) {
        cout << "No players to display.\n";
    } else {
        for (size_t i = 0; i < scores.size(); ++i) {
            cout << "Rank" << i + 1 << ": " << scores[i].username << " - Total High Score: " << scores[i].score << " pts\n";
        }
    }
}
