#pragma once
#include <boost/asio/io_context.hpp>

class http_client {
public:
  http_client(boost::asio::io_context &io_context);
  std::string page(std::string const &link);

private:
  boost::asio::io_context &m_io_context;
};
