#pragma once
#include <memory>
#include <vector>
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "../Window/Window.h"


struct WidgetConfig {
	WidgetConfig(sf::Vector2f size, sf::Vector2f pos) : WIDGET_SIZE(size), WIDGET_POS(pos) {}
	sf::Vector2f WIDGET_SIZE;
	sf::Vector2f WIDGET_POS;
};

class Widget : public std::enable_shared_from_this<Widget> {
public:
	Widget(const WidgetConfig& config, std::shared_ptr<Window> parent_window);

	Widget(const Widget&) = delete;
	Widget& operator=(const Widget&) = delete;

	Widget(Widget&&) = default;
	Widget& operator=(Widget&&) = default;

	void Draw() const;

	void HandleEvents(const sf::Event& event);

	virtual void RegisterWidget(bool drawable = 1, bool handle_event = 1);

	inline const sf::Vector2f& GetWidgetSize() const {
		return size_;
	}
	inline const sf::Vector2f& GetWidgetPos() const {
		return { pos_.x, pos_.y - start_pos_y };
	}

	virtual void AddInnerWidget(std::shared_ptr<Widget> widget, bool drawable = 1, bool handle_event = 1);

protected:

	void SetConfig(const WidgetConfig& config);

	virtual void DrawSelf() const = 0;
	virtual void HandleEventsSelf(const sf::Event& event) = 0;

	std::shared_ptr<Window> parent_window_;

	sf::Vector2f size_;
	sf::Vector2f pos_;

	std::vector<std::shared_ptr<Widget>> event_inner_widgets_;
	std::vector<std::shared_ptr<Widget>> draw_inner_widgets_;

	static unsigned start_pos_y; // Custom menu var
};

