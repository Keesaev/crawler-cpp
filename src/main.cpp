#include "crawler.h"
#include "http_client.h"             // TEMP
#include "html_parser.h"                  // TEMP
#include <boost/asio/io_context.hpp> // TEMP

#include <iostream> // TEMP

int main(int argc, char *argv[]) {

  boost::asio::io_context io_context;
  http_client client(io_context);

  html_parser p("crawler-test.com"); // TEMP test link
  for (const auto &i : p.links(client.page("crawler-test.com/"))) {
    std::cout << i << std::endl;
  }

  // crawler c("crawler-test.com/", 2);
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
