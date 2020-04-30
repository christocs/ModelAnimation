#pragma once

#include <array>

namespace Afk {
  template<typename V, typename... T>
  constexpr auto array_of(T &&... t) -> std::array<V, sizeof...(T)> {
    return {{std::forward<T>(t)...}};
  }
}
