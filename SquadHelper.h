

#ifndef SQUADHELPER_H
#define SQUADHELPER_H

#include "Player_Organizer.h"
#include <unordered_map>
#include <vector>

int calculateChem(const std::vector<Player>& squad) {
    std::unordered_map<std::string, int> clubCount, nationCount, leagueCount;
    int totalChem = 0;

    // Count appearances
    for (const Player& p : squad) {
        clubCount[p.team]++;
        nationCount[p.nation]++;
        leagueCount[p.league]++;
    }

    for (const Player& p : squad) {
        int chem = 0;

        // Club chemistry
        int clubNum = clubCount[p.team];
        if (clubNum >= 7) chem += 3;
        else if (clubNum >= 4) chem += 2;
        else if (clubNum >= 2) chem += 1;

        // Nation chemistry
        int nationNum = nationCount[p.nation];
        if (nationNum >= 8) chem += 3;
        else if (nationNum >= 5) chem += 2;
        else if (nationNum >= 2) chem += 1;

        // League chemistry
        int leagueNum = leagueCount[p.league];
        if (leagueNum >= 8) chem += 3;
        else if (leagueNum >= 5) chem += 2;
        else if (leagueNum >= 3) chem += 1;

        // Clamp each player's chem to 3 max (optional if needed)
        if (chem > 3) chem = 3;

        totalChem += chem;
    }

    // Chemistry cap: 11 players × 3 = 33 max
    if (totalChem > 33) totalChem = 33;
    return totalChem;
}

int calculateAverageRating(const std::vector<Player>& squad) {
    if (squad.empty()) return 0;

    int totalRating = 0;
    for (const Player& p : squad) {
        totalRating += p.rating;
    }

    return totalRating / static_cast<int>(squad.size());
}

int calculateTotalSquadValue(const std::vector<Player>& squad) {
    int totalValue = 0;
    for (const Player& p : squad) {
        totalValue += p.value;
    }
    return totalValue;
}

#endif //SQUADHELPER_H
