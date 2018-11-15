#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include "types.h"
#include "list.h"

namespace nerd {

class JsonProcessor {
 public:
  JsonProcessor(List<Pair>& pairs);

  String Output();
  bool IsNumber(const String& text);
  void ProcessString(String& text);

 private:
  List<Pair>& pairs_;
};

} // namespace nerd

#endif /* end of include guard */
