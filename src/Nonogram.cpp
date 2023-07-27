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
    int is_horizontal = (i < height_) ? height_ : 0;
    i = (i < height_) ? i : (i - height_);
    Line& line = is_horizontal ? horizontal_lines_[i] : vertical_lines_[i];
    int start_index = line.start_index(), end_index = line.end_index();
    if (line.clue_size() == 0 || line.last_solving_ <= flag) continue;
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
    deque<int> next_quick_solving_order;
    deque<int> next_solving_order;
    vector<Line>& perpendicular_lines =
        (is_horizontal) ? vertical_lines_ : horizontal_lines_;
    for (int j = start_index; j < end_index; j++) {
      if (perpendicular_lines[j][i] != line[j]) {
        perpendicular_lines[j][i] = line[j];
        perpendicular_lines[j].last_solving_ = 2;
        next_quick_solving_order.push_back(j + is_horizontal);
        if (flag == NORMAL && is_in_order[j + is_horizontal])
          next_solving_order.push_back(j + is_horizontal);
      }
    }
    if (!next_quick_solving_order.empty()) {
      line.last_solving_ = flag;
      solve_(next_quick_solving_order, is_in_order, QUICK);
    }
    if (!next_solving_order.empty())
      solve_(next_solving_order, is_in_order, NORMAL);
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