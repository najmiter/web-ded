#pragma once

#include "sprite.hpp"

namespace WebDed {
class Animation : Sprite {
public:
    Animation() = default;
    Animation(rl::Image&& texture);

    auto animate(float dt) -> void;
    auto reset(rl::Vector2 pos) -> void;

private:
    rl::Vector2 m_Position{};
    int m_Frame{};
    float m_FrameTime{};
    bool m_IsActive{};
};
}
