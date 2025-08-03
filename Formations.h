#ifndef FORMATIONS_H
#define FORMATIONS_H

#include <string>
#include <vector>
#include <unordered_map>

class Formations {
private:
    std::unordered_map<std::string, std::vector<std::string>> formationMap;

public:
    Formations() {
        formationMap = {
            {"3-1-4-2", {"GK", "CB", "CB", "CB", "CDM", "LM", "CM", "CM", "RM", "ST", "ST"}},
            {"3-4-1-2", {"GK", "CB", "CB", "CB", "LM", "CM", "CM", "RM", "CAM", "ST", "ST"}},
            {"3-4-2-1", {"GK", "CB", "CB", "CB", "LM", "CM", "CM", "RM", "LF", "RF", "ST"}},
            {"3-4-3", {"GK", "CB", "CB", "CB", "LM", "CM", "CM", "RM", "LW", "RW", "ST"}},
            {"3-5-1-1", {"GK", "CB", "CB", "CB", "CDM", "LM", "CM", "CM", "RM", "CAM", "ST"}},
            {"3-5-2", {"GK", "CB", "CB", "CB", "CDM", "CDM", "LM", "CAM", "RM", "ST", "ST"}},

            {"4-1-2-1-2", {"GK", "LB", "CB", "CB", "RB", "CDM", "LM", "RM", "CAM", "ST", "ST"}},
            {"4-1-2-1-2 Wide", {"GK", "LB", "CB", "CB", "RB", "CDM", "LM", "RM", "CAM", "ST", "ST"}},
            {"4-1-4-1", {"GK", "LB", "CB", "CB", "RB", "CDM", "LM", "CM", "CM", "RM", "ST"}},
            {"4-2-2-2", {"GK", "LB", "CB", "CB", "RB", "CDM", "CDM", "LAM", "RAM", "ST", "ST"}},
            {"4-2-3-1", {"GK", "LB", "CB", "CB", "RB", "CDM", "CDM", "CAM", "LM", "RM", "ST"}},
            {"4-2-3-1 Wide", {"GK", "LB", "CB", "CB", "RB", "CDM", "CDM", "CAM", "LM", "RM", "ST"}},
            {"4-3-1-2", {"GK", "LB", "CB", "CB", "RB", "CM", "CM", "CM", "CAM", "ST", "ST"}},
            {"4-3-2-1", {"GK", "LB", "CB", "CB", "RB", "CM", "CM", "CM", "LF", "RF", "ST"}},
            {"4-3-3", {"GK", "LB", "CB", "CB", "RB", "CM", "CM", "CM", "LW", "RW", "ST"}},
            {"4-3-3 Attack", {"GK", "LB", "CB", "CB", "RB", "CM", "CAM", "CDM", "LW", "RW", "ST"}},
            {"4-3-3 Defend", {"GK", "LB", "CB", "CB", "RB", "CDM", "CDM", "CM", "LW", "RW", "ST"}},
            {"4-3-3 Flat", {"GK", "LB", "CB", "CB", "RB", "CM", "CM", "CM", "LW", "RW", "ST"}},
            {"4-3-3 False 9", {"GK", "LB", "CB", "CB", "RB", "CM", "CM", "CM", "LW", "RW", "CF"}},
            {"4-4-1-1", {"GK", "LB", "CB", "CB", "RB", "LM", "CM", "CM", "RM", "CF", "ST"}},
            {"4-4-2", {"GK", "LB", "CB", "CB", "RB", "LM", "CM", "CM", "RM", "ST", "ST"}},
            {"4-4-2 Flat", {"GK", "LB", "CB", "CB", "RB", "LM", "CM", "CM", "RM", "ST", "ST"}},
            {"4-4-2 Holding", {"GK", "LB", "CB", "CB", "RB", "LM", "CDM", "CDM", "RM", "ST", "ST"}},

            {"5-2-1-2", {"GK", "LWB", "CB", "CB", "CB", "RWB", "CM", "CM", "CAM", "ST", "ST"}},
            {"5-2-2-1", {"GK", "LWB", "CB", "CB", "CB", "RWB", "CM", "CM", "LF", "RF", "ST"}},
            {"5-3-2", {"GK", "LWB", "CB", "CB", "CB", "RWB", "CM", "CM", "CM", "ST", "ST"}},
            {"5-4-1", {"GK", "LWB", "CB", "CB", "CB", "RWB", "LM", "CM", "CM", "RM", "ST"}}
        };
    }

    std::vector<std::string> getPositions(const std::string& formationName) const {
        auto it = formationMap.find(formationName);
        if (it != formationMap.end()) {
            return it->second;
        }
        return {"Invalid formation"};
    }

    std::vector<std::string> getAllFormationNames() const {
        std::vector<std::string> keys;
        for (const auto& kv : formationMap) {
            keys.push_back(kv.first);
        }
        return keys;
    }
};

#endif // FORMATIONS_H
