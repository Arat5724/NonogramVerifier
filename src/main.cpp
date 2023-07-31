#include <sys/time.h>

#include "Nonogram.hpp"
#include "NonogramLoader.hpp"

int main(int argc, char* argv[]) {
  string filename;
  if (argc == 1) {
    cout << "Enter the filename: ";
    getline(cin, filename);
  } else if (argc == 2) {
    filename = argv[1];
  } else {
    cout << "Enter only one filename" << endl;
    return 0;
  }
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
