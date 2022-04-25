#pragma once
#include <string>
#include <vector>

class parser {
public:
  parser();
  std::vector<std::string> links(std::string &&page);
};
