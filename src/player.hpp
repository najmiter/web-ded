#pragma once

#include <cstdint>
#include <vector>
#include "constants.hpp"
#include "erasable-texture.hpp"
#include "sprite.hpp"

namespace WebDed {
class Player : public Sprite {
public:
    Player();
    Player(rl::Image&& texture);

    auto move(float dt) noexcept -> void;
    auto handleInput(float dt) noexcept -> void;
    auto update(float dt) noexcept -> void;
    auto checkShouldDie() noexcept -> bool;
    auto draw(float dt, rl::Color tint = rl::Color::White()) noexcept -> void;
    auto setSpeed(float) noexcept -> void;
    auto pg13Shit() noexcept -> void;
    auto getBullets() noexcept -> std::vector<Sprite>&;
    auto promoteBro() noexcept -> void;

private:
    uint64_t m_Score{};
    rl::Sound m_LaserSound{};
    ErasableTexture<Sprite> m_Bullets{};
    static inline rl::Texture2D s_BulletTexture{};
};
}
