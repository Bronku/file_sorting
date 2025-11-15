#pragma once
#include <array>
#include <iostream>
#include <random>

class Record {
public:
    std::array<int, 5> a;
    int x;

    int evaluate() const
    {
        int out = 0;
        int x_n = 1;
        for (int i = 0; i < 5; i++) {
            out += a[i] * x_n;
            x_n *= x;
        }
        return out;
    }

    bool operator<(const Record& other) const
    {
        return evaluate() < other.evaluate();
    }

    friend std::ostream& operator<<(std::ostream& os, const Record& r)
    {
        os << r.a[0] << ' ' << r.a[1] << ' ' << r.a[2] << ' '
           << r.a[3] << ' ' << r.a[4] << ' ' << r.x;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Record& r)
    {
        is >> r.a[0] >> r.a[1] >> r.a[2] >> r.a[3] >> r.a[4] >> r.x;
        return is;
    }

    static Record random()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dist;

        Record r;
        for (int& val : r.a) {
            val = dist(gen);
        }
        r.x = dist(gen);
        return r;
    }
};