#pragma once

#include "record.hpp"
#include <stdexcept>
#include <vector>

class Buffer;

class SubBuffer {
private:
    Record* data_;
    size_t size_;

    friend class Buffer;

    SubBuffer(Record* data, size_t size)
        : data_(data)
        , size_(size)
    {
    }

public:
    // for std::sort support
    using iterator = Record*;
    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }

    Record& operator[](size_t index)
    {
        return data_[index];
    }

    size_t size() const { return size_; }
};

class Buffer {
private:
    std::vector<Record> storage_;

public:
    explicit Buffer(size_t total_size)
        : storage_(total_size)
    {
    }

    std::vector<SubBuffer> divide(size_t n)
    {
        if (n == 0) {
            throw std::invalid_argument("Cannot divide buffer into 0 pieces");
        }

        size_t total_size = storage_.size();
        if (total_size % n != 0) {
            throw std::invalid_argument("Size not divisible by n");
        }

        std::vector<SubBuffer> result(n);
        size_t piece_size = total_size / n;
        for (size_t i = 0; i < n; ++i) {
            result[i] = SubBuffer(storage_.data() + i * piece_size, piece_size);
        }

        return result;
    }
};
