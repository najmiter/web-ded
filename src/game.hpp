#pragma once

#include <string_view>
#include <unordered_map>
#include "Texture.hpp"
#include "Window.hpp"
#include "constants.hpp"
#include "player.hpp"
#include "sprite.hpp"
#include "raylib-cpp.hpp"
#include "trash.hpp"

namespace WebDed {

namespace rl = raylib;

class Game {
public:
    Game();

    auto setupWindow() const -> rl::Window;
    auto loadAssets() -> void;
    auto run() noexcept -> void;
    auto spawnTrash() noexcept -> void;
    auto handleTrash() noexcept -> void;
    auto handlePlayer() -> void;

    auto constexpr static inline getSize() { return s_Size; }

private:
    rl::Window m_Window{};
    std::unordered_map<Asset, rl::Texture2D> m_Sprites{};
    static inline rl::Vector2 s_Size{1280, 720};
    std::vector<Trash> m_Trash{};
    Player m_Player{};
};

}
