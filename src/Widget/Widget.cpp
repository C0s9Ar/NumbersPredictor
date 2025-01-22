#include "Widget.h"

unsigned Widget::start_pos_y = 0;

Widget::Widget(const WidgetConfig& config, std::shared_ptr<Window> parent_window) : parent_window_(parent_window) {
	SetConfig(config);
	if (!parent_window_) {
		throw std::invalid_argument("parent_window must not be nullptr");
	}
}

void Widget::RegisterWidget(bool drawable, bool handle_event) {
	if (drawable)
		parent_window_->AddDrawableWidget(shared_from_this());
	if (handle_event)
		parent_window_->AddHandleEventsWidget(shared_from_this());
}


void Widget::AddInnerWidget(std::shared_ptr<Widget> widget, bool drawable, bool handle_event) {
	if (!widget) throw std::runtime_error("Invalid inner widget");

	if (drawable)
		draw_inner_widgets_.push_back(widget);
	if (handle_event)
		event_inner_widgets_.push_back(widget);
}

void Widget::Draw() const {
	DrawSelf();

	for (auto& ele : draw_inner_widgets_)
		ele->Draw();
}

void Widget::HandleEvents(const sf::Event& event) {
	HandleEventsSelf(event);

	for (auto& ele : event_inner_widgets_)
		ele->HandleEvents(event);
}

void Widget::SetConfig(const WidgetConfig& config) {
	size_ = config.WIDGET_SIZE;
	pos_ = sf::Vector2f(config.WIDGET_POS.x, config.WIDGET_POS.y + start_pos_y);
}
