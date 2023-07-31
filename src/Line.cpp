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
  return this->get_cache_(this->capacity_ + this->clue_size_, this->clue_size_);
  return (this->end_index_ - this->start_index_) * this->clue_size_ -
         this->sum_clues_;
}

int Line::solve() {
  int mark_status = this->mark_();
  if (mark_status == ERROR) return ERROR;
  if (mark_status == END) return SUCCESS;

  this->dp_ = new vector<vector<vector<int>*>>(
      this->cells_.size() + 1, vector<vector<int>*>(this->clue_size_, nullptr));
  this->is_visited_ = new vector<vector<bool>>(
      this->cells_.size() + 1, vector<bool>(this->clue_size_, false));
  this->update_prefix_sum_();

  this->last_x_ = this->end_index_ - 1;
  for (; this->last_x_ >= this->start_index_ && this->cells_[last_x_] != X;
       this->last_x_--) {
  }

  vector<int>* result_front = this->solve_(this->start_index_, 0);
  if (result_front != nullptr)
    for (int i = this->start_index_; i < this->end_index_; i++)
      (*this)[i] = (*result_front)[i - this->start_index_];
  for (vector<vector<int>*> v : *(this->dp_))
    for (vector<int>* v2 : v)
      if (v2 != nullptr) delete v2;
  delete this->dp_;
  delete this->is_visited_;
  if (result_front == nullptr) return ERROR;
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

vector<int>* Line::solve_(int start_index, int clue_index) {
  vector<int>& cells = this->cells_;
  vector<vector<vector<int>*>>& dp = *this->dp_;
  vector<vector<bool>>& is_visited = *this->is_visited_;

  if (is_visited[start_index][clue_index]) return dp[start_index][clue_index];
  is_visited[start_index][clue_index] = true;
  if (this->end_index_ - start_index - this->prefix_sum_[clue_index] < 0)
    return nullptr;

  vector<int>* obvious_result =
      this->solve_get_obvious_result_(start_index, clue_index);
  if (obvious_result != nullptr) {
    dp[start_index][clue_index] = obvious_result;
    return obvious_result;
  }

  vector<int>* next = this->solve_get_next_(start_index, clue_index);
  int clue = this->clues_[clue_index];
  int size = clue_index == this->clue_size_ - 1 ? this->end_index_ - start_index
                                                : clue + 1;
  if (this->solve_is_valid_(start_index, clue, size) == ERROR) {
    dp[start_index][clue_index] = next;
    return next;
  }

  vector<int>* result_back = nullptr;
  if (clue_index < this->clue_size_ - 1) {
    result_back = this->solve_(start_index + size, clue_index + 1);
    if (result_back == nullptr) {
      dp[start_index][clue_index] = next;
      return next;
    }
  }

  vector<int>* result_front = new vector<int>(size);
  for (int i = 0; i < clue; i++) (*result_front)[i] = O;
  for (int i = clue; i < size; i++) (*result_front)[i] = X;
  if (result_back != nullptr)
    result_front->insert(result_front->end(), result_back->begin(),
                         result_back->end());
  if (next != nullptr) {
    for (int i = 0; i < (*result_front).size(); i++)
      if ((*result_front)[i] != (*next)[i] && (*result_front)[i] != B)
        (*result_front)[i] = B;
    delete next;
  }

  dp[start_index][clue_index] = result_front;
  return result_front;
}

vector<int>* Line::solve_get_obvious_result_(int start_index, int clue_index) {
  if (start_index <= this->last_x_) return nullptr;
  for (int i = start_index; i < this->end_index_; i++)
    if (this->cells_[i] != B) return nullptr;

  vector<int>* result = new vector<int>(this->end_index_ - start_index, B);
  int capacity = this->end_index_ - start_index - this->prefix_sum_[clue_index];
  int index = -1;
  for (; clue_index < this->clue_size_; clue_index++) {
    index += this->clues_[clue_index];
    int block_size = this->clues_[clue_index] - capacity;
    for (int i = 0; i < block_size; i++) (*result)[index - i] = O;
    index++;
    if (index != result->size() && capacity == 0) (*result)[index] = X;
  }
  return result;
}

vector<int>* Line::solve_get_next_(int start_index, int clue_index) {
  if (this->cells_[start_index] == O) return nullptr;
  vector<int>* temp = this->solve_(start_index + 1, clue_index);
  if (temp == nullptr) return nullptr;
  vector<int>* next = new vector<int>(1, X);
  next->insert(next->end(), temp->begin(), temp->end());
  return next;
}

int Line::solve_is_valid_(int start_index, int clue, int size) {
  for (int i = 0; i < clue; i++)
    if (this->cells_[start_index + i] == X) return ERROR;
  for (int i = clue; i < size; i++)
    if (this->cells_[start_index + i] == O) return ERROR;
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

#ifdef DEBUG
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
#endif