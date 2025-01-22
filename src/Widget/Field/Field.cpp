#include "Field.h"


sf::Font GetTextFont(const std::string& path) {
	sf::Font font;
	if (!font.loadFromFile(path))
		throw std::runtime_error("Failed to load font");
	return font;
}

sf::Texture GetTexture(const std::string& path) {
	sf::Texture texture;
	if (!texture.loadFromFile(path))
		throw std::runtime_error("Failed to load texture");
	return texture;
}

Field::Field(const FieldConfig& config, std::shared_ptr<Window> window) : Widget(config, window) {
	SetConfig(config);
}


void Field::SetConfig(const FieldConfig& config) {
	Widget::SetConfig(config);

	font_size_ = config.FONT_SIZE;
	font_color_ = config.TEXT_COLOR;

	scale_image_ = config.SCALE_IMAGE;
	font_path_ = config.FONT_PATH;
}

void Field::DrawSelf() const {
	if (text_)
		parent_window_->DrawObject(*text_);
	if (image_)
		parent_window_->DrawObject(*image_);
}

void Field::HandleEventsSelf(const sf::Event& event) {}

void Field::AddText(const std::string& text) {
	font_ = std::make_shared<sf::Font>(GetTextFont(font_path_));
	//font_->setSmooth(false);

	text_.emplace();
	text_->setFont(*font_);
	text_->setString(text);
	text_->setCharacterSize(font_size_);
	text_->setFillColor(font_color_);
	text_->setStyle(sf::Text::Style::Bold);

	sf::FloatRect bounds = text_->getLocalBounds();
	text_->setPosition(pos_.x + (size_.x - bounds.width) / 2 - bounds.left,
		pos_.y + (size_.y - bounds.height) / 2 - bounds.top);
}

void Field::AddImage(const std::string& path) {
	texture_ = std::make_shared<sf::Texture>(GetTexture(path));
	image_.emplace();
	image_->setTexture(*texture_);

	if (scale_image_) {
		sf::Vector2u texture_size = texture_->getSize();
		image_->setScale(size_.x / static_cast<float>(texture_size.x), size_.y / static_cast<float>(texture_size.y));
		image_->setPosition(pos_);
	}
	else {
		sf::FloatRect bounds = image_->getLocalBounds();
		image_->setPosition(pos_.x + (size_.x - bounds.width) / 2 - bounds.left, pos_.y + (size_.y - bounds.height) / 2 - bounds.top);
	}
}
