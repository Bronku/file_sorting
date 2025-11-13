#pragma once
#include "record.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

// Main buffer pool - owns all memory
class BufferPool {
private:
    std::vector<Record> storage;
    int buffer_capacity;
    int num_buffers;

public:
    BufferPool(int num_buffers, int buffer_capacity)
        : storage(num_buffers * buffer_capacity)
        , buffer_capacity(buffer_capacity)
        , num_buffers(num_buffers)
    {
    }

    Record* get_buffer_ptr(int index)
    {
        return storage.data() + (index * buffer_capacity);
    }

    int get_buffer_capacity() const { return buffer_capacity; }
    int get_num_buffers() const { return num_buffers; }
    int total_capacity() const { return storage.size(); }

    // Sort entire pool (for stage 1 - creating runs)
    void sort_all(int count)
    {
        std::sort(storage.begin(), storage.begin() + count);
    }
};

// View into a section of the buffer pool
class BufferView {
private:
    Record* data;
    int capacity;
    int length;

public:
    BufferView(Record* ptr, int cap)
        : data(ptr)
        , capacity(cap)
        , length(0)
    {
    }

    // Read up to capacity records from stream
    int read_from(std::istream& in)
    {
        length = 0;
        for (int i = 0; i < capacity; i++) {
            if (!(in >> data[i])) {
                break;
            }
            length++;
        }
        return length;
    }

    // Write all current records to stream
    bool write_to(std::ostream& out) const
    {
        for (int i = 0; i < length; i++) {
            out << data[i] << '\n';
            if (!out)
                return false;
        }
        return true;
    }

    // For stage 2: peek at front record without consuming
    const Record& front() const { return data[0]; }

    // For stage 2: consume the front record
    void pop_front()
    {
        if (length > 0) {
            for (int i = 0; i < length - 1; i++) {
                data[i] = data[i + 1];
            }
            length--;
        }
    }

    // Refill from stream when buffer becomes empty during merge
    bool refill_from(std::istream& in)
    {
        return read_from(in) > 0;
    }

    int size() const { return length; }
    int get_capacity() const { return capacity; }
    bool empty() const { return length == 0; }
    bool full() const { return length == capacity; }

    Record& operator[](int i) { return data[i]; }
    const Record& operator[](int i) const { return data[i]; }
};

// For output buffer during merge
class OutputBuffer {
private:
    Record* data;
    int capacity;
    int length;

public:
    OutputBuffer(Record* ptr, int cap)
        : data(ptr)
        , capacity(cap)
        , length(0)
    {
    }

    // Add record to output buffer
    bool add(const Record& rec)
    {
        if (length >= capacity) {
            return false; // Buffer full
        }
        data[length++] = rec;
        return true;
    }

    // Flush buffer to stream and reset
    bool flush_to(std::ostream& out)
    {
        for (int i = 0; i < length; i++) {
            out << data[i] << '\n';
            if (!out)
                return false;
        }
        length = 0;
        return true;
    }

    bool full() const { return length >= capacity; }
    int size() const { return length; }
};
