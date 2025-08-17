#include "game.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <format>
#include <random>
#include <string_view>

#include "button.hpp"
#include "constants.hpp"
#include "player.hpp"
#include "raylib.h"
#include "sprite.hpp"
#include "timer.hpp"
#include "trash.hpp"

namespace WebDed {
Game::Game()
    : m_Window(setupWindow()),
    m_RandomTrash(Sprite(g_SpriteAssets.at(Utils::pickRandom(g_Trashes)), {Game::s_Size.x / 2 - 47.f, 500.f}, 0.f, {0.f, 0.f}))
{
    rl::AudioDevice::Init();
    s_Size.x = (float)GetMonitorWidth(0);
    s_Size.y = (float)GetMonitorHeight(0);
    m_Player = Player(rl::LoadImage(g_SpriteAssets.at(Asset::PLAYER))); // bcz player needs audio device
    m_Explosion = Animation(rl::LoadImage(Utils::joinPath("assets", "thush.png").string()));
    m_PlayBg.Load(Utils::joinPath("assets", "music", "play-bg.mp3").string());
    m_MenuBg.Load(Utils::joinPath("assets", "music", "menu-bg.mp3").string());
    m_ExplosionSound.Load(Utils::joinPath("assets", "music", "explosion.wav").string());
}

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
    auto dt = m_Window.GetFrameTime();
    m_Player.handleInput(dt);
    m_Player.draw(dt);
}

auto Game::run() noexcept -> void {
    // i know `this` is not necessary but it makes a little sense in a way
    this->loadAssets();

    // fixed fps
    m_Window.SetTargetFPS(60);

    // for now just do this
    m_PlayBg.SetVolume(0.3f);
    m_MenuBg.SetVolume(0.3f);

    // bg (#181818)
    rl::Color background{24, 24, 24};

    // spawn trash randomly
    auto timer = Timer(1.f, true, true, [&] {
        this->spawnTrash();
    });

    auto onMenu = [&] {
        if (!m_MenuBg.IsPlaying()) {
            m_MenuBg.Play();
            m_PlayBg.Stop();
        }
        m_MenuBg.Update();
    };

    while (!m_Window.ShouldClose()) {
        if (s_GameState == GameState::QUIT) break;

        auto timeSinceStarted = m_Window.GetTime() - m_GameStartedAt;
        if (timeSinceStarted >= 60.0 * 3.0 / (float)s_Difficulty) {
            s_GameState = GameState::GAME_WON;
        }

        m_Window.BeginDrawing();
        m_Window.ClearBackground(background);

        if (m_Window.IsResized()) {
            s_Size.x = m_Window.GetWidth();
            s_Size.y = m_Window.GetHeight();
        }

        auto dt = m_Window.GetFrameTime();
        m_Explosion.animate(dt);

        switch (s_GameState) {
            case GameState::PLAY: {
                if (!m_PlayBg.IsPlaying()) {
                    m_PlayBg.Play();
                    m_MenuBg.Stop();
                }
                m_PlayBg.Update();

                this->checkCollisions();
                m_Trashes.drawVisible(dt);
                this->handlePlayer();
                this->renderScore();
                break;
            }
            case GameState::GAME_OVER: {
                onMenu();
                this->renderGameOver();
                break;
            }
            case GameState::MENU: {
                onMenu();
                this->renderMenu();
                break;
            }
            case GameState::GAME_WON: {
                this->renderGameWon();
                break;
            }
            case GameState::DIFFICULTY: {
                this->renderDifficultyMenu();
                break;
            }
            default: {};
        }

        timer.update();
        m_Window.EndDrawing();
    }
}

auto Game::spawnTrash() noexcept -> void {
    if (s_GameState != GameState::PLAY) return;

    auto trashSizeX{100};

    std::random_device rd;
    auto gen = std::mt19937(rd());

    std::uniform_int_distribution<int> distX(0, (int)s_Size.GetX() - trashSizeX);
    std::uniform_real_distribution<float> distSpeed(80.f, 120.f);

    auto x = distX(gen);
    auto speed = distSpeed(gen);

    auto randomTrash = Utils::pickRandom(g_Trashes);
    m_Trashes.add(Trash(m_Sprites.at(randomTrash), {(float)x, -200.f}, speed, {0.f, 1.f}));
}

