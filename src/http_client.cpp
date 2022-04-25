#include "http_client.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream> // TEMP

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;

http_client::http_client(boost::asio::io_context &io_context)
    : m_io_context(io_context) {}

std::string http_client::page(std::string const &link) {
  // Separate host and target
  std::string host, target;
  const auto separator = link.find('/');
  if (separator == std::string::npos) {
    host = link;
    target = "/";
  } else {
    host = link.substr(0, separator);
    target = link.substr(separator, link.length() - separator);
  }

  std::cout << "host " << host << " target " << target << std::endl;

  try {
    asio::ip::tcp::resolver resolver(m_io_context);
    beast::tcp_stream stream(m_io_context);
    stream.connect(resolver.resolve(host, "80"));

    http::request<http::string_body> req{http::verb::get, target, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(stream, req);

    beast::flat_buffer buffer;
    http::response<http::string_body> res;

    http::read(stream, buffer, res);

    // Gracefully close the socket
    beast::error_code ec;
    stream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, ec);

    return res.body();

  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return "";
}
