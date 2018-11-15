#include "http_parser.h"

namespace nerd {

HTTPParser::HTTPParser(const String& http_request) : request_(http_request) {}

String HTTPParser::Next(String& content, const String& token) {
  uint16_t loc = content.indexOf(token);
  if (loc >= 0) {
    String response = content.substring(0, loc);
    content = content.substring(loc + token.length());
    return response;
  }
  return "";
}

String HTTPParser::NextLine() {
  return Next(request_, "\r\n");
}

void HTTPParser::ParseHeader() {
  String first_line = NextLine();
  method_ = Next(first_line, " ");
  url_ = Next(first_line, " ");

  // read rest of the header
  while (true) {
    String line = NextLine();
    if (line.length() == 0) {
      break;
    }
  }
}

void HTTPParser::ParseBody() {
  String rest = request_;
  while (true) {
    bool stop = false;

    String entry = Next(rest, "&");
    if (entry.length() == 0) {
      entry = rest;
      stop = true;
    }

    String key = Next(entry, "=");
    String value = entry;
    pairs_.Add(Pair{key, value});

    if (stop || rest.length() == 0) break;
  }
}


}  // namespace nerd
