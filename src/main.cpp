#include <sys/time.h>

#include "Nonogram.hpp"
#include "NonogramLoader.hpp"

int main(void) {
  cout << "Enter the filename: ";
  string filename;
  getline(cin, filename);
  Nonogram nonogram = NonogramLoader::load(filename);
  struct timeval start, end;
  gettimeofday(&start, NULL);
  nonogram.solve();
  gettimeofday(&end, NULL);
  nonogram.print();
  cout << "Time elapsed: "
       << (end.tv_sec - start.tv_sec) * 1000 +
              (end.tv_usec - start.tv_usec) / 1000
       << "ms" << endl;
  system("pause");
  return 0;
}
