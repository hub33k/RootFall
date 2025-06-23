#pragma once

#include "KeyCodes.hpp"

#include <map>
#include <string>

namespace hub33k {

  enum ActionName {};

  enum ActionType {
    START = 0,
    END,
  };

  using ActionMap = std::map<KeyCode, std::string>;

  // eg. {"move_right", ActionType::START}
  class Action {
  public:
    Action() = delete;
    Action(std::string name, const ActionType &type);

    [[nodiscard]] const std::string &GetName() const { return m_Name; }
    [[nodiscard]] const ActionType &GetType() const { return m_Type; }

    [[nodiscard]] std::string ToString() const;

  private:
    std::string m_Name;
    ActionType m_Type;
  };

} // namespace hub33k
