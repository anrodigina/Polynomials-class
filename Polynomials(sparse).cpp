#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

template<typename T>
class Polynomial {
 private:
    std::map<int, T> data;

 public:
    explicit Polynomial(const std::vector<T> &v) {
        for (size_t i = 0; i != v.size(); i++) {
            if (v[i] != 0) {
                data.insert({i, v[i]});
            }
        }
    }

    explicit Polynomial(const T x = T(0)) {
        data.insert({0, x});
        DeleteZero();
    }

    template<typename IT>
    Polynomial(IT begin, IT end) {
        size_t count = 0;
        while (begin != end) {
            if (*begin != 0) {
                data.insert({count, *begin});
            }
            ++begin;
            ++count;
        }
    }

    bool operator==(const Polynomial<T> &other) const {
        return this->data == other.data;
    }

    bool operator==(const T x) const {
        return *this == Polynomial(x);
    }

    bool operator!=(const Polynomial<T> &other) const {
        return !(*this == other);
    }

    bool operator!=(const T x) const {
        return !(*this == x);
    }

    int Degree() const {
        if (data.empty())
            return -1;
        return data.rbegin()->first;
    }

    Polynomial &operator+=(const Polynomial<T> &other) {
        for (auto c : other.data) {
            data[c.first] += c.second;
        }
        DeleteZero();
        return *this;
    }

    Polynomial operator+(const Polynomial<T> &other) const {
        auto C = *this;
        C += other;
        return C;
    }

    Polynomial &operator+=(const T x) {
        *this += Polynomial<T>(x);
        return *this;
    }

    Polynomial operator+(const T x) const {
        auto C = *this;
        C += x;
        return C;
    }

    Polynomial &operator-=(const Polynomial<T> &other) {
        for (auto c : other.data) {
            data[c.first] -= c.second;
        }
        DeleteZero();
        return *this;
    }

    Polynomial operator-(const Polynomial<T> &other) const {
        auto C = *this;
        C -= other;
        return C;
    }

    Polynomial &operator-=(const T x) {
        *this -= Polynomial<T>(x);
        return *this;
    }

    Polynomial operator-(const T x) const {
        auto C = *this;
        C -= x;
        return C;
    }

    T operator()(const T x) const {
        T result = 0;
        for (size_t i = this->Degree(); i != 0; i--) {
            if (data.count(i)) {
                result = result * x + data.at(i);
            } else {
                result *= x;
            }
        }
        if (data.count(0)) {
            result = result * x + data.at(0);
        } else {
            result *= x;
        }
        return result;
    }


    Polynomial &operator*=(const Polynomial &other) {
        Polynomial<T> R;
        for (auto c : data) {
            for (auto d : other.data) {
                R.data[c.first + d.first] += c.second * d.second;
            }
        }
        *this = R;
        DeleteZero();
        return *this;
    }

    Polynomial &operator*=(const T &other) {
        *this *= Polynomial(other);
        return *this;
    }

    Polynomial operator*(const T &other) const {
        auto C = *this;
        C *= other;
        return C;
    }

    Polynomial operator*(const Polynomial &other) const {
        auto C = *this;
        C *= other;
        return C;
    }

    T operator[](size_t x) const {
        if (data.count(x)) {
            return data.at(x);
        }
        return T(0);
    }

    Polynomial operator&(const Polynomial &other) const {
        Polynomial<T> result;
        for (int i = this->Degree(); i != 0; --i) {
            if (data.count(i)) {
                result = result * other + data.at(i);
            } else {
                result *= other;
            }
        }
        if (data.count(0)) {
            result = result * other + data.at(0);
        } else {
            result *= other;
        }
        return result;
    }

    Polynomial operator%(const Polynomial &other) const {
        auto C = *this;
        auto Sum = Polynomial();
        while (C.Degree() >= other.Degree()) {
            auto ratio = C.data.at(C.Degree()) / other.data.at(other.Degree());
            int t = C.Degree() - other.Degree();
            std::vector<T> v(t + 1);
            v.back() = ratio;
            Polynomial<T> X = Polynomial(v);
            C -= X * other;
            Sum += X;
        }
        return C;
    }

    Polynomial operator/(const Polynomial &other) const {
        auto C = *this;
        auto Sum = Polynomial();
        while (C.Degree() >= other.Degree()) {
            auto ratio = C.data.at(C.Degree()) / other.data.at(other.Degree());
            int t = C.Degree() - other.Degree();
            std::vector<T> v(t + 1);
            v.back() = ratio;
            Polynomial<T> X = Polynomial(v);
            C -= X * other;
            Sum += X;
        }
        return Sum;
    }

    Polynomial operator,(const Polynomial &other) const {
        auto A = *this;
        auto B = other;
        while (B.Degree() > -1) {
            A = A % B;
            std::swap(A, B);
        }
        if (A[A.Degree()]!= 1) {
            A = A / Polynomial(A[A.Degree()]);
        }
        return A;
    }

    typename
    std::map<int, T>::const_iterator begin() const {
        return data.cbegin();
    }

    typename
    std::map<int, T>::iterator
    begin() {
        return data.begin();
    }

    typename
    std::map<int, T>::const_iterator end() const {
        return data.cend();
    }

    typename
    std::map<int, T>::iterator end() {
        return data.end();
    }

 private:
    void DeleteZero() {
        std::vector<int> blacklist;
        for (auto a : data) {
            if (a.second == 0) {
                blacklist.push_back(a.first);
            }
        }
        for (auto a : blacklist) {
            data.erase(a);
        }
    }
};

template<typename T>
std::ostream &operator<<(std::ostream &out, const Polynomial<T> P) {
    for (int i = P.Degree(); i > 0; --i) {
        if ((i != P.Degree()) && (P[i]) > 0) {
            out << "+";
        } else if (P[i] < 0) {
            out << "-";
        }
        if ((P[i] != 0) && (std::abs(P[i]) != 1)) {
            out << std::abs(P[i]) << "*";
        }
        if (P[i] != 0) {
            out << "x";
            if ((i != 1) && (i != 0)) {
                out << "^" << i;
            }
        }
    }
    if ((P[0] > 0) && (P.Degree() > 0)) {
        out << "+";
    }
    if ((P[0] != 0) || (P.Degree() == -1)) {
        out << P[0];
    }
    return out;
}
