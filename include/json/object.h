#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <json/type.h>

namespace json {

class object {
private:
  using value =
      std::variant<std::nullptr_t, bool, int, double, std::string,
                   std::vector<object>, std::map<std::string, object>>;

  type _type;
  value _value;

  void setArrayIfNull();
  void setObjectIfNull();

  static type getType(value &data);

public:
  object();
  object(value &data);
  ~object();

  void operator=(value &data);

  bool isNull() const;
  bool isBoolean() const;
  bool isInteger() const;
  bool isDouble() const;
  bool isString() const;
  bool isArray() const;
  bool isObject() const;

  // For arrays
  void push_back(value &data);
  void push_back(value data);
  void push_back(object &data);
  value &operator[](size_t index);

  // For objects
  void insert(const std::string &itemKey, value &itemValue);
  void insert(const std::string &itemKey, value itemValue);
  void insert(const std::string &itemKey, object &itemValue);
  bool contains(const std::string &itemKey);
  value &operator[](const std::string &itemKey);

  // For arrays and objects
  size_t size();
};

} // namespace json