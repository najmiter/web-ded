#pragma once

#include <string_view>
#include <unordered_map>
#include "Texture.hpp"
#include "constants.hpp"
#include "sprite.hpp"
#include "raylib-cpp.hpp"

namespace WebDed {

namespace rl = raylib;

class Game {
public:
    Game() = default;

    auto setupWindow() const -> rl::Window;
    auto loadAssets() -> void;
    auto run() noexcept -> void;
    auto spawnTrash() noexcept -> void;

    auto static getSize() { return s_Size; }
private:
    std::unordered_map<Asset, rl::Texture2D> m_Sprites{};
    static inline rl::Vector2 s_Size{1280, 720};
    std::vector<Sprite> m_Trash{};
};

}
