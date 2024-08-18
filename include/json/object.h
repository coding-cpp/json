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
  using Value =
      std::variant<std::nullptr_t, bool, long long int, double, std::string>;
  using AllValues = std::variant<std::nullptr_t, bool, long long int, double,
                                 std::string, std::vector<json::object>,
                                 std::map<std::string, json::object>>;

  json::TYPE type;

  void clearVariant();

  void clearArray();
  void setArrayIfUndefined();
  void assertIsArray();

  void clearMap();
  void setMapIfUndefined();
  void assertIsMap();

protected:
  json::object::Value value;
  std::vector<json::object> array;
  std::map<std::string, json::object> map;

public:
  // Constructors
  object();
  object(json::object::AllValues data);
  object(const char *data);
  ~object();

  // Assignment operators
  json::object &operator=(std::nullptr_t data);
  json::object &operator=(bool data);
  json::object &operator=(int data);
  json::object &operator=(long long int data);
  json::object &operator=(double data);
  json::object &operator=(const char *data);
  json::object &operator=(const std::string &data);
  json::object &operator=(std::vector<json::object> data);
  json::object &operator=(std::map<std::string, json::object> data);

  // Array operations
  json::object &operator[](const size_t index);
  json::object &operator[](const int index);
  void push_back(const json::object &data);
  void pop_back();
  void reserve(const size_t size);
  void resize(const size_t size);
  void shrink_to_fit();

  // Map operations
  json::object &operator[](const char *key);
  json::object &operator[](const std::string &key);
  std::map<std::string, json::object>::iterator find(const std::string key);
  std::map<std::string, json::object>::iterator begin();
  std::map<std::string, json::object>::iterator end();
  void insert(const std::string key, json::object::AllValues value);

  // Array and Map operators
  void clear();
  size_t size() const;

  // Object operators
  void reset();
  void dump(std::string filename, size_t indent = 0) const;
  std::string dumps(size_t indent = 0, size_t baseIndent = 0) const;

  // Conversion Operators
  operator std::string() const;
  operator int() const;
  operator long long int() const;
  operator double() const;
  operator bool() const;

  // Output functions
  friend std::ostream &operator<<(std::ostream &os, const json::object &obj);
};

} // namespace json