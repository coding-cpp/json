// Includes required
#include <json/object.h>
#include <json/parse.h>

// Using namespace
using namespace json;

int main(int argc, char **argv) {
  object person;
  person["age"] = 21;
  person.insert("name", "Adit");
  person["height"] = 186.5;
  person["alive"] = true;

  object languages;
  languages.push_back("German");
  languages.push_back("English");
  languages[0] = "Hindi";

  person["languages"] = languages;
  person["languages"].push_back("French");
  logger::info(person.dumps(2));

  std::string name = person["name"];
  logger::info(name);

  int age = person["age"];
  logger::info(std::to_string(age));

  bool alive = person["alive"];
  logger::info(alive ? "Alive" : "Dead");

  parser jsonParser;
  object obj = jsonParser.loads(person.dumps(2));
  logger::success(obj.dumps(2));

  return EXIT_SUCCESS;
}