#include <iostream>

#include <json/object.h>

int main(int argc, char **argv) {
  json::object person;
  person.insert("name", "Adit");
  person.insert("age", 20);

  json::object languages;
  languages.push_back("c++");
  languages.push_back("python");
  languages.push_back("javascript");
  person.insert("languages", languages);

  json::object socials;
  socials.insert("github", "https://github.com/Jadit19");
  socials.insert("instagram", "https://instagram.com/jadit_19");
  person.insert("socials", socials);

  person.dumps("../person.json", 2);

  return 0;
}