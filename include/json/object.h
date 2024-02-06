#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include <logger/log.h>

#include <json/type.h>

namespace json {

class object {
private:
  using value =
      std::variant<std::nullptr_t, bool, long long int, double, std::string>;
  using all_values =
      std::variant<std::nullptr_t, bool, long long int, double, std::string,
                   std::vector<object>, std::map<std::string, object>>;

  type _type;

  void clearVariant();

  void clearArray();
  void setArrayIfUndefined();
  void assertIsArray();

  void clearMap();
  void setMapIfUndefined();
  void assertIsMap();

protected:
  value _value;
  std::vector<object> _array;
  std::map<std::string, object> _map;

public:
  object();
  object(all_values data);
  ~object();

  // Equal to operators
  object &operator=(std::nullptr_t data);
  object &operator=(bool data);
  object &operator=(int data);
  object &operator=(long long int data);
  object &operator=(double data);
  object &operator=(const char *data);
  object &operator=(std::vector<object> data);
  object &operator=(std::map<std::string, object> data);

  // Array operators
  object &operator[](const size_t index);
  const object &operator[](const size_t index) const;
  void push_back(all_values data);
  void push_back(object data);

  // Map operators
  object &operator[](const std::string &key);
  const object &operator[](const std::string &key) const;
  void insert(const std::string key, all_values data);

  // Array and Map operators
  size_t size();
  void clear();

  // object operators
  void reset();
  void dump(std::string path, size_t indent = 0);
  std::string dumps(size_t indent = 0, size_t baseIndent = 0);

  // Output functions
  friend std::ostream &operator<<(std::ostream &os, object &obj);
};

} // namespace json