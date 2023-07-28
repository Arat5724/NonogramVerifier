#ifndef NONOGRAM_SOLVER_HPP
#define NONOGRAM_SOLVER_HPP

#include <algorithm>

#include "Nonogram.hpp"

class NonogramSolver {
 public:
  static void solve(Nonogram& nonogram);

 private:
  static deque<int> get_order_(Nonogram& nonogram);
};

#endif  // NONOGRAM_SOLVER_HPP