#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http.hpp>

class http_client : public std::enable_shared_from_this<http_client> {
public:
  http_client(boost::asio::io_context &io_context);
  void get_page_async(
      std::string const &link,
      std::function<void(std::string page, boost::beast::error_code ec)>
          callback);

private:
  void on_resolve(boost::beast::error_code ec,
                  boost::asio::ip::tcp::resolver::results_type results);
  void on_connect(boost::beast::error_code ec,
                  boost::asio::ip::tcp::resolver::results_type::endpoint_type);
  void on_write(boost::beast::error_code ec, std::size_t bytes_transferred);
  void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);

private:
  // TODO store request as const field instead of construcing every time
  boost::asio::io_context &m_io_context;

  boost::asio::ip::tcp::resolver m_resolver;
  boost::beast::tcp_stream m_stream;
  boost::beast::flat_buffer m_buffer;
  boost::beast::http::request<boost::beast::http::string_body> m_request;
  boost::beast::http::response<boost::beast::http::string_body> m_result;

  std::function<void(std::string page, boost::beast::error_code ec)> _callback;
};
