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

void json::object::setArrayIfNull() {
  if (this->isNull()) {
    this->_type = type::Array;
    this->_value = std::vector<object>();
  }
  return;
}

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

std::string json::object::dump(int indent, int baseline) {
  std::string result = "";
  indent = indent < 0 ? 0 : indent;

  if (this->isNull()) {
    result += "null";
  } else if (this->isBoolean()) {
    result += std::get<bool>(this->_value) ? "true" : "false";
  } else if (this->isInteger()) {
    result += std::to_string(std::get<int>(this->_value));
  } else if (this->isDouble()) {
    result += std::to_string(std::get<double>(this->_value));
  } else if (this->isString()) {
    result += "\"" + std::get<std::string>(this->_value) + "\"";
  } else if (this->isArray()) {
    result += "[";
    result += indent > 0 ? "\n" : "";
    for (size_t i = 0; i < this->size(); i++) {
      result += std::string(indent, ' ') +
                std::get<std::vector<object>>(this->_value)[i].dump(indent * 2,
                                                                    indent);
      if (i != this->size() - 1) {
        result += ",";
      }
      result += indent > 0 ? "\n" : "";
    }
    result += std::string(baseline, ' ') + "]";
  } else if (this->isObject()) {
    result += "{";
    result += indent > 0 ? "\n" : "";
    size_t i = 0;
    for (std::pair<const std::string, object> &item :
         std::get<std::map<std::string, object>>(this->_value)) {
      result += std::string(indent, ' ') + "\"" + item.first +
                "\": " + item.second.dump(indent * 2, indent);
      if (i != this->size() - 1) {
        result += ",";
      }
      result += indent > 0 ? "\n" : "";
      i++;
    }
    result += std::string(baseline, ' ') + "}";
  } else {
    throw std::runtime_error("[ERROR] json::object::dump(int indent = 0) - "
                             "Invalid type.");
  }

  if (indent == 0) {
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace),
                 result.end());
  }
  return result;
}
void json::object::dumps(const std::string &filePath, int indent) {
  std::ofstream jsonFile(filePath);
  if (!jsonFile.is_open()) {
    throw std::runtime_error("[ERROR] json::object::dumps(const std::string "
                             "&filePath, int indent = 0) - Cannot open " +
                             filePath);
  }

  std::string _json = this->dump(indent);
  jsonFile << _json;
  jsonFile.close();

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