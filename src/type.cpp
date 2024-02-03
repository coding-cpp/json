#include <json/type.h>

json::type::type() : _type(0) { return; }

json::type::~type() { return; }

void json::type::set(int newType) {
  this->_type = newType;
  return;
}

int json::type::get() const { return this->_type; }
