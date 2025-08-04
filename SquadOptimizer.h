#ifndef SQUADOPTIMIZER_H
#define SQUADOPTIMIZER_H
#include "Player_Organizer.h"
#include "Requirements.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include <unordered_set>


//Node representation to be used in A*
struct SquadNode {
  std::vector<Player> currentSquad; // Players added so far
  std::unordered_set<std::string> usedNames; // Keeps track of already used players to prevent duplicates
  int positionIndex; // Which position in formation we’re filling next
  int costSoFar;     // Current squad value
  int estimatedTotalCost; // costSoFar + heuristic
  int totalChemistry; // Chemistry of the current squad

  bool operator>(const SquadNode& other) const {
    return estimatedTotalCost > other.estimatedTotalCost; // For min-heap
  }
};

//Helper function that estimates the potential chemistry boost a player would have to a squad
int estChemBoost(const Player& player, const std::vector<Player>& currentSquad) {
    int boost = 0;
    for(const Player& p : currentSquad) {
        if(player.nation == p.nation) boost++;
        if(player.team == p.team) boost++;
        if(player.league == p.league) boost++;
    }
    return boost;
}


//Heuristic [h(n)]: Estimates the remaining cost based on the cheapest player per position
int heuristic(const std::vector<std::string>& remainingPositions, const std::unordered_map<std::string, std::vector<Player>>& playerByPosition) {
  int estValue = 0;
  for (const auto& pos : remainingPositions) {
    if (playerByPosition.count(pos) && !playerByPosition.at(pos).empty()) {
      estValue += playerByPosition.at(pos)[0].value; //Use the cheapest player's value to minimize cost
    }
  }
  return estValue;
}


/*
  A* optimization function. A* is an informed search algorithm that starts at a specific starting node of a
  graph and aims to find a path to the given goal node having the smallest cost. At each iteration,
  A* selects the path that minimizes f(n) = g(n) + h(n) where
  - n is the next node on the path
  - g(n) is the cost of the path from the start node to n
  - h(n) is a heuristic function that estimates the cost of the cheapest path from n to the goal

  Information from https://en.wikipedia.org/wiki/A*_search_algorithm
*/

std::vector<Player> AStarSquadOptimizer(
    const std::vector<std::string>& formation,
    const std::unordered_map<std::string, std::vector<Player>>& playerByPosition,
    const SquadRequirements& req
) {

    std::priority_queue<SquadNode, std::vector<SquadNode>, std::greater<SquadNode>> pq; // The open set of nodes to try

    //Initialize the starting node. Represents an empty squad
    SquadNode start;
    start.positionIndex = 0;
    start.costSoFar = 0;
    start.estimatedTotalCost = heuristic(formation, playerByPosition);
    start.totalChemistry = 0;
    pq.push(start);

    //Main search Loop: Explores the lowest estimated-cost node at each step
    while (!pq.empty()) {
        SquadNode current = pq.top(); // Represents the best squad so far
        pq.pop();

        //Once a full squad is made, validate if it meets all requirements
        if (current.positionIndex == formation.size()) {
            if (DoesSquadMeetRequirements(current.currentSquad, req)) {
                return current.currentSquad; //Found a valid squad and returns it
            }
            continue; //No valid squad was found so we keep going
        }

        std::string currentPosition = formation[current.positionIndex];
        if (!playerByPosition.count(currentPosition)) continue;

        // Create and sort a list of the best candidates per position based on a score.
        // Score is calculated by finding each player's rating-value ratio and multiplying it by
        // the estimated chemistry boost they will have to the squad.
        // 0.1 is multiplied to the chemistry boost so that the algorithm doesn't heavily favor increasing chemistry
        std::vector<Player> candidates = playerByPosition.at(currentPosition);
        std::sort(candidates.begin(), candidates.end(), [&](const Player& a, const Player& b) {
            double aScore = (a.value > 0 ? (double)a.rating / a.value : a.rating) + 0.3 * estChemBoost(a, current.currentSquad);
            double bScore = (b.value > 0 ? (double)b.rating / b.value : b.rating) + 0.3 * estChemBoost(b, current.currentSquad);
            return aScore > bScore;
        });

        //Try all candidates for the current position
        for (const Player& p : candidates) {
            if (current.usedNames.count(p.name)) continue; //Checks if current player has already been selected
            if (current.costSoFar + p.value > req.maxBudget) continue; //Checks if we have gone above budget

            //Create the next state with the found player added to the squad
            SquadNode next = current;
            next.currentSquad.push_back(p);
            next.usedNames.insert(p.name); //Update the current list of used players
            next.positionIndex++; //Move on to the next position
            next.costSoFar += p.value; //Update the current cost of our squad
            next.totalChemistry = calculateChem(next.currentSquad); //Update the total chemistry of the squad

            // Checks if squad has 6 or more players in it already. Then checks if it is possible to reach
            // required rating or squad chemistry. If it is impossible, then skips the loop. This makes the
            // program work faster since we aren't trying every single player to fill a squad that is impossible to make
            if (next.currentSquad.size() >= 6) {
                int projectedRating = calculateAverageRating(next.currentSquad) * next.currentSquad.size();
                int remainingSlots = 11 - next.currentSquad.size();
                projectedRating += remainingSlots * 99;
                projectedRating /= 11;

                if (projectedRating < req.minOverallRating * 0.95) continue;

                int maxPossibleChem = next.totalChemistry + remainingSlots * 3;
                if (maxPossibleChem < req.minTeamChemistry * 0.95) continue;
            }

            // Compute remaining cost and push next node
            std::vector<std::string> remainingPositions(formation.begin() + next.positionIndex, formation.end());
            int h = heuristic(remainingPositions, playerByPosition);
            next.estimatedTotalCost = next.costSoFar + h;
            pq.push(next);
        }
    }

    return {}; //Case for where no squad is found
}

