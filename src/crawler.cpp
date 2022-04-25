#include "crawler.h"
#include <boost/asio.hpp>

#include <iostream>

crawler::crawler(std::string const &root_path, int max_depth)
    : m_io_context(), m_root(root_path), m_max_depth(max_depth) {}

void crawler::run() { m_io_context.run(); }

void crawler::crawl(std::string root, int depth) {
  if (depth >= m_max_depth)
    return;

  http_client client(m_io_context);

  for (auto link : m_parser.links(client.page(root))) {
    std::cout << link << std::endl;
    boost::asio::post(m_io_context, std::bind(&crawler::crawl, this,
                                              std::move(link), depth + 1));
  }
}
