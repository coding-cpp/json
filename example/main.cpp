#include <json/object.h>

int main(int argc, char **argv) {
  json::object obj;

  obj.push("Hello");
  obj.push("World");
  std::cout << obj.dump(2) << std::endl;
  std::cout << "---------------------------" << std::endl;

  obj.clear();
  obj.insert("Name", "Adit");
  obj.insert("Gender", "Male");
  obj.insert("Age", 20);

  json::object languages;
  languages.push("English");
  languages.push("Hindi");
  languages.push("French");
  languages.push("Punjabi");

  obj.insert("Languages", languages);
  std::cout << obj.dump(4) << std::endl;

  return 0;
}