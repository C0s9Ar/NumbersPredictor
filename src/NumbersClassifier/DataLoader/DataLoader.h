#pragma once
#include <vector>
#include <string>

struct DataPoint {
    std::vector<double> inputs;
    size_t label;
};

std::vector<DataPoint> LoadData(const std::string& filename);
