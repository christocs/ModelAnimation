#include "afk/io/Log.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>

#include "afk/debug/Assert.hpp"

using Afk::Io::Log;

using std::ofstream;
using std::ostringstream;
using std::filesystem::path;

Log::Log() {
  auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  auto ss = ostringstream{};
  ss << std::put_time(std::localtime(&t), "%T-%F") << ".log";

  this->log_path = Afk::get_absolute_path(".log") / ss.str();
  std::filesystem::create_directory(this->log_path.parent_path());
  this->log_file = ofstream{this->log_path};
  afk_assert(this->log_file.is_open(), "Failed to open log file");
}
