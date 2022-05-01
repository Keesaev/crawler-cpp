#include "crawler.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::string link;
  if (argc < 2) {
    std::cout << "Usage: ./crawler-cpp <link>";
    link = "crawler-test.com";
  } else {
    link = argv[1];

    // temp solution:
    auto http_prefix = link.find("http://");
    if (http_prefix != std::string::npos) {
      link = link.substr(7, link.length() - 7);
    }
    auto https_prefix = link.find("https://");
    if (https_prefix != std::string::npos) {
      link = link.substr(8, link.length() - 8);
    }
  }

  crawler c(link, 3);
  c.run();
  // c.print_tree();

  std::cout << std::flush;

  return 0;
}
