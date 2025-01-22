#pragma once
#include "../Widget.h"
#include <optional>
#include <SFML/Graphics/RectangleShape.hpp>
#include <functional>


struct ButtonConfig : WidgetConfig {
    ButtonConfig(sf::Vector2f size, sf::Vector2f pos) : WidgetConfig(size, pos) {}
    
    sf::Color MAIN_COLOR = sf::Color(40, 40, 40);
    sf::Color HOVER_COLOR = sf::Color(62, 62, 62);
    sf::Color CLICKED_COLOR = sf::Color(82, 82, 82);

    std::function<void()> ON_CLICK_EVENT = nullptr;
};

class Button : public Widget {
public:
    Button(const ButtonConfig& config, std::shared_ptr<Window> window);

    void SetOnClickHandler(std::function<void()> handler); // !unnecessary function! its better to use config instead

    void SetConfig(const ButtonConfig& config);
private:
    void HandleEventsSelf(const sf::Event& event) override;
    void DrawSelf() const override;

    bool IsHovered(const sf::Vector2i& mouse_pos) const;
    void OnClick();

    sf::RectangleShape button_rect_;

    sf::Color main_color_;
    sf::Color hover_color_;
    sf::Color clicked_color_;

    std::function<void()> on_click_handler_;
    
    bool was_pressed_ = false;
};
