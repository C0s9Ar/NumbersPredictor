#include "Application/Application.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


int main() {
    int window_width = 400;
    int window_height = 500;
    WindowConfig window_config(window_width, window_height);

    Application app(window_config);
    app.Run();

    return 0;
}
