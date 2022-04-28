#include "http_client.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <boost/bind.hpp>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;

http_client::http_client(boost::asio::io_context &io_context)
    : m_io_context(io_context), m_resolver(io_context), m_stream(io_context) {}

void http_client::get_page_async(
    std::string const &link,
    std::function<void(std::string page, beast::error_code error)> callback) {

  _callback = callback;

  // TODO cut off https:// part

  std::string host;
  // Separate host and target
  const auto separator = link.find('/');
  if (separator == std::string::npos) {
    m_request.target("/");
    host = link;
  } else {
    m_request.target(link.substr(separator, link.length() - separator));
    host = link.substr(0, separator);
  }

  m_request.set(http::field::host, host);
  m_request.version(11);
  m_request.method(http::verb::get);
  m_request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  m_resolver.async_resolve(
      std::move(host), "80",
      boost::bind(&http_client::on_resolve, shared_from_this(),
                  boost::placeholders::_1, boost::placeholders::_2));
}

void http_client::on_resolve(beast::error_code ec,
                             asio::ip::tcp::resolver::results_type results) {
  if (ec)
    return _callback("", ec);

  m_stream.expires_after(std::chrono::seconds(5));
  m_stream.async_connect(
      results, boost::bind(&http_client::on_connect, shared_from_this(),
                           boost::placeholders::_1, boost::placeholders::_2));
}

void http_client::on_connect(
    beast::error_code ec,
    asio::ip::tcp::resolver::results_type::endpoint_type) {
  if (ec)
    return _callback("", ec);

  m_stream.expires_after(std::chrono::seconds(5));
  http::async_write(m_stream, m_request,
                    boost::bind(&http_client::on_write, shared_from_this(),
                                boost::placeholders::_1,
                                boost::placeholders::_2));
}

void http_client::on_write(beast::error_code ec,
                           std::size_t bytes_transferred) {
  if (ec)
    return _callback("", ec);

  http::async_read(m_stream, m_buffer, m_result,
                   boost::bind(&http_client::on_read, shared_from_this(),
                               boost::placeholders::_1,
                               boost::placeholders::_2));
}

void http_client::on_read(beast::error_code ec, std::size_t bytes_transferred) {
  if (ec)
    return _callback("", ec);

  m_stream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, ec);
  _callback(m_result.body(), boost::system::error_code());
}
