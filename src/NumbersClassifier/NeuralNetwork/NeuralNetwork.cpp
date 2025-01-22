#include "../DataLoader/DataLoader.h"
#include "NeuralNetwork.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <random>

NeuralNetwork::NeuralNetwork(std::vector<size_t> model, Function act_func)
    : model_(model), act_func_(act_func) {
    weights_.reserve(model.size() - 1);
    bias_ = std::vector<std::vector<double>>(model.size() - 1, std::vector<double>());

    for (int i = 0; i < model.size() - 1; i++) {
        Matrix matrix(model[i + 1], model[i]);
        matrix.Rand();
        weights_.push_back(matrix);

        bias_[i] = std::vector<double>(model[i + 1], 0);
        for (int j = 0; j < model[i + 1]; j++)
            bias_[i][j] = (rand() % 50) * 0.06f / (model_[i] + 15);
    }

    neurons_val_ = std::vector<std::vector<double>>(model.size());
    neurons_err_ = std::vector<std::vector<double>>(model.size());

    for (size_t i = 0; i < model.size(); ++i) {
        neurons_val_[i] = std::vector<double>(model[i]);
        neurons_err_[i] = std::vector<double>(model[i]);
    }

    neuron_bias_val_ = std::vector<double>(model.size() - 1, 1);
}

NeuralNetwork::NeuralNetwork(const std::string& filename) : act_func_(RELU) {
    LoadWeights(filename);
    neurons_val_ = std::vector<std::vector<double>>(model_.size());
    neurons_err_ = std::vector<std::vector<double>>(model_.size());

    for (size_t i = 0; i < model_.size(); ++i) {
        neurons_val_[i] = std::vector<double>(model_[i]);
        neurons_err_[i] = std::vector<double>(model_[i]);
    }

    neuron_bias_val_ = std::vector<double>(model_.size() - 1, 1);
}

void NeuralNetwork::SetInput(const std::vector<double>& values) {
    if (neurons_val_[0].size() != values.size())
        throw std::runtime_error("Error input values");
    for (int i = 0; i < values.size(); i++)
        neurons_val_[0][i] = values[i];
}

std::vector<double> NeuralNetwork::ForwardFeed() {
    for (int i = 1; i < model_.size(); i++) {
        Matrix::Multi(weights_[i - 1], neurons_val_[i - 1], neurons_val_[i]);
        Matrix::SumVector(neurons_val_[i], bias_[i - 1], model_[i]);
        act_func_.Use(neurons_val_[i]);
    }

    return neurons_val_.back();
}

void NeuralNetwork::Backpropagation(double expect) {
    for (int i = 0; i < model_.back(); i++) {
        if (i != (int)expect)
            neurons_err_.back()[i] = -neurons_val_.back()[i] * act_func_.UseDer(neurons_val_.back()[i]);
        else
            neurons_err_.back()[i] = (1 - neurons_val_.back()[i]) * act_func_.UseDer(neurons_val_.back()[i]);
    }

    for (int i = model_.size() - 2; i > 0; i--) {
        Matrix::MultiTranspose(weights_[i], neurons_err_[i + 1], neurons_err_[i]);
        for (int j = 0; j < model_[i]; j++) {
            neurons_err_[i][j] *= act_func_.UseDer(neurons_val_[i][j]);
        }
    }
}

void NeuralNetwork::WeightsUpdate(double learning_rate) {
    for (int i = 0; i < model_.size() - 1; i++) {
        for (int j = 0; j < model_[i + 1]; j++) {
            for (int k = 0; k < model_[i]; k++) {
                weights_[i](j, k) += neurons_val_[i][k] * neurons_err_[i + 1][j] * learning_rate;
            }
        }
    }

    for (int i = 0; i < model_.size() - 1; i++) {
        for (int j = 0; j < model_[i + 1]; j++) {
            bias_[i][j] += neurons_err_[i + 1][j] * learning_rate;
        }
    }
}

