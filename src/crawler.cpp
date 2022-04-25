#include "crawler.h"
#include <boost/asio.hpp>

#include <iostream>

crawler::crawler(std::string const &root_path, int max_depth)
    : m_io_context(), m_root_node(new node(root_path)), m_max_depth(max_depth) {
}

void crawler::run() {
  crawl(m_root_node);
  /*
  boost::asio::post(m_io_context,
                    std::bind(&crawler::crawl, this, std::ref(m_root_node), 0));
  m_io_context.run();*/
}

void crawler::crawl(node *root_node, int depth) {
  m_visited_nodes.insert({root_node->link(), root_node});

  for (int i = 0; i < depth; i++)
    std::cout << "-";
  std::cout << root_node->link() << std::endl;

  if (depth >= m_max_depth)
    return;

  http_client client(m_io_context);
  html_parser html_parser(root_node->link());

  for (auto link : html_parser.links(client.page(root_node->link()))) {
    // already visited
    if (m_visited_nodes.find(link) == m_visited_nodes.end()) {
      node *n = new node(link);
      root_node->add_child(n);
      crawl(n, depth + 1);
    }
  }
}

crawler::~crawler() { delete m_root_node; }
