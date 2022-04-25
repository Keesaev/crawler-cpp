#include "crawler.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::string link;
  if (argc < 2) {
    std::cout << "Usage: ./crawler-cpp <link>";
    link = "crawler-test.com";
  } else {
    link = argv[1];
  }

  crawler c("crawler-test.com/", 2);
  c.run();

  return 0;
}
