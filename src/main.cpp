#include "crawler.h"
#include <iostream>

// TODO use boost::program_options

int main(int argc, char *argv[]) {
  std::string link;
  int depth = 1e9; // infinite depth

  if (argc < 2) {
    std::cout << "Usage: ./crawler-cpp <link>";
    link = "crawler-test.com";
  } else {
    link = argv[1];

    if (argc == 3)
      depth = std::atoi(argv[2]);
  }

  crawler c(link, depth);
  c.run();
  // c.print_tree();

  std::cout << std::flush;

  return 0;
}
