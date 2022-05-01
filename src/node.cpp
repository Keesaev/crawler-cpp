#include "node.h"

node::node(std::string const &link) : m_link(link) {}

void node::add_child(node *child) { m_children.push_back(child); }

const std::vector<node *> *const node::children() const { return &m_children; }

bool node::has_children() const { return !m_children.empty(); }

std::string node::link() const { return m_link; }

boost::beast::error_code node::error_code() const { return m_error_code; }

void node::error_code(boost::beast::error_code const &code) {
  m_error_code = code;
}

node::~node() {
  for (auto &child : m_children)
    delete child;
}
