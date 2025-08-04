#include <iostream>
#include "Player_Organizer.h"
#include "Formations.h"
#include "Requirements.h"
#include <SquadOptimizer.h>
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

    for(int i = 0; i<line.size(); i++){
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

//Function that reads the csv file and creates all Player objects. Each player is then added to the vector containing all players and another vector that contains all versions of each specific player
void readData(
    const std::string& filename,
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
    for (int i = 0; i < headers.size(); i++) {
        columnIndex[headers[i]] = i;
    }

    //Main loop that goes through each line and creates each player object to append to the vector containing all Players
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
            value = value/100;

            std::string nation = cells[columnIndex["nationality_name"]];
            std::string league = cells[columnIndex["league_name"]];
            std::string team = cells[columnIndex["club_name"]];

            Player player(name,positions,rating,value,nation,league,team);
            allPlayers.push_back(player);
            playersByName[name].push_back(player);
        } catch (...){
            continue; //Skips bad rows
        }
    }
}



int main() {

    std::vector<Player> allPlayers;
    std::unordered_map<std::string,std::vector<Player>> playersByName;


    //Change the file to your own absolute path if you want to use this program. Change this path before submitting
    readData("C:/Users/lucas/Downloads/DSA-Project-3-main/DSA-Project-3-main/male_players.csv", allPlayers, playersByName);

    //Prompt user for requirements
    SquadRequirements req = Requirements();
    Formations formations;
    std::vector<std::string> formation = formations.getPositions(req.formation);

    // Build position-based map. This makes A* much more efficient so that the program only loops through valid players for each position rather than all players.
    std::unordered_map<std::string, std::vector<Player>> playerByPosition;
    for (const Player& p : allPlayers) {
        if (p.rating >= req.minOverallRating) {
            for (const std::string& pos : p.positions) {
                playerByPosition[pos].push_back(p);
            }
        }
    }

    for (auto& [pos, vec] : playerByPosition) {
        std::sort(vec.begin(), vec.end(), [](const Player& a, const Player& b) {
            return a.value < b.value;
        });
    }

    //Run A* function to find the best squad given the requirements
    std::vector<Player> bestSquad = AStarSquadOptimizer(formation, playerByPosition, req);


    if (bestSquad.empty()) {
        std::cout << "\nNo valid squad could be generated with the given constraints.\n";
    } else {
        std::cout << "\n--- Optimal Squad ---\n";
        for (const Player& p : bestSquad) {
            std::cout << p.name << " | Rating: " << p.rating << " | Value: " << p.value << "\n";

        }
        std::cout << "Total Chemistry: " << calculateChem(bestSquad) << "\n";
        std::cout << "Total Value: " << calculateTotalSquadValue(bestSquad) << "\n";
    }


/*
    const std::string target = "V. van Dijk";
    if (playersByName.count(target)) {
        for (const Player& p : playersByName[target]) {
            std::cout << p.name << " | Rating: " << p.rating << " | positions " << p.positions[0] << "| Value:" << p.value << "\n";
        }
    } else {
        std::cout << target << " not found.\n";
    }

*/


    return 0;
}
