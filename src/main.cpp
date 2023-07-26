#include <sys/time.h>

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
  struct timeval start, end;
  gettimeofday(&start, NULL);
  NonogramSolver::solve(nonogram);
  gettimeofday(&end, NULL);
  nonogram.print();
  cout << "Time elapsed: "
       << (end.tv_sec - start.tv_sec) * 1000 +
              (end.tv_usec - start.tv_usec) / 1000
       << "ms" << endl;
  system("pause");
  return 0;
}
