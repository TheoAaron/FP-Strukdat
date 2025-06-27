#include "../../include/preference/criteria.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>

MultiCriteria::MultiCriteria() {
    weights = {
        {"distance", 0.25},
        {"time", 0.35},
        {"cost", 0.30},
    };
}

std::string MultiCriteria::getCriteria() {
    std::cout << "\n== Masukkan Bobot Preferensi ==\n";
    std::cout << "Total bobot harus bernilai 1.0\n";
    
    double total = 0.0;
    for (auto& [Criteria, weight] : weights) {
        std::cout << "Bobot untuk " << Criteria << " (current: " << weight << "): ";
        std::cin >> weight;
        total += weight;
    }
    
    if (std::abs(total - 1.0) > 1e-6) {
        std::cout << "Total bobot tidak sama dengan 1.0, normalisasi dilakukan\n";
        for (auto& [Criteria, weight] : weights) {
            weight /= total;
        }
    }
    
    return "multi";
}

std::map<std::string, double> MultiCriteria::getWeights() const {
    return weights;
}

void MultiCriteria::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open weights file: " + filename);
    }

    weights.clear();
    std::string line;
    
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string key;
        double value;
        
        if (getline(ss, key, ',') && (ss >> value)) {
            weights[key] = value;
        }
    }
    
    double total = std::accumulate(weights.begin(), weights.end(), 0.0,
        [](double sum, const auto& pair) { return sum + pair.second; });
        
    if (total > 0) {
        for (auto& [key, value] : weights) {
            value /= total;
        }
    }
}

double MultiCriteria::calculateScore(
    const std::map<std::string, double>& attributes) const 
{
    double score = 0.0;
    for (const auto& [Criteria, weight] : weights) {
        if (attributes.find(Criteria) != attributes.end()) {
            score += weight * attributes.at(Criteria);
        }
    }
    return score;
}