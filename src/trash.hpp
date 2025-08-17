#pragma once

#include <cstdint>
#include "sprite.hpp"

namespace WebDed {
class Trash : public Sprite {
public:
    Trash() = default;
    Trash(rl::Image&& texture);
    Trash(rl::Image&& texture, rl::Vector2 pos, float speed, rl::Vector2 dir);

    auto move(float dt) noexcept -> void;
    auto update(float dt) noexcept -> void;
    auto checkShouldDie() noexcept -> bool;
    auto getHitsTaken() const noexcept -> uint8_t;
    auto takeHit() noexcept -> void;

private:
    uint8_t m_HitsTaken{};
};
}
