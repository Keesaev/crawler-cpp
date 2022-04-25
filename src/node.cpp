#include "node.h"

node::node(std::string const &link) : m_link(link) {}

void node::add_child(node *child) { m_children.push_back(child); }

std::list<node *> *const node::children() { return &m_children; }

bool node::has_children() const { return !m_children.empty(); }

std::string node::link() const { return m_link; }

node::~node() {
  for (auto &child : m_children)
    delete child;
}
