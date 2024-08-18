#include <utility>

#include <json/object.h>

// Constructors
json::object::object() : type(json::TYPE::UNDEFINED), value(nullptr) { return; }
json::object::object(json::object::AllValues data)
    : type(json::TYPE::UNDEFINED), value(nullptr) {
  this->reset();
  switch (data.index()) {
  case 0:
    this->value = std::get<std::nullptr_t>(data);
    this->type = json::TYPE::NUL;
    break;
  case 1:
    this->value = std::get<bool>(data);
    this->type = json::TYPE::BOOLEAN;
    break;
  case 2:
    this->value = std::get<long long int>(data);
    this->type = json::TYPE::INTEGER;
    break;
  case 3:
    this->value = std::get<double>(data);
    this->type = json::TYPE::NUMBER;
    break;
  case 4:
    this->value = std::get<std::string>(data);
    this->type = json::TYPE::STRING;
    break;
  case 5:
    this->array = std::get<std::vector<json::object>>(data);
    this->type = json::TYPE::ARRAY;
    break;
  case 6:
    this->map = std::get<std::map<std::string, json::object>>(data);
    this->type = json::TYPE::OBJECT;
    break;
  default:
    logger::error("The data type is not supported",
                  "json::object::object(json::object::AllValues data)");
    break;
  }
  return;
}
json::object::object(const char *data)
    : type(json::TYPE::UNDEFINED), value(nullptr) {
  *this = std::string(data);
  return;
}
json::object::~object() {
  this->clearVariant();
  return;
}

// Assignment operators
json::object &json::object::operator=(std::nullptr_t data) {
  return *this = json::object(data);
}
json::object &json::object::operator=(bool data) {
  return *this = json::object(data);
}
json::object &json::object::operator=(int data) {
  return *this = json::object(data);
}
json::object &json::object::operator=(long long int data) {
  return *this = json::object(data);
}
json::object &json::object::operator=(double data) {
  return *this = json::object(data);
}
json::object &json::object::operator=(const char *data) {
  return *this = json::object(data);
}
json::object &json::object::operator=(const std::string &data) {
  return *this = json::object(data);
}
json::object &json::object::operator=(std::vector<json::object> data) {
  return *this = json::object(data);
}
json::object &
json::object::operator=(std::map<std::string, json::object> data) {
  return *this = json::object(data);
}

// Array operations
json::object &json::object::operator[](size_t index) {
  this->assertIsArray();
  if (index >= this->array.size()) {
    this->array.resize(index + 1);
  }
  return this->array[index];
}
json::object &json::object::operator[](int index) {
  return this->operator[]((size_t)index);
}
void json::object::push_back(const json::object &data) {
  this->assertIsArray();
  this->array.push_back(data);
  return;
}
void json::object::pop_back() {
  this->assertIsArray();
  this->array.pop_back();
  return;
}
void json::object::reserve(size_t size) {
  this->assertIsArray();
  this->array.reserve(size);
  return;
}
void json::object::resize(size_t size) {
  this->assertIsArray();
  this->array.resize(size);
  return;
}
void json::object::shrink_to_fit() {
  this->assertIsArray();
  this->array.shrink_to_fit();
  return;
}

// Map operations
json::object &json::object::operator[](const char *key) {
  return this->operator[](std::string(key));
}
json::object &json::object::operator[](const std::string &key) {
  this->assertIsMap();
  return this->map[key];
}
std::map<std::string, json::object>::iterator
json::object::find(const std::string key) {
  this->assertIsMap();
  return this->map.find(key);
}
std::map<std::string, json::object>::iterator json::object::begin() {
  this->assertIsMap();
  return this->map.begin();
}
std::map<std::string, json::object>::iterator json::object::end() {
  this->assertIsMap();
  return this->map.end();
}
void json::object::insert(const std::string key,
                          json::object::AllValues value) {
  this->assertIsMap();
  this->map[key] = json::object(value);
  return;
}

// Array and Map operators
void json::object::clear() {
  if (this->type == json::TYPE::ARRAY) {
    this->clearArray();
  } else if (this->type == json::TYPE::OBJECT) {
    this->clearMap();
  } else {
    logger::error("The object is not an array or a map",
                  "void json::object::clear()");
  }
  return;
}
size_t json::object::size() const {
  if (this->type == json::TYPE::ARRAY) {
    return this->array.size();
  } else if (this->type == json::TYPE::OBJECT) {
    return this->map.size();
  } else if (this->type == json::TYPE::STRING) {
    return std::get<std::string>(this->value).size();
  } else {
    logger::error("The object is not a string, array, or map",
                  "size_t json::object::size()");
  }
  return 0;
}

