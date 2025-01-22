#pragma once
#include "../Widget.h"
#include <functional>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

struct SliderConfig : WidgetConfig {
	SliderConfig(sf::Vector2f size, sf::Vector2f pos) : WidgetConfig(size, pos) {}

	unsigned DELTA = 5;
	unsigned VALUE = DELTA / 2;

	sf::Color KNOB_COLOR = sf::Color(40, 40, 40);
	sf::Color LINE_COLOR = sf::Color(35, 35, 35);
	sf::Color BACKGROUND_LINE_COLOR = sf::Color(52, 52, 52);

	float LINE_TO_KNOB_RATIO = 0.8f; // Note: monitors cant display float pixels. Unfortunately :(

	std::function<void(unsigned)> ON_SET_EVENT = nullptr;
};


class Slider : public Widget {
public:
	Slider(const SliderConfig& config, std::shared_ptr<Window> window);

	void SetValue(unsigned value);

	inline unsigned GetValue() const {
		return value_;
	}

	void SetConfig(const SliderConfig& config);

private:

	void DrawSelf() const override;
    void HandleEventsSelf(const sf::Event& event) override;

	void MoveKnobTo(float x_pos);

	unsigned value_;
	unsigned delta_;

	sf::RectangleShape knob_;
	sf::Color knob_color_;

	sf::RectangleShape line_;
	sf::Color line_color_;

	sf::RectangleShape background_line_;
	sf::Color background_line_color_;

	float line_to_knob_ratio_;

	bool dragging_ = false;
	sf::Vector2f mouse_offset_;

	std::function<void(unsigned)> on_set_handler_;
};

