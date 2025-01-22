#pragma once
#include "../Widget.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

struct BoxConfig : WidgetConfig{
	BoxConfig(sf::Vector2f size, sf::Vector2f pos) : WidgetConfig(size, pos) {}
};


class Box : public Widget {
public:
	Box(const BoxConfig& config, std::shared_ptr<Window> window);

	void SetConfig(const BoxConfig& config);

	inline void SetVisible(bool show) { show_ = show; } // Debug option
private:
	void DrawSelf() const override;
	void HandleEventsSelf(const sf::Event& event) override;

	bool show_;

	sf::RectangleShape rect_;
};

