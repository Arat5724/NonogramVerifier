#ifndef NONOGRAM_LOADER_HPP
#define NONOGRAM_LOADER_HPP

#include <fstream>

#include "nonogram.hpp"

class NonogramLoader {
 public:
  static Nonogram load(const string& filename) {
    ifstream fin;
    fin.open(filename);
    if (!fin.is_open()) {
      cout << "File not found." << endl;
      exit(1);
    }
    string url, s_width, s_height, raw_string;
    fin >> url >> s_width >> s_height >> raw_string;
    fin.close();
    if (url != "www.logichome.org") {
      cout << "Invalid format." << endl;
      exit(1);
    }
    int height, width;
    height = stoi(s_height);
    width = stoi(s_width);
    if (height < 1 || width < 1 && height * width != raw_string.length()) {
      cout << "Invalid dimensions." << endl;
      exit(1);
    }
    vector<Line> horizontal_lines;
    vector<Line> vertical_lines;

    for (int i = 0; i < height; i++)
      horizontal_lines.push_back(Line(raw_string.substr(i * width, width)));
    for (int i = 0; i < width; i++) {
      string vertical_string;
      for (int j = 0; j < height; j++)
        vertical_string.push_back(raw_string[j * width + i]);
      vertical_lines.push_back(Line(vertical_string));
    }

    string chars;
    fin.open("setting");
    if (fin.is_open()) {
      getline(fin, chars);
      fin.close();
      chars = chars.substr(0, 3);
    } else {
      cout << "setting file doesn't exist." << endl;
      chars = "O.X";
    }
    return Nonogram(height, width, horizontal_lines, vertical_lines, chars);
  }
};

#endif  // NONOGRAM_LOADER_HPP