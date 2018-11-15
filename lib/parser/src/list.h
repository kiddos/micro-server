#ifndef LIST_H
#define LIST_H

#include <Arduino.h>

namespace nerd {

template <typename T>
class List {
 public:
  List() : data_(nullptr), size_(0) {}

  List(const List<T>& l) : data_(new T[l.size()]), size_(l.size()) {
    for (uint16_t i = 0; i < size_; ++i) {
      data_[i] = l.data_[i];
    }
  }

  ~List() {
    if (data_) {
      delete[] data_;
    }
  }

  void Add(const T& entry) {
    if (!data_) {
      data_ = new T[1];
      data_[0] = entry;
      size_ = 1;
    } else {
      T* temp = data_;
      data_ = new T[size_ + 1];
      for (uint16_t i = 0; i < size_; ++i) {
        data_[i] = temp[i];
      }
      data_[size_] = entry;
      ++size_;

      delete[] temp;
    }
  }

  T& operator[](int i) { return data_[i]; }

  uint16_t size() const { return size_; }

 private:
  T* data_;
  uint16_t size_;
};

} // namespace nerd

#endif /* end of include guard */
