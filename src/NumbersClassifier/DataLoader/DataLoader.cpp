#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

std::vector<DataPoint> LoadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::vector<DataPoint> dataset;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        DataPoint point;
        point.inputs = std::vector<double>(784, 0);

        std::istringstream label_stream(line);
        label_stream >> point.label;

        for (size_t row = 0; row < 28; ++row) {
            if (!std::getline(file, line)) {
                throw std::runtime_error("Unexpected end of file while reading image data");
            }

            for (size_t col = 0; col < 28; ++col) {
                if (line[col] == '1') {
                    point.inputs[row * 28 + col] = 1.0;
                }
                else {
                    point.inputs[row * 28 + col] = 0.0;
                }
            }
        }

        dataset.push_back(point);
    }

    return dataset;
}
