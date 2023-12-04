#include <json/print.h>

void json::print::success(const std::string &message, bool newLine) {
  std::cout << GREEN << "[SUCCESS]: " << RESET << message;
  if (newLine)
    std::cout << std::endl;

  return;
}

void json::print::info(const std::string &message, bool newLine) {
  std::cout << BLUE << "[INFO]   : " << RESET << message;
  if (newLine)
    std::cout << std::endl;

  return;
}

void json::print::warning(const std::string &message, bool newLine) {
  std::cout << YELLOW << "[WARNING]: " << RESET << message;
  if (newLine)
    std::cout << std::endl;

  return;
}

void json::print::error(const std::string &message,
                        const std::string &runtimeErrorMessage, bool newLine) {
  std::cerr << RED << "[ERROR]  : " << RESET << message;
  if (newLine)
    std::cerr << std::endl;
  if (runtimeErrorMessage != "")
    throw std::runtime_error(runtimeErrorMessage);

  return;
}