#include <filesystem>
#include <concepts>
#include <random>
#include <string>
#include <string_view>
#include <print>

namespace fs = std::filesystem;

namespace WebDed {
namespace Utils {

template <typename... Strings>
    requires ((std::convertible_to<Strings, fs::path> || std::convertible_to<Strings, std::string_view>) && ...)
auto joinPath(const Strings&... str) -> fs::path {
    fs::path result;
    ((result /= fs::path(str)), ...);
    // std::println("\n\n\nresult: {}\n\n\n", result.c_str());
    return result;
}

template <typename Container>
auto pickRandom(const Container& collection) -> const typename Container::value_type& {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, collection.size() - 1);
    return collection[dist(gen)];
}

template <typename T, std::size_t N>
constexpr auto pickRandom(const T (&arr)[N]) -> const T& {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, N - 1);
    return arr[dist(gen)];
}


}
}
