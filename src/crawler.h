#pragma once
#include "http_client.h"
#include "html_parser.h"

#include <boost/asio/io_context.hpp>

class crawler {
public:
  crawler(std::string const &root, int max_depth);
  void run();

private:
  void crawl(std::string root, int depth);

private:
  boost::asio::io_context m_io_context;

  const std::string m_root;
  const int m_max_depth;
};
