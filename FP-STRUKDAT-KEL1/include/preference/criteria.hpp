#pragma once

#include <string>
#include <map>

enum class Criteria {
    TIME,
    DISTANCE,
    COST
};

class MultiCriteria {
public:
    MultiCriteria();
    std::string getCriteria();
    std::map<std::string, double> getWeights() const;
    void loadFromFile(const std::string& filename);
    double calculateScore(const std::map<std::string, double>& attributes) const;
private:
    std::map<std::string, double> weights;
};
