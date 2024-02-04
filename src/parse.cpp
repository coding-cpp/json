#include <json/parse.h>

json::parser::parser() { return; }

json::parser::parser(std::string data) : input(data) { return; }

json::parser::~parser() { return; }

json::object json::parser::load(std::string data) {
  this->input = data;
  this->index = 0;
  this->output.reset();

  this->output = this->parseValue();
  return this->output;
}
json::object json::parser::loads(std::string path) {
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error("File not found");

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  return this->load(buffer.str());
}

json::object json::parser::parseValue() {
  this->skipWhitespace();
  char firstChar = this->getCurrChar();

  std::cout << firstChar << std::endl;
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

  // std::cout << this->getPrevChar() << this->getCurrChar() << std::endl;
  throw std::runtime_error("Invalid JSON (first character)");
}

json::object json::parser::parseBooleanNullOrUndefined() {
  std::string keyword = "";
  keyword += this->getCurrChar();
  char nextChar;

  while (isalpha(nextChar = this->getNextChar())) {
    keyword += nextChar;
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
  number += this->getCurrChar();
  char nextChar;

  bool isInt = true;
  while (std::isdigit(nextChar = getNextChar()) || nextChar == '.' ||
         nextChar == 'e' || nextChar == 'E' || nextChar == '+' ||
         nextChar == '-') {
    number += nextChar;
    if (nextChar == '.')
      isInt = false;
  }

  if (isInt)
    return object(std::stoll(number));
  return object(std::stod(number));
}
json::object json::parser::parseString() {
  std::string value = "";
  char nextChar = this->getNextChar();

  if (nextChar == '"') {
    this->getNextChar();
    return object(value);
  }

  while (true) {
    value += nextChar;
    nextChar = this->getNextChar();

    if (nextChar == '"') {
      if (this->getPrevChar() != '\\')
        break;
    }
  }

  this->getNextChar();
  return object(value);
}
json::object json::parser::parseArray() {
  std::vector<object> emptyVector;
  object result(emptyVector);
  this->getNextChar();
  this->skipWhitespace();
  char nextChar;

  while (true) {
    nextChar = this->getCurrChar();

    if (nextChar == ']')
      break;
    else if (nextChar == ',') {
      this->getNextChar();
      continue;
    }

    result.push_back(this->parseValue());
    this->skipWhitespace();
  }

  this->getNextChar();
  return result;
}
json::object json::parser::parseMap() {
  std::map<std::string, object> emptyMap;
  object result(emptyMap);
  char nextChar;

  while (true) {
    this->getNextChar();
    this->skipWhitespace();
    nextChar = this->getCurrChar();

    if (nextChar == '}')
      break;
    else if (nextChar == ',')
      continue;

    if (nextChar != '"')
      throw std::runtime_error("Invalid JSON (map key)");

    std::string key = "";
    nextChar = this->getNextChar();
    if (nextChar == '"')
      throw std::runtime_error("Invalid JSON (map key)");

    while (true) {
      key += nextChar;
      nextChar = this->getNextChar();

      if (nextChar == '"') {
        if (this->getPrevChar() != '\\')
          break;
      }
    }

    this->getNextChar();
    this->skipWhitespace();
    nextChar = this->getCurrChar();
    if (nextChar != ':')
      throw std::runtime_error("Invalid JSON (map key-value)");

    this->getNextChar();
    result[key] = this->parseValue();
  }

  this->getNextChar();
  return result;
}

void json::parser::skipWhitespace() {
  while (std::isspace(this->input[this->index])) {
    this->index++;
  }
  return;
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
char json::parser::getNextChar() {
  this->index++;
  return this->getCurrChar();
}