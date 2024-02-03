#include <json/parse.h>

json::parser::parser() { return; }

json::parser::parser(std::string data) : input(data) { return; }

json::parser::~parser() { return; }

json::object json::parser::parse(std::string data) {
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

  throw std::runtime_error("Invalid JSON");
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

  throw std::runtime_error("Invalid JSON");
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
    return object(std::stoi(number));
  return object(std::stod(number));
}
json::object json::parser::parseString() {
  std::string value = "";
  value += this->getNextChar();
  char nextChar;

  while ((nextChar = this->getNextChar()) != '"')
    value += nextChar;
  this->getNextChar();

  return object(value);
}
json::object json::parser::parseArray() {
  std::vector<object> emptyVector;
  object result(emptyVector);

  while (true) {
    this->getNextChar();
    this->skipWhitespace();
    if (this->getCurrChar() == ']')
      break;

    result.push_back(this->parseValue());
    this->skipWhitespace();

    if (this->getCurrChar() == ',') {
      continue;
    } else if (this->getCurrChar() == ']')
      break;
    else
      throw std::runtime_error("Invalid JSON");
  }

  this->getNextChar();
  return result;
}
json::object json::parser::parseMap() {
  std::map<std::string, object> emptyMap;
  object result(emptyMap);

  while (true) {
    this->getNextChar();
    this->skipWhitespace();
    if (this->getCurrChar() == '}')
      break;

    if (this->getCurrChar() == '"') {
      std::string key = "";
      char nextChar;
      while ((nextChar = this->getNextChar()) != '"')
        key += nextChar;

      this->getNextChar();
      this->skipWhitespace();
      if (this->getCurrChar() == ':') {
        this->getNextChar();
        result[key] = this->parseValue();

        this->skipWhitespace();
        nextChar = this->getCurrChar();
        if (nextChar == ',')
          continue;
        else if (nextChar == '}')
          break;
        else
          throw std::runtime_error("Invalid JSON");
      } else {
        throw std::runtime_error("Invalid JSON");
      }
    } else {
      throw std::runtime_error("Invalid JSON");
    }
  }

  return result;
}

void json::parser::skipWhitespace() {
  while (std::isspace(this->input[this->index])) {
    this->index++;
  }
  return;
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