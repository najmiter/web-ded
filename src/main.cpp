#include "raylib-cpp.hpp"

namespace rl = raylib;

int main()
{
    rl::Window window(800, 450, "raylib-cpp example window");
    rl::Color background = rl::Color::White();

    while (!window.ShouldClose())
    {
        window.BeginDrawing();
        window.ClearBackground(background);
        rl::DrawText("Hello, raylib-cpp!", 190, 200, 20, rl::Color::LightGray());
        window.EndDrawing();
    }
    return 0;
}
