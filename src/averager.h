#pragma once
#include <vector>

class Averager {
public:
    explicit Averager(int capacity);
    void push(double val);
    double mean() const;

private:
    std::vector<double> buf_;
    int cap_;
    int pos_ = 0;
    int count_ = 0;
    double sum_ = 0;
};
