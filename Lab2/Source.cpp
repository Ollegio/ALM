#include <stdio.h> 
#include <iostream> 
#include <string>
#include <bitset>
#include <fstream>
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

	bitsetArithmetic<N> bitsetMultiply(bitsetArithmetic<N>& x_2, const bitsetArithmetic<N>& y_2) {
		bitsetArithmetic<N> res;
		// мы хотим минимизировать количество операций сдвига и сложения
		if (x_2.count() < y_2.count()) {
			for (int i = 0; i < N; i++)
				if (x_2[i])
					res = bitsetAdd(res, y_2 << i);
		}
		else {
			for (int i = 0; i < N; i++)
				if (y_2[i])
					res = bitsetAdd(res, x_2 << i);
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

bitsetArithmetic<8> mul48(bitsetArithmetic<4>& x, const bitsetArithmetic<4>& y) {
	bitsetArithmetic<8> res, x_2(x.to_ulong()), y_2(y.to_ulong());
	// мы хотим минимизировать количество операций сдвига и сложения
	if (x_2.count() < y_2.count()) {
		for (int i = 0; i < 8; i++)
			if (x_2[i])
				res = res + y_2 << i;
	}
	else {
		for (int i = 0; i < 8; i++)
			if (y_2[i])
				res = res + x_2 << i;
	}

	return res;
}

int main() {
	const int READERROR = 0b00001,
		WRONGOPERATION = 0b00010,
		WRONGOPERAND = 0b00100,
		ZERODIVERROR = 0b01000,
		FILEERROR = 0b10000;
	int error = 0b0000;
	bitsetArithmetic<4> oprnd_1, oprnd_2, div_res;
	bitsetArithmetic<8> mul_res;
	bitset<2> div(3), mul(0), operation;
	ifstream fin("input.txt");
	ofstream fout("output.txt");
	if (fin.fail()) {
		error |= FILEERROR;
		fout << "bad file. error code: " << bitset<5>(error);
		return 0;
	}
	char operat_buf[3], operand_buf[5];

	int curState = 0;
	bool start = true;

	while (true) {
		if (curState == 0 && !start)
			break;

		else
			switch (curState) {
			case 0:
				if (start) {
					start = false;
					fout << "S0\n";
					fout << "Operation registry: " << operation << endl;
					fout << "Operand 1: " << oprnd_1 << endl;
					fout << "Operand 2: " << oprnd_2 << endl;
					fout << "error code: " << bitset<5>(error);
					curState = 1;
				}
				else {
					return 0;
				}
				break;
			case 1:
				if (fin.fail()) {
					error |= FILEERROR;
					fout << "bad file. error code: " << bitset<5>(error);
					curState = 0;
				}
				else {
					fout << "S1\n";
					fout << "Operation registry: " << operation << endl;
					fout << "Operand 1: " << oprnd_1 << endl;
					fout << "Operand 2: " << oprnd_2 << endl;
					fout << "error code: " << bitset<5>(error);
					curState = 2;
				}
				break;
			case 2:
				if (fin.read(operat_buf, 2)) {
					if (count(operat_buf, operat_buf + 2, '1') + count(operat_buf, operat_buf + 2, '0') == 2) {
						operation = bitset<2>(operat_buf, 2);
						fout << "S2\n";
						fout << "Operation registry: " << operation << endl;
						fout << "Operand 1: " << oprnd_1 << endl;
						fout << "Operand 2: " << oprnd_2 << endl;
						fout << "error code: " << bitset<5>(error);
						curState = 3;
					}
					else {
						error |= WRONGOPERATION;
						fout << "wrong operation. error code: " << bitset<5>(error);
						curState = 0;
					}
				}
				else {
					error |= READERROR;
					fout << "read error. error code: " << bitset<5>(error);
					curState = 0;
				}
				curState = 3;
				break;
			case 3:
				if (fin.read(operand_buf, 4))
					if (count(operand_buf, operand_buf + 4, '1') + count(operand_buf, operand_buf + 4, '0') == 4) {
						oprnd_1 = bitset<4>(operand_buf, 4);
						fout << "S3\n";
						fout << "Operation registry: " << operation << endl;
						fout << "Operand 1: " << oprnd_1 << endl;
						fout << "Operand 2: " << oprnd_2 << endl;
						fout << "error code: " << bitset<5>(error);
						curState = 4;
					}
					else {
						error |= WRONGOPERAND;
						fout << "wrong operand. error code: " << bitset<5>(error);
						curState = 0;
					}
				else {
					error |= READERROR;
					fout << "read error. error code: " << bitset<4>(error);
					curState = 0;
				}

				break;
			case 4:
				if (fin.read(operand_buf, 4))
					if (count(operand_buf, operand_buf + 4, '1') + count(operand_buf, operand_buf + 4, '0') == 4) {
						oprnd_2 = bitset<4>(operand_buf, 4);
						fout << "S4\n";
						fout << "Operation registry: " << operation << endl;
						fout << "Operand 1: " << oprnd_1 << endl;
						fout << "Operand 2: " << oprnd_2 << endl;
						fout << "error code: " << bitset<5>(error);
						curState = 5;
					}
					else {
						error |= WRONGOPERAND;
						fout << "wrong operand. error code: " << bitset<5>(error);
						curState = 0;
					}
				else {
					error |= READERROR;
					fout << "read error. error code: " << bitset<4>(error);
					curState = 0;
				}
				break;
			case 5:
				if (operation == div) {
					if (oprnd_2.any()) {
						div_res = oprnd_1 / oprnd_2;
						fout << "S5\n";
						fout << "Operation registry: " << operation << endl;
						fout << "Operand 1: " << oprnd_1 << endl;
						fout << "Operand 2: " << oprnd_2 << endl;
						fout << "error code: " << bitset<5>(error);
						fout << "Result: " << div_res;
						curState = 0;
					}
					else {
						error |= ZERODIVERROR;
						fout << "division by zero. error code: " << bitset<4>(error);
						curState = 0;
					}
				}
				else {
					mul_res = mul48(oprnd_1, oprnd_2);
					fout << "S5\n";
					fout << "Operation registry: " << operation << endl;
					fout << "Operand 1: " << oprnd_1 << endl;
					fout << "Operand 2: " << oprnd_2 << endl;
					fout << "error code: " << bitset<5>(error);
					fout << "Result: " << mul_res;
					curState = 0;
				}
				break;
			}
	}


}