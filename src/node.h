#pragma once
#include <string>
#include <vector>

#include <boost/beast/core/error.hpp>

class node {
public:
  node(std::string const &link);
  ~node();

  void add_child(node *child);
  const std::vector<node *> *const children() const;

  bool has_children() const;
  std::string link() const;
  boost::beast::error_code error_code() const;

  void error_code(boost::beast::error_code const &);

private:
  const std::string m_link;
  std::vector<node *> m_children;
  boost::beast::error_code m_error_code;
};
