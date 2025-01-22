#include "Button.h"

Button::Button(const ButtonConfig& config, std::shared_ptr<Window> window) : Widget(config, window) {
    SetConfig(config);

}

void Button::SetConfig(const ButtonConfig& config) {
    Widget::SetConfig(config);
    main_color_ = config.MAIN_COLOR;
    hover_color_ = config.HOVER_COLOR;
    clicked_color_ = config.CLICKED_COLOR;

    on_click_handler_ = config.ON_CLICK_EVENT;

    button_rect_.setSize(size_);
    button_rect_.setPosition(pos_);
    button_rect_.setFillColor(main_color_);
}

void Button::SetOnClickHandler(std::function<void()> handler) {
    on_click_handler_ = std::move(handler);
}

void Button::HandleEventsSelf(const sf::Event& event) {
    auto mouse_pos = sf::Mouse::getPosition(*parent_window_->GetRenderWindowPtr());
    bool is_hovered = IsHovered(mouse_pos);


    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        was_pressed_ = is_hovered;
        if (is_hovered)
            button_rect_.setFillColor(clicked_color_);
    }

    if (!was_pressed_) {
        if (is_hovered) 
            button_rect_.setFillColor(hover_color_);
        else 
            button_rect_.setFillColor(main_color_);
    }


    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (was_pressed_ && is_hovered) 
            OnClick();
        was_pressed_ = false;
    }
}


void Button::DrawSelf() const {
    parent_window_->DrawObject(button_rect_);
}

bool Button::IsHovered(const sf::Vector2i& mouse_pos) const {
    return button_rect_.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos));
}

void Button::OnClick() {
    if (on_click_handler_)
        on_click_handler_();
}