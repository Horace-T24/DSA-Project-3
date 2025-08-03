#ifndef SQUADOPTIMIZER_H
#define SQUADOPTIMIZER_H
#include "Player_Organizer.h"
#include "Requirements.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>



//Node representation to be used in A*
struct SquadState {
  std::vector<Player> squad;
  std::vector<std::string> positionsLeft;
  int costSoFar; //Holds the current squad cost
  int estRemaining; //Heuristic (cost to fill remaining spots

  bool operator>(const SquadState& other) const {
    return (costSoFar + estRemaining) > (other.costSoFar + other.estRemaining);
  }
};

//Min heap implementation to store the top N squads
class SquadHeap {
private:
  struct Compare {
    bool operator()(const std::vector<Player>& a,const std::vector<Player>& b) {
      return calculateChem(a) < calculateChem(b);
    }
  };
  std::priority_queue<std::vector<Player>, std::vector<std::vector<Player>>, Compare> heap;
  size_t maxSize;

public:
  SquadHeap(size_t n) : maxSize(n) {}

  void insert(const std::vector<Player>& squad) {
    heap.push(squad);
    if(heap.size() > maxSize) heap.pop();
  }

  std::vector<std::vector<Player>> getTopSquads() {
    std::vector<std::vector<Player>> result;
    while(!heap.empty()) {
      result.push_back(heap.top());
      heap.pop();
    }
    std::reverse(result.begin(),result.end());
    return result;
  }
};

//Heuristic [h(n)]: currently just the avg value of lowest rated players
int estimateRemainingCost(int remaining, const std::vector<Player>& candidates) {
  int est=0;
  int count = 0;
  for (const Player& p:candidates) {
    est+=p.value;
    count++;
    if(count>=remaining) break;
  }
  return est;
}

/*
  A* optimization function. A* is an informed search algorithm that starts at a specific starting node of a
  graph and aims to find a path to the given goal node having the smallest cost. At each iteration,
  A* selects the path that minimizes f(n) = g(n) + h(n) where
  - n is the next nodeon the path
  - g(n) is the cost of the path from the start node to n
  - h(n) is a heuristic function that estimates the cost of the cheapest path from n to the goal

  Information from https://en.wikipedia.org/wiki/A*_search_algorithm
*/
std::vector<std::vector<Player>> AStarSquadOptimizer(const std::vector<Player>& allCandidates,
                                                     const std::vector<std::string>& formation,
                                                     const SquadRequirements& req,
                                                     int topN =5) {

  SquadHeap topSquads(topN);
  std::priority_queue<SquadState, std::vector<SquadState>, std::greater<SquadState>> pq;

  SquadState start;
  start.positionsLeft = formation;
  start.costSoFar = 0;
  start.estRemaining = estimateRemainingCost(11, allCandidates);

  pq.push(start);

  while (!pq.empty()) {
    SquadState current = pq.top();
    pq.pop();

    if(current.positionsLeft.empty()) {
      if(DoesSquadMeetRequirements(current.squad, req)) {
        topSquads.insert(current.squad);
      }
      continue;
    }

    std::string nextPos = current.positionsLeft[0];
    std::vector<std::string> restPositions(current.positionsLeft.begin()+1, current.positionsLeft.end());

    for(const Player& p : allCandidates) {
      if(std::find(p.positions.begin(), p.positions.end(), nextPos) == p.positions.end()) continue;
      if(std::find_if(current.squad.begin(),current.squad.end(),
        [&](const Player& x){return x.name == p.name;}) != current.squad.end()) continue;


      SquadState next = current;
      next.squad.push_back(p);
      next.positionsLeft = restPositions;
      next.costSoFar += p.value;
      next.estRemaining = estimateRemainingCost((int)next.positionsLeft.size(), allCandidates);

      if(next.costSoFar + next.estRemaining <= req.maxBudget) {
        pq.push(next);
      }
    }
  }

  return topSquads.getTopSquads();
}




#endif

