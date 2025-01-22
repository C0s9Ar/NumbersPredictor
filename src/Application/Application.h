#pragma once

#include "../NumbersClassifier/NumbersClassifier.h"
#include "../Widget/Box/Box.h"
#include "../Widget/Button/Button.h"
#include "../Widget/Canvas/Canvas.h"
#include "../Widget/Field/Field.h"
#include "../Widget/Menu/Menu.h"
#include "../Widget/Slider/Slider.h"
#include "../Window/Window.h"


class Application {
public:
	Application(const WindowConfig& config);

	void Run();

private:
	void InitializeWidgets();
	void InitializeMenu();
	void InitializeCanvas();
	void InitializeButtons();
	void InitializeSlider();

	// Window
	std::shared_ptr<Window> window_;

	// Widgets
	std::shared_ptr<Canvas> canvas_;
	std::shared_ptr<Button> reset_button_;
	std::shared_ptr<Button> predict_button_;
	std::shared_ptr<Slider> slider_;
	std::shared_ptr<Field> prediction_field_;
	std::shared_ptr<Menu> menu_;

	// Classifier
	NumbersClassifier classifier_;
};
