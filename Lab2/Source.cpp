#include <stdio.h> 
#include <iostream> 
#include <string>
#include <bitset>
using namespace std;

template<unsigned int N>
class bitsetArithmetic : public bitset<N> {

	bool fullAdder(bool b1, bool b2, bool& carry) {
		bool sum = (b1 ^ b2) ^ carry;
		carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
		return sum;

	}

	bool fullSubtractor(bool b1, bool b2, bool& borrow) {
		bool diff;
		if (borrow) {
			diff = !(b1 ^ b2);
			borrow = !b1 || (b1 && b2);
		}
		else {
			diff = b1 ^ b2;
			borrow = !b1 && b2;
		}
		return diff;

	}

	bitsetArithmetic<N> bitsetAdd(bitsetArithmetic<N> x, const bitsetArithmetic<N> y) {
		bool carry = false;
		for (int i = 0; i < N; i++) {
			x[i] = fullAdder(x[i], y[i], carry);
		}

		return x;
	}

	bitsetArithmetic<N> bitsetSubtract(bitsetArithmetic<N> x, const bitsetArithmetic<N> y) {
		bool borrow = false;
		for (int i = 0; i < N; i++) {
			if (borrow) {
				if (x[i]) {
					x[i] = y[i];
					borrow = y[i];
				}
				else {
					x[i] = !y[i];
					borrow = true;
				}
			}
			else {
				if (x[i]) {
					x[i] = !y[i];
					borrow = false;
				}
				else {
					x[i] = y[i];
					borrow = y[i];
				}
			}
		}

		return x;
	}

	bitsetArithmetic<N> bitsetMultiply(bitsetArithmetic<N>& x, const bitsetArithmetic<N>& y) {
		bitsetArithmetic<N> res;

		// мы хотим минимизировать количество операций сдвига и сложения
		if (x.count() < y.count()) {
			for (int i = 0; i < N; i++)
				if (x[i])
					res = bitsetAdd(res, y << i);
		}
		else {
			for (int i = 0; i < N; i++)
				if (y[i])
					res = bitsetAdd(res, x << i);
		}

		return res;
	}


	void bitsetDivide(bitsetArithmetic<N> x, bitsetArithmetic<N> y,
		bitsetArithmetic<N>& q, bitsetArithmetic<N>& r) {
		if (y.none()) {
			throw domain_error("division by zero undefined");
		}
		q.reset();
		r.reset();
		if (x.none()) {
			return;
		}
		if (x == y) {
			q[0] = 1;
			return;
		}
		r = x;
		if (x < y) {
			return;
		}
		// подсчитать количество значащих цифр в делителе и делимом
		unsigned int sig_x;
		for (int i = N - 1; i >= 0; i--) {
			sig_x = i;
			if (x[i]) break;
		}
		unsigned int sig_y;
		for (int i = N - 1; i >= 0; i--) {
			sig_y = i;
			if (y[i]) break;
		}
		// выровнять делитель по отношению к делимому
		unsigned int k = (sig_x - sig_y);
		y <<= k;
		// обеспечить правильное число шагов цикла
		k += 1;
		// удлиненный алгоритм деления со сдвигом и вычитанием
		while (k--) {
			// сдвинуть частное влево
			if (y <= r) {
				// добавить новую цифру к частному
				q[k] = true;
				r = r - y;
			}
			// сдвинуть делитель вправо
			y >>= 1;
		}

	}

	bitsetArithmetic<N> div(bitsetArithmetic<N> x, bitsetArithmetic<N> y) {
		bitsetArithmetic<N> d;
		bitsetArithmetic<N> m;
		bitsetDivide(x, y, d, m);
		return d;
	}

	bitsetArithmetic<N> mod(bitsetArithmetic<N> x, bitsetArithmetic<N> y) {
		bitsetArithmetic<N> d;
		bitsetArithmetic<N> m;
		bitsetDivide(x, y, d, m);
		return m;
	}

public:
	bitsetArithmetic<N>() : bitset<N>() {};
	bitsetArithmetic<N>(bitset<N> std_bs) : bitset<N>(std_bs) {};
	bitsetArithmetic<N>(string s) : bitset<N>(s) {};

	bool operator<(const bitsetArithmetic<N>& y) {
		bitsetArithmetic<N>& x = *this;
		for (int i = N - 1; i >= 0; i--) {
			if (x[i] && !y[i]) return false;
			if (!x[i] && y[i]) return true;
		}
		return false;

	}

	bool operator<=(const bitsetArithmetic<N>& y) {
		bitsetArithmetic<N>& x = *this;
		for (int i = N - 1; i >= 0; i--) {
			if (x[i] && !y[i]) return false;
			if (!x[i] && y[i]) return true;
		}
		return true;

	}

	bool operator>=(const bitsetArithmetic<N>& y) {
		bitsetArithmetic<N>& x = *this;
		for (int i = N - 1; i >= 0; i--) {
			if (x[i] && !y[i]) return true;
			if (!x[i] && y[i]) return false;
		}
		return true;

	}

	bool operator>(const bitsetArithmetic<N>& y) {
		bitsetArithmetic<N>& x = *this;
		for (int i = N - 1; i >= 0; i--) {
			if (x[i] && !y[i]) return true;
			if (!x[i] && y[i]) return false;
		}
		return false;

	}

	bitsetArithmetic<N> operator+ (bitsetArithmetic<N> r) {
		return bitsetAdd(*this, r);
	}

	bitsetArithmetic<N> operator- (bitsetArithmetic<N> r) {
		return bitsetSubtract(*this, r);
	}

	bitsetArithmetic<N> operator/ (bitsetArithmetic<N> r) {
		return div(*this, r);
	}

	bitsetArithmetic<N> operator% (bitsetArithmetic<N> r) {
		return mod(*this, r);
	}

	bitsetArithmetic<N> operator* (bitsetArithmetic<N> r) {
		return bitsetMultiply(*this, r);
	}
};