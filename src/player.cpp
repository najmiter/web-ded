#include <print>

#include "player.hpp"
#include "game.hpp"

namespace WebDed {
Player::Player(rl::Image&& texture)
    : Sprite(std::move(texture)) {
        m_Position.x = Game::getSize().x / 2.f - (float)m_Texture.width / 2.f;
        m_Position.y = Game::getSize().y / 2.f - (float)m_Texture.height / 2.f;
    }

auto Player::move(float dt) noexcept -> void {
    auto cx{m_Position.x};
    auto cy{m_Position.y};

    m_Position.x += m_Direction.x * m_Speed * dt;
    m_Position.y += m_Direction.y * m_Speed * dt;

    // could "simplify" it with some math but idk any
    if (m_Position.x < 0 || m_Position.x + m_Texture.width >= Game::getSize().x) {
        m_Position.x = cx;
    }
    if (m_Position.y < 0 || m_Position.y + m_Texture.height >= Game::getSize().y) {
        m_Position.y = cy;
    }
}

auto Player::draw(rl::Color tint) const noexcept -> void {
    if (!m_Texture.IsValid()) return;

    m_Texture.Draw(m_Position.x, m_Position.y, tint);
}

auto Player::checkShouldDie() noexcept -> bool {
    return Sprite::checkShouldDie();
}

auto Player::update(float dt) noexcept -> void {
    this->move(dt);
}

auto Player::handleInput(float dt) noexcept -> void {
    // std::println("----> Input handled");
    auto isGoingRight {
        rl::Keyboard::IsKeyDown(KeyboardKey::KEY_RIGHT) ||
        rl::Keyboard::IsKeyDown(KeyboardKey::KEY_D)
    };
    auto isGoingLeft {
        rl::Keyboard::IsKeyDown(KeyboardKey::KEY_LEFT) ||
        rl::Keyboard::IsKeyDown(KeyboardKey::KEY_A)
    };
    auto isGoingUp {
        rl::Keyboard::IsKeyDown(KeyboardKey::KEY_UP) ||
        rl::Keyboard::IsKeyDown(KeyboardKey::KEY_W)
    };
    auto isGoingDown {
        rl::Keyboard::IsKeyDown(KeyboardKey::KEY_DOWN) ||
        rl::Keyboard::IsKeyDown(KeyboardKey::KEY_S)
    };

    m_Direction.x = (float)isGoingRight - (float)isGoingLeft;
    m_Direction.y = (float)isGoingDown - (float)isGoingUp;

    // std::println("\n\n----> Input handled {}, {}\n\n", m_Direction.x, m_Direction.y);

    this->update(dt);
}

auto Player::setSpeed(float speed) noexcept -> void {
    m_Speed = speed;
}

auto Player::pg13Shit() noexcept -> void {

}

}
