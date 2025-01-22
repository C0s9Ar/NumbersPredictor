#include "../Widget/Canvas/Canvas.h"
#include "../Widget/Widget.h"
#include "Window.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <windows.h>

Window::Window(const WindowConfig& config) : window_(std::make_shared<sf::RenderWindow>(sf::VideoMode(config.WIDTH, config.HEIGHT), 
											config.TITLE, config.CUSTOM_MENU ? sf::Style::None : sf::Style::Close)) {
	window_->setFramerateLimit(60);
	clear_color_ = config.CLEAR_COLOR;
}

bool Window::IsOpen() const {
	return window_->isOpen();
}

void Window::ProcessEvents() {
	sf::Event event;

	if (window_->pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			Close();
	}
	if (window_->hasFocus()) {
		for (const auto widget : handle_events_widgets_) {
			if (widget)
				widget->HandleEvents(event);
		}
	}
}


void Window::Clear() {
	window_->clear(clear_color_);
}

void Window::DrawObject(const sf::Drawable& drawable) {
	window_->draw(drawable);
}
void Window::DrawWidgets() {
	for (const auto widget : drawable_widgets_) {
		if (widget)
			widget->Draw();
	}
}

void Window::Display() {
	window_->display();
}

void Window::Minimize() { // WIN32 API :(
	auto hwnd = window_->getSystemHandle();
	ShowWindow(hwnd, SW_MINIMIZE);
}