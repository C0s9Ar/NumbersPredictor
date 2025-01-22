#pragma once
#include "ActivateFunction.h"
#include "Matrix/Matrix.h"
#include <string>

class NeuralNetwork {
public:

	NeuralNetwork(std::vector<size_t> model, Function act_func);

	NeuralNetwork(const std::string& filename);


	void Train(std::string dataset_file, double learning_rate, size_t epoch);

	void Test(std::string dataset_file);

	int PredictNumber(const std::vector<double>& inputs);

	std::vector<double> PredictValues(const std::vector<double>& inputs);

	void SaveWeights(const std::string& filename);
private:

	void LoadWeights(const std::string& filename);

	void Backpropagation(double expect);

	void WeightsUpdate(double learning_rate);

	void SetInput(const std::vector<double>& values);
	std::vector<double> ForwardFeed();


	std::vector<size_t> model_;
	ActivateFunction act_func_;

	std::vector<Matrix> weights_;

	std::vector<std::vector<double>> neurons_val_, neurons_err_;

	std::vector<std::vector<double>> bias_;
	std::vector<double> neuron_bias_val_;

};

