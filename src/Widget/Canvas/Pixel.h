#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Pixel {
public:
	Pixel(unsigned size, const sf::Vector2f& pos, const std::shared_ptr<sf::Color>& clear_color, const std::shared_ptr<sf::Color>& draw_color, bool state = 0);

	inline sf::RectangleShape& GetRectangle() {
		return rect_;
	}
	inline const sf::RectangleShape& GetRectangle() const {
		return rect_;
	}


	void SetState(bool state);

	inline bool GetState() const {
		return state_;
	}

private:
	bool state_;
	const unsigned size_;
	sf::RectangleShape rect_;

	std::shared_ptr<sf::Color> clear_color_;
	std::shared_ptr<sf::Color> draw_color_;
};

