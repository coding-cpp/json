#include <json/object.h>

json::object::object() : _type(type::Null), _value(nullptr) { return; }
json::object::object(bool data) : _type(type::Boolean), _value(data) { return; }
json::object::object(int data) : _type(type::Integer), _value(data) { return; }
json::object::object(double data) : _type(type::Number), _value(data) {
  return;
}
json::object::object(const std::string &data)
    : _type(type::String), _value(data) {
  return;
}
json::object::object(const char *data)
    : _type(type::String), _value(std::string(data)) {
  return;
}
json::object::object(const std::vector<object> &data)
    : _type(type::Array), _value(data) {
  return;
}
json::object::object(const std::map<std::string, object> &data)
    : _type(type::Object), _value(data) {
  return;
}
json::object::object(value &data) {
  if (std::holds_alternative<std::nullptr_t>(data)) {
    this->_type = type::Null;
    this->_value = nullptr;
  } else if (std::holds_alternative<bool>(data)) {
    this->_type = type::Boolean;
    this->_value = std::get<bool>(data);
  } else if (std::holds_alternative<int>(data)) {
    this->_type = type::Integer;
    this->_value = std::get<int>(data);
  } else if (std::holds_alternative<double>(data)) {
    this->_type = type::Number;
    this->_value = std::get<double>(data);
  } else if (std::holds_alternative<std::string>(data)) {
    this->_type = type::String;
    this->_value = std::get<std::string>(data);
  } else if (std::holds_alternative<std::vector<object>>(data)) {
    this->_type = type::Array;
    this->_value = std::get<std::vector<object>>(data);
  } else if (std::holds_alternative<std::map<std::string, object>>(data)) {
    this->_type = type::Object;
    this->_value = std::get<std::map<std::string, object>>(data);
  } else {
    print::error("Unknown type!", "json::object::object(value &data)");
  }

  return;
}
json::object::~object() { return; }

void json::object::operator=(bool data) {
  this->_type = type::Boolean;
  this->_value = data;

  return;
}
void json::object::operator=(int data) {
  this->_type = type::Integer;
  this->_value = data;

  return;
}
void json::object::operator=(double data) {
  this->_type = type::Number;
  this->_value = data;

  return;
}
void json::object::operator=(const std::string &data) {
  this->_type = type::String;
  this->_value = data;

  return;
}
void json::object::operator=(const char *data) {
  this->_type = type::String;
  this->_value = std::string(data);

  return;
}
void json::object::operator=(const std::vector<object> &data) {
  this->_type = type::Array;
  this->_value = data;

  return;
}
void json::object::operator=(const std::map<std::string, object> &data) {
  this->_type = type::Object;
  this->_value = data;

  return;
}

bool json::object::isNull() const { return this->_type == type::Null; }
bool json::object::isInteger() const { return this->_type == type::Integer; }
bool json::object::isBoolean() const { return this->_type == type::Boolean; }
bool json::object::isNumber() const { return this->_type == type::Number; }
bool json::object::isString() const { return this->_type == type::String; }
bool json::object::isArray() const { return this->_type == type::Array; }
bool json::object::isObject() const { return this->_type == type::Object; }

void json::object::push(value &data) {
  this->setArrayIfNull();
  if (!this->isArray()) {
    print::error("Unable to push data to non-array object!",
                 "void json::object::push(value &data)");
  }

  std::get<std::vector<object>>(this->_value).push_back(object(data));
  return;
}
void json::object::push(value data) {
  this->setArrayIfNull();
  if (!this->isArray()) {
    print::error("Unable to push data to non-array object!",
                 "void json::object::push(value data)");
  }

  std::get<std::vector<object>>(this->_value).push_back(object(data));
  return;
}
void json::object::push(object &data) {
  this->setArrayIfNull();
  if (!this->isArray()) {
    print::error("Unable to push data to non-array object!",
                 "void json::object::push(object &data)");
  }

  std::get<std::vector<object>>(this->_value).push_back(data);
  return;
}
json::object::value &json::object::operator[](const size_t index) {
  this->setArrayIfNull();
  if (!this->isArray()) {
    print::error("Unable to get data from non-array object!",
                 "value &json::object::operator[](const size_t index)");
  }

  return std::get<std::vector<object>>(this->_value)[index]._value;
}

