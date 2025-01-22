#pragma once
#include <vector>
#include <iostream>
class Matrix {
public:
	Matrix(size_t row, size_t col);
	
	void Rand();
	static void Multi(const Matrix& mat, const std::vector<double>& vec, std::vector<double> &result);
	static void MultiTranspose(const Matrix& mat, const std::vector<double>& vec, std::vector<double>& result);
	static void SumVector(std::vector<double> &a, const std::vector<double>& b, int n);
	
	double& operator() (int i, int j);

	friend std::ostream& operator<< (std::ostream& os, const Matrix& matrix);
	friend std::istream& operator>> (std::istream& is, Matrix& matrix);

	inline size_t Rows() const { return row_; }
	inline size_t Cols() const { return col_; }

private:
	std::vector<std::vector<double>> matrix_;
	size_t row_, col_;
};

