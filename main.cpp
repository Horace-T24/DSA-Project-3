#include <iostream>
#include "Player_Organizer.h"
#include "Formations.h"
#include "Requirements.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

//Helper which splits each line of the CSV file respecting fields with quotes
std::vector<std::string> parseCSVLine(const std::string& line){
    std::vector<std::string> cells;
    std::string cell;
    bool inQuotes = false;

    for(size_t i = 0; i<line.size(); i++){
        char c = line[i];
        if(c == '"'){
            inQuotes = !inQuotes;
        } else if (c==',' && !inQuotes){
            cells.push_back(cell);
            cell.clear();
        }else{
            cell += c;
        }
    }
    cells.push_back(cell);
    return cells;
}

//Helper for splitting the position field (Ex: "ST, LW") into a vector
std::vector<std::string> splitPositions(const std::string& str){
    std::vector<std::string> positions;
    std::stringstream ss(str);
    std::string pos;

    while(std::getline(ss,pos, ',')){
        pos.erase(0, pos.find_first_not_of(" \t"));
        pos.erase(pos.find_last_not_of(" \t") + 1);
        positions.push_back(pos);
    }
    return positions;
}


void readData(
    const std::string filename, 
    std::vector<Player>& allPlayers,
    std::unordered_map<std::string, 
    std::vector<Player>>& playersByName)
{    
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr <<"Error opening file. \n";
        return;
    }
    
    std::string line;
    std::getline(file,line); //Header: includes all column names
    std::vector<std::string> headers = parseCSVLine(line);

    //Maps column names to an index
    std::unordered_map<std::string, int> columnIndex;
    for (int i = 0; i < headers.size(); ++i) {
        columnIndex[headers[i]] = i;
    }
    
    while(std::getline(file,line)){
        std::vector<std::string> cells = parseCSVLine(line);
        if(cells.size() < headers.size()) continue; //skips any bad rows just in case

        try{
            std::string name = cells[columnIndex["short_name"]];
            std::vector<std::string> positions = splitPositions(cells[columnIndex["player_positions"]]);

            int rating = 0;
            std::string ratingStr = cells[columnIndex["overall"]];
            if(!ratingStr.empty()) rating = std::stoi(ratingStr);

            int value = 0;
            std::string valueStr = cells[columnIndex["value_eur"]];
            if(!valueStr.empty()) value = std::stoi(valueStr);

            std::string nation = cells[columnIndex["nationality_name"]];
            std::string league = cells[columnIndex["league_name"]];
            std::string team = cells[columnIndex["club_name"]];

            Player player(name,positions,rating,value,nation,league,team);
            allPLayers.push_back(player);
            playersByName[name].push_back(player);
        } catch (...){
            continue; //Skips bad rows
        }
    }
}



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
   //current code to read in csv file and test if it works

    std::vector<Player> allPlayers;
    std::unordered_map<std::string, std::vector<Player>> playersByName;

    readData("male_players.csv", allPlayers, playersByName);

    const std::string target = "K. Mbappé";
    if (playersByName.count(target)) {
        for (const Player& p : playersByName[target]) {
            std::cout << p.name << " | Rating: " << p.rating << " | Team: " << p.team << "\n";
        }
    } else {
        std::cout << target << " not found.\n";
    }




    
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
