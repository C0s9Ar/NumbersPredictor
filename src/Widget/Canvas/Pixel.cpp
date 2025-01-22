#include "Pixel.h"


Pixel::Pixel(unsigned size, const sf::Vector2f& pos, const std::shared_ptr<sf::Color>& clear_color, const std::shared_ptr<sf::Color>& draw_color, bool state) : size_(size), state_(state) {
	rect_ = sf::RectangleShape(sf::Vector2f(size_, size_));
	rect_.setPosition(pos);
	clear_color_ = clear_color;
	draw_color_ = draw_color;
	SetState(state_);
}

void Pixel::SetState(bool state) {
	state_ = state;
	state_ ? rect_.setFillColor(*draw_color_) : rect_.setFillColor(*clear_color_);
}