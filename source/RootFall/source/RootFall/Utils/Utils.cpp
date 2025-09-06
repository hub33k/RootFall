#include "Utils.hpp"

namespace hub33k {

  std::string ReadFile(const std::string &path) {
    if (!std::filesystem::exists(path)) {
      HK_LOG_ERROR("File does not exist: {0}", path);
      throw std::runtime_error("File does not exist: " + path);
    }

    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    return content;
  }

} // namespace hub33k