auto Game::checkCollisions() noexcept -> void {
    auto& trashes = m_Trashes.getTextures();
    auto& bullets = m_Player.getBullets();

    auto erasables{
        std::remove_if(trashes.begin(), trashes.end(), [&](Trash& trash) {
            auto dead = std::find_if(bullets.begin(), bullets.end(), [&](Sprite& bullet) {
                return trash.checkCollisionWithOther(&bullet);
            });
            // a hit
            if (dead != bullets.end()) {
                bullets.erase(dead);
                trash.takeHit();
                if (trash.getHitsTaken() < (uint8_t)s_Difficulty) {
                    return false;
                }
                m_Player.promoteBro(); // give a score
                m_ExplosionSound.Play();
                m_Explosion.reset(trash.getPosition());
                return true;
            }
            return false;
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

auto Game::renderMenu() -> void {
    auto xCenter = Game::s_Size.x / 2;
    auto textColor = rl::Color{40,40,40};
    auto bSize = rl::Vector2{100.f, 50.f};
    auto getListener = [](GameState s) {
        return [=] {
            s_GameState = s;
        };
    };
    auto playBtn = Button("Play", textColor, {xCenter - bSize.x / 2, 150.f}, bSize, getListener(GameState::DIFFICULTY), {30.f, 15.f});
    auto quitBtn = Button("Quit", textColor, {xCenter - bSize.x / 2, bSize.y + 160.f}, bSize, getListener(GameState::QUIT), {32.f, 15.f});

    playBtn.render();
    quitBtn.render();
}

auto Game::renderFinalState() -> void {
    auto xCenter = Game::s_Size.x / 2;
    auto pSize = rl::Vector2{100.f, 50.f};
    auto pOnClick = [&] {
        m_GameStartedAt = m_Window.GetTime();
        s_GameState = GameState::PLAY;
        m_Trashes.clean();
        m_Player.reset();
    };
    auto playBtn = Button("Restart", rl::Color{40,40,40}, {xCenter - pSize.x / 2, 150.f}, pSize, pOnClick, {10.f, 15.f});

    auto qSize = rl::Vector2{100.f, 50.f};
    auto qOnClick = [] {
        s_GameState = GameState::QUIT;
    };
    auto quitBtn = Button("Quit", rl::Color{40,40,40}, {xCenter - qSize.x / 2, pSize.y + 160.f}, qSize, qOnClick, {32.f, 15.f});

    rl::DrawText(std::format("Score: {}", m_Player.getScore()), xCenter - pSize.x / 2, 100.f, 20, rl::Color::RayWhite());
    playBtn.render();
    quitBtn.render();
}

auto Game::renderGameOver() -> void {
    auto xCenter = Game::s_Size.x / 2;
    this->renderFinalState();

    rl::DrawText("Dam bro, you turned into of these", xCenter - 170.f, 400.f, 20.f, rl::Color::Red());
    m_RandomTrash.setPosition({Game::s_Size.x / 2 - 47.f, 500.f});
    m_RandomTrash.draw();
}

auto Game::renderScore() -> void {
    auto timeRemaining = (60.0f * 3.0f / (float)s_Difficulty) - (m_Window.GetTime() - m_GameStartedAt);
    rl::DrawText(std::format("Score: {}", m_Player.getScore()), 10, 10, 20, rl::Color::RayWhite());
    rl::DrawText(std::format("Time Remaining: {}s", (int)std::ceilf(timeRemaining)), 10, 40, 16, rl::Color::RayWhite());
}

auto Game::renderDifficultyMenu() -> void {
    auto xCenter = Game::s_Size.x / 2;
    auto bSize = rl::Vector2{100.f, 50.f};
    auto getListener = [&](Difficulty dif) {
        auto on = [&] { m_GameStartedAt = m_Window.GetTime(); };
        return [=] {
            on();
            s_Difficulty = dif;
            s_GameState = GameState::PLAY;
        };
    };
    auto lowBtn = Button(" Low ", rl::Color{40,40,40}, {xCenter - bSize.x / 2, 150.f}, bSize, getListener(Difficulty::LOW), {25.f, 15.f});
    auto medBtn = Button("Medium", rl::Color{40,150,40}, {xCenter - bSize.x / 2, bSize.y + 160.f}, bSize, getListener(Difficulty::MEDIIUM), {17.f, 15.f});
    auto highBtn = Button(" High ", rl::Color{150,40,40}, {xCenter - bSize.x / 2, bSize.y + 220.f}, bSize, getListener(Difficulty::HIGH), {22.f, 15.f});

    rl::DrawText("Select difficulty", xCenter - bSize.x / 2 - 35.f, 100.f, 20, rl::Color::RayWhite());
    lowBtn.render();
    medBtn.render();
    highBtn.render();
}

auto Game::renderGameWon() -> void {
    auto xCenter = Game::s_Size.x / 2;
    this->renderFinalState();

    rl::DrawText("You cleaned the trash & WON THE GAME!", xCenter - 210.f, 400.f, 20.f, rl::Color::Green());
    m_Player.setPosition({Game::s_Size.x / 2 - 47.f, 500.f});
    m_Player.draw(0);
}
}  // namespace WebDed
