// ASAN env
#ifndef _WIN32
extern "C" const char *__asan_default_options() {
  return "verify_asan_link_order=0";
}
#endif

#include "crawler.h"
#include "http_client.h"             // TEMP
#include <boost/asio/io_context.hpp> // TEMP

int main(int argc, char *argv[]) {

  boost::asio::io_context io_context;
  http_client client(io_context);
  client.page("crawler-test.com/");

  // crawler c("en.wikipedia.org/wiki/Web_crawler", 2);
  // c.run();

  return 0;
}

/*

root_link = "google.com"

crawl(string link, int depth){
  for(const auto & link : get_all_links(link)){
    post(crawl(link, depth));
  }
}

*/
