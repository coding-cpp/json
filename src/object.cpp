#include <json/object.h>

json::object::object()
    : _value(nullptr), _map(std::map<std::string, object>()),
      _array(std::vector<object>()), _type(type()) {
  return;
}
json::object::object(all_values data) {
  if (data.index() == 0)
    *this = std::get<std::nullptr_t>(data);
  else if (data.index() == 1)
    *this = std::get<bool>(data);
  else if (data.index() == 2)
    *this = std::get<long long int>(data);
  else if (data.index() == 3)
    *this = std::get<double>(data);
  else if (data.index() == 4)
    *this = std::get<std::string>(data).c_str();
  else if (data.index() == 5)
    *this = std::get<std::vector<object>>(data);
  else if (data.index() == 6)
    *this = std::get<std::map<std::string, object>>(data);
  else
    logger::error("Invalid data type passed",
                  "json::object::object(all_values data)");
  return;
}
json::object::~object() { return; }

// Equal to operators ---------------------------------

json::object &json::object::operator=(std::nullptr_t data) {
  this->_value = data;
  this->_type.set(type::null);
  this->clearArray();
  this->clearMap();
  return *this;
}
json::object &json::object::operator=(bool data) {
  this->_value = data;
  this->_type.set(type::boolean);
  this->clearArray();
  this->clearMap();
  return *this;
}
json::object &json::object::operator=(int data) {
  this->_value = static_cast<long long int>(data);
  this->_type.set(type::integer);
  this->clearArray();
  this->clearMap();
  return *this;
}
json::object &json::object::operator=(long long int data) {
  this->_value = data;
  this->_type.set(type::integer);
  this->clearArray();
  this->clearMap();
  return *this;
}
json::object &json::object::operator=(double data) {
  this->_value = data;
  this->_type.set(type::number);
  this->clearArray();
  this->clearMap();
  return *this;
}
json::object &json::object::operator=(const char *data) {
  this->_value = data;
  this->_type.set(type::string);
  this->clearArray();
  this->clearMap();
  return *this;
}
json::object &json::object::operator=(std::vector<object> data) {
  this->_array = data;
  this->_type.set(type::array);
  this->clearVariant();
  this->clearMap();
  return *this;
}
json::object &json::object::operator=(std::map<std::string, object> data) {
  this->_map = data;
  this->_type.set(type::map);
  this->clearVariant();
  this->clearMap();
  return *this;
}

// Array operators ------------------------------------

json::object &json::object::operator[](const size_t index) {
  this->assertIsArray();
  if (index >= this->_array.size()) {
    logger::error("Index out of range",
                  "json::object &json::object::operator[](const size_t index)");
  }

  return this->_array[index];
}
const json::object &json::object::operator[](const size_t index) const {
  if (!this->_type.get() == type::array) {
    logger::error("This object is not an array",
                  "const json::object &json::object::operator[](const size_t "
                  "index) const");
  }

  if (index >= this->_array.size()) {
    logger::error("Index out of range",
                  "const json::object &json::object::operator[](const size_t "
                  "index) const");
  }

  return this->_array.at(index);
}
void json::object::pop_back() {
  this->assertIsArray();
  this->_array.pop_back();
  return;
}
void json::object::push_back(all_values data) {
  this->assertIsArray();
  object *newObject = new object(data);
  this->_array.push_back(*newObject);
  return;
}
void json::object::push_back(object data) {
  this->assertIsArray();
  this->_array.push_back(data);
  return;
}
void json::object::reserve(size_t size) {
  this->assertIsArray();
  this->_array.reserve(size);
  return;
}
void json::object::resize(size_t size) {
  this->assertIsArray();
  this->_array.resize(size);
  return;
}
void json::object::shrink_to_fit() {
  this->assertIsArray();
  this->_array.shrink_to_fit();
  return;
}

// Map operators --------------------------------------

json::object &json::object::operator[](const std::string &key) {
  if (!this->_type.get() == type::map) {
    logger::error(
        "This object is not an object",
        "json::object &json::object::operator[](const std::string &key)");
  }

  this->_type.set(type::map);
  return this->_map[key];
}
const json::object &json::object::operator[](const std::string &key) const {
  if (this->_type.get() != type::map) {
    logger::error("This object is not an object",
                  "const json::object &json::object::operator[](const "
                  "std::string &key) const");
  }

  return this->_map.at(key);
}
void json::object::erase(const std::string key) {
  this->assertIsMap();
  this->_map.erase(key);
  return;
}
std::map<std::string, json::object>::iterator
json::object::find(const std::string key) {
  this->assertIsMap();
  return this->_map.find(key);
}
void json::object::insert(const std::string key, all_values data) {
  this->assertIsMap();
  object *newObject = new object(data);
  this->_map[key] = *newObject;
  return;
}

// Array and Map operators ----------------------------

