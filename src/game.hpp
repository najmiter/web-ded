#pragma once

#include <cmath>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Font.hpp"
#include "animation.hpp"
#include "constants.hpp"
#include "erasable-texture.hpp"
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
    auto handlePlayer() -> void;
    auto checkCollisions() noexcept -> void;
    auto renderMenu() -> void;
    auto renderGameOver() -> void;
    auto renderScore() -> void;
    auto renderDifficultyMenu() -> void;
    auto renderGameWon() -> void;

    auto constexpr static inline getSize() -> const rl::Vector2& { return s_Size; }
    auto constexpr static inline getDifficulty() -> Difficulty { return s_Difficulty; }
    auto constexpr static inline getFont() -> rl::Font& { return s_Font; }

private:
    // renders common ui for WIN and LOSE states
    auto renderFinalState() -> void;

private:
    rl::Window m_Window{};
    std::unordered_map<Asset, rl::Texture2D> m_Sprites{};
    ErasableTexture<Trash> m_Trashes{};
    Player m_Player{};
    Sprite m_RandomTrash{};

    float m_GameStartedAt{};

    Animation m_Explosion{};
    rl::Music m_PlayBg{};
    rl::Music m_MenuBg{};
    rl::Sound m_ExplosionSound{};

    static inline rl::Font s_Font{};
    static inline GameState s_GameState{GameState::MENU};
    static inline Difficulty s_Difficulty{Difficulty::LOW};
    static inline rl::Vector2 s_Size{(float)GetMonitorWidth(0), (float)GetMonitorHeight(0)};
};
}
