#include <iostream>

#include <json/object.h>

int main(int argc, char **argv) {
  json::object person;
  person.insert("name", "Adit");
  person.insert("age", 20);

  json::object launguages;
  launguages.push_back("English");
  launguages.push_back("Hindi");
  launguages.push_back("French");
  person.insert("launguages", launguages);

  return 0;
}