void json::object::insert(const std::string &itemKey, value &itemValue) {
  this->setObjectIfNull();
  if (!this->isObject()) {
    print::error("Unable to insert data to non-object object!",
                 "void json::object::insert(const std::string& itemKey, value& "
                 "itemValue)");
  }

  std::get<std::map<std::string, object>>(this->_value)[itemKey] =
      object(itemValue);
  return;
}
void json::object::insert(const std::string &itemKey, value itemValue) {
  this->setObjectIfNull();
  if (!this->isObject()) {
    print::error("Unable to insert data to non-object object!",
                 "void json::object::insert(const std::string& itemKey, value "
                 "itemValue)");
  }

  std::get<std::map<std::string, object>>(this->_value)[itemKey] =
      object(itemValue);
  return;
}
void json::object::insert(const std::string &itemKey, object &itemValue) {
  this->setObjectIfNull();
  if (!this->isObject()) {
    print::error(
        "Unable to insert data to non-object object!",
        "void json::object::insert(const std::string& itemKey, object& "
        "itemValue)");
  }

  std::get<std::map<std::string, object>>(this->_value)[itemKey] = itemValue;
  return;
}
bool json::object::contains(const std::string &itemKey) {
  this->setObjectIfNull();
  if (!this->isObject()) {
    print::error("Unable to reference non-object object via string!",
                 "bool json::object::contains(const std::string &itemKey)");
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
    print::error("Unable to reference non-object object via string!",
                 "json::object::value &json::object::operator[](const "
                 "std::string &itemKey)");
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

  print::error("Unable to get size of non-(string, array, object)",
               "size_t json::object::size()");
  return 0;
}
json::type json::object::getType() { return this->_type; }

std::string json::object::dump(int indent, int baseIndent) const {
  if (this->isNull()) {
    return "null";
  } else if (this->isBoolean()) {
    return std::get<bool>(this->_value) ? "true" : "false";
  } else if (this->isInteger()) {
    return std::to_string(std::get<int>(this->_value));
  } else if (this->isNumber()) {
    return std::to_string(std::get<double>(this->_value));
  } else if (this->isString()) {
    return "\"" + std::get<std::string>(this->_value) + "\"";
  }

  size_t _size;
  std::string result;
  if (baseIndent == -1) {
    baseIndent = indent;
  }

  if (this->isArray()) {
    _size = std::get<std::vector<object>>(this->_value).size();
    result = "[";
    if (indent > 0)
      result += "\n";

    for (size_t i = 0; i < _size; i++) {
      result += std::string(indent, ' ') +
                std::get<std::vector<object>>(this->_value)[i].dump(
                    indent + baseIndent, baseIndent);
      if (i != _size - 1) {
        result += ",";
      }
      if (indent > 0)
        result += "\n";
    }

    result += std::string(indent - baseIndent, ' ') + "]";
  } else {
    _size = std::get<std::map<std::string, object>>(this->_value).size();
    result = "{";
    if (indent > 0)
      result += "\n";

    size_t i = 0;
    for (const std::pair<const std::string, object> &item :
         std::get<std::map<std::string, object>>(this->_value)) {
      result += std::string(indent, ' ') + "\"" + item.first + "\":";
      if (indent > 0)
        result += " ";
      result += item.second.dump(indent + baseIndent, baseIndent);

      if (i != _size - 1)
        result += ",";
      if (indent > 0)
        result += "\n";
      i++;
    }

    result += std::string(indent - baseIndent, ' ') + "}";
  }

  return result;
}
void json::object::dumps(const std::string &filePath, int indent) {
  std::ofstream jsonFile(filePath);
  if (!jsonFile.is_open()) {
    print::error("Unable to open " + filePath,
                 "void json::object::dumps(const std::string &filePath, int "
                 "indent = 0)");
  }

  std::string _json = this->dump(indent);
  jsonFile.write(_json.c_str(), _json.size());
  jsonFile.close();

  return;
}

void json::object::copyTo(object &obj) {
  switch (this->_type) {
  case type::Boolean:
    obj = std::get<bool>(this->_value);
    break;

  case type::Integer:
    obj = std::get<int>(this->_value);
    break;

  case type::Number:
    obj = std::get<double>(this->_value);
    break;

  case type::String:
    obj = std::get<std::string>(this->_value);
    break;

  case type::Array:
    obj = std::get<std::vector<object>>(this->_value);
    break;

  case type::Object:
    obj = std::get<std::map<std::string, object>>(this->_value);
    break;

  default:
    obj = nullptr;
    break;
  }

  return;
}
json::object json::object::copy() {
  object newObject;
  this->copyTo(newObject);

  return newObject;
}
void json::object::clear() {
  this->_type = type::Null;
  this->_value = nullptr;

  return;
}

void json::object::setArrayIfNull() {
  if (this->_type == type::Null) {
    this->_type = type::Array;
    this->_value = std::vector<object>();
  }

  return;
}
void json::object::setObjectIfNull() {
  if (this->_type == type::Null) {
    this->_type = type::Object;
    this->_value = std::map<std::string, object>();
  }

  return;
}
