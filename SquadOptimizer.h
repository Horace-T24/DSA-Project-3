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
  std::vector<Player> currentSquad;
  std::unordered_set<std::string> usedNames;
  int positionIndex; // Which position in formation we’re filling next
  int costSoFar;     // e.g., total value so far
  int estimatedTotalCost; // costSoFar + heuristic
  int totalChemistry;

  bool operator>(const SquadNode& other) const {
    return estimatedTotalCost > other.estimatedTotalCost; // For min-heap
  }
};

//Heuristic [h(n)]
int heuristic(const std::vector<std::string>& remainingPositions, const std::unordered_map<std::string, std::vector<Player>>& playerByPosition) {
  int estValue = 0;
  for (const auto& pos : remainingPositions) {
    if (playerByPosition.count(pos) && !playerByPosition.at(pos).empty()) {
      estValue += playerByPosition.at(pos)[0].value; // cheapest due to sorted list
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
    using PQ = std::priority_queue<SquadNode, std::vector<SquadNode>, std::greater<SquadNode>>;
    PQ openSet;

    SquadNode start;
    start.positionIndex = 0;
    start.costSoFar = 0;
    start.estimatedTotalCost = heuristic(formation, playerByPosition);
    start.totalChemistry = 0;

    openSet.push(start);

    while (!openSet.empty()) {
        SquadNode current = openSet.top();
        openSet.pop();

        if (current.positionIndex == formation.size()) {
            if (DoesSquadMeetRequirements(current.currentSquad, req)) {
                return current.currentSquad;
            }
            continue;
        }

        std::string currentPosition = formation[current.positionIndex];

        if (!playerByPosition.count(currentPosition)) continue;

        for (const Player& p : playerByPosition.at(currentPosition)) {
            if (current.usedNames.count(p.name)) continue;
            if (current.costSoFar + p.value > req.maxBudget) continue;

            SquadNode next = current;
            next.currentSquad.push_back(p);
            next.usedNames.insert(p.name);
            next.positionIndex++;
            next.costSoFar += p.value;
            next.totalChemistry = calculateChem(next.currentSquad);

            // Prune bad branches
            if (next.currentSquad.size() >= 5) {
                int projectedRating = calculateAverageRating(next.currentSquad) * next.currentSquad.size();
                int remainingSlots = 11 - next.currentSquad.size();
                projectedRating += remainingSlots * 99;
                projectedRating /= 11;

                if (projectedRating < req.minOverallRating) continue;

                int maxPossibleChem = next.totalChemistry + remainingSlots * 3;
                if (maxPossibleChem < req.minTeamChemistry) continue;
            }

            std::vector<std::string> remainingPositions(
                formation.begin() + next.positionIndex, formation.end());

            int h = heuristic(remainingPositions, playerByPosition);
            next.estimatedTotalCost = next.costSoFar + h;

            openSet.push(next);
        }
    }

    return {};
}





#endif

