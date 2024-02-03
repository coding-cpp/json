#pragma once

#include <cctype>
#include <sstream>

#include <json/object.h>

namespace json {

class parser {
private:
  std::string input;
  object output;

  size_t index;

  object parseValue();
  object parseBooleanNullOrUndefined();
  object parseNumber();
  object parseString();
  object parseArray();
  object parseMap();

  void skipWhitespace();
  char getCurrChar();
  char getNextChar();

public:
  parser();
  parser(std::string data);
  ~parser();

  object parse(std::string data);
};

} // namespace json