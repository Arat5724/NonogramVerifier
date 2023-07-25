#include "line.hpp"

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

int Line::get_rank() const {
  return (this->end_index_ - this->start_index_) * this->clue_size_ -
         this->sum_clues_;
}

int Line::quick_solve() { return this->solve_(1) && this->mark_(); }

int Line::solve() { return this->solve_(0) && this->mark_(); }

/*
 * return value:
 *  None
 */
void Line::update_capacity_() {
  this->capacity_ = this->end_index_ - this->start_index_ - this->sum_clues_;
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
 *  None
 */
int Line::cells_empty_() {
  for (int i = this->start_index_; i < this->end_index_; i++) {
    if (this->cells_[i]) return 0;
  }
  return 1;
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
  // begin
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
  // end
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
  // mark intersection
  if (!this->clue_size_) {
    for (int i = this->start_index_; i < this->end_index_; i++) {
      if (this->cells_[i] == O) return ERROR;
      this->cells_[i] = X;
    }
    return END;
  };
  if (this->cells_empty_()) {
    int clue_index = this->start_index_ - 1;
    int block_size;
    for (int clue : this->clues_) {
      clue_index += clue;
      block_size = clue - this->capacity_;
      for (int i = 0; i < block_size; i++) {
        if (this->cells_[clue_index - i] == X) return ERROR;
        this->cells_[clue_index - i] = O;
      }
      clue_index++;
      if (clue_index != this->end_index_ && this->capacity_ == 0) {
        if (this->cells_[clue_index] == O) return ERROR;
        this->cells_[clue_index] = X;
      }
    }
    if (this->capacity_ == 0) {
      this->clues_.clear();
      this->clue_size_ = 0;
      this->sum_clues_ = 0;
      this->start_index_ = this->end_index_;
    }
    return END;
  }
  return SUCCESS;
}

/*
 * return value:
 *  SUCCESS: success
 *  ERROR: error
 *  END: Don't have to continue
 */
int Line::mark_clue_front_(int pos) {
  if (pos < 0) throw exception();
  if (pos > this->capacity_) return END;
  int i = this->start_index_;
  while (i < pos + this->start_index_) {
    if (this->cells_[i] == O) return END;
    this->cells_[i] = X;
    i++;
  }
  while (i < pos + this->start_index_ + this->clues_.front()) {
    if (this->cells_[i] == X) return ERROR;
    this->cells_[i] = O;
    i++;
  }
  if (i != this->end_index_) {
    if (this->cells_[i] == O) return ERROR;
    this->cells_[i] = X;
  }
  pop_clue_front_(i + 1);
  return SUCCESS;
}

/*
 * return value:
 *  SUCCESS: success
 *  ERROR: error
 *  END: Don't have to continue
 */
int Line::mark_clue_back_(int pos) {
  if (pos < 0) throw exception();
  if (pos > this->capacity_) return END;
  int i = this->end_index_ - 1;
  while (i > this->end_index_ - 1 - pos) {
    if (this->cells_[i] == O) return END;
    this->cells_[i] = X;
    i--;
  }
  while (i > this->end_index_ - 1 - pos - this->clues_.back()) {
    if (this->cells_[i] == X) return ERROR;
    this->cells_[i] = O;
    i--;
  }
  if (i != this->start_index_ - 1) {
    if (this->cells_[i] == O) return ERROR;
    this->cells_[i] = X;
  }
  pop_clue_back_(i);
  return SUCCESS;
}

/*
 * return value:
 *  FRONT
 *  BACK
 */
int Line::get_start_point_(int flag) {
  int number_none_blank_front, number_none_blank_back;
  number_none_blank_front = number_none_blank_back = 0;
  for (int i = this->start_index_; i < this->end_index_ && !(*this)[i];
       i++, number_none_blank_front++) {
  }
  number_none_blank_front -= this->clues_.front();
  for (int i = this->end_index_ - 1; i >= this->start_index_ && !(*this)[i];
       i--, number_none_blank_back++) {
  }
  number_none_blank_back -= this->clues_.back();

  if (number_none_blank_front < number_none_blank_back ||
      (number_none_blank_front == number_none_blank_back) && flag == 3) {
    return FRONT;
  } else {
    return BACK;
  }
}

/*
 * return value:
 *  SUCCESS: success
 *  ERROR: error
 */
int Line::solve_(int flag) {
#ifdef DEBUG
  this->print();
#endif
  if (this->clue_size_ == 0) {
    for (int i = this->start_index_; i < this->end_index_; i++) {
      if ((*this)[i] == O) return ERROR;
      (*this)[i] = X;
    }
    return SUCCESS;
  }

  int mark_status;
  mark_status = this->mark_();
  if (mark_status == END || flag >= FRONT + BACK) return SUCCESS;
  if (mark_status == ERROR) return ERROR;

  int start_point = this->get_start_point_(flag);
  bool is_copied = false;
  Line temp_line = *this;
  if (flag) flag += start_point;
  for (int pos = 0; pos <= this->capacity_; pos++) {
    Line new_line = *this;

    if (start_point == FRONT)
      mark_status = new_line.mark_clue_front_(pos);
    else
      mark_status = new_line.mark_clue_back_(pos);

    if (mark_status == END) break;
    // skip if error occurs.
    if (mark_status == ERROR) continue;
    if (new_line.solve_(flag) == ERROR) continue;

    if (!is_copied) {
      temp_line.cells_ = new_line.cells_;
      is_copied = true;
    } else {
      temp_line.intersect_(new_line);
    }
  }
  if (is_copied) {
    *this = temp_line;
    return SUCCESS;
  }
  return ERROR;  // No solution
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

void Line::intersect_(const Line& other) {
  for (int i = this->start_index_; i < this->end_index_; i++) {
    if (this->cells_[i] != other.cells_[i] && this->cells_[i])
      this->cells_[i] = B;
  }
}

#ifdef DEBUG
void Line::print() const {
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