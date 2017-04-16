#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

template<typename T>
class Polynomial {
 private:
    std::vector<T> data;

 public:
    explicit Polynomial(const std::vector<T> &v) : data(v) {
    }

    explicit Polynomial(const T x = T(0)) : data({x}) {
    }

    template<typename IT>
    Polynomial(IT begin, IT end) {
        while (begin != end) {
            data.push_back(*begin);
            ++begin;
        }
    }

    bool operator==(const Polynomial<T> &other) const {
        if (this->Degree() != other.Degree()) {
            return false;
        } else {
            for (size_t i = 0; i != this->Degree() + 1; i++) {
                if (data[i] != other.data[i]) {
                    return false;
                }
            }
        }
        return true;
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
        for (int i = static_cast<int> (data.size()) - 1; i >= 0; i--) {
            if (data[i] != 0) {
                return i;
            }
        }
        return -1;
    }

    Polynomial &operator+=(const Polynomial<T> &other) {
        for (int i = 0; i < other.data.size(); i++) {
            if (i < data.size()) {
                data[i] += other.data[i];
            } else {
                data.push_back(other.data[i]);
            }
        }
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
        for (int i = 0; i < other.data.size(); i++) {
            if (i < data.size()) {
                data[i] -= other.data[i];
            } else {
                data.push_back(-other.data[i]);
            }
        }
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
            result = result * x + data[i];
        }
        return result * x + data[0];
    }

    Polynomial &operator*=(const T &other) {
        for (int i = 0; i < data.size(); i++) {
            data[i] *= other;
        }
        return *this;
    }

    Polynomial &operator*=(const Polynomial &other) {
        std::vector<T> result(data.size() + other.data.size(), T(0));
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < other.data.size(); j++) {
                result[j + i] += data[i] * other.data[j];
            }
        }
        Polynomial<T> R(result);
        *this = R;
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
        if (x < data.size()) {
            return data[x];
        }
        return T(0);
    }

    Polynomial operator&(const Polynomial &other) const {
        Polynomial<T> result;
        for (int i = this->Degree(); i != 0; --i) {
            result = result * other + data[i];
        }
        return result * other + data[0];
    }

    Polynomial operator%(const Polynomial &other) const {
        auto C = *this;
        auto Sum = Polynomial();
        while (C.Degree() >= other.Degree()) {
            auto ratio = C.data[C.Degree()] / other.data[other.Degree()];
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
            auto ratio = C.data[C.Degree()] / other.data[other.Degree()];
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
    std::vector<T>::const_iterator begin() const {
        return data.cbegin();
    }

    typename
    std::vector<T>::iterator
    begin() {
        return data.begin();
    }

    typename
    std::vector<T>::const_iterator end() const {
        return data.cbegin() + std::max(1, this->Degree() + 1);
    }

    typename
    std::vector<T>::iterator end() {
        return data.begin() + std::max(1, this->Degree() + 1);
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
