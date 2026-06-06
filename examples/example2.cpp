#include <fstream>

#include <print.hpp>

int main() {
  std::ofstream out{"log.txt"};
  print("hello", out);
  return 0;
}
