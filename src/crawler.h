#pragma once

#include "html_parser.h"
#include "node.h"

#include <boost/asio/io_context.hpp>
#include <boost/beast/core/error.hpp>

#include <unordered_set>

class crawler {
public:
  crawler(std::string const &root, int max_depth);

  void run();
  void print_tree() const;

private:
  void on_page_received(node *root_node, int depth, std::string page,
                        boost::beast::error_code ec);
  void _print_tree(const node *const root_node, int depth = 1) const;

private:
  boost::asio::io_context m_io_context;
  // TODO change to unordered_set and add operator== to node class
  std::unordered_set<node *> m_visited_nodes;

  std::unique_ptr<node> m_root_node;
  const int m_max_depth;
};
