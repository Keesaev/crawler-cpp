#pragma once
#include <string>
#include <vector>

// Straightforward html html_parser that finds <href="<link>"> pattern
// Returns full links concatinating with root link

class html_parser {
public:
  html_parser(std::string const &root);
  std::vector<std::string> links(std::string &&page);

private:
  std::string m_root;
};
