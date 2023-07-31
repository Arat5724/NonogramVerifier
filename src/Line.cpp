#include "Line.hpp"

Line::Line() {}

Line::Line(const string& clue_string)
    : clue_size_(0),
      sum_clues_(0),
      cells_(vector<int>(clue_string.length(), 0)),
      start_index_(0),
      end_index_(clue_string.length()) {
  char status = B;
  int clue;

  for (char c : clue_string) {
    if (c == '1') {
      if (status == B) {
        status = O;
        this->clue_size_++;
        clue = 1;
      } else {
        clue++;
      }
    } else if (c == '0') {
      if (status == O) {
        status = B;
        this->clues_.push_back(clue);
      }
    } else {
      throw exception();
    }
  }
  if (status == O) this->clues_.push_back(clue);
  for (int clue : this->clues_) this->sum_clues_ += clue;
  if (this->clue_size_ > 0) this->sum_clues_ += this->clue_size_ - 1;
  this->update_capacity_();
}

Line::Line(const Line& other)
    : clues_(other.clues_),
      clue_size_(other.clue_size_),
      sum_clues_(other.sum_clues_),
      capacity_(other.capacity_),
      cells_(other.cells_),
      start_index_(other.start_index_),
      end_index_(other.end_index_) {}

Line& Line::operator=(const Line& other) {
  this->clues_ = other.clues_;
  this->clue_size_ = other.clue_size_;
  this->sum_clues_ = other.sum_clues_;
  this->capacity_ = other.capacity_;
  this->cells_ = other.cells_;
  this->start_index_ = other.start_index_;
  this->end_index_ = other.end_index_;
  return *this;
}

Line::~Line() {}

vector<vector<long long>> Line::cache_ =
    vector<vector<long long>>(111, vector<long long>(111, 0));

vector<vector<vector<int>>> Line::dp_ = vector<vector<vector<int>>>(
    301, vector<vector<int>>(150, vector<int>(301)));

vector<vector<int>> Line::is_visited_ =
    vector<vector<int>>(301, vector<int>(150));

long long Line::get_cache_(int n, int r) {
  if (this->cache_[n][r]) return this->cache_[n][r];
  if (n - r < r) r = n - r;
  if (r == 0) {
    this->cache_[n][r] = 1;
  } else {
    this->cache_[n][r] =
        this->get_cache_(n - 1, r - 1) + this->get_cache_(n - 1, r);
  }
  return this->cache_[n][r];
}

long long Line::get_rank() {
  if (this->clue_size_ == 0 || this->capacity_ == 0) return 0;
  // return this->get_cache_(this->capacity_ + this->clue_size_,
  // this->clue_size_);
  return (this->end_index_ - this->start_index_) * this->clue_size_ -
         this->sum_clues_;
}

int Line::solve() {
  int mark_status = this->mark_();
  if (mark_status == ERROR) return ERROR;
  if (mark_status == END) return SUCCESS;
  for (int i = 0; i < this->cells_.size() + 1; i++)
    for (int j = 0; j < this->clue_size_; j++) this->is_visited_[i][j] = NO;
  this->update_prefix_sum_();
  this->last_x_ = this->end_index_ - 1;
  for (; this->last_x_ >= this->start_index_ && this->cells_[last_x_] == B;
       this->last_x_--) {
  }
  if (this->solve_(this->start_index_, 0) == ERROR) return ERROR;
  vector<int>& result = this->dp_[this->start_index_][0];
  for (int i = this->start_index_; i < this->end_index_; i++)
    (*this)[i] = result[i];
  if (this->update_clues_() == ERROR) return ERROR;
  return SUCCESS;
}

/*
 * return value:
 *  None
 */
void Line::update_capacity_() {
  this->capacity_ = this->end_index_ - this->start_index_ - this->sum_clues_;
}

void Line::update_prefix_sum_() {
  if (this->prefix_sum_.size() == this->clue_size_) return;
  this->prefix_sum_ = vector<int>(this->clues_.begin(), this->clues_.end());
  for (int i = this->clue_size_ - 2; i >= 0; i--)
    this->prefix_sum_[i] += this->prefix_sum_[i + 1] + 1;
}