// Best-Fit Squad Builder
// Best-Fit (Greedy) Squad Builder Algorithm
// Builds a squad by selecting the best player (based on rating/value) for each position one by one
std::vector<Player> BestFitSquadOptimizer(
    const std::vector<std::string>& formation,
    const std::unordered_map<std::string, std::vector<Player>>& playerByPosition,
    const SquadRequirements& req)
{
    std::vector<Player> bestFitSquad;
    std::unordered_set<std::string> usedPlayers; // Used to prevent picking duplicate players
    int totalCost = 0;


    // Loops through each required position in the given formation
    for(const std::string& pos : formation) {
        if(!playerByPosition.count(pos)) continue;


        // Create and sort a list of best candidates per position based on a score.
        // Score is calculated by finding each player's rating-value ratio and multiplying it by
        // the estimated chemistry boost they will have to the squad.
        // 0.1 is multiplied to the chemistry boost so that the algorithm doesn't heavily favor increasing chemistry


        std::vector<Player> candidates = playerByPosition.at(pos);
        std::sort(candidates.begin(), candidates.end(), [&](const Player& a, const Player& b) {
            double aScore = (a.value > 0 ? (double)a.rating / a.value : a.rating)  + 0.1 * estChemBoost(a, bestFitSquad);
            double bScore = (b.value > 0 ? (double)b.rating / b.value : b.rating) + 0.1 * estChemBoost(b, bestFitSquad);
            return aScore > bScore;
        });

        bool found = false;
        int tried =0;

        for(const Player& p : candidates) {
            if(tried >= 250) break;
            if(usedPlayers.count(p.name)) continue;
            if(totalCost + p.value > req.maxBudget) continue;

            std::vector<Player> tempSquad = bestFitSquad;
            tempSquad.push_back(p);

            if (calculateChem(tempSquad) < req.minTeamChemistry && tempSquad.size() == 11) {
                ++tried;
                continue;
            }

            // Add found player to the squad and update state of the algorithm
            bestFitSquad.push_back(p);
            usedPlayers.insert(p.name);
            totalCost += p.value;
            found = true;
            break; // Uses the first valid player
        }

        if (!found) {
            return {}; // Means that this position could not be filled
        }
    }
    if(!DoesSquadMeetRequirements(bestFitSquad, req)) {
        return {}; //If not valid squad is found, return an empty team
    }

    return bestFitSquad;
}



#endif

