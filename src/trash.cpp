#include "trash.hpp"

namespace WebDed {
Trash::Trash(rl::Image&& texture)
    : Sprite(std::move(texture)) { }

Trash::Trash(rl::Image&& texture, rl::Vector2 pos, float speed, rl::Vector2 dir)
    : Sprite(std::move(texture), pos, speed, dir)  { }

auto Trash::move(float dt) noexcept -> void {
    Sprite::move(dt);
}

auto Trash::checkShouldDie() noexcept -> bool {
    return Sprite::checkShouldDie();
}

auto Trash::update(float dt) noexcept -> void {
    Sprite::move(dt);
}

auto Trash::getHitsTaken() const noexcept -> uint8_t {
    return m_HitsTaken;
}

auto Trash::takeHit() noexcept -> void {
    m_HitsTaken++;
}
}
