#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "types.h"
#include "list.h"

namespace nerd {

class HTTPParser {
 public:
  HTTPParser(const String& http_request);

  String Next(String& content, const String& token);
  String NextLine();
  void ParseHeader();
  void ParseBody();

  String& request() { return request_; }
  String& method() { return method_; }
  String& url() { return url_; }

  uint16_t pair_count() const { return pairs_.size(); }
  Pair& operator[](int i) { return pairs_[i]; }
  List<Pair>& pairs() { return pairs_; }

 private:
  String request_;
  String method_;
  String url_;

  List<Pair> pairs_;
};

}  // namespace nerd

#endif /* end of include guard */
