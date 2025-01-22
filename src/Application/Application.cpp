#include "Application.h"
#include <cmath>
#include <ctime>
#include <iostream>

unsigned CenterByCoord(unsigned start, unsigned end, unsigned size) {
    return start + (end - start) / 2 - size / 2;
}

Application::Application(const WindowConfig& config)
    : window_(std::make_shared<Window>(config)) {
    srand(time(nullptr));
    InitializeWidgets();
}

void Application::Run() {
    while (window_->IsOpen()) {
        window_->Clear();
        window_->ProcessEvents();
        window_->DrawWidgets();
        window_->Display();
    }
}

void Application::InitializeWidgets() {
    InitializeMenu();
    InitializeCanvas();
    InitializeButtons();
    InitializeSlider();
}

void Application::InitializeMenu() {
    int menu_height = 50;
    MenuConfig menu_config(sf::Vector2f(window_->GetWidth(), menu_height));
    menu_ = std::make_shared<Menu>(menu_config, window_);
    menu_->RegisterWidget();
}

void Application::InitializeCanvas() {
    unsigned pixel_size = 10;
    int canvas_size = 28;
    CanvasConfig canvas_config(sf::Vector2f(0, 120), canvas_size, pixel_size);
    canvas_ = std::make_shared<Canvas>(canvas_config, window_);
    canvas_->RegisterWidget();

    // Prediction field setup
    BoxConfig pred_val_box_conf(
        { window_->GetWidth() - (canvas_config.WIDGET_POS.x + canvas_config.WIDGET_SIZE.x), canvas_config.WIDGET_SIZE.y },
        { canvas_config.WIDGET_POS.x + canvas_config.WIDGET_SIZE.x, canvas_config.WIDGET_POS.y });

    FieldConfig pred_val_conf(pred_val_box_conf);
    pred_val_conf.FONT_SIZE = 196;
    prediction_field_ = std::make_shared<Field>(pred_val_conf, window_);
    prediction_field_->AddText("0");
    prediction_field_->RegisterWidget();
}

void Application::InitializeButtons() {
    unsigned button_width = 110;
    unsigned button_height = 52;

    // Reset canvas button
    ButtonConfig reset_button_config(
        sf::Vector2f(button_width, button_height),
        sf::Vector2f(20, canvas_->GetWidgetPos().y / 2 - button_height));
    reset_button_config.ON_CLICK_EVENT = [this]() { canvas_->ClearCanvas(); };
    reset_button_ = std::make_shared<Button>(reset_button_config, window_);
    reset_button_->RegisterWidget();

    FieldConfig reset_button_icon_config(reset_button_config);
    std::shared_ptr<Field> reset_button_icon = std::make_shared<Field>(reset_button_icon_config, window_);
    reset_button_icon->AddImage("res/img/reset_button.png");
    reset_button_->AddInnerWidget(reset_button_icon);

    // Predict button
    ButtonConfig predict_button_config(
        sf::Vector2f(button_width, button_height),
        sf::Vector2f(reset_button_config.WIDGET_SIZE.x + reset_button_config.WIDGET_POS.x * 2, canvas_->GetWidgetPos().y / 2 - button_height));
    predict_button_config.ON_CLICK_EVENT = [this]() {
        prediction_field_->AddText(std::to_string(classifier_.Classify(canvas_->GetMapInputs())));
        };
    predict_button_ = std::make_shared<Button>(predict_button_config, window_);
    predict_button_->RegisterWidget();

    FieldConfig predict_button_icon_config(predict_button_config);
    std::shared_ptr<Field> predict_button_icon = std::make_shared<Field>(predict_button_icon_config, window_);
    predict_button_icon->AddText("Predict");
    predict_button_->AddInnerWidget(predict_button_icon);
}

void Application::InitializeSlider() {
    unsigned slider_height = 40;
    unsigned slider_width = 200;
    SliderConfig slider_config(
        sf::Vector2f(slider_width, slider_height),
        sf::Vector2f(
            CenterByCoord(0, canvas_->GetWidgetSize().x, slider_width),
            CenterByCoord(reset_button_->GetWidgetPos().y + reset_button_->GetWidgetSize().y, canvas_->GetWidgetPos().y, slider_height)));
    slider_config.ON_SET_EVENT = [this](unsigned thickness) { canvas_->SetThickness(thickness); };
    slider_ = std::make_shared<Slider>(slider_config, window_);
    slider_->RegisterWidget();
}
