#pragma once
#include <vector>
#include "NeuralNetwork/NeuralNetwork.h"

class NumbersClassifier {
private:
	std::vector<NeuralNetwork> nets_;

public:
	NumbersClassifier();

	int Classify(const std::vector<double> &image);
};