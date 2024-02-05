#include <json/parse.h>

json::parser::parser() { return; }

json::parser::parser(std::string data) : input(data) { return; }

json::parser::~parser() { return; }

json::object json::parser::load(std::string path) {
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error("File not found");

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  return this->loads(buffer.str());
}
json::object json::parser::loads(std::string data) {
  this->input = data;
  this->index = 0;
  this->output.reset();

  this->output = this->parseValue();
  return this->output;
}

json::object json::parser::parseValue() {
  this->skipWhitespace();
  char firstChar = this->getCurrChar();

  if (firstChar == 't' || firstChar == 'f' || firstChar == 'n' ||
      firstChar == 'u')
    return this->parseBooleanNullOrUndefined();
  else if (std::isdigit(firstChar))
    return this->parseNumber();
  else if (firstChar == '"')
    return this->parseString();
  else if (firstChar == '[')
    return this->parseArray();
  else if (firstChar == '{')
    return this->parseMap();

  throw std::runtime_error("Invalid JSON (first character)");
}

json::object json::parser::parseBooleanNullOrUndefined() {
  std::string keyword = "";
  char currChar = this->getCurrChar();

  while (isalpha(currChar)) {
    keyword += currChar;
    this->index++;
    currChar = this->getCurrChar();
  }

  if (keyword == "true")
    return object(true);
  else if (keyword == "false")
    return object(false);
  else if (keyword == "null")
    return object(nullptr);
  else if (keyword == "undefined")
    return object();

  throw std::runtime_error("Invalid JSON (bool, null, undefined)");
}
json::object json::parser::parseNumber() {
  std::string number = "";
  char currChar = this->getCurrChar();
  bool isInt = true;

  while (std::isdigit(currChar) || currChar == '.' || currChar == 'e' ||
         currChar == 'E' || currChar == '+' || currChar == '-') {
    number += currChar;
    if (currChar == '.') {
      isInt = false;
    }

    this->index++;
    currChar = this->getCurrChar();
  }

  if (isInt)
    return object(std::stoll(number));
  else
    return object(std::stod(number));
}
json::object json::parser::parseString() {
  std::string value = "";
  this->index++;
  char currChar = this->getCurrChar();

  if (currChar != '"') {
    while (true) {
      value += currChar;
      this->index++;
      currChar = this->getCurrChar();

      if (currChar == '"') {
        if (this->shouldStringBeEscaped()) {
          break;
        }
      }
    }
  }

  this->index++;
  return object(value);
}
json::object json::parser::parseArray() {
  std::vector<object> emptyVector;
  object result(emptyVector);
  this->index++;
  char currChar;

  while (true) {
    this->skipWhitespace();
    currChar = this->getCurrChar();

    if (currChar == ']') {
      this->index++;
      break;
    } else if (currChar == ',') {
      this->index++;
      continue;
    }

    result.push_back(this->parseValue());
  }

  return result;
}
json::object json::parser::parseMap() {
  std::map<std::string, object> emptyMap;
  object result(emptyMap);
  this->index++;
  char currChar;

  std::string key = "";
  while (true) {
    this->skipWhitespace();
    currChar = this->getCurrChar();

    if (currChar == '}') {
      this->index++;
      break;
    } else if (currChar == ',') {
      this->index++;
      continue;
    }

    if (currChar != '"')
      throw std::runtime_error("Invalid JSON (map key)");

    this->index++;
    currChar = this->getCurrChar();
    if (currChar == '"')
      throw std::runtime_error("Invalid JSON (map key)");

    key = "";
    while (true) {
      key += currChar;
      this->index++;
      currChar = this->getCurrChar();

      if (currChar == '"') {
        if (this->shouldStringBeEscaped()) {
          this->index++;
          break;
        }
      }
    }

    this->skipWhitespace();
    currChar = this->getCurrChar();
    if (currChar != ':')
      throw std::runtime_error("Invalid JSON (map-key value)");

    this->index++;
    result[key] = this->parseValue();
  }

  return result;
}

void json::parser::skipWhitespace() {
  while (std::isspace(this->input[this->index])) {
    this->index++;
  }
  return;
}
bool json::parser::shouldStringBeEscaped() {
  int backslashes = 0;
  for (int i = this->index - 1; i >= 0; i--) {
    if (this->input[i] == '\\') {
      backslashes++;
    } else {
      break;
    }
  }
  if (backslashes % 2 == 0) {
    return true;
  }
  return false;
}
char json::parser::getPrevChar() {
  if (this->index > 0) {
    return this->input[this->index - 1];
  }
  return '\0';
}
char json::parser::getCurrChar() {
  if (this->index < this->input.size()) {
    return this->input[this->index];
  }
  return '\0';
}