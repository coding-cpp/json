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
  bool shouldStringBeEscaped();
  char getPrevChar();
  char getCurrChar();

public:
  parser();
  parser(std::string data);
  ~parser();

  object load(std::string data);
  object loads(std::string path);
};

} // namespace json