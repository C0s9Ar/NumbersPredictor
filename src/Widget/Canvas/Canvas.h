#pragma once
#include "Pixel.h"
#include "../Widget.h"
#include <vector>

struct CanvasConfig : WidgetConfig {
	CanvasConfig(sf::Vector2f pos, unsigned size, unsigned pixel_size) : 
		WidgetConfig(sf::Vector2f(pixel_size * size, pixel_size * size), pos),
		WIDTH(size), HEIGHT(size), PIXEL_SIZE(pixel_size) { }	
	unsigned WIDTH = 28; // MNIST database is 28x28
	unsigned HEIGHT = 28;
	unsigned PEN_THICKNESS = 2;

	std::shared_ptr<sf::Color> CLEAR_COLOR = std::make_shared<sf::Color>(195, 195, 195);
	std::shared_ptr<sf::Color> DRAW_COLOR = std::make_shared<sf::Color>(22, 22, 22);

	unsigned PIXEL_SIZE = 10;
};


class Canvas : public Widget {
public:
	Canvas(const CanvasConfig& config, std::shared_ptr<Window> window);

	inline const std::vector<Pixel>& GetPixelMap() const {
		return pixels_;
	}

	Pixel* GetPixelAt(unsigned x, unsigned y);

	void ClearCanvas();

	void SetThickness(unsigned thickness);

	void SetConfig(const CanvasConfig& config);

	std::vector<double> GetMapInputs();
private:

	void DrawAt(sf::Vector2i& pos, sf::Vector2i& prev_pos, unsigned thickness);
	void DrawThickPixel(int x, int y, unsigned thickness);

	void DrawSelf() const override;
	void HandleEventsSelf(const sf::Event& event) override;

	unsigned width_;
	unsigned height_;

	std::vector<Pixel> pixels_;
	unsigned pen_thickness_ = 1;

	unsigned pixel_size_;

	std::shared_ptr<sf::Color> clear_color_;
	std::shared_ptr<sf::Color> draw_color_;
};