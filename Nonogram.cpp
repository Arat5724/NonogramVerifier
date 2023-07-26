#include "Nonogram.hpp"

Nonogram::Nonogram(int height, int width, vector<Line> horizontal_lines,
                   vector<Line> vertical_lines, string chars)
    : height_(height),
      width_(width),
      horizontal_lines_(horizontal_lines),
      vertical_lines_(vertical_lines),
      chars_(chars) {}

pair<int, int> Nonogram::cells_info() const {
  int filled = 0, non_filled = 0;
  for (Line line : horizontal_lines_) {
    for (int i = 0; i < width_; i++) {
      if (line[i] == O)
        filled++;
      else if (line[i] == X)
        non_filled++;
    }
  }
  return {filled, non_filled};
}

int Nonogram::get_rank(int pos) const {
  if (pos < height_) return horizontal_lines_[pos].get_rank();
  return vertical_lines_[pos - height_].get_rank();
}

void Nonogram::quick_solve(const deque<int>& order) {
  solve_(order, vector<bool>(), QUICK);
}

void Nonogram::solve(const deque<int>& order, const vector<bool>& is_in_order) {
  solve_(order, is_in_order, NORMAL);
}

void Nonogram::solve_(const deque<int>& order, const vector<bool>& is_in_order,
                      int flag) {
  for (int i : order) {
    deque<int> next_quick_solving_order;
    deque<int> next_solving_order;
    int direction = (i < height_) ? H : V;
    i = (i < height_) ? i : (i - height_);
    Line& line = direction == H ? horizontal_lines_[i] : vertical_lines_[i];
#ifdef DEBUG
    cout << "direction: " << direction << endl;
    cout << "i: " << i << endl;
    cout << "line: ";
    line.print();
#endif
    int status = (flag == QUICK) ? line.quick_solve() : line.solve();
#ifdef DEBUG
    cout << "line: ";
    line.print();
#endif
    if (status == ERROR) throw exception();
    if (direction == H) {
      for (int j = 0; j < width_; j++) {
        if (vertical_lines_[j][i] != line[j]) {
          vertical_lines_[j][i] = line[j];
          next_quick_solving_order.push_back(j + height_);
          if (flag == NORMAL && is_in_order[j + height_])
            next_solving_order.push_back(j + height_);
        }
      }
    } else {
      for (int j = 0; j < height_; j++) {
        if (horizontal_lines_[j][i] != line[j]) {
          horizontal_lines_[j][i] = line[j];
          next_quick_solving_order.push_back(j);
          if (flag == NORMAL && is_in_order[j]) next_solving_order.push_back(j);
        }
      }
    }
    solve_(next_quick_solving_order, is_in_order, QUICK);
    if (flag == NORMAL) solve_(next_solving_order, is_in_order, NORMAL);
  }
}

void Nonogram::print() const {
  char filled = chars_[0];
  char non_filled = chars_[1];
  char empty = chars_[2];
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      cout << (horizontal_lines_[i][j] == O   ? filled
               : horizontal_lines_[i][j] == X ? non_filled
                                              : empty);
    }
    cout << endl;
  }
}