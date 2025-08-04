#ifndef REQUIREMENTS_H
#define REQUIREMENTS_H
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include "Formations.h"
#include "Player_Organizer.h" // For Player struct
#include "SquadHelper.h" //For helper functions

struct SquadRequirements {
    std::string formation;
    int maxBudget;
    int minOverallRating;
    int minTeamChemistry;

    std::vector<std::string> nations;
    std::vector<int> nationCounts;

    std::vector<std::string> leagues;
    std::vector<int> leagueCounts;

    std::vector<std::string> clubs;
    std::vector<int> clubCounts;

    SquadRequirements() = default;

    SquadRequirements(const std::string& formation,
                     int maxBudget,
                     int minOverallRating,
                     int minTeamChemistry,
                     const std::vector<std::string>& nations,
                     const std::vector<int>& nationCounts,
                     const std::vector<std::string>& leagues,
                     const std::vector<int>& leagueCounts,
                     const std::vector<std::string>& clubs,
                     const std::vector<int>& clubCounts)
       : formation(formation),
         maxBudget(maxBudget),
         minOverallRating(minOverallRating),
         minTeamChemistry(minTeamChemistry),
         nations(nations),
         nationCounts(nationCounts),
         leagues(leagues),
         leagueCounts(leagueCounts),
         clubs(clubs),
         clubCounts(clubCounts) {}
};

// Trim whitespace from string ends
inline std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Split comma-separated string into trimmed tokens
inline std::vector<std::string> splitString(const std::string& input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;
    while (getline(ss, item, ',')) {
        std::string trimmed = trim(item);
        if (!trimmed.empty()) {
            result.push_back(trimmed);
        }
    }
    return result;
}

