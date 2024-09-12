#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <vector>

namespace util {
  struct HexColor {
    uint8_t r, g, b;

    HexColor();
    HexColor(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    HexColor(const std::string& hex);

    std::vector<uint8_t> encode() const;
  };

  std::default_random_engine& getEngine();

  template <typename T>
  T rand(T min, T max) {
    std::default_random_engine& engine = getEngine();

    if constexpr (std::is_integral<T>::value) {
      std::uniform_int_distribution<T> dis(min, max);

      return dis(engine);
    } else if constexpr (std::is_floating_point<T>::value) {
      std::uniform_real_distribution<T> dis(min, max);

      return dis(engine);
    }
  }

  template <typename T>
  T rand(T max) {
    return rand<T>(0, max);
  }

  template <class T>
  class FreeList {
  public:
    FreeList();

    int insert(const T& element);
    int range() const;

    void erase(int n);
    void clear();

    T& operator[](int n);
    const T& operator[](int n) const;

  private:
    union FreeElement {
      T element;
      int next;
    };

    std::vector<FreeElement> data;

    int first_free;
  };

  template <class T>
  FreeList<T>::FreeList() : first_free(-1) {}

  template <class T>
  int FreeList<T>::insert(const T& element) {
    if (first_free != -1) {
      const int index = first_free;
      first_free = data[first_free].next;
      data[index].element = element;
      return index;
    } else {
      FreeElement fe;
      fe.element = element;
      data.push_back(fe);
      return data.size() - 1;
    }
  }

  template <class T>
  int FreeList<T>::range() const {
    return data.size();
  }

  template <class T>
  void FreeList<T>::erase(int n) {
    data[n].next = first_free;
    first_free = n;
  }

  template <class T>
  void FreeList<T>::clear() {
    data.clear();
    first_free = -1;
  }

  template <class T>
  T& FreeList<T>::operator[](int n) {
    return data[n].element;
  }

  template <class T>
  const T& FreeList<T>::operator[](int n) const {
    return data[n].element;
  }
} // namespace util
