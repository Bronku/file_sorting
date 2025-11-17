#pragma once
#include <array>
#include <iostream>

class Record {
public:
    std::array<int, 5> a;
    int x;

    int evaluate() const;
    bool operator<(const Record& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Record& r);
    friend std::istream& operator>>(std::istream& is, Record& r);

    static Record random();
};
