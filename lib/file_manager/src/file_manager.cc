#include "file_manager.h"

namespace nerd {

FileManager::FileManager() : index_(0) {}

void FileManager::Next() {
  ++index_;
}

String FileManager::GetFileName() {
  String filename = data_dir_ + "/data_" + String(index_) + ".txt";
  return filename;
}

String FileManager::NextFileName() {
  String filename = GetFileName();
  Next();
  return filename;
}

}  // namespace nerd
