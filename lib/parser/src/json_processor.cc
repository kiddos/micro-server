#include "json_processor.h"

namespace nerd {

JsonProcessor::JsonProcessor(List<Pair>& pairs) : pairs_(pairs) {}

String JsonProcessor::Output() {
  String output = "{\n";
  for (uint16_t i = 0; i < pairs_.size(); ++i) {
    output += "  \"" + pairs_[i].key + "\": ";
    if (IsNumber(pairs_[i].value)) {
      output += pairs_[i].value;
    } else {
      ProcessString(pairs_[i].value);
      output += "\"" + pairs_[i].value + "\"";
    }

    output += ",\n";
  }
  output += "}";
  return output;
}

bool JsonProcessor::IsNumber(const String& text) {
  for (uint16_t i = 0; i < text.length(); ++i) {
    if ((text[i] < '0' || text[i] > '9') && text[i] != '.' && text[i] != '-') {
      return false;
    }
  }
  return true;
}

void JsonProcessor::ProcessString(String& text) {
  for (uint16_t i = 0; i < text.length(); ++i) {
    if (text[i] == '+') {
      text[i] = ' ';
    }
  }
}

}  // namespace nerd
