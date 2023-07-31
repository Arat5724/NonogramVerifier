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

#define QUICK_MAX 4

#define INIT 2

class Line {
 public:
  static vector<vector<long long>> cache_;
  long long get_cache_(int n, int r);
  long long get_rank();

  Line();
  Line(const string& clue_string);
  Line(const Line& other);
  Line& operator=(const Line& other);
  int& operator[](int index) { return this->cells_[index]; }
  const int& operator[](int index) const { return this->cells_[index]; }
  ~Line();

  int solve();

  int clue_size() const { return this->clue_size_; }
  int start_index() const { return this->start_index_; }
  int end_index() const { return this->end_index_; }
#ifdef DEBUG
  void print();

 public:
#else
 private:
#endif

  deque<int> clues_;
  int clue_size_;
  int sum_clues_;
  int capacity_;
  vector<int> cells_;
  int start_index_;
  int end_index_;

  void update_capacity_();
  void update_prefix_sum_();

  vector<vector<vector<int>*>>* dp_;
  vector<vector<bool>>* is_visited_;
  vector<int> prefix_sum_;
  int last_x_;

  vector<int>* solve_(int start_index, int clue_index);
  vector<int>* solve_get_obvious_result_(int start_index, int clue_index);
  vector<int>* solve_get_next_(int start_index, int clue_index);
  int solve_is_valid_(int start_index, int clue, int size);
  int update_clues_();

  int mark_();
  void pop_clue_front_(int new_start_index);
  void pop_clue_back_(int new_end_index);
  int update_start_index_();
  int update_end_index_();
};

#endif  // CLUE_HPP