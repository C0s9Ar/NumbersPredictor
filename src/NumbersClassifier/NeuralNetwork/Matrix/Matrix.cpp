#include "Matrix.h"
#include <stdexcept>
#include <omp.h>
#include <thread>
#include <random>

#define THREADS std::thread::hardware_concurrency() / 3

Matrix::Matrix(size_t row, size_t col) :
    row_(row), col_(col), matrix_(std::vector<std::vector<double>>(row, std::vector<double>(col, 0))) {
}

void Matrix::Rand() {
    double std_dev = std::sqrt(2.0 / (row_ + col_)); // Xavier initialization
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, std_dev);

    for (int i = 0; i < row_; i++) {
        for (auto& ele : matrix_[i]) {
            ele = distribution(generator);
        }
    }
}

void Matrix::Multi(const Matrix& mat, const std::vector<double>& vec, std::vector<double>& result) {
    if (mat.col_ != vec.size())
        throw std::runtime_error("Invalid multiplication!");

#pragma omp parallel for num_threads(THREADS)

    for (int i = 0; i < mat.row_; i++) {
        double buffer = 0;
        for (int j = 0; j < mat.col_; j++) {
            buffer += mat.matrix_[i][j] * vec[j];
        }
        result[i] = buffer;
    }
}

void Matrix::SumVector(std::vector<double>& a, const std::vector<double>& b, int n) {
#pragma omp parallel for num_threads(THREADS)

    for (int i = 0; i < n; i++) {
        a[i] += b[i];
    }
}

void Matrix::MultiTranspose(const Matrix& mat, const std::vector<double>& vec, std::vector<double>& result) {
    if (mat.row_ != vec.size())
        throw std::runtime_error("Invalid multiplication!");

#pragma omp parallel for num_threads(THREADS)

    for (int i = 0; i < mat.col_; i++) {
        double buffer = 0;
        for (int j = 0; j < mat.row_; j++) {
            buffer += mat.matrix_[j][i] * vec[j];
        }
        result[i] = buffer;
    }
}

double& Matrix::operator()(int i, int j) {
    return matrix_[i][j];
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (const auto& vec : matrix.matrix_)
        for (const auto& ele : vec)
            os << ele << " ";
    return os;
}

std::istream& operator>>(std::istream& is, Matrix& matrix) {
    for (auto& vec : matrix.matrix_)
        for (auto& ele : vec)
            is >> ele;
    return is;
}
