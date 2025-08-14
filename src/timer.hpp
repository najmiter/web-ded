#pragma once

#include <cstdint>
#include <functional>

namespace WebDed {
class Timer {
public:
    Timer(float duration, bool shouldRepeat = false, bool isActive = false, std::function<void()> onComplete = nullptr);

    auto activate() noexcept -> void;
    auto deActivate() noexcept -> void;
    auto update() noexcept -> void;

private:
    float m_StartedAt{};
    float m_Duration{};
    bool m_ShouldRepeat{};
    bool m_IsActive{};
    std::function<void()> m_OnComplete{};
};
}
