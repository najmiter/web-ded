#pragma once

#include <functional>
#include <string_view>

#include "raylib-cpp.hpp"

namespace WebDed {
namespace rl = raylib;
class Button {
public:
    Button(std::string_view text, rl::Color bg, rl::Vector2 pos, rl::Vector2 size, std::function<void()> onClick = nullptr, rl::Vector2 pad = {});

    auto render() -> void;

private:
    bool m_IsHovered{};
    std::string_view m_Text{};
    rl::Color m_BgColor{};
    rl::Vector2 m_Position{};
    rl::Vector2 m_Size{};
    std::function<void()> m_OnClick{};
    rl::Vector2 m_Padding{};

};
}
