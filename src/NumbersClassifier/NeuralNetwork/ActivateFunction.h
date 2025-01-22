#pragma once
#include <vector>

enum Function { SIGMOID, RELU, THX };

class ActivateFunction {
public:

	ActivateFunction(Function func);
	void Use(std::vector<double>& vec);
	void UseDer(std::vector<double>& vec);
	double UseDer(double value);

	inline Function GetFunction() const { return act_func_; }

private:
	Function act_func_;
};

