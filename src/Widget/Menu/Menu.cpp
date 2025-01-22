#include "Menu.h"

#include "../Button/Button.h"
#include "../Field/Field.h"


Menu::Menu(const MenuConfig& config, std::shared_ptr<Window> window) : Widget(config, window) {
    SetConfig(config);
}

void Menu::SetConfig(const MenuConfig& config) {
    button_amount_ = 0;

    Widget::SetConfig(config);
    size_ = config.WIDGET_SIZE;
    pos_ = config.WIDGET_POS; // Maybe for some kind of vertical menu bar :P
    body_color_ = config.BODY_COLOR;

    menu_body_.setSize(size_);
    menu_body_.setPosition(pos_);
    menu_body_.setFillColor(body_color_);

    button_width_ = config.BUTTON_WIDTH;

    CreateButton(
        "res/img/close_button.png",
        sf::Vector2f(size_.x - button_width_ * (button_amount_ + 1), 0),
        sf::Vector2f(button_width_, size_.y),
        [window = parent_window_]() { if (window) window->Close(); }
    );

    CreateButton(
        "res/img/minimize_button.png",
        sf::Vector2f(size_.x - button_width_ * (button_amount_ + 1), 0),
        sf::Vector2f(button_width_, size_.y),
        [window = parent_window_]() { if (window) window->Minimize(); }
    );

    
    FieldConfig title_conf(sf::Vector2f(size_.x - button_width_ * button_amount_, size_.y), sf::Vector2f(0, 0));
    std::shared_ptr<Field> title = std::make_shared<Field>(title_conf, parent_window_);
    title->AddText(config.TITLE_TEXT);
    AddInnerWidget(title, true, false);

    Widget::start_pos_y = size_.y; // Custom start Y position for widgets
}

void Menu::HandleEventsSelf(const sf::Event& event) {
    DraggingEvent(event);
}

void Menu::DrawSelf() const {
    parent_window_->DrawObject(menu_body_);
}


void Menu::DraggingEvent(const sf::Event& event) {
    static bool dragging = false;
    static sf::Vector2i initial_mouse_offset;

    auto window_ptr = parent_window_->GetRenderWindowPtr();
    if (!window_ptr) return;


    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window_ptr);
        if (menu_body_.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos)) && mouse_pos.x < size_.x - button_width_ * button_amount_) {
            dragging = true;
            initial_mouse_offset = sf::Mouse::getPosition() - window_ptr->getPosition();
        }
    }

    if (event.type == sf::Event::MouseMoved && dragging) {
        sf::Vector2i global_mouse_pos = sf::Mouse::getPosition();
        window_ptr->setPosition(global_mouse_pos - initial_mouse_offset);
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        dragging = false;
    }
}

void Menu::RegisterWidget() {
    parent_window_->AddDrawableWidget(shared_from_this());
    parent_window_->AddHandleEventsWidget(shared_from_this());
}

void Menu::CreateButton(const std::string& icon_path, const sf::Vector2f& pos, const sf::Vector2f& size, const std::function<void()>& on_click) {
    button_amount_++;
    ButtonConfig btn_conf(size, pos);
    btn_conf.ON_CLICK_EVENT = on_click;
    std::shared_ptr<Button> button = std::make_shared<Button>(btn_conf, parent_window_);

    FieldConfig icon_conf(size, pos);
    std::shared_ptr<Field> button_icon = std::make_shared<Field>(icon_conf, parent_window_);
    button_icon->AddImage(icon_path);

    button->AddInnerWidget(button_icon, true, false);
    AddInnerWidget(button, true, true);
}