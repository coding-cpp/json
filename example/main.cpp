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

  object languages;
  languages.push_back("German");
  languages.push_back("English");
  languages[0] = "Hindi";

  person["languages"] = languages;
  person["languages"].push_back("French");

  std::cout << person << std::endl << std::endl;

  parser jsonParser;
  object obj = jsonParser.loads(person.dumps(2));
  logger::success(obj.dumps(2));

  return EXIT_SUCCESS;
}