#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <Arduino.h>

namespace nerd {

class FileManager {
 public:
  FileManager();

  uint16_t index() const { return index_; }
  void set_index(uint16_t index) { index_ = index; }
  void set_data_dir(const String& data_dir) { data_dir_ = data_dir; }
  void Next();
  String GetFileName();
  String NextFileName();

 private:
  String data_dir_;
  uint16_t index_;
};

} // namespace nerd

#endif /* end of include guard */
