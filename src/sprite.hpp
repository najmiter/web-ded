#pragma once

#include <cstdint>
#include <string_view>

#include "raylib-cpp.hpp"

namespace WebDed {
namespace rl = raylib;
class Sprite {
public:
    Sprite() = default;
    Sprite(rl::Image&& texture);
    Sprite(rl::Image texture, rl::Vector2 pos, float speed, rl::Vector2 dir);

    auto move(float dt) noexcept -> void;
    auto update(float dt) noexcept -> void;
    auto checkShouldDie() noexcept -> bool;
    auto draw(rl::Color tint = rl::Color::White()) const noexcept -> void;

protected:
    bool m_ShouldDie{};
    float m_Speed{500};

    rl::Vector2 m_Direction{};
    rl::Vector2 m_Size{0, 0};
    rl::Vector2 m_Position{0, 0};
    rl::Texture2D m_Texture{};
};
}
