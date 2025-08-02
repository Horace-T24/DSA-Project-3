#include <iostream>
#include "Player_Organizer.h"
#include "Formations.h"
#include "Requirements.h"

// Add your Requirements() function here or #include the file where it's defined

using namespace std;

// Helper: Build the best team based on filtered players and required formation
vector<Player> BuildBestTeam(const vector<Player>& candidates, const vector<string>& formationPositions) {
    unordered_map<string, Player> bestPlayers;

    for (const string& position : formationPositions) {
        int bestRating = -1;
        Player bestPlayer;

        for (const Player& p : candidates) {
            if (p.position == position && p.rating > bestRating &&
                bestPlayers.find(p.name) == bestPlayers.end()) {
                bestRating = p.rating;
                bestPlayer = p;
            }
        }

        if (bestRating != -1)
            bestPlayers[bestPlayer.name] = bestPlayer;
    }

    vector<Player> finalTeam;
    for (const string& pos : formationPositions) {
        for (const auto& [_, player] : bestPlayers) {
            if (player.position == pos) {
                finalTeam.push_back(player);
                break;
            }
        }
    }

    return finalTeam;
}

int main() {
    // Step 1: Ask for requirements
    Requirements();

    // Step 2: Set up formation manager
    Formations formations;

    string formationName;
    cout << "\nEnter your preferred formation (example: 4-2-3-1 Wide): ";
    getline(cin, formationName);
    vector<string> formationPositions = formations.getPositions(formationName);

    if (formationPositions.size() == 1 && formationPositions[0] == "Invalid formation") {
        cout << "Invalid formation entered. Exiting." << endl;
        return 1;
    }

    // Step 3: Create PlayerOrganizer and mock sample players
    PlayerOrganizer organizer;

    // Sample players — in practice, load from CSV
    organizer.AddPlayer(Player("Kane", "ST", 90, "4-2-3-1 Wide", "England", "Premier League", "Tottenham"));
    organizer.AddPlayer(Player("Rodri", "CDM", 89, "4-2-3-1 Wide", "Spain", "Premier League", "Man City"));
    organizer.AddPlayer(Player("Zidane", "CAM", 91, "4-2-3-1 Wide", "France", "Icons", "Icons"));
    organizer.AddPlayer(Player("Saka", "RM", 88, "4-2-3-1 Wide", "England", "Premier League", "Arsenal"));
    organizer.AddPlayer(Player("Mainoo", "CDM", 82, "4-2-3-1 Wide", "England", "Premier League", "Man United"));
    organizer.AddPlayer(Player("Aït-Nouri", "LM", 85, "4-2-3-1 Wide", "Algeria", "Premier League", "Wolves"));
    organizer.AddPlayer(Player("Courtois", "GK", 89, "4-2-3-1 Wide", "Belgium", "La Liga", "Real Madrid"));
    organizer.AddPlayer(Player("Van Dijk", "CB", 91, "4-2-3-1 Wide", "Netherlands", "Premier League", "Liverpool"));
    organizer.AddPlayer(Player("Schär", "CB", 84, "4-2-3-1 Wide", "Switzerland", "Premier League", "Newcastle"));
    organizer.AddPlayer(Player("Kerkez", "LB", 83, "4-2-3-1 Wide", "Hungary", "Premier League", "Bournemouth"));
    organizer.AddPlayer(Player("Klostermann", "CB", 83, "4-2-3-1 Wide", "Germany", "Bundesliga", "RB Leipzig"));

    // Step 4: Filter (customize this using your Requirements() return values)
    vector<Player> filteredPlayers = organizer.FilterPlayers(80, formationName, "", "Premier League", "");

    // Step 5: Build the best team
    vector<Player> bestTeam = BuildBestTeam(filteredPlayers, formationPositions);

    // Step 6: Display
    cout << "\n--- Best Team ---" << endl;
    for (const Player& p : bestTeam) {
        cout << p.position << ": " << p.name << " (" << p.rating << ")" << endl;
    }

    return 0;
}
