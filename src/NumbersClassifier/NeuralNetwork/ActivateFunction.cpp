#include "ActivateFunction.h"
#include <stdexcept>

ActivateFunction::ActivateFunction(Function function) : act_func_(function) {}

void ActivateFunction::Use(std::vector<double>& vec) {
	switch (act_func_) {
	case SIGMOID:
		for (int i = 0; i < vec.size(); i++)
			vec[i] = 1 / (1 + exp(-vec[i]));
		break;

	case RELU:
		for (int i = 0; i < vec.size(); i++) {
			if (vec[i] < 0)
				vec[i] *= 0.01f;
			else if (vec[i] > 1)
				vec[i] = 1 + 0.01f * (vec[i] - 1);
		}
		break;

	case THX:
		for (int i = 0; i < vec.size(); i++) {
			if (vec[i] < 0)
				vec[i] = 0.01f * (exp(vec[i]) - exp(-vec[i])) / (exp(vec[i]) + exp(-vec[i]));
			else
				vec[i] = (exp(vec[i]) - exp(-vec[i])) / (exp(vec[i]) + exp(-vec[i]));
		}
		break;

	default:
		throw std::runtime_error("Invalid function");
		break;
	}
}

void ActivateFunction::UseDer(std::vector<double>& vec) {
	switch (act_func_) {
	case SIGMOID:
		for (int i = 0; i < vec.size(); i++)
			vec[i] = vec[i] * (1 - vec[i]);
		break;

	case RELU:
		for (int i = 0; i < vec.size(); i++) {
			if (vec[i] < 0 || vec[i] > 1)
				vec[i] = 0.01f;
			else
				vec[i] = 1;
		}
		break;

	case THX:
		for (int i = 0; i < vec.size(); i++) {
			if (vec[i] < 0)
				vec[i] = 0.01f * (1 - vec[i] * vec[i]);
			else
				vec[i] = 1 - vec[i] * vec[i];
		}
		break;

	default:
		throw std::runtime_error("Invalid function");
		break;
	}
}

double ActivateFunction::UseDer(double value) {
	switch (act_func_) {
	case SIGMOID:
		value = value * (1 - value);
		break;
	case RELU:
		if (value < 0 || value > 1)
			value = 0.01f;
		else
			value = 1;
		break;
	case THX:
		if (value < 0)
			value = 0.01f * (1 - value * value);
		else
			value = 1 - value * value;
		break;
	default:
		throw std::runtime_error("Invalid function");
		break;
	}
	return value;
}