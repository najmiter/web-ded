#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>

#include "sprite.hpp"

#define ERASABLE_TEMPLATE template <typename T> \
    requires std::is_base_of_v<Sprite, T>

namespace WebDed {
ERASABLE_TEMPLATE
class ErasableTexture : Sprite {
   public:
    auto drawVisible(float dt) noexcept -> void;
    auto add(T&& sprite) noexcept -> void;
    auto getTextures() noexcept -> std::vector<T>&;

   private:
    std::vector<T> m_Textures{};
};

ERASABLE_TEMPLATE
auto ErasableTexture<T>::drawVisible(float dt) noexcept -> void {
    auto erasables{
        std::remove_if(m_Textures.begin(), m_Textures.end(), [](T& trash) {
            return trash.checkShouldDie();
        })};
    m_Textures.erase(erasables, m_Textures.end());

    std::for_each(m_Textures.begin(), m_Textures.end(), [&](T& trash) {
        trash.update(dt);
        trash.draw();
    });
}

ERASABLE_TEMPLATE
auto ErasableTexture<T>::add(T&& sprite) noexcept -> void {
    m_Textures.emplace_back(std::move(sprite));
}

ERASABLE_TEMPLATE
auto ErasableTexture<T>::getTextures() noexcept -> std::vector<T>& {
    return m_Textures;
}
}  // namespace WebDed