int Line::solve_(int start_index, int clue_index) {
  vector<int>& cells = this->cells_;

  int& current_status = this->is_visited_[start_index][clue_index];

  vector<int>& current = this->dp_[start_index][clue_index];
  vector<int>& next = this->dp_[start_index + 1][clue_index];

  if (current_status != NO) return current_status;

  if ((this->end_index_ - start_index - this->prefix_sum_[clue_index] < 0) ||
      (start_index != this->start_index_ && cells[start_index - 1] == O)) {
    current_status = ERROR;
    return ERROR;
  }
  // if (this->solve_get_obvious_result_(start_index, clue_index) == SUCCESS) {
  //   current_status = SUCCESS;
  //   return SUCCESS;
  // }

  int next_status = this->solve_(start_index + 1, clue_index);
  int clue = this->clues_[clue_index];
  int size = clue_index == this->clue_size_ - 1 ? this->end_index_ - start_index
                                                : clue + 1;

  if ((this->solve_front_(start_index, clue_index, clue, size) == ERROR) ||
      this->solve_back_(start_index, clue_index, clue, size) == ERROR) {
    current_status = next_status;
    if (next_status == SUCCESS) {
      current[start_index] = X;
      for (int i = start_index + 1; i < this->end_index_; i++)
        current[i] = next[i];
    }
    return next_status;
  }
  if (next_status == SUCCESS) {
    current[start_index] = B;
    for (int i = start_index + 1; i < this->end_index_; i++)
      if (current[i] != next[i] && current[i] != B) current[i] = B;
  }
  current_status = SUCCESS;
  return current_status;
}

int Line::solve_get_obvious_result_(int start_index, int clue_index) {
  if (start_index <= this->last_x_) return ERROR;

  vector<int>& current = this->dp_[start_index][clue_index];

  int capacity = this->end_index_ - start_index - this->prefix_sum_[clue_index];
  int index = start_index - 1;
  for (; clue_index < this->clue_size_; clue_index++) {
    index += this->clues_[clue_index];
    int block_size = this->clues_[clue_index] - capacity;
    for (int i = 0; i < block_size; i++) current[index - i] = O;
    index++;
    if (index != this->end_index_ && capacity == 0) current[index] = X;
  }
  return SUCCESS;
}

int Line::solve_get_next_(int start_index, int clue_index) {
  if (this->cells_[start_index] == O) ERROR;
  return this->solve_(start_index + 1, clue_index);
}

int Line::solve_front_(int start_index, int clue_index, int clue, int size) {
  vector<int>& current = this->dp_[start_index][clue_index];
  for (int i = 0; i < clue; i++) {
    if (this->cells_[start_index + i] == X) return ERROR;
    current[start_index + i] = O;
  }
  for (int i = clue; i < size; i++) {
    if (this->cells_[start_index + i] == O) return ERROR;
    current[start_index + i] = X;
  }
  return SUCCESS;
}

int Line::solve_back_(int start_index, int clue_index, int clue, int size) {
  if (clue_index >= this->clue_size_ - 1) return SUCCESS;
  if (this->solve_(start_index + size, clue_index + 1) == ERROR) return ERROR;
  vector<int>& current = this->dp_[start_index][clue_index];
  vector<int>& back = this->dp_[start_index + size][clue_index + 1];
  for (int i = start_index + size; i < this->end_index_; i++)
    current[i] = back[i];
  return SUCCESS;
}

/*
 * return value:
 *  SUCCESS: success
 *  ERROR: error
 */
int Line::update_clues_() {
  if (this->clue_size_ == 0) return SUCCESS;
  while (this->clue_size_) {
    int block_size = 0;
    int i = this->start_index_;
    while (i < this->end_index_ && (*this)[i] == O) {
      i++;
      block_size++;
    }
    if (block_size == this->clues_.front()) {
      while (i < this->end_index_ && (*this)[i] == X) i++;
      this->pop_clue_front_(i);
    } else if (block_size < this->clues_.front())
      break;
    else
      return ERROR;
  }
  while (this->clue_size_) {
    int block_size = 0;
    int i = this->end_index_ - 1;
    while (i >= this->start_index_ && (*this)[i] == O) {
      i--;
      block_size++;
    }
    if (block_size == this->clues_.back()) {
      while (i >= this->start_index_ && (*this)[i] == X) i--;
      this->pop_clue_back_(i + 1);
    } else if (block_size < this->clues_.back())
      break;
    else
      return ERROR;
  }
  return SUCCESS;
}
/*
 * return value:
 *  SUCCESS: success
 *  ERROR: error
 *  END: Don't have to continue
 */
