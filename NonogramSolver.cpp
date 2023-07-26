#include "NonogramSolver.hpp"

static bool pair_compare(const pair<int, int>& a, const pair<int, int>& b) {
  return a.second < b.second;
}

deque<int> NonogramSolver::get_order_(Nonogram& nonogram) {
  vector<pair<int, int>> indices_ranks;
  int lines_size = nonogram.height() + nonogram.width();
  for (int i = 0; i < lines_size; i++) {
    indices_ranks.push_back({i, nonogram.get_rank(i)});
  }
  sort(indices_ranks.begin(), indices_ranks.end(), pair_compare);
  deque<int> order;
  for (pair<int, int> index_rank : indices_ranks) {
    order.push_back(index_rank.first);
  }
  return order;
}

void NonogramSolver::solve(Nonogram& nonogram) {
  pair<int, int> prev_cells_info;
  pair<int, int> current_cells_info = nonogram.cells_info();
  do {
    deque<int> order = get_order_(nonogram);
    nonogram.quick_solve(order);
    order = get_order_(nonogram);
    deque<int> solving_order;
    vector<bool> is_in_order(nonogram.height() + nonogram.width(), false);
    for (int pos : order) {
      solving_order.push_back(pos);
      is_in_order[pos] = true;
      nonogram.solve(solving_order, is_in_order);
    }
    prev_cells_info = current_cells_info;
    current_cells_info = nonogram.cells_info();
  } while (prev_cells_info != current_cells_info);
}