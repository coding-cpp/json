#pragma once

#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include <json/print.h>
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
  object(bool data);
  object(int data);
  object(double data);
  object(const std::string &data);
  object(const char *data);
  object(const std::vector<object> &data);
  object(const std::map<std::string, object> &data);
  object(value &data);
  ~object();

  void operator=(bool data);
  void operator=(int data);
  void operator=(double data);
  void operator=(const std::string &data);
  void operator=(const char *data);
  void operator=(const std::vector<object> &data);
  void operator=(const std::map<std::string, object> &data);

  bool isNull() const;
  bool isBoolean() const;
  bool isInteger() const;
  bool isNumber() const;
  bool isString() const;
  bool isArray() const;
  bool isObject() const;

  // For array
  void push(value &data);
  void push(value data);
  void push(object &data);
  value &operator[](const size_t index);

  // For object
  void insert(const std::string &itemKey, value &itemValue);
  void insert(const std::string &itemKey, value itemValue);
  void insert(const std::string &itemKey, object &itemValue);
  bool contains(const std::string &itemKey);
  value &operator[](const std::string &itemKey);

  size_t size();
  type getType();

  std::string dump(int indent = 0, int baseIndent = -1) const;
  void dumps(const std::string &filePath, int indent = 0);

  void copyTo(object &obj);
  object copy();
  void clear();
};

} // namespace json
