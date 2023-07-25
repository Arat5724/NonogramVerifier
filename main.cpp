#include "Nonogram.hpp"
#include "NonogramLoader.hpp"
#include "NonogramSolver.hpp"

int main(void) {
#ifdef DEBUG
  test1();
  return 0;
#endif
  string filename;
  cout << "Enter the filename: ";
  getline(cin, filename);
  Nonogram nonogram = NonogramLoader::load(filename);
  NonogramSolver::solve(nonogram);
  nonogram.print();
  system("pause");
  return 0;
}
