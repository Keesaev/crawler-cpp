#pragma once

#include "html_parser.h"
#include "http_client.h"
#include "node.h"

#include <boost/asio/io_context.hpp>
#include <unordered_map>

class crawler {
public:
  crawler(std::string const &root, int max_depth);
  ~crawler();

  void run();

private:
  void crawl(node *root_node, int depth = 0);

private:
  boost::asio::io_context m_io_context;
  std::unordered_map<std::string, node *> m_visited_nodes;

  node *m_root_node;
  const int m_max_depth;
};
