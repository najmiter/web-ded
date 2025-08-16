#include "animation.hpp"

namespace WebDed {
Animation::Animation(rl::Image&& texture)
    : Sprite(std::move(texture)) {}

auto Animation::animate(float dt) -> void {
    if (m_IsActive) {
        m_FrameTime += dt;

        if (m_FrameTime >= 0.05f) {
            m_Frame++;
            m_FrameTime = 0.f;
        }

        // done
        if (m_Frame > 15) {
            m_IsActive = false;
        } else {
            int col = m_Frame % 4;
            int row = m_Frame / 4;

            rl::Rectangle src = {col * 64.f, row * 64.f, 64.f, 64.f};
            m_Texture.Draw(src, m_Position);
        }
    }
}

auto Animation::reset(rl::Vector2 pos) -> void {
    m_Position = pos;
    m_Frame = 0;
    m_FrameTime = 0.f;
    m_IsActive = true;
}
}  // namespace WebDed