inline int getValidatedInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt << std::endl;
        if (std::cin >> value) {
            if (value >= min && value <= max) {
                break;
            } else {
                std::cout << "Invalid value. Please enter a number between " << min << " and " << max << "." << std::endl;
            }
        } else {
            std::cout << "Invalid input. Please enter a valid integer value." << std::endl;
            std::cin.clear();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

inline std::vector<std::string> getValidatedStringList(const std::string& prompt) {
    std::string input;
    std::vector<std::string> items;
    while (true) {
        std::cout << prompt << std::endl;
        getline(std::cin, input);
        items = splitString(input);

        bool allValid = true;
        for (const std::string& item : items) {
            bool hasNonSpace = false;
            for (char c : item) {
                if (!isalnum(c) && !isspace(c) && c != '-' && c != '\'' && c != '.' && c != '&') {
                    allValid = false;
                    break;
                }
                if (!isspace(c)) hasNonSpace = true;
            }
            if (!hasNonSpace) allValid = false;
            if (!allValid) break;
        }

        if (!items.empty() && allValid) {
            break;
        } else {
            std::cout << "Invalid input. Please enter names using letters, numbers, spaces, or symbols like - . ' &" << std::endl;
        }
    }
    return items;
}

inline std::vector<int> getValidatedIntList(const std::string& prompt, size_t expectedCount, int min, int max, int maxTotal) {
    std::string input;
    std::vector<int> numbers;

    while (true) {
        std::cout << prompt << std::endl;
        getline(std::cin, input);
        std::vector<std::string> parts = splitString(input);
        numbers.clear();

        if (parts.size() != expectedCount) {
            std::cout << "You must enter exactly " << expectedCount << " values." << std::endl;
            continue;
        }

        bool allValid = true;
        int total = 0;

        for (const std::string& str : parts) {
            try {
                int value = std::stoi(str);
                if (value < min || value > max) {
                    allValid = false;
                    break;
                }
                total += value;
                numbers.push_back(value);
            } catch (...) {
                allValid = false;
                break;
            }
        }

        if (!allValid) {
            std::cout << "All entries must be numbers between " << min << " and " << max << "." << std::endl;
        } else if (total > maxTotal) {
            std::cout << "Total requirement exceeds " << maxTotal << ". Please enter smaller values." << std::endl;
        } else {
            break;
        }
    }

    return numbers;
}

// Main Requirements input function — returns SquadRequirements
inline SquadRequirements Requirements() {
    SquadRequirements req;

    Formations formations;

    // Formation input & validation
    while (true) {
        std::cout << "Enter your desired formation (e.g., 4-2-3-1):" << std::endl;
        getline(std::cin, req.formation);

        auto posList = formations.getPositions(req.formation);
        if (posList.size() == 1 && posList[0] == "Invalid formation") {
            std::cout << "Invalid formation. Please enter a valid formation from the list." << std::endl;
            continue;
        }
        break;
    }

    req.maxBudget = getValidatedInt("What is your maximum squad budget?", 1, std::numeric_limits<int>::max());
    req.minOverallRating = getValidatedInt("What is your required squad overall rating? (0-99)", 0, 99);
    req.minTeamChemistry = getValidatedInt("What is your required squad team chemistry? (0-33)", 0, 33);

    std::cout << "Do you want to specify required nations? (y/n): ";
    char c; std::cin >> c; std::cin.ignore();
    if (c == 'y' || c == 'Y') {
        req.nations = getValidatedStringList("Enter required nations (e.g., England, France, Germany):");
        req.nationCounts = getValidatedIntList("Enter number of players required from each nation:", req.nations.size(), 1, 11, 11);
    }
    /*
    req.nations = getValidatedStringList("Enter required nations (e.g., England, France, Germany):");
    req.nationCounts = getValidatedIntList("Enter number of players required from each nation:", req.nations.size(), 1, 11, 11);
*/
    std::cout << "Do you want to specify required leagues? (y/n): ";
    std::cin >> c; std::cin.ignore();
    if (c == 'y' || c == 'Y') {
        req.leagues = getValidatedStringList("Enter required leagues (e.g., Premier League, Ligue 1):");
        req.leagueCounts = getValidatedIntList("Enter number of players required from each league:", req.leagues.size(), 1, 11, 11);
    }
    /*
    req.leagues = getValidatedStringList("Enter required leagues (e.g., Premier League, Ligue 1):");
    req.leagueCounts = getValidatedIntList("Enter number of players required from each league:", req.leagues.size(), 1, 11, 11);
*/
    std::cout << "Do you want to specify required clubs? (y/n): ";
    std::cin >> c; std::cin.ignore();
    if (c == 'y' || c == 'Y') {
        req.clubs = getValidatedStringList("Enter required clubs (e.g., Manchester City, PSG):");
        req.clubCounts = getValidatedIntList("Enter number of players required from each club:", req.clubs.size(), 1, 11, 11);
    }
    /*
    req.clubs = getValidatedStringList("Enter required clubs (e.g., Manchester City, PSG):");
    req.clubCounts = getValidatedIntList("Enter number of players required from each club:", req.clubs.size(), 1, 11, 11);
*/
    // Summary
    std::cout << "\n--- Squad Requirements Summary ---" << std::endl;
    std::cout << "Formation: " << req.formation << std::endl;
    std::cout << "Max Budget: " << req.maxBudget << std::endl;
    std::cout << "Minimum Overall Rating: " << req.minOverallRating << std::endl;
    std::cout << "Minimum Team Chemistry: " << req.minTeamChemistry << std::endl;

    std::cout << "\nNation Requirements:" << std::endl;
    for (size_t i = 0; i < req.nations.size(); ++i) {
        std::cout << "- " << req.nations[i] << ": " << req.nationCounts[i] << " player(s)" << std::endl;
    }

    std::cout << "\nLeague Requirements:" << std::endl;
    for (size_t i = 0; i < req.leagues.size(); ++i) {
        std::cout << "- " << req.leagues[i] << ": " << req.leagueCounts[i] << " player(s)" << std::endl;
    }

    std::cout << "\nClub Requirements:" << std::endl;
    for (size_t i = 0; i < req.clubs.size(); ++i) {
        std::cout << "- " << req.clubs[i] << ": " << req.clubCounts[i] << " player(s)" << std::endl;
    }

    return req;
}

// Check if squad meets requirements
inline bool DoesSquadMeetRequirements(const std::vector<Player>& squad, const SquadRequirements& req) {
    // Must have exactly 11 players
    if (squad.size() != 11) return false;

    // Check formation length matches squad size
    Formations formations;
    auto formationPositions = formations.getPositions(req.formation);
    if (formationPositions.size() != squad.size()) return false;

    // Check total squad value <= maxBudget
    int totalValue = calculateTotalSquadValue(squad);
    if (totalValue > req.maxBudget) return false;

    // Check average rating >= required minOverallRating
    int avgRating = calculateAverageRating(squad);
    if (avgRating < req.minOverallRating) return false;

    // Check chemistry >= minTeamChemistry
    int chem = calculateChem(squad);
    if (chem < req.minTeamChemistry) return false;

    // Check nations counts
    std::unordered_map<std::string, int> nationCountMap;
    for (const auto& player : squad) nationCountMap[player.nation]++;
    /*
    for (size_t i = 0; i < req.nations.size(); i++) {
        if (nationCountMap[req.nations[i]] < req.nationCounts[i]) return false;
    }
    */
    if (!req.nations.empty()) {
        for (size_t i = 0; i < req.nations.size(); i++) {
            if (nationCountMap[req.nations[i]] < req.nationCounts[i]) return false;
        }
    }

    // Check leagues counts
    std::unordered_map<std::string, int> leagueCountMap;
    for (const auto& player : squad) leagueCountMap[player.league]++;
    /*
    for (size_t i = 0; i < req.leagues.size(); i++) {
        if (leagueCountMap[req.leagues[i]] < req.leagueCounts[i]) return false;
    }
*/
    if (!req.leagues.empty()) {
        for (size_t i = 0; i < req.leagues.size(); i++) {
            if (leagueCountMap[req.leagues[i]] < req.leagueCounts[i]) return false;
        }
    }

    // Check clubs counts
    std::unordered_map<std::string, int> clubCountMap;
    for (const auto& player : squad) clubCountMap[player.team]++;
    /*
    for (size_t i = 0; i < req.clubs.size(); i++) {
        if (clubCountMap[req.clubs[i]] < req.clubCounts[i]) return false;
    }
*/
    if (!req.clubs.empty()) {
        for (size_t i = 0; i < req.clubs.size(); i++) {
            if (clubCountMap[req.clubs[i]] < req.clubCounts[i]) return false;
        }
    }
    return true;
}

#endif // REQUIREMENTS_H

