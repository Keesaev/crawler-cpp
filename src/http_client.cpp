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

  try {
    asio::ip::tcp::resolver resolver(m_io_context);
    beast::tcp_stream stream(m_io_context);

    // Resolve http port
    stream.connect(resolver.resolve(host, "80"));

    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, "/", 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(stream, buffer, res);

    // Write the message to standard out
    std::cout << res << std::endl;

    // Gracefully close the socket
    beast::error_code ec;
    stream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes
    // so don't bother reporting it.
    //
    if (ec && ec != beast::errc::not_connected)
      std::cout << ec.message();
    // throw beast::system_error{ec};

  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return "";
}
