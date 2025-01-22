#include "Box.h"

Box::Box(const BoxConfig& config, std::shared_ptr<Window> window) : Widget(config, window) {
	SetConfig(config);
}

void Box::SetConfig(const BoxConfig& config) {
	Widget::SetConfig(config);

	rect_.setFillColor(sf::Color::Red);
	rect_.setSize(size_);
	rect_.setPosition(pos_);
}


void Box::DrawSelf() const {
	if (show_)
		parent_window_->DrawObject(rect_);
}
void Box::HandleEventsSelf(const sf::Event& event) {}
