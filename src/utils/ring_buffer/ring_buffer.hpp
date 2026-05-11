#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

template <typename T>
class RingBuffer {
public:
    explicit RingBuffer(size_t capacity)
        : data_(capacity + 1), head_(0), tail_(0) {}

    void push(const T& item) {
        data_[tail_] = item;
        tail_ = (tail_ + 1) % data_.size();
        if (tail_ == head_) {
            head_ = (head_ + 1) % data_.size();
        }
    }

    T pop() {
        if (empty()) {
            throw std::underflow_error("RingBuffer: pop on empty buffer");
        }
        T item = data_[head_];
        head_ = (head_ + 1) % data_.size();
        return item;
    }

    T& front() {
        if (empty()) {
            throw std::underflow_error("RingBuffer: front on empty buffer");
        }
        return data_[head_];
    }

    T& back() {
        if (empty()) {
            throw std::underflow_error("RingBuffer: back on empty buffer");
        }
        return data_[(tail_ - 1 + data_.size()) % data_.size()];
    }

    bool empty() const { return head_ == tail_; }

    bool full() const {
        return (tail_ + 1) % data_.size() == head_;
    }

    size_t size() const {
        return (tail_ - head_ + data_.size()) % data_.size();
    }

    size_t capacity() const { return data_.size() - 1; }

    void clear() {
        head_ = tail_ = 0;
    }

private:
    std::vector<T> data_;
    size_t head_;
    size_t tail_;
};
