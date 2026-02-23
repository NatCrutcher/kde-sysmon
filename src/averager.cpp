#include "averager.h"

Averager::Averager(int capacity) : buf_(capacity, 0), cap_(capacity) {}

void Averager::push(double val) {
    if (count_ == cap_) {
        sum_ -= buf_[pos_];
    } else {
        count_++;
    }
    buf_[pos_] = val;
    sum_ += val;
    pos_ = (pos_ + 1) % cap_;
}

double Averager::mean() const {
    return count_ > 0 ? sum_ / count_ : 0;
}
