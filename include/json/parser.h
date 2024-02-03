#pragma once

#include <cctype>
#include <sstream>

#include <json/object.h>

namespace json {

class parser {
public:
  static object parse(const std::string &json) {
    parser p(json);
    return p.parseValue();
  }

private:
  explicit parser(const std::string &json) : input(json), index(0) {}

  char getNextChar() {
    if (index < input.length()) {
      return input[index++];
    }
    return '\0'; // End of string
  }

  void skipWhitespace() {
    while (std::isspace(input[index])) {
      index++;
    }
  }

  object parseValue() {
    skipWhitespace();
    char firstChar = getNextChar();

    if (firstChar == '{') {
      return parseObject();
    } else if (firstChar == '[') {
      return parseArray();
    } else if (firstChar == '"') {
      return parseString();
    } else if (firstChar == 't' || firstChar == 'f' || firstChar == 'n') {
      return parseBooleanOrNull();
    } else if (std::isdigit(firstChar) || firstChar == '-') {
      return parseNumber(firstChar);
    }

    // Handle other cases or throw an error for invalid JSON
    throw std::runtime_error("Invalid JSON");
  }

  object parseObject() {
    object result;
    while (true) {
      skipWhitespace();
      char nextChar = getNextChar();
      if (nextChar == '}') {
        break; // End of object
      } else if (nextChar == '"') {
        std::string key = parseString().as<std::string>();
        skipWhitespace();
        if (getNextChar() == ':') {
          result[key] = parseValue();
          skipWhitespace();
          if (getNextChar() == ',') {
            continue; // More key-value pairs
          } else if (getNextChar() == '}') {
            break; // End of object
          } else {
            throw std::runtime_error("Invalid JSON");
          }
        } else {
          throw std::runtime_error("Invalid JSON");
        }
      } else {
        throw std::runtime_error("Invalid JSON");
      }
    }
    return result;
  }

  object parseArray() {
    object result(std::vector<object>());
    while (true) {
      skipWhitespace();
      char nextChar = getNextChar();
      if (nextChar == ']') {
        break; // End of array
      } else {
        result.push_back(parseValue());
        skipWhitespace();
        if (getNextChar() == ',') {
          continue; // More elements in the array
        } else if (getNextChar() == ']') {
          break; // End of array
        } else {
          throw std::runtime_error("Invalid JSON");
        }
      }
    }
    return result;
  }

  object parseString() {
    std::string value;
    char currentChar;
    while ((currentChar = getNextChar()) != '"') {
      value += currentChar;
    }
    return object(value);
  }

  object parseBooleanOrNull() {
    std::string keyword;
    char currentChar;
    while (std::isalpha(currentChar = getNextChar())) {
      keyword += currentChar;
    }

    if (keyword == "true") {
      return object(true);
    } else if (keyword == "false") {
      return object(false);
    } else if (keyword == "null") {
      return object(nullptr);
    } else {
      throw std::runtime_error("Invalid JSON");
    }
  }

  object parseNumber(char firstChar) {
    std::string number(1, firstChar);
    char currentChar;
    while (std::isdigit(currentChar = getNextChar()) || currentChar == '.' ||
           currentChar == 'e' || currentChar == 'E' || currentChar == '+' ||
           currentChar == '-') {
      number += currentChar;
    }
    return object(std::stod(number));
  }

  std::string input;
  size_t index;
};

} // namespace json