#ifndef PLAYER_ORGANIZER_H
#define PLAYER_ORGANIZER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct Player {
    std::string name;
    std::string position;    // e.g., "ST", "CM", "GK", etc.
    int rating;              // Overall rating
    std::string formation;   // Formation suitability, e.g., "4-4-2", "4-3-3"
    std::string nation;
    std::string league;
    std::string team;

    Player() = default;
    Player(const std::string& name,
           const std::string& position,
           int rating,
           const std::string& formation,
           const std::string& nation,
           const std::string& league,
           const std::string& team)
        : name(name), position(position), rating(rating),
          formation(formation), nation(nation), league(league), team(team) {}
};

class PlayerOrganizer {
private:
    std::vector<Player> players;

    // Optional: Indexes for fast filtering
    std::unordered_multimap<std::string, size_t> nationIndex;
    std::unordered_multimap<std::string, size_t> leagueIndex;
    std::unordered_multimap<std::string, size_t> teamIndex;
    std::unordered_multimap<std::string, size_t> formationIndex;
    // Could add rating buckets or just filter linearly

    void indexPlayer(const Player& player, size_t idx);

public:
    PlayerOrganizer() = default;

    // Add a player and update indexes
    void AddPlayer(const Player& player);

    // Get all players
    const std::vector<Player>& GetAllPlayers() const;

    // Filtering functions
    std::vector<Player> GetPlayersByRating(int minRating) const;
    std::vector<Player> GetPlayersByFormation(const std::string& formation) const;
    std::vector<Player> GetPlayersByNation(const std::string& nation) const;
    std::vector<Player> GetPlayersByLeague(const std::string& league) const;
    std::vector<Player> GetPlayersByTeam(const std::string& team) const;

    // Combined filtering: returns players matching all non-empty filters and min rating
    std::vector<Player> FilterPlayers(int minRating = 0,
                                      const std::string& formation = "",
                                      const std::string& nation = "",
                                      const std::string& league = "",
                                      const std::string& team = "") const;
};

#endif // PLAYER_ORGANIZER_H
