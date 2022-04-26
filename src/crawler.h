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
  void print_tree() const;

private:
  void crawl(node *root_node, int depth = 0);
  void _print_tree(const node *const root_node, int depth = 0) const;

private:
  boost::asio::io_context m_io_context;
  std::unordered_map<std::string, node *> m_visited_nodes;

  node *m_root_node;
  const int m_max_depth;
};
