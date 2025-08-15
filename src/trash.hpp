#pragma once

#include "sprite.hpp"

namespace WebDed {
class Trash : public Sprite {
public:
    Trash() = default;
    Trash(rl::Image&& texture);
    Trash(rl::Image texture, rl::Vector2 pos, float speed, rl::Vector2 dir);

    auto move(float dt) noexcept -> void;
    auto update(float dt) noexcept -> void;
    auto checkShouldDie() noexcept -> bool;
};
}
