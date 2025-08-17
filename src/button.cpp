#include "button.hpp"
#include "raylib.h"

namespace WebDed {
Button::Button(std::string_view text, rl::Color bg, rl::Vector2 pos, rl::Vector2 size, std::function<void()> onClick, rl::Vector2 pad)
    : m_Text(text), m_BgColor(bg), m_Position(pos), m_Size(size), m_OnClick(onClick), m_Padding(pad) { }

auto Button::render() -> void {
    rl::Rectangle rect = {m_Position.x, m_Position.y, m_Size.x, m_Size.y};
    rl::Vector2 mAt = rl::Mouse::GetPosition();
    auto isHovered = rect.CheckCollision(mAt);
    auto isClicked = isHovered && rl::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON);

    if (isClicked && m_OnClick) m_OnClick();
    if (isHovered) rl::Mouse::SetCursor(MOUSE_CURSOR_POINTING_HAND);

    m_BgColor.a = isHovered ? 150 : 255;
    rect.DrawRounded(0.5f, 10, m_BgColor);
    rl::DrawText(m_Text.data(), rect.x + m_Padding.x, rect.y + m_Padding.y, 20, rl::Color::RayWhite());
}
}
