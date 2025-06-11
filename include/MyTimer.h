#ifndef MY_TIMER_H
#define MY_TIMER_H

#include <chrono>
#include <cstddef>

// 主要是计算各个部分的时间
class MyTimer {
public:
  static auto Now() { return std::chrono::high_resolution_clock::now(); }

  template <typename T>
  static auto
  Duration(const std::chrono::high_resolution_clock::time_point &start,
           const std::chrono::high_resolution_clock::time_point &end) {
    return std::chrono::duration_cast<T>(end - start);
  }

  // search 就是 knn search 时间，包含 approx search 和 exact search
  // 单位 us
  static size_t search_timecount_us_;
  static size_t exact_search_timecount_us_;
};

// 记录一些次数
class MyCnt {
public:
  // exact search 的 ts 数量
  static size_t exact_search_item_num;
};

#endif
