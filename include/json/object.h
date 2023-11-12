#pragma once

#include <algorithm>
#include <fstream>
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

  static type getType(value &data);

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

  // Overall
  std::string dump(int indent = 0, int baseline = 0);
  void dumps(const std::string &filePath, int indent = 0);
};

} // namespace json