// Object operators
void json::object::reset() {
  this->clearVariant();
  this->clearArray();
  this->clearMap();
  this->type = json::TYPE::UNDEFINED;
  return;
}
void json::object::dump(std::string filename, size_t indent) const {
  std::ofstream file;
  file.open(filename);
  if (!file.is_open()) {
    logger::error(
        "Failed to open file " + filename,
        "void json::object::dump(std::string filename, size_t indent)");
  }

  file << this->dumps(indent);
  file.close();
  return;
}
std::string json::object::dumps(size_t indent, size_t baseIndent) const {
  std::string result = "";
  int newBaseIndent = indent + baseIndent;
  switch (this->type) {
  case json::TYPE::UNDEFINED:
    result = "undefined";
    break;
  case json::TYPE::NUL:
    result = "null";
    break;
  case json::TYPE::BOOLEAN:
    result = std::get<bool>(this->value) ? "true" : "false";
    break;
  case json::TYPE::INTEGER:
    result = std::to_string(std::get<long long int>(this->value));
    break;
  case json::TYPE::NUMBER:
    result = std::to_string(std::get<double>(this->value));
    break;
  case json::TYPE::STRING:
    result = "\"" + std::get<std::string>(this->value) + "\"";
    break;
  case json::TYPE::ARRAY:
    result = "[";
    if (this->size() == 0) {
      result += "]";
      return result;
    }
    if (indent > 0)
      result += "\n";
    for (size_t i = 0; i < this->size(); i++) {
      result += std::string(newBaseIndent, ' ');
      result += this->array[i].dumps(indent, newBaseIndent);
      if (i + 1 != this->size())
        result += ",";
      if (indent > 0)
        result += "\n";
      else if (i + 1 != this->size())
        result += " ";
    }
    result += std::string(baseIndent, ' ') + "]";
    break;
  case json::TYPE::OBJECT:
    result = "{";
    if (this->size() == 0) {
      result += "}";
      return result;
    }
    if (indent > 0)
      result += "\n";
    for (std::map<std::string, json::object>::const_iterator it =
             this->map.begin();
         it != this->map.end(); it++) {
      result += std::string(newBaseIndent, ' ');
      result += "\"" + it->first + "\": ";
      result += it->second.dumps(indent, newBaseIndent);
      if (std::next(it) != this->map.end())
        result += ",";
      if (indent > 0)
        result += "\n";
      else if (std::next(it) != this->map.end())
        result += " ";
    }
    result += std::string(baseIndent, ' ') + "}";
  }

  return result;
}

// Conversion Operators
json::object::operator std::string() const {
  if (this->type == json::TYPE::STRING) {
    return std::get<std::string>(this->value);
  } else {
    logger::error("The object is not a string",
                  "json::object::operator std::string()");
  }
  return "";
}
json::object::operator int() const {
  if (this->type == json::TYPE::INTEGER) {
    return (int)std::get<long long int>(this->value);
  } else {
    logger::error("The object is not an integer",
                  "json::object::operator int()");
  }
  return 0;
}
json::object::operator long long int() const {
  if (this->type == json::TYPE::INTEGER) {
    return (long long int)std::get<long long int>(this->value);
  } else {
    logger::error("The object is not an integer",
                  "json::object::operator long long int()");
  }
  return 0;
}
json::object::operator double() const {
  if (this->type == json::TYPE::NUMBER) {
    return std::get<double>(this->value);
  } else {
    logger::error("The object is not a number",
                  "json::object::operator double()");
  }
  return 0.0;
}
json::object::operator bool() const {
  if (this->type == json::TYPE::BOOLEAN) {
    return std::get<bool>(this->value);
  } else {
    logger::error("The object is not a boolean",
                  "json::object::operator bool()");
  }
  return false;
}

// Private methods
void json::object::clearVariant() {
  this->value = nullptr;
  return;
}

void json::object::clearArray() {
  this->array.clear();
  return;
}
void json::object::setArrayIfUndefined() {
  if (this->type == json::TYPE::UNDEFINED) {
    this->clearVariant();
    this->clearMap();
    this->type = json::TYPE::ARRAY;
  }
  return;
}
void json::object::assertIsArray() noexcept(false) {
  this->setArrayIfUndefined();
  if (this->type != json::TYPE::ARRAY) {
    logger::error("The object is not an array",
                  "void json::object::assertIsArray()");
  }
  return;
}

void json::object::clearMap() {
  this->map.clear();
  return;
}
void json::object::setMapIfUndefined() {
  if (this->type == json::TYPE::UNDEFINED) {
    this->clearVariant();
    this->clearArray();
    this->type = json::TYPE::OBJECT;
  }
  return;
}
void json::object::assertIsMap() noexcept(false) {
  this->setMapIfUndefined();
  if (this->type != json::TYPE::OBJECT) {
    logger::error("The object is not a map",
                  "void json::object::assertIsMap()");
  }
  return;
}

// Friend functions
namespace json {

std::ostream &operator<<(std::ostream &os, object &obj) {
  os << obj.dumps(2);
  return os;
}

} // namespace json