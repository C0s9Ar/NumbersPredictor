#include "Slider.h"

Slider::Slider(const SliderConfig& config, std::shared_ptr<Window> window) : Widget(config, window) {
	SetConfig(config);
}

void Slider::SetConfig(const SliderConfig& config) {
	Widget::SetConfig(config);

	delta_ = config.DELTA;
	value_ = std::min(config.VALUE, delta_);

	knob_color_ = config.KNOB_COLOR;
	line_color_ = config.LINE_COLOR;
	background_line_color_ = config.BACKGROUND_LINE_COLOR;

	line_to_knob_ratio_ = config.LINE_TO_KNOB_RATIO;
	on_set_handler_ = config.ON_SET_EVENT;
	if (on_set_handler_)
		on_set_handler_(value_);

	float knob_width = size_.x / delta_;
	knob_.setFillColor(knob_color_);
	knob_.setSize({ knob_width, size_.y });
	knob_.setPosition({ pos_.x + size_.x - knob_width, pos_.y });

	float line_height = size_.y * line_to_knob_ratio_;
	float line_y = pos_.y + (size_.y - line_height) / 2.0f;
	line_.setFillColor(line_color_);
	line_.setSize({ size_.x - knob_width, line_height });
	line_.setPosition({ pos_.x, line_y });

	background_line_.setFillColor(background_line_color_);
	background_line_.setSize({ size_.x, line_height });
	background_line_.setPosition({ pos_.x, line_y });
	
	SetValue(value_);
}

void Slider::DrawSelf() const {
	parent_window_->DrawObject(background_line_);
	parent_window_->DrawObject(line_);
	parent_window_->DrawObject(knob_);
}

void Slider::HandleEventsSelf(const sf::Event& event) {
	sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*parent_window_->GetRenderWindowPtr()));

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (knob_.getGlobalBounds().contains(mouse_pos)) {
			dragging_ = true;
			mouse_offset_ = mouse_pos - knob_.getPosition();
		}
		else if (background_line_.getGlobalBounds().contains(mouse_pos)) {
			MoveKnobTo(mouse_pos.x);
			dragging_ = true;
		}
	}
	else if (event.type == sf::Event::MouseMoved && dragging_) {
		MoveKnobTo(mouse_pos.x - mouse_offset_.x);
	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		dragging_ = false;
		if (on_set_handler_) on_set_handler_(value_ + 1);
	}
}

void Slider::SetValue(unsigned value) {
	value_ = std::min(value, delta_ - 1);

	float length = background_line_.getSize().x;
	float min_x = background_line_.getPosition().x;

	knob_.setPosition({ min_x + (length / delta_) * value_, knob_.getPosition().y });
	line_.setSize({ knob_.getPosition().x - min_x, line_.getSize().y });
}

void Slider::MoveKnobTo(float x_pos) {
	float min_x = background_line_.getPosition().x;
	float max_x = min_x + background_line_.getSize().x - knob_.getSize().x;

	x_pos = std::clamp(x_pos, min_x, max_x);

	knob_.setPosition({ x_pos, knob_.getPosition().y });

	unsigned new_value = static_cast<unsigned>(((x_pos - min_x) / background_line_.getSize().x) * delta_);
	SetValue(new_value);
}
