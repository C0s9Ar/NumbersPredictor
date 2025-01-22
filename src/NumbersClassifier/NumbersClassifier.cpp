#include "NumbersClassifier.h"
#include <algorithm>
#include <map>


NumbersClassifier::NumbersClassifier() {
	NeuralNetwork nn1("res/model/model1.nnm");
	NeuralNetwork nn2("res/model/model2.nnm");
	NeuralNetwork nn3("res/model/model3.nnm");
	NeuralNetwork nn4("res/model/model4.nnm");
	NeuralNetwork nn5("res/model/model5.nnm");
	NeuralNetwork nn6("res/model/model6.nnm");

	nets_.push_back(nn1);
	nets_.push_back(nn2);
	nets_.push_back(nn3);
	nets_.push_back(nn4);
	nets_.push_back(nn5);
	nets_.push_back(nn6);
}

std::vector<double> StretchBinaryImage(const std::vector<double>& input_image, int src_width = 28, int src_height = 28) {
	if (src_width * src_height != input_image.size()) 
		throw std::invalid_argument("Input image dimensions do not match the data size.");
	
	int min_x = src_width, max_x = 0, min_y = src_height, max_y = 0;
	for (int y = 0; y < src_height; ++y) {
		for (int x = 0; x < src_width; ++x) {
			if (input_image[y * src_width + x] > 0) {
				min_x = std::min(min_x, x);
				max_x = std::max(max_x, x);
				min_y = std::min(min_y, y);
				max_y = std::max(max_y, y);
			}
		}
	}

	if (min_x > max_x || min_y > max_y)
		return std::vector<double>(input_image.size(), 0);

	int object_width = max_x - min_x + 1;
	int object_height = max_y - min_y + 1;

	double scale_x = static_cast<double>(src_width) / object_width;
	double scale_y = static_cast<double>(src_height) / object_height;

	double scale = std::min(scale_x, scale_y);

	int new_width = static_cast<int>(object_width * scale);
	int new_height = static_cast<int>(object_height * scale);

	int offset_x = (src_width - new_width) / 2;
	int offset_y = (src_height - new_height) / 2;

	std::vector<double> output_image(src_width * src_height, 0);

	for (int y = 0; y < new_height; ++y) {
		for (int x = 0; x < new_width; ++x) {
			int src_x = static_cast<int>(min_x + x / scale);
			int src_y = static_cast<int>(min_y + y / scale);

			if (src_x >= 0 && src_x < src_width && src_y >= 0 && src_y < src_height) {
				output_image[(y + offset_y) * src_width + (x + offset_x)] =
					input_image[src_y * src_width + src_x];
			}
		}
	}

	return output_image;
}

int NumbersClassifier::Classify(const std::vector<double>& image) {
	auto stretched = StretchBinaryImage(image);
	std::vector<int> frequency(10, 0);
	for (auto net : nets_) {
		int predicted = net.PredictNumber(stretched);
		frequency[predicted]++;
	}
	return std::distance(frequency.begin(), std::max_element(frequency.begin(), frequency.end()));
}