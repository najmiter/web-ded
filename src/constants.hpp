#pragma once

#include <cstdint>
#include <string_view>
#include <unordered_map>

#include "utils.cpp"

namespace WebDed {
enum class Asset {
    PLAYER = 0, LASER,
    TRASH_0,TRASH_1,TRASH_2,TRASH_3,
};

// tried string_view but the stupid garbage collectors have poisoned my soul!!
const static inline std::unordered_map<Asset, const std::string> g_SpriteAssets {
    {Asset::PLAYER, Utils::joinPath("assets", "protagonist.png").string()},
    {Asset::LASER, Utils::joinPath("assets", "laser.png").string()},
    {Asset::TRASH_0, Utils::joinPath("assets", "trash", "0.png").string()},
    {Asset::TRASH_1, Utils::joinPath("assets", "trash", "1.png").string()},
    {Asset::TRASH_2, Utils::joinPath("assets", "trash", "2.png").string()},
    {Asset::TRASH_3, Utils::joinPath("assets", "trash", "3.png").string()},
};

constexpr Asset g_AllAssets[] = {
    Asset::PLAYER,
    Asset::LASER,
    Asset::TRASH_0,
    Asset::TRASH_1,
    Asset::TRASH_2,
    Asset::TRASH_3,
};

constexpr Asset g_Trashes[] = {
    Asset::TRASH_0,
    Asset::TRASH_1,
    Asset::TRASH_2,
    Asset::TRASH_3,
};

enum class GameState {
    MENU = 0,
    PLAY,
    GAME_OVER,
    GAME_WON,
    QUIT
};
}
