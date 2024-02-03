#pragma once

namespace json {

class type {
private:
  int _type;

public:
  type();
  ~type();

  static const int undefined = 0;
  static const int null = 1;
  static const int boolean = 2;
  static const int integer = 3;
  static const int number = 4;
  static const int string = 5;
  static const int array = 6;
  static const int map = 7;

  void set(int newType);
  int get() const;
};

} // namespace json