#pragma once
#include <list>
#include <string>

class node {
public:
  node(std::string const &link);
  ~node();

  void add_child(node *child);
  std::list<node *> *const children();

  bool has_children() const;
  std::string link() const;

private:
  const std::string m_link;
  std::list<node *> m_children;
};
