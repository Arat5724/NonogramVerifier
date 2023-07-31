#include "Nonogram.hpp"

#include <sys/time.h>

Nonogram::Nonogram(int height, int width, vector<Line> horizontal_lines,
                   vector<Line> vertical_lines, string chars)
    : height_(height),
      width_(width),
      horizontal_lines_(horizontal_lines),
      vertical_lines_(vertical_lines),
      chars_(chars) {}

long long Nonogram::get_rank(int pos) {
  if (pos < height_) return horizontal_lines_[pos].get_rank();
  return vertical_lines_[pos - height_].get_rank();
}

void Nonogram::solve() {
  priority_queue<pair<long long, int>> pq;
  vector<stack<long long>> is_in_pq_stack(this->height() + this->width());
  for (int i = 0; i < this->height() + this->width(); i++) {
    long long rank = this->get_rank(i);
    pq.push({-rank, i});
    is_in_pq_stack[i].push(rank);
  }
  long long n = 0;
  while (!pq.empty()) {
    n++;
    pair<long long, int> top = pq.top();
    pq.pop();
    long long rank = top.first;
    int i = top.second;
    is_in_pq_stack[i].pop();
    // cout << "n: " << n << ", i: " << i << ", rank: " << -rank << endl;
    int is_horizontal = (i < height_) ? height_ : 0;
    i = (i < height_) ? i : (i - height_);
    Line& line = is_horizontal ? horizontal_lines_[i] : vertical_lines_[i];
    // line.print();
    int start_index = line.start_index(), end_index = line.end_index();
    if (line.clue_size() == 0 && line.start_index() == line.end_index())
      continue;
    if (line.solve() == ERROR) {
      line.print();
      cout << "ERROR" << endl;
      throw exception();
    }
    // line.print();
    vector<Line>& perpendicular_lines =
        (is_horizontal) ? vertical_lines_ : horizontal_lines_;
    for (int j = start_index; j < end_index; j++) {
      if (perpendicular_lines[j][i] != line[j]) {
        perpendicular_lines[j][i] = line[j];
        long long prank = this->get_rank(j + is_horizontal);
        if (is_in_pq_stack[j + is_horizontal].empty() ||
            is_in_pq_stack[j + is_horizontal].top() > prank) {
          pq.push({-prank, j + is_horizontal});
          is_in_pq_stack[j + is_horizontal].push(prank);
        }
      }
    }
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