#ifndef CLUE_HPP
#define CLUE_HPP

#include <deque>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define X -1
#define B 0
#define O 1

#define SUCCESS 0
#define ERROR 1
#define END 2  // Don't have to continue

#define FRONT 3
#define BACK 2

#define INIT 2

class Line {
 public:
  Line();
  Line(const string& clue_string);
  Line(const Line& other);
  Line& operator=(const Line& other);
  int& operator[](int index) { return this->cells_[index]; }
  const int& operator[](int index) const { return this->cells_[index]; }
  ~Line();

  int last_solving_;
  int clue_size() const { return this->clue_size_; }
  int start_index() const { return this->start_index_; }
  int end_index() const { return this->end_index_; }

  int get_rank() const;

  int quick_solve();
  int solve();

#ifdef DEBUG
  void print() const;
#endif
 private:
  deque<int> clues_;
  int clue_size_;
  int sum_clues_;

  int capacity_;

  vector<int> cells_;
  int start_index_;
  int end_index_;

  void update_capacity_();

  void pop_clue_front_(int new_start_index);
  void pop_clue_back_(int new_end_index);

  int solve_(int flag);
  int get_start_point_(int flag);

  int cells_empty_();

  int mark_();
  int mark_clue_front_(int pos);
  int mark_clue_back_(int pos);

  int update_clues_();

  int update_start_index_();
  int update_end_index_();

  void intersect_(const Line& other);
};

#endif  // CLUE_HPP