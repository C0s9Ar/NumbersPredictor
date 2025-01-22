#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

class Widget;

struct WindowConfig {
	WindowConfig(unsigned width, unsigned height) : WIDTH(width), HEIGHT(height) {}
	std::string TITLE = "";
	unsigned WIDTH = 400;
	unsigned HEIGHT = 500;
	sf::Color CLEAR_COLOR = sf::Color(22, 22, 22);
	bool CUSTOM_MENU = true;
};

class Window {
public:
	Window(const WindowConfig& config);

	bool IsOpen() const;
	void ProcessEvents();
	void Clear();
	void DrawObject(const sf::Drawable& drawable);
	void Display();
	
	
	void DrawWidgets();

	inline const std::shared_ptr<sf::RenderWindow> GetRenderWindowPtr() const {
		return window_;
	}

	inline sf::Vector2i& GetPerviousMousePos() {
		return prev_mouse_pos_;
	}

	inline void SetPerviousMousePos(sf::Vector2i pos) {
		prev_mouse_pos_ = pos;
	}

	inline void AddDrawableWidget(std::shared_ptr<Widget> widget) {
		drawable_widgets_.push_back(widget);
	}

	inline void AddHandleEventsWidget(std::shared_ptr<Widget> widget) {
		handle_events_widgets_.push_back(widget);
	}

	inline void Close() {
		window_->close();
	}
	void Minimize();

	inline int GetWidth() const { return window_->getSize().x; }
	inline int GetHeight() const { return window_->getSize().y; }

private:
	std::shared_ptr<sf::RenderWindow> window_;

	sf::Vector2i prev_mouse_pos_ = {-1, -1};

	std::vector<std::shared_ptr<Widget>> drawable_widgets_;

	std::vector<std::shared_ptr<Widget>> handle_events_widgets_;

	sf::Color clear_color_;
};
