#include "game.hpp"
#include "constants.hpp"
#include "player.hpp"
#include "raylib.h"
#include "sprite.hpp"
#include "timer.hpp"

#include <algorithm>
#include <print>
#include <random>
#include <string_view>

namespace WebDed {
Game::Game() : m_Window(setupWindow()) {}

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
    m_Player = Player(rl::LoadImage(g_SpriteAssets.at(Asset::PLAYER)));

    m_Sprites.insert({
        Asset::LASER, rl::LoadImage(g_SpriteAssets.at(Asset::LASER)),
    });
    m_Sprites.insert({
        Asset::TRASH_0, rl::LoadImage(g_SpriteAssets.at(Asset::TRASH_0)),
    });
    m_Sprites.insert({
        Asset::TRASH_1, rl::LoadImage(g_SpriteAssets.at(Asset::TRASH_1)),
    });
    m_Sprites.insert({
        Asset::TRASH_2, rl::LoadImage(g_SpriteAssets.at(Asset::TRASH_2)),
    });
    m_Sprites.insert({
        Asset::TRASH_3, rl::LoadImage(g_SpriteAssets.at(Asset::TRASH_3)),
    });
}

auto Game::handleTrash() noexcept -> void {
    auto erasables {
        std::remove_if(m_Trash.begin(), m_Trash.end(), [&](Sprite &trash) {
            trash.update(m_Window.GetFrameTime());
            trash.draw();
            return trash.checkShouldDie();
        })
    };
    m_Trash.erase(erasables, m_Trash.end());
}

auto Game::handlePlayer() -> void {
    m_Player.handleInput(m_Window.GetFrameTime());
    m_Player.draw();
}

auto Game::run() noexcept -> void {
    // i know `this` is not necessary but it makes a little sense in a way
    this->loadAssets();

    // bg (#181818)
    rl::Color background{24, 24, 24};

    auto timer = Timer(2, true, true, [&] {
        this->spawnTrash();
    });

    while (!m_Window.ShouldClose()) {
        m_Window.BeginDrawing();
        m_Window.ClearBackground(background);

        this->handleTrash();
        this->handlePlayer();

        timer.update();

        m_Window.EndDrawing();
    }
}

auto Game::spawnTrash() noexcept -> void {
    auto trashSizeX{100};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, (int)s_Size.GetX() - trashSizeX);
    auto x = dist(gen);

    auto randomTrash = Utils::pickRandom(g_Trashes);
    m_Trash.emplace_back(Trash(m_Sprites.at(randomTrash), {(float)x, -200.f}, 100.f, {0.f, 1.f}));
    std::println("Spawned some trash: {}", m_Trash.size());
}

}  // namespace WebDed