int Line::mark_() {
  if (update_start_index_() == ERROR) return ERROR;
  if (update_end_index_() == ERROR) return ERROR;
  // front
  while (this->clue_size_ && this->cells_[this->start_index_] == O) {
    int i = this->start_index_ + 1;
    while (i < this->start_index_ + this->clues_.front()) {
      if (this->cells_[i] == X) return ERROR;
      this->cells_[i] = O;
      i++;
    }
    if (i != this->end_index_) {
      if (this->cells_[i] == O) return ERROR;
      this->cells_[i] = X;
    }
    pop_clue_front_(i + 1);
    if (update_start_index_() == ERROR) return ERROR;
  }
  // back
  while (this->clue_size_ && this->cells_[this->end_index_ - 1] == O) {
    int i = this->end_index_ - 2;
    while (i > this->end_index_ - 1 - this->clues_.back()) {
      if (this->cells_[i] == X) return ERROR;
      this->cells_[i] = O;
      i--;
    }
    if (i != this->start_index_ - 1) {
      if (this->cells_[i] == O) return ERROR;
      this->cells_[i] = X;
    }
    pop_clue_back_(i);
    if (update_end_index_() == ERROR) return ERROR;
  }
  if (this->clue_size_ == 0) {
    for (int i = this->start_index_; i < this->end_index_; i++) {
      if (this->cells_[i] == O) return ERROR;
      this->cells_[i] = X;
    }
    return END;
  }
  return SUCCESS;
}
/*
 * return value:
 *  None
 */
void Line::pop_clue_front_(int new_start_index) {
  this->start_index_ = new_start_index;
  this->sum_clues_ -= this->clues_.front();
  this->clues_.pop_front();
  this->clue_size_--;
  if (this->clue_size_) this->sum_clues_ -= 1;
  this->update_capacity_();
}

/*
 * return value:
 *  None
 */
void Line::pop_clue_back_(int new_end_index) {
  this->end_index_ = new_end_index;
  this->sum_clues_ -= this->clues_.back();
  this->clues_.pop_back();
  this->clue_size_--;
  if (this->clue_size_) this->sum_clues_ -= 1;
  this->update_capacity_();
}

/*
 * return value:
 *  SUCCESS: success
 *  ERROR: error
 */
int Line::update_start_index_() {
  if (this->clue_size_ == 0) return SUCCESS;
  int new_start_index = this->start_index_;
  while (new_start_index == this->start_index_ &&
         this->start_index_ < this->end_index_) {
    while (new_start_index < this->end_index_ &&
           this->cells_[new_start_index] != X)
      new_start_index++;
    if (new_start_index - this->start_index_ < this->clues_.front()) {
      for (int i = this->start_index_; i < new_start_index; i++) {
        if (this->cells_[i] == O) return ERROR;
        this->cells_[i] = X;
      }
      while (new_start_index < this->end_index_ &&
             this->cells_[new_start_index] == X)
        new_start_index++;
      this->start_index_ = new_start_index;
    }
  }
  this->update_capacity_();
  if (this->capacity_ < 0) return ERROR;
  return SUCCESS;
}

/*
 * return value:
 *  SUCCESS: success
 *  ERROR: error
 */
int Line::update_end_index_() {
  if (this->clue_size_ == 0) return SUCCESS;
  int new_end_index = this->end_index_;
  while (new_end_index == this->end_index_ &&
         this->start_index_ < this->end_index_) {
    while (new_end_index > this->start_index_ &&
           this->cells_[new_end_index - 1] != X)
      new_end_index--;
    if (this->end_index_ - new_end_index < this->clues_.back()) {
      for (int i = new_end_index; i < this->end_index_; i++) {
        if (this->cells_[i] == O) return ERROR;
        this->cells_[i] = X;
      }
      while (new_end_index > this->start_index_ &&
             this->cells_[new_end_index - 1] == X)
        new_end_index--;
      this->end_index_ = new_end_index;
    }
  }
  this->update_capacity_();
  if (this->capacity_ < 0) return ERROR;
  return SUCCESS;
}

// #ifdef DEBUG
void Line::print() {
  cout << "rank: " << this->get_rank() << endl;
  cout << "\nline.clues_= {";
  for (int clue : this->clues_) cout << clue << ", ";
  cout << "};" << endl;

  cout << "line.clue_size_= " << this->clue_size_ << ";" << endl
       << "line.sum_clues_= " << this->sum_clues_ << ";" << endl
       << "line.capacity_= " << this->capacity_ << ";" << endl;

  cout << "line.cells_= {";
  for (int cell : this->cells_) {
    if (cell == X)
      cout << "X,";
    else if (cell == O)
      cout << "O,";
    else
      cout << "B,";
  }
  cout << "};" << endl;
  cout << "line.start_index_= " << this->start_index_ << ";" << endl
       << "line.end_index_= " << this->end_index_ << ";" << endl
       << "-------------------" << endl;
}
// #endif