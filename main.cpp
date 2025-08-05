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
#include <chrono>

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
            value = value/500;

            std::string nation = cells[columnIndex["nationality_name"]];
            std::string league = cells[columnIndex["league_name"]];
            std::string team = cells[columnIndex["club_name"]];
            //std::string version = cells[columnIndex["fifa_version"]];

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

    std::string file ="male_players.csv";
    char c;
    std::cout<< "Do you have the male_players.csv loaded into your IDE? (y/n)?\n";
    std::cin>>c;

    while (c != 'y' && c != 'Y' && c != 'n' && c != 'N') {
        std::cout << "Invalid input. Please enter (y/n): ";
        std::cin >> c;
    }
    if (c == 'n' || c == 'N') {
        std::cout << "Please enter the absolute path to the male_players.csv: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, file);
    }

    //file = "C:/Users/lucas/OneDrive/Desktop/male_players.csv";
    //Change the file to your own absolute path if you want to use this program. Change this path before submitting
    readData(file, allPlayers, playersByName);

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
            return a.value > b.value;
        });
    }

    std::string choice;
    std::cout<<"Choose which algorithm to use by typing 1,2, or 3:\n";
    std::cout<<"1. A*\n";
    std::cout<<"2. Best Fit\n";
    std::cout<<"3. Both\n";
    std::cin>>choice;


    if(choice == "1" || choice == "3") {
        auto start = std::chrono::system_clock::now();
        std::vector<Player> bestSquad = AStarSquadOptimizer(formation, playerByPosition, req);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> duration = end - start;

        if (bestSquad.empty()) {
            std::cout << "\nNo valid squad could be generated with the given constraints.\n";
        } else {
            std::cout << "\n--- A* Squad---\n";
            for (const Player& p : bestSquad) {
                std::cout << p.name << " | Rating: " << p.rating << " | Value: " << p.value  << "\n";
            }
            std::cout << "Total Chemistry: " << calculateChem(bestSquad) << "\n";
            std::cout << "Total Value: " << calculateTotalSquadValue(bestSquad) << "\n";
            std::cout << "Rating: " << calculateAverageRating(bestSquad) << "\n";
            std::cout << "A* runtime: " << duration.count() << " seconds\n";
        }
    }

    //Run A* function to find the best squad given the requirements
    if(choice == "2" || choice == "3") {
        auto start = std::chrono::system_clock::now();
        std::vector<Player> bestSquad = BestFitSquadOptimizer(formation, playerByPosition, req);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> duration = end - start;

        if (bestSquad.empty()) {
            std::cout << "\nNo valid squad could be generated with the given constraints.\n";
        } else {
            std::cout << "\n--- Best Fit Squad---\n";
            for (const Player& p : bestSquad) {
                std::cout << p.name << " | Rating: " << p.rating << " | Value: " << p.value << "\n";
            }
            std::cout << "Total Chemistry: " << calculateChem(bestSquad) << "\n";
            std::cout << "Total Value: " << calculateTotalSquadValue(bestSquad) << "\n";
            std::cout << "Rating: " << calculateAverageRating(bestSquad) << "\n";
            std::cout << "Best Fit runtime: " << duration.count() << " seconds\n";
        }
    }

    return 0;
}
