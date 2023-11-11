#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace json {

class object {
public:
  using value =
      std::variant<std::nullptr_t, bool, int, double, std::string,
                   std::vector<object>, std::map<std::string, object>>;

  object();
  ~object();

private:
  value data;
};

} // namespace json