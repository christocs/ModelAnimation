#pragma once
#include <functional>

#include "Event.hpp"

namespace Afk {
  using Callback = std::function<void(const Afk::Event &)>;
}
