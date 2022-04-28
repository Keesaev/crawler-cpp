#include "crawler.h"
#include "http_client.h"

#include <boost/asio.hpp>

#include <iostream>

crawler::crawler(std::string const &root_path, int max_depth)
    : m_io_context(), m_root_node(std::make_unique<node>(root_path)),
      m_max_depth(max_depth) {}

void crawler::run() {
  m_visited_nodes.insert({m_root_node->link(), m_root_node.get()});

  std::make_shared<http_client>(m_io_context)
      ->get_page_async(m_root_node->link(),
                       std::bind(&crawler::on_page_received, this,
                                 m_root_node->link(), std::placeholders::_1,
                                 std::placeholders::_2));
  m_io_context.run();
}

// TODO add depth
// TODO pass node ptr and bind it
void crawler::on_page_received(std::string link, std::string page,
                               boost::beast::error_code ec) {
  if (ec) {
    // TODO add 'error' member to node class and dump ec
    return;
  }

  std::cout << link << std::endl;
  auto root_node = m_visited_nodes.find(link)->second;
  html_parser html_parser(root_node->link());

  for (auto link : html_parser.links(std::move(page))) {
    if (m_visited_nodes.find(link) == m_visited_nodes.end()) {
      node *n = new node(link);
      root_node->add_child(n);
      m_visited_nodes.insert({link, n});

      std::make_shared<http_client>(m_io_context)
          ->get_page_async(link, std::bind(&crawler::on_page_received, this,
                                           link, std::placeholders::_1,
                                           std::placeholders::_2));
    }
  }
}

void crawler::print_tree() const { _print_tree(m_root_node.get()); }

// DFS
void crawler::_print_tree(const node *const root_node, int depth) const {
  // "pretty" tab
  for (int i = 0; i < depth; i++)
    std::cout << '-';
  std::cout << root_node->link() << '\n';

  for (const auto child : *root_node->children()) {
    _print_tree(child, depth + 1);
  }
}
