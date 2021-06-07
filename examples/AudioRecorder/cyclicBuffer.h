#pragma once

template<typename T>
struct cyclicBuffer
{
  T *buf;
  unsigned int bufSize;
  unsigned int count;
  unsigned int currentPos;

  cyclicBuffer(const unsigned int L) : bufSize(L), count(0), currentPos(bufSize - 1) {
    buf = (T*)malloc(L * sizeof(T));
  }

  bool is_empty() const {
    return (count == 0);
  }

  bool is_full() const {
    return (count == bufSize);
  }

  unsigned int len() const {
    return count;
  }

  // Peek the n-th antiquated data pointer, 0 <= n < bufSize
  // 0-th is the oldest
  T* nth(const unsigned int n) const {
    if (n > count) {
      return NULL;
    }
    unsigned int tmp = (currentPos - count + n + 1 + bufSize) % bufSize;
    return &buf[tmp];
  }

  void reset() {
    count = 0;
    currentPos = bufSize - 1;
  }

  // Reset and resize the buffer
  void resize(const unsigned int newSize) {
    free(buf);
    buf = (T*)malloc(newSize * sizeof(T));
    reset();
  }

  void push(const T newData) {
    currentPos = (currentPos + 1) % bufSize;
    memcpy(&buf[currentPos], &newData, sizeof(newData));
    count = min(count + 1, bufSize);
  }

  // Pop out the latest data from the buffer
  // Not changing the actual data, just move the position and count marker 1 step back
  T* pop() {
    if (!is_empty()) {
      unsigned int tmp = currentPos;
      count -= 1;
      currentPos = (currentPos + bufSize - 1) % bufSize;
      return &buf[tmp];
    }
    return NULL;
  }
};
