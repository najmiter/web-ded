#include <print>

#include "player.hpp"
#include "Sound.hpp"
#include "game.hpp"
#include "raylib.h"
#include "sprite.hpp"

namespace WebDed {
Player::Player() {
    // m_LaserSound.Load(Utils::joinPath("assets", "music", "laser-gun.wav").string());
}

Player::Player(rl::Image&& texture)
    : Sprite(std::move(texture)) {
        m_Position.x = Game::getSize().x / 2.f - (float)m_Texture.width / 2.f;
        m_Position.y = Game::getSize().y / 2.f - (float)m_Texture.height / 2.f;
        rl::Image img = g_SpriteAssets.at(Asset::LASER);
        img.Resize(10, 30);
        s_BulletTexture = img;
        m_LaserSound.Load(Utils::joinPath("assets", "music", "laser-gun.wav").string());
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

auto Player::draw(float dt, rl::Color tint) noexcept -> void {
    if (!m_Texture.IsValid()) return;

    m_Bullets.drawVisible(dt);
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
    if (rl::Keyboard::IsKeyPressed(KeyboardKey::KEY_SPACE)) {
        this->pg13Shit();
    }
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
    auto speed{500.f};
    m_Bullets.add(
        Sprite(s_BulletTexture, {m_Position.x + m_Size.x / 2 - (float)s_BulletTexture.width / 2, m_Position.y + m_Size.y / 2}, speed, {0.f, -1.f})
    );
    m_LaserSound.Play();
    // std::println("Spawned bullet: {}", m_Bullets.getTextures().size());
}

auto Player::getBullets() noexcept -> std::vector<Sprite>& {
    return m_Bullets.getTextures();
}

auto Player::promoteBro() noexcept -> void {
    m_Score++;
}

auto Player::getScore() const noexcept -> uint64_t {
    return m_Score;
}

auto Player::reset() noexcept -> void {
    m_Score = 0ull;
    m_Bullets.clean();
    m_Position.x = Game::getSize().x / 2.f - (float)m_Texture.width / 2.f;
    m_Position.y = Game::getSize().y / 2.f - (float)m_Texture.height / 2.f;
}

}
