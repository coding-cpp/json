#include <json/object.h>

json::object::object() : _type(type::Null) { return; }

json::object::object(value &data) {
  this->_type = this->getType(data);
  this->_value = std::move(data);
  return;
}

json::object::~object() { return; }

void json::object::operator=(value &data) {
  this->_type = this->getType(data);
  this->_value = std::move(data);
  return;
}

bool json::object::isNull() const { return this->_type == type::Null; }
bool json::object::isBoolean() const { return this->_type == type::Boolean; }
bool json::object::isInteger() const { return this->_type == type::Integer; }
bool json::object::isDouble() const { return this->_type == type::Double; }
bool json::object::isString() const { return this->_type == type::String; }
bool json::object::isArray() const { return this->_type == type::Array; }
bool json::object::isObject() const { return this->_type == type::Object; }

void json::object::push_back(value &data) {
  this->setArrayIfNull();
  if (!this->isArray()) {
    throw std::runtime_error("[ERROR] json::object::push_back(value "
                             "&data) - Cannot push_back into non-array.");
    return;
  }

  std::get<std::vector<object>>(this->_value).push_back(object(data));
  return;
}
void json::object::push_back(value data) {
  this->setArrayIfNull();
  if (!this->isArray()) {
    throw std::runtime_error("[ERROR] json::object::push_back(value "
                             "data) - Cannot push_back into non-array.");
    return;
  }

  std::get<std::vector<object>>(this->_value).push_back(object(data));
  return;
}
void json::object::push_back(object &data) {
  this->setArrayIfNull();
  if (!this->isArray()) {
    throw std::runtime_error("[ERROR] json::object::push_back(object &data) - "
                             "Cannot push_back into non-array.");
    return;
  }

  std::get<std::vector<object>>(this->_value).push_back(std::move(data));
  return;
}
json::object::value &json::object::operator[](size_t index) {
  this->setArrayIfNull();
  if (!this->isArray()) {
    throw std::runtime_error("[ERROR] &json::object::operator=(size_t index) - "
                             "Cannot index non-array.");
  }

  return std::get<std::vector<object>>(this->_value)[index]._value;
}

void json::object::insert(const std::string &itemKey, value &itemValue) {
  this->setObjectIfNull();
  if (!this->isObject()) {
    throw std::runtime_error(
        "[ERROR] json::object::insert(const std::string &itemKey, value "
        "&itemValue) - Cannot insert into non-objects");
    return;
  }

  std::get<std::map<std::string, object>>(this->_value)[itemKey] =
      object(itemValue);
  return;
}
void json::object::insert(const std::string &itemKey, value itemValue) {
  this->setObjectIfNull();
  if (!this->isObject()) {
    throw std::runtime_error(
        "[ERROR] json::object::insert(const std::string &itemKey, value "
        "itemValue) - Cannot insert into non-objects");
    return;
  }

  std::get<std::map<std::string, object>>(this->_value)[itemKey] =
      object(itemValue);
  return;
}
void json::object::insert(const std::string &itemKey, object &itemValue) {
  this->setObjectIfNull();
  if (!this->isObject()) {
    throw std::runtime_error(
        "[ERROR] json::object::insert(const std::string &itemKey, object "
        "&itemValue) - Cannot insert into non-objects");
    return;
  }

  std::get<std::map<std::string, object>>(this->_value)[itemKey] = itemValue;
  return;
}
bool json::object::contains(const std::string &itemKey) {
  this->setObjectIfNull();
  if (!this->isObject()) {
    throw std::runtime_error("[ERROR] json::object::contains(const std::string "
                             "&itemKey) - Cannot reference non-objects");
    return false;
  }

  if (std::get<std::map<std::string, object>>(this->_value).find(itemKey) ==
      std::get<std::map<std::string, object>>(this->_value).end()) {
    return false;
  }
  return true;
}
json::object::value &json::object::operator[](const std::string &itemKey) {
  this->setObjectIfNull();
  if (!this->isObject()) {
    throw std::runtime_error("[ERROR] json::object::contains(const std::string "
                             "&itemKey) - Cannot reference non-objects");
  }

  return std::get<std::map<std::string, object>>(this->_value)[itemKey]._value;
}

size_t json::object::size() {
  if (this->isArray()) {
    return std::get<std::vector<object>>(this->_value).size();
  }

  if (this->isObject()) {
    return std::get<std::map<std::string, object>>(this->_value).size();
  }

  if (this->isString()) {
    return std::get<std::string>(this->_value).size();
  }

  throw std::runtime_error("[ERROR] json::object::size() - Cannot get size of "
                           "non-(string, array, map)");
  return 0;
}

void json::object::setArrayIfNull() {
  if (this->isNull()) {
    this->_type = type::Array;
    this->_value = std::vector<object>();
  }
  return;
}
void json::object::setObjectIfNull() {
  if (this->isNull()) {
    this->_type = type::Object;
    this->_value = std::map<std::string, object>();
  }
  return;
}
json::type json::object::getType(value &data) {
  if (std::holds_alternative<std::nullptr_t>(data)) {
    return type::Null;
  } else if (std::holds_alternative<bool>(data)) {
    return type::Boolean;
  } else if (std::holds_alternative<int>(data)) {
    return type::Integer;
  } else if (std::holds_alternative<double>(data)) {
    return type::Double;
  } else if (std::holds_alternative<std::string>(data)) {
    return type::String;
  } else if (std::holds_alternative<std::vector<object>>(data)) {
    return type::Array;
  } else if (std::holds_alternative<std::map<std::string, object>>(data)) {
    return type::Object;
  } else {
    throw std::runtime_error(
        "[ERROR] json::object::object(value &objectValue) - "
        "Invalid type.");
  }
}