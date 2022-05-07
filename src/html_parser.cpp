#include "html_parser.h"

html_parser::html_parser(std::string const &root) : m_root(root) {
  if (m_root.back() == '/')
    m_root.pop_back();
}

// parse all 'href' links
std::vector<std::string> html_parser::links(std::string &&page) {
  std::vector<std::string> links;
  auto opening_tag = page.find("href=\"", 0);

  while (opening_tag != std::string::npos) {
    opening_tag += 6; // += sizeos(href=")
    auto closing_tag = page.find("\"", opening_tag);
    if (closing_tag != std::string::npos) {
      auto link = page.substr(opening_tag, closing_tag - opening_tag);

      // Cut off http(s):// part
      if (link.find("https://") != std::string::npos) {
        link = link.substr(8, link.length() - 8);
      } else if (link.find("http://") != std::string::npos) {
        link = link.substr(7, link.length() - 7);
      }

      links.emplace_back(link.front() == '/' ? m_root + link : std::move(link));
    } else {
      break;
    }

    opening_tag = page.find("href=\"", closing_tag);
  }
  return links;
}
