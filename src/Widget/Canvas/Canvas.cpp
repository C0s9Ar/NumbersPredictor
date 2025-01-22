#include "Canvas.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <algorithm>

Canvas::Canvas(const CanvasConfig& config, std::shared_ptr<Window> window) : Widget(config, window) {
	SetConfig(config);
}

void Canvas::SetConfig(const CanvasConfig& config) {
	Widget::SetConfig(config);
	height_ = config.HEIGHT;
	width_ = config.WIDTH;
	pen_thickness_ = config.PEN_THICKNESS;

	clear_color_ = config.CLEAR_COLOR;
	draw_color_ = config.DRAW_COLOR;

	pixel_size_ = config.PIXEL_SIZE;

	pixels_.reserve(width_ * height_);
	for (int i = 0; i < height_; i++) {
		for (int j = 0; j < width_; j++) {
			unsigned x = j * pixel_size_ + pos_.x;
			unsigned y = i * pixel_size_ + pos_.y;
			Pixel pixel(pixel_size_, sf::Vector2f(x, y), clear_color_, draw_color_, false);
			pixels_.push_back(pixel);
		}
	}
}

void Canvas::DrawSelf() const {
	for (int i = 0; i < height_; i++) {
		for (int j = 0; j < width_; j++) {
			auto& pixel = pixels_[i * width_ + j];
			parent_window_->DrawObject(pixel.GetRectangle());
		}
	}
}

void Canvas::HandleEventsSelf(const sf::Event& event) {
	auto window_ptr = parent_window_->GetRenderWindowPtr();
	if (!window_ptr) return;

	sf::Vector2i mouse_window_pos = sf::Mouse::getPosition(*window_ptr);

	sf::Vector2f mouse_canvas_pos = static_cast<sf::Vector2f>(mouse_window_pos) - pos_;

	bool mouse_in_canvas = mouse_canvas_pos.x >= 0 && mouse_canvas_pos.x < width_ * pixel_size_ &&
		mouse_canvas_pos.y >= 0 && mouse_canvas_pos.y < height_ * pixel_size_;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_in_canvas) {
		sf::Vector2i local_pos = static_cast<sf::Vector2i>(mouse_canvas_pos + pos_);

		sf::Vector2i prev_pos = parent_window_->GetPerviousMousePos();

		if (prev_pos.x >= 0 && prev_pos.y >= 0) {
			DrawAt(local_pos, prev_pos, pen_thickness_);
		}

		parent_window_->SetPerviousMousePos(local_pos);
	}
	else {
		parent_window_->SetPerviousMousePos(sf::Vector2i(-1, -1));
	}
}



Pixel* Canvas::GetPixelAt(unsigned x, unsigned y) {
	if ((x < pos_.x || x >= pos_.x + width_ * pixel_size_) ||
		(y < pos_.y || y >= pos_.y + height_ * pixel_size_)) {
		return nullptr;
	}

	unsigned x_index = (x - pos_.x) / pixel_size_;
	unsigned y_index = (y - pos_.y) / pixel_size_;

	return &pixels_[y_index * width_ + x_index];
}

void Canvas::DrawAt(sf::Vector2i& pos, sf::Vector2i& prev_pos, unsigned thickness) {
	thickness = (thickness - 1) * pixel_size_;
	if (prev_pos.x != -1 && prev_pos.y != -1) {
		int dx = std::abs(pos.x - prev_pos.x);
		int dy = std::abs(pos.y - prev_pos.y);
		int sx = (prev_pos.x < pos.x) ? 1 : -1;
		int sy = (prev_pos.y < pos.y) ? 1 : -1;
		int err = dx - dy;

		sf::Vector2i current = prev_pos;
		while (true) {
			DrawThickPixel(current.x, current.y, thickness);
			if (current == pos) break;

			int e2 = 2 * err;
			if (e2 > -dy) {
				err -= dy;
				current.x += sx;
			}
			if (e2 < dx) {
				err += dx;
				current.y += sy;
			}
		}
	}
	prev_pos = pos;
}

void Canvas::DrawThickPixel(int x, int y, unsigned thickness) {
	int radius = thickness / 2;
	for (int i = -radius; i <= radius; ++i) {
		for (int j = -radius; j <= radius; ++j) {
			if (i * i + j * j <= radius * radius) {
				auto pixel = GetPixelAt(x + i, y + j);
				if (pixel != nullptr) {
					pixel->SetState(1);
				}
			}
		}
	}
}

void Canvas::ClearCanvas() {
	for (auto& pixel : pixels_)
		pixel.SetState(0);
}

void Canvas::SetThickness(unsigned thickness) {
	pen_thickness_ = std::max(thickness, 1u);
}


std::vector<double> Canvas::GetMapInputs() {
	std::vector<double> result(pixels_.size(), 0);
	for (int i = 0; i < pixels_.size(); i++) {
		result[i] = pixels_[i].GetState() ? 1. : 0.;
	}
	return result;
}