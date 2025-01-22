#pragma once
#include "../Widget.h"
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>


struct FieldConfig : WidgetConfig {
	FieldConfig(sf::Vector2f size, sf::Vector2f pos) : WidgetConfig(size, pos) {}
	
	FieldConfig(const WidgetConfig& config) : WidgetConfig(config) {}

	bool SCALE_IMAGE = false;
	unsigned FONT_SIZE = 16;
	sf::Color TEXT_COLOR = sf::Color(195, 195, 195);
	std::string FONT_PATH = "res/font/font.ttf";
};

class Field : public Widget {
public:
	Field(const FieldConfig& config, std::shared_ptr<Window> window);

	void AddText(const std::string& text);
	void AddImage(const std::string& path);


	void SetConfig(const FieldConfig& config);
private:
	void DrawSelf() const override;
	void HandleEventsSelf(const sf::Event& event) override;

	std::optional<sf::Text> text_;
	std::shared_ptr<sf::Font> font_;
	unsigned font_size_;
	sf::Color font_color_;


	std::optional<sf::Sprite> image_;
	std::shared_ptr<sf::Texture> texture_;
	bool scale_image_;

	std::string font_path_;
};