size_t json::object::size() {
  if (this->_type.get() == type::array) {
    return this->_array.size();
  } else if (this->_type.get() == type::map) {
    return this->_map.size();
  } else {
    logger::error("Cannot get size of non-[array, map] objects",
                  "size_t json::object::size()");
  }
  return -1;
}
void json::object::clear() {
  if (this->_type.get() == type::array) {
    this->_array.clear();
  } else if (this->_type.get() == type::map) {
    this->_map.clear();
  } else {
    logger::error("Cannot clear non-[array, map] objects",
                  "void json::object::clear()");
  }
  return;
}

// object operators -----------------------------------

void json::object::reset() {
  this->clearVariant();
  this->clearArray();
  this->clearMap();
  this->_type.set(type::undefined);
  return;
}
void json::object::dump(std::string path, size_t indent) {
  std::ofstream jsonFile(path);
  if (!jsonFile.is_open()) {
    logger::error("Unable to open " + path,
                  "void json::object::dump(std::string path, size_t indent)");
  }

  std::string stringified = this->dumps(indent);
  jsonFile.write(stringified.c_str(), stringified.size());
  jsonFile.close();
  return;
}
std::string json::object::dumps(size_t indent, size_t baseIndent) {
  if (this->_type.get() == type::undefined) {
    return "undefined";
  } else if (this->_type.get() == type::null) {
    return "null";
  } else if (this->_type.get() == type::boolean) {
    bool val = std::get<bool>(this->_value);
    if (val)
      return "true";
    else
      return "false";
  } else if (this->_type.get() == type::integer) {
    return std::to_string(std::get<long long int>(this->_value));
  } else if (this->_type.get() == type::number) {
    return std::to_string(std::get<double>(this->_value));
  } else if (this->_type.get() == type::string) {
    return "\"" + std::get<std::string>(this->_value) + "\"";
  }

  size_t _size = this->size();
  std::string result;
  int newBaseIndent = indent + baseIndent;

  if (this->_type.get() == type::array) {
    result += "[";
    if (_size == 0) {
      result += "]";
      return result;
    }
    if (indent > 0)
      result += "\n";
    for (size_t i = 0; i < _size; i++) {
      result += std::string(newBaseIndent, ' ');
      result += this->_array[i].dumps(indent, newBaseIndent);
      if (i + 1 != _size)
        result += ",";
      if (indent > 0)
        result += "\n";
      else if (i + 1 != _size)
        result += " ";
    }
    result += std::string(baseIndent, ' ') + "]";
  } else {
    result += "{";
    if (_size == 0) {
      result += "}";
      return result;
    }
    if (indent > 0)
      result += "\n";
    for (auto it = this->_map.begin(); it != this->_map.end(); it++) {
      result += std::string(newBaseIndent, ' ');
      result += "\"" + it->first + "\": ";
      result += it->second.dumps(indent, newBaseIndent);
      if (std::next(it) != this->_map.end())
        result += ",";
      if (indent > 0)
        result += "\n";
      else if (std::next(it) != this->_map.end())
        result += " ";
    }
    result += std::string(baseIndent, ' ') + "}";
  }

  return result;
}

// Output functions -----------------------------------

void json::object::clearVariant() {
  this->_value = {};
  return;
}

void json::object::clearArray() {
  this->_array.clear();
  return;
}
void json::object::setArrayIfUndefined() {
  if (this->_type.get() == type::undefined) {
    this->clearVariant();
    this->clearMap();
    this->_type.set(type::array);
  }
  return;
}
void json::object::assertIsArray() {
  this->setArrayIfUndefined();
  if (this->_type.get() != type::array) {
    logger::error("This object is not an array",
                  "void json::object::assertIsArray()");
  }
}

void json::object::clearMap() {
  this->_map.clear();
  return;
}
void json::object::setMapIfUndefined() {
  if (this->_type.get() == type::undefined) {
    this->clearVariant();
    this->clearArray();
    this->_type.set(type::map);
  }
  return;
}
void json::object::assertIsMap() {
  this->setMapIfUndefined();
  if (!this->_type.get() == type::map) {
    logger::error("This object is not an object",
                  "void json::object::assertIsMap()");
  }
}

// Friend functions -----------------------------------

namespace json {

std::ostream &operator<<(std::ostream &os, object &obj) {
  os << obj.dumps(2);
  return os;
}

object::operator std::nullptr_t() const {
  if (this->_type.get() != type::null) {
    logger::error("This object is not a null",
                  "json::object::operator std::nullptr_t()");
  }

  return nullptr;
}
object::operator double() const {
  if (this->_type.get() != type::number && this->_type.get() != type::integer &&
      this->_type.get() != type::boolean) {
    logger::error("This object is not a number",
                  "json::object::operator double()");
  }

  if (this->_type.get() == type::integer)
    return static_cast<double>(std::get<long long int>(this->_value));
  else if (this->_type.get() == type::boolean)
    return static_cast<double>(std::get<bool>(this->_value));
  else
    return std::get<double>(this->_value);
}
object::operator std::string() const {
  if (this->_type.get() != type::string) {
    logger::error("This object is not a string",
                  "json::object::operator std::string()");
  }

  return std::get<std::string>(this->_value);
}

} // namespace json