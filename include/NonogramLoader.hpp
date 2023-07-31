#ifndef NONOGRAM_LOADER_HPP
#define NONOGRAM_LOADER_HPP

#include <fstream>

#include "Nonogram.hpp"

class NonogramLoader {
 public:
  static Nonogram load(const string& filename) {
    ifstream fin;
    fin.open(filename);
    if (!fin.is_open()) print_error("File not found.");

    string url, s_width, s_height, raw_string;
    fin >> url >> s_width >> s_height >> raw_string;
    fin.close();
    if (url != "www.logichome.org") print_error("Invalid url.");

    int height, width;
    height = stoi(s_height);
    width = stoi(s_width);
    if (height < 1 || width < 1 && height * width != raw_string.length())
      print_error("Invalid dimensions.");
    if (height > 300 || width > 300) print_error("Too large.");

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
      chars = "@ O";
    }
    return Nonogram(height, width, horizontal_lines, vertical_lines, chars);
  }

  static void print_error(const string& message) {
    cout << message << endl;
    system("pause");
    exit(1);
  }
};

#endif  // NONOGRAM_LOADER_HPP