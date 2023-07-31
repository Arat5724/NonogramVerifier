#ifndef NONOGRAM_HPP
#define NONOGRAM_HPP

#include <queue>
#include <stack>

#include "Line.hpp"
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
           vector<Line> vertical_lines, string chars);
  long long get_rank(int pos);
  void solve();
  int height() const { return height_; }
  int width() const { return width_; }
  void print() const;

#ifdef DEBUG
 public:
#else
 private:
#endif
  Nonogram();
  int height_;
  int width_;
  vector<Line> horizontal_lines_;
  vector<Line> vertical_lines_;
  string chars_;
};

#endif  // NONOGRAM_HPP