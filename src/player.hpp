#pragma once

#include <vector>
#include "sprite.hpp"

namespace WebDed {
class Player : public Sprite {
public:
    Player() = default;
    Player(rl::Image&& texture);

    auto move(float dt) noexcept -> void;
    auto handleInput(float dt) noexcept -> void;
    auto update(float dt) noexcept -> void;
    auto checkShouldDie() noexcept -> bool;
    auto draw(rl::Color tint = rl::Color::White()) const noexcept -> void;
    auto setSpeed(float) noexcept -> void;
    auto pg13Shit() noexcept -> void;

private:
    std::vector<Sprite> m_Bullets{};
};
}
