#include "game.hpp"
#include "constants.hpp"
#include "raylib.h"
#include "sprite.hpp"
#include "timer.hpp"

#include <algorithm>
#include <iostream>
#include <random>
#include <string_view>

namespace WebDed {
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
        Asset::LASER, rl::LoadImage(g_SpriteAssets.at(Asset::LASER)),
    });
    m_Sprites.insert({
        Asset::PLAYER, rl::LoadImage(g_SpriteAssets.at(Asset::PLAYER)),
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

auto Game::run() noexcept -> void {
    // i know `this` is not necessary but it makes a little sense in a way
    auto window{this->setupWindow()};
    this->loadAssets();
    auto timer = Timer(2, true, true, [&] {
        this->spawnTrash();
    });

    // bg
    rl::Color background{24, 24, 24, 255};

    auto s = Sprite(m_Sprites.at(Asset::PLAYER), {0, 0}, 100, {1, 0});
    while (!window.ShouldClose()) {
        window.BeginDrawing();
        window.ClearBackground(background);

        m_Trash.erase(
            std::remove_if(m_Trash.begin(), m_Trash.end(), [&](Sprite &trash) {
                trash.draw();
                trash.update(window.GetFrameTime());
                return trash.checkShouldDie();
            }),
            m_Trash.end()
        );
        timer.update();
        s.draw();

        window.EndDrawing();
    }

    window.Close();
}

auto Game::spawnTrash() noexcept -> void {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, (int)s_Size.GetX());
    auto x = dist(gen);

    auto randomTrash = Utils::pickRandom(trashes);
    m_Trash.emplace_back(Sprite(m_Sprites.at(randomTrash), {(float)x, -200.f}, 100.f, {0.f, 1.f}));
    std::cout << "Spawned new trash: " << m_Trash.size() << "\n";
}

}  // namespace WebDed
