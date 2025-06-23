#include "Action.hpp"

namespace hub33k {

  Action::Action(std::string name, const ActionType &type) : m_Name(std::move(name)), m_Type(type) {}

  std::string Action::ToString() const {
    const std::string type = m_Type == ActionType::START ? "start" : "end";
    return m_Name + " (" + type + ")";
  }

} // namespace hub33k
