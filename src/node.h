#pragma once
#include <string>
#include <vector>

class node {
public:
  node(std::string const &link);
  ~node();

  void add_child(node *child);
  const std::vector<node *> *const children() const;

  bool has_children() const;
  std::string link() const;

private:
  const std::string m_link;
  std::vector<node *> m_children;
};
