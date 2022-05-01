#include "crawler.h"
#include "http_client.h"

#include <boost/asio.hpp>

#include <algorithm>
#include <iostream>

crawler::crawler(std::string const &root_path, int max_depth)
    : m_io_context(), m_root_node(std::make_unique<node>(root_path)),
      m_max_depth(max_depth) {}

void crawler::run() {
  m_visited_nodes.insert(m_root_node.get());

  std::make_shared<http_client>(m_io_context)
      ->get_page_async(m_root_node->link(),
                       std::bind(&crawler::on_page_received, this,
                                 m_root_node.get(), 1, std::placeholders::_1,
                                 std::placeholders::_2));
  m_io_context.run();
}

void crawler::on_page_received(node *root_node, int depth, std::string page,
                               boost::beast::error_code ec) {
  if (ec) {
    std::cerr << depth << ": " << root_node->link() << ": " << ec.message()
              << '\n';
    root_node->error_code(ec);
    return;
  }

  std::cout << depth << ": " << root_node->link() << '\n';

  for (auto link : html_parser(root_node->link()).links(std::move(page))) {
    if (std::find_if(m_visited_nodes.begin(), m_visited_nodes.end(),
                     [&link](node *n) { return link == n->link(); }) ==
        m_visited_nodes.end()) {
      node *child_node = new node(link);
      root_node->add_child(child_node);
      m_visited_nodes.insert(child_node);

      // Parse next page if max depth not reached
      if (depth + 1 < m_max_depth)
        std::make_shared<http_client>(m_io_context)
            ->get_page_async(child_node->link(),
                             std::bind(&crawler::on_page_received, this,
                                       child_node, depth + 1,
                                       std::placeholders::_1,
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

  if (root_node->error_code())
    std::cout << depth << " " << root_node->link() << ": "
              << root_node->error_code() << '\n';
  else
    std::cout << depth << " " << root_node->link() << '\n';

  for (const auto child : *root_node->children()) {
    _print_tree(child, depth);
  }
}
