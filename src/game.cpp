#include "game.hpp"

#include <algorithm>
#include <print>
#include <random>
#include <string_view>

#include "RayCollision.hpp"
#include "Rectangle.hpp"
#include "constants.hpp"
#include "player.hpp"
#include "raylib.h"
#include "sprite.hpp"
#include "timer.hpp"
#include "trash.hpp"

namespace WebDed {
Game::Game()
    : m_Window(setupWindow()),
      m_Player(Player(rl::LoadImage(g_SpriteAssets.at(Asset::PLAYER)))) {}

auto Game::setupWindow() const -> rl::Window {
    rl::Window window(s_Size.GetX(), s_Size.GetY(), "Web Ded");

#ifdef _WIN32
    // set icon (windows only)
    auto icon{rl::LoadImage(g_SpriteAssets.at(Asset::PLAYER))};
    window.SetIcon(icon);
    UnloadImage(icon);
#endif

    // make resizable
    window.SetState(ConfigFlags::FLAG_WINDOW_RESIZABLE);

    return window;
}

auto Game::loadAssets() -> void {
    m_Sprites.insert({
        Asset::LASER,
        rl::LoadImage(g_SpriteAssets.at(Asset::LASER)),
    });
    m_Sprites.insert({
        Asset::TRASH_0,
        rl::LoadImage(g_SpriteAssets.at(Asset::TRASH_0)),
    });
    m_Sprites.insert({
        Asset::TRASH_1,
        rl::LoadImage(g_SpriteAssets.at(Asset::TRASH_1)),
    });
    m_Sprites.insert({
        Asset::TRASH_2,
        rl::LoadImage(g_SpriteAssets.at(Asset::TRASH_2)),
    });
    m_Sprites.insert({
        Asset::TRASH_3,
        rl::LoadImage(g_SpriteAssets.at(Asset::TRASH_3)),
    });
}

auto Game::handlePlayer() -> void {
    auto dt{m_Window.GetFrameTime()};
    m_Player.handleInput(dt);
    m_Player.draw(dt);
}

auto Game::run() noexcept -> void {
    // i know `this` is not necessary but it makes a little sense in a way
    this->loadAssets();

    // bg (#181818)
    rl::Color background{24, 24, 24};

    // spawn trash randomly
    auto timer = Timer(1.f, true, true, [&] {
        this->spawnTrash();
    });

    while (!m_Window.ShouldClose()) {
        m_Window.BeginDrawing();
        m_Window.ClearBackground(background);

        switch (s_GameState) {
            case GameState::PLAY: {
                auto dt = m_Window.GetFrameTime();
                m_Trashes.drawVisible(dt);
                this->handlePlayer();
                this->checkCollisions();
                break;
            }
            case GameState::GAME_OVER: {
                std::println("<GAME OVER>");
                break;
            }
            default: {};
        }

        timer.update();
        m_Window.EndDrawing();
    }
}

auto Game::spawnTrash() noexcept -> void {
    auto trashSizeX{100};

    std::random_device rd;
    auto gen{std::mt19937(rd())};

    std::uniform_int_distribution<int> distX(0, (int)s_Size.GetX() - trashSizeX);
    std::uniform_real_distribution<float> distSpeed(80.f, 120.f);

    auto x{distX(gen)};
    auto speed{distSpeed(gen)};

    auto randomTrash{Utils::pickRandom(g_Trashes)};
    m_Trashes.add(Trash(m_Sprites.at(randomTrash), {(float)x, -200.f}, speed, {0.f, 1.f}));
    // std::println("Spawned some trash: {}", m_Trashes.getTextures().size());
}

auto Game::checkCollisions() noexcept -> void {
    auto& trashes = m_Trashes.getTextures();
    auto& bullets = m_Player.getBullets();

    auto erasables{
        std::remove_if(trashes.begin(), trashes.end(), [&](Trash& trash) {
            return std::any_of(bullets.begin(), bullets.end(), [&](Sprite& bullet) {
                return trash.checkCollisionWithOther(&bullet);
            });
            // return trash.checkCollisionWithOther(&m_Player);
        })};
    trashes.erase(erasables, trashes.end());

    auto isGameOver{
        std::any_of(trashes.begin(), trashes.end(), [&](Trash& trash) {
            return trash.checkCollisionWithOther(&m_Player);
        })};
    if (isGameOver) {
        s_GameState = GameState::GAME_OVER;
    }
}

}  // namespace WebDed
