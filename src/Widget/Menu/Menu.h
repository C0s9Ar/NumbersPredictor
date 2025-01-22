#pragma once
#include "../Widget.h"
#include <functional>
#include <string>

struct MenuConfig : WidgetConfig {
	MenuConfig(sf::Vector2f size, sf::Vector2f pos = { 0, 0 }) : WidgetConfig(size, pos) {}
	unsigned BUTTON_WIDTH = 80;
	sf::Color BODY_COLOR = sf::Color(30, 30, 30);

	std::string TITLE_TEXT = "Number Prediction";
};

class Menu : public Widget {
public:

	Menu(const MenuConfig& config, std::shared_ptr<Window> window);

	void RegisterWidget();

	void SetConfig(const MenuConfig& config);

private:
	void CreateButton(const std::string& icon_path, const sf::Vector2f& pos, const sf::Vector2f& size, const std::function<void()>& on_click);

	void DrawSelf() const override;
	void HandleEventsSelf(const sf::Event& event) override;

	void DraggingEvent(const sf::Event& event);

	sf::RectangleShape menu_body_;
	sf::Color body_color_;


	unsigned button_width_;
	unsigned button_amount_;
};

