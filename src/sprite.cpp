#include <cstdint>
#include <string_view>

#include "game.hpp"
#include "sprite.hpp"

namespace WebDed {
Sprite::Sprite(rl::Image texture, rl::Vector2 pos, float speed, rl::Vector2 dir)
    : m_Speed(speed), m_Direction(dir),  m_Position(pos), m_Texture(texture)  {
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
    m_ShouldDie = m_Position.GetY() + m_Size.GetY() < -300.0f || m_Position.GetY() > Game::getSize().GetY();
    return m_ShouldDie;
}
}
