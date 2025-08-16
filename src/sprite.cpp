#include <algorithm>
#include <cstdint>
#include <print>
#include <string_view>

#include "game.hpp"
#include "raylib.h"
#include "sprite.hpp"

namespace WebDed {
Sprite::Sprite(rl::Image&& texture)
    : m_Texture(std::move(texture)) {
        m_Size.x = m_Texture.width;
        m_Size.y = m_Texture.height;
    }

Sprite::Sprite(rl::Image&& texture, rl::Vector2 pos, float speed, rl::Vector2 dir)
    : m_Speed(speed), m_Direction(dir), m_Position(pos), m_Texture(std::move(texture))  {
        m_Size.x = m_Texture.width;
        m_Size.y = m_Texture.height;
}

Sprite::Sprite(rl::Image& texture, rl::Vector2 pos, float speed, rl::Vector2 dir)
    : m_Speed(speed), m_Direction(dir), m_Position(pos), m_Texture(texture)  {
        m_Size.x = m_Texture.width;
        m_Size.y = m_Texture.height;
}

auto Sprite::draw(rl::Color tint) const noexcept -> void {
    if (!m_Texture.IsValid()) return;

    m_Texture.Draw(m_Position.GetX(), m_Position.GetY(), tint);
}

auto Sprite::move(float dt) noexcept -> void {
    m_Position.SetX(m_Position.GetX() + m_Direction.GetX() * m_Speed * dt);
    m_Position.SetY(m_Position.GetY() + m_Direction.GetY() * m_Speed * dt);
}

auto Sprite::update(float dt) noexcept -> void {
    this->move(dt);
}

auto Sprite::checkShouldDie() noexcept -> bool {
    auto topOffset{-300.0f};
    m_ShouldDie = m_Position.GetY() + m_Size.GetY() < topOffset || m_Position.GetY() > Game::getSize().GetY();
    return m_ShouldDie;
}

auto Sprite::checkCollisionWithOther(Sprite const* other) const noexcept  -> bool {
    rl::Rectangle rectP {
        m_Position.x, m_Position.y,
        (float)m_Texture.width, (float)m_Texture.height
    };
    rl::Rectangle rectT {
        other->m_Position.x, other->m_Position.y,
        (float)other->m_Texture.width, (float)other->m_Texture.height
    };

    return rectP.CheckCollision(rectT);
}

}