void NeuralNetwork::SaveWeights(const std::string& filename) {
    std::ofstream out_file(filename, std::ios::trunc);
    if (!out_file) {
        std::cerr << "Error opening file for saving weights!" << std::endl;
        return;
    }

    // Model
    for (const auto& layer_size : model_) {
        out_file << layer_size << " ";
    }
    out_file << std::endl;

    // Weights
    for (const auto& mat : weights_) {
        out_file << mat << std::endl;
    }

    // Biases
    for (const auto& vec : bias_) {
        for (const auto& ele : vec) {
            out_file << ele << " ";
        }
        out_file << std::endl;
    }

    // Activate Function
    out_file << (int)act_func_.GetFunction();

    out_file.close();
}

void NeuralNetwork::LoadWeights(const std::string& filename) {
    std::ifstream in_file(filename);
    if (!in_file) {
        std::cerr << "Error loading the model!" << std::endl;
        return;
    }

    std::string line;

    // Model
    std::getline(in_file, line);
    std::istringstream model_stream(line);
    model_.clear();
    size_t size;
    while (model_stream >> size) {
        model_.push_back(size);
    }

    // Weights
    weights_.clear();
    for (size_t i = 0; i < model_.size() - 1; ++i) {
        std::getline(in_file, line);
        std::istringstream weight_stream(line);
        Matrix mat(model_[i + 1], model_[i]);
        for (size_t r = 0; r < mat.Rows(); ++r) {
            for (size_t c = 0; c < mat.Cols(); ++c) {
                weight_stream >> mat(r, c);
            }
        }
        weights_.push_back(mat);
    }

    // Biases
    bias_.clear();
    for (size_t i = 0; i < model_.size() - 1; ++i) {
        std::getline(in_file, line);
        std::istringstream bias_stream(line);
        std::vector<double> vec;
        double bias;
        while (bias_stream >> bias) {
            vec.push_back(bias);
        }
        bias_.push_back(vec);
    }

    // Activate Function
    std::getline(in_file, line);
    std::istringstream act_fn_stream(line);
    int act_fn_number = 0;
    act_fn_stream >> act_fn_number;
    act_func_ = ActivateFunction::ActivateFunction((Function)act_fn_number);

    in_file.close();
}

void NeuralNetwork::Train(std::string dataset_file, double learning_rate, size_t epoch) {
    auto dataset = LoadData(dataset_file);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(dataset.begin(), dataset.end(), g);

    for (int i = 0; i < epoch; i++) {
        double ra = 0;

        auto current_time = std::chrono::high_resolution_clock::now();

        for (int j = 0; j < dataset.size(); j++) {
            SetInput(dataset[j].inputs);

            std::vector<double> predict = ForwardFeed();
            int predicted_label = std::distance(predict.begin(), std::max_element(predict.begin(), predict.end()));

            if (predicted_label != dataset[j].label) {
                Backpropagation(dataset[j].label);
                WeightsUpdate(learning_rate * exp(-i / (float)epoch));
            }
            else
                ra++;
        }

        auto elapsed_time = std::chrono::high_resolution_clock::now() - current_time;
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time);

        std::cout << "epoch: " << i << "\tra: " << ra / (double)dataset.size() << "\ttime: " << duration_ms.count() << " ms" << std::endl;
    }
}


void NeuralNetwork::Test(std::string dataset_file) {
    auto dataset = LoadData(dataset_file);
    double ra = 0;
    auto current_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < dataset.size(); i++) {
        SetInput(dataset[i].inputs);
        std::vector<double> output = ForwardFeed();
        int predicted_label = std::distance(output.begin(), std::max_element(output.begin(), output.end()));

        if (predicted_label == dataset[i].label)
            ra++;
    }

    auto elapsed_time = std::chrono::high_resolution_clock::now() - current_time;
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time);

    std::cout << "test results: " << ra / (double)dataset.size() << "\ttime: " << duration_ms.count() << " ms" << std::endl;
}

int NeuralNetwork::PredictNumber(const std::vector<double>& inputs) {
    SetInput(inputs);
    std::vector<double> output = ForwardFeed();
    return std::distance(output.begin(), std::max_element(output.begin(), output.end()));
}

std::vector<double> NeuralNetwork::PredictValues(const std::vector<double>& inputs) {
    SetInput(inputs);
    std::vector<double> output = ForwardFeed();
    return output;
}
