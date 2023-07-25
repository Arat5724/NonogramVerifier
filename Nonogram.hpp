#ifndef NONOGRAM_HPP
#define NONOGRAM_HPP

#include "line.hpp"

#ifdef DEBUG
int test1();
#endif

#define H 0
#define V 1

#define NORMAL 0
#define QUICK 1

class Nonogram {
 public:
  Nonogram(int height, int width, vector<Line> horizontal_lines,
           vector<Line> vertical_lines);
  int get_rank(int pos) const;
  void quick_solve(const deque<int>& order);
  void solve(const deque<int>& order, const vector<bool>& is_in);
  int height() const { return height_; }
  int width() const { return width_; }
  void print() const;

 private:
  int height_;
  int width_;
  vector<Line> horizontal_lines_;
  vector<Line> vertical_lines_;
  Nonogram();
  void solve_(const deque<int>& order, const vector<bool>& is_in, int flag);
};

#endif  // NONOGRAM_HPP