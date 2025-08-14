#include "raylib.h"
#include "timer.hpp"

namespace WebDed {
Timer::Timer(float duration, bool shouldRepeat, bool isActive, std::function<void()> onComplete)
    : m_Duration(duration), m_ShouldRepeat(shouldRepeat), m_IsActive(isActive), m_OnComplete(onComplete) {}

auto Timer::activate() noexcept -> void {
    m_IsActive = true;
    m_StartedAt = GetTime();
}

auto Timer::deActivate() noexcept -> void {
    m_IsActive = false;
    m_StartedAt = 0.0f;
    if (m_ShouldRepeat) {
        this->activate();
    }
}

auto Timer::update() noexcept -> void {
    if (!m_IsActive) return;

    if (GetTime() - m_StartedAt >= m_Duration) {
        if (m_OnComplete && m_StartedAt) m_OnComplete();
        this->deActivate();
    }
}
}
