#include "../include/record.hpp"
#include <random>

int Record::evaluate() const
{
    int out = 0;
    int x_n = 1;
    for (int i = 0; i < 5; i++) {
        out += a[i] * x_n;
        x_n *= x;
    }
    return out;
}

bool Record::operator<(const Record& other) const
{
    return evaluate() < other.evaluate();
}

Record Record::random()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist;

    Record r {};
    for (int& val : r.a) {
        val = dist(gen);
    }
    r.x = dist(gen);
    return r;
}

std::ostream& operator<<(std::ostream& os, const Record& r)
{

    os << r.a[0] << ' ' << r.a[1] << ' ' << r.a[2] << ' '
       << r.a[3] << ' ' << r.a[4] << ' ' << r.x;
    return os;
}

std::istream& operator>>(std::istream& is, Record& r)
{
    is >> r.a[0] >> r.a[1] >> r.a[2] >> r.a[3] >> r.a[4] >> r.x;
    return is;
}
