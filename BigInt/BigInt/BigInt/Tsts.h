#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <iomanip>
#include <sstream>

using namespace std;

class BigInt {
	// основание системы счисления (1 000 000 000)
	static const int BASE = 1000000000;

	// внутреннее хранилище числа
	vector<int> val;

	// знак числа
	bool neg;

	void _remove_leading_zeros();
	void _shift_right();

public:
	// класс-исключение, бросаемое при делении на ноль
	class divide_by_zero : public std::exception {  };

	BigInt();
	BigInt(string);
	BigInt(signed char);
	BigInt(unsigned char);
	BigInt(signed short);
	BigInt(unsigned short);
	BigInt(signed int);
	BigInt(unsigned int);
	BigInt(signed long);
	BigInt(unsigned long);
	BigInt(signed long long);
	BigInt(unsigned long long);

	friend std::ostream& operator <<(std::ostream&, const BigInt&);
	operator std::string() const;
	const BigInt operator +() const;
	const BigInt operator -() const;
	const BigInt operator ++();
	const BigInt operator ++(int);
	const BigInt operator --();
	const BigInt operator --(int);
	friend bool operator ==(const BigInt&, const BigInt&);
	friend bool operator <(const BigInt&, const BigInt&);
	friend bool operator !=(const BigInt&, const BigInt&);
	friend bool operator <=(const BigInt&, const BigInt&);
	friend bool operator >(const BigInt&, const BigInt&);
	friend bool operator >=(const BigInt&, const BigInt&);
	friend const BigInt operator +(BigInt, const BigInt&);
	BigInt& operator +=(const BigInt&);
	friend const BigInt operator -(BigInt, const BigInt&);
	BigInt& operator -=(const BigInt&);
	friend const BigInt operator *(const BigInt&, const BigInt&);
	BigInt& operator *=(const BigInt&);
	friend const BigInt operator /(const BigInt&, const BigInt&);
	BigInt& operator /=(const BigInt&);
	friend const BigInt operator %(const BigInt&, const BigInt&);
	BigInt& operator %=(const BigInt&);

	bool odd() const;
	bool even() const;
	const BigInt pow(BigInt) const;
};

// создает длинное целое число со значением 0
BigInt::BigInt() 
{
}

// создает длинное целое число из C++-строки
BigInt::BigInt(string str) 
{
	if (str.length() == 0) {
		this->neg = false;
	}
	else {
		if (str[0] == '-') {
			str = str.substr(1);
			this->neg = true;
		}
		else {
			this->neg = false;
		}

		for (long long i = str.length(); i > 0; i -= 9) {
			if (i < 9)
				this->val.push_back(atoi(str.substr(0, i).c_str()));
			else
				this->val.push_back(atoi(str.substr(i - 9, 9).c_str()));
		}

		this->_remove_leading_zeros();
	}
}

// удаляет ведущие нули
void BigInt::_remove_leading_zeros() {
	while (this->val.size() > 1 && this->val.back() == 0) {
		this->val.pop_back();
	}

	if (this->val.size() == 1 && this->val[0] == 0) this->neg = false;
}

// печатает число в поток вывода
std::ostream& operator <<(std::ostream& os, const BigInt& bi) {
	if (bi.val.empty()) os << 0;
	else {
		if (bi.neg) os << '-';
		os << bi.val.back();
		char old_fill = os.fill('0');
		for (long long i = static_cast<long long>(bi.val.size()) - 2; i >= 0; --i) os << std::setw(9) << bi.val[i];
		os.fill(old_fill);
	}

	return os;
}

// сравнивает два числа на равенство
bool operator ==(const BigInt& left, const BigInt& right) {
	if (left.neg != right.neg) return false;
	if (left.val.empty()) {
		if (right.val.empty() || (right.val.size() == 1 && right.val[0] == 0)) return true;
		else return false;
	}

	if (right.val.empty()) {
		if (left.val.size() == 1 && left.val[0] == 0) return true;
		else return false;
	}

	if (left.val.size() != right.val.size()) return false;
	for (size_t i = 0; i < left.val.size(); ++i) if (left.val[i] != right.val[i]) return false;

	return true;
}

// возвращает копию переданного числа
const BigInt BigInt::operator +() const {
	return BigInt(*this);
}

// возвращает переданное число с другим знаком
const BigInt BigInt::operator -() const {
	BigInt copy(*this);
	copy.neg = !copy.neg;
	return copy;
}

// проверяет, является ли левый операнд меньше правого
bool operator <(const BigInt& left, const BigInt& right) {
	if (left == right) return false;
	if (left.neg) {
		if (right.neg) return ((-right) < (-left));
		else return true;
	}
	else if (right.neg) return false;
	else {
		if (left.val.size() != right.val.size()) {
			return left.val.size() < right.val.size();
		}
		else {
			for (long long i = left.val.size() - 1; i >= 0; --i) {
				if (left.val[i] != right.val[i]) return left.val[i] < right.val[i];
			}

			return false;
		}
	}
}

// сравнивает два числа на неравенство
bool operator !=(const BigInt& left, const BigInt& right) {
	return !(left == right);
}

// проверяет, является ли левый операнд меньше либо равен правого
bool operator <=(const BigInt& left, const BigInt& right) {
	return (left < right || left == right);
}

// проверяет, является ли левый операнд больше правого
bool operator >(const BigInt& left, const BigInt& right) {
	return !(left <= right);
}

// проверяет, является ли левый операнд больше либо равен правого
bool operator >=(const BigInt& left, const BigInt& right) {
	return !(left < right);
}

// складывает два числа
const BigInt operator +(BigInt left, const BigInt& right) {
	if (left.neg) {
		if (right.neg) return -(-left + (-right));
		else return right - (-left);
	}
	else if (right.neg) return left - (-right);
	int carry = 0;
	for (size_t i = 0; i < std::max(left.val.size(), right.val.size()) || carry != 0; ++i) {
		if (i == left.val.size()) left.val.push_back(0);
		left.val[i] += carry + (i < right.val.size() ? right.val[i] : 0);
		carry = left.val[i] >= BigInt::BASE;
		if (carry != 0) left.val[i] -= BigInt::BASE;
	}

	return left;
}

// прибавляет к текущему числу новое
BigInt& BigInt::operator +=(const BigInt& value) {
	return *this = (*this + value);
}

// префиксный инкремент
const BigInt BigInt::operator++() {
	return (*this += 1);
}

// преобразует число к строке
BigInt::operator std::string() const {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

// преобразует signed char к BigInt
BigInt::BigInt(signed char c) {
	if (c < 0) this->neg = true;
	else this->neg = false;
	this->val.push_back(std::abs(c));
}

// преобразует unsigned char к BigInt
BigInt::BigInt(unsigned char c) {
	this->neg = false;
	this->val.push_back(c);
}

// преобразует signed short к BigInt
BigInt::BigInt(signed short s) {
	if (s < 0) this->neg = true;
	else this->neg = false;
	this->val.push_back(std::abs(s));
}

// преобразует unsigned short к BigInt
BigInt::BigInt(unsigned short s) {
	this->neg = false;
	this->val.push_back(s);
}

// преобразует signed int к BigInt
BigInt::BigInt(signed int i) {
	if (i < 0) this->neg = true;
	else this->neg = false;
	this->val.push_back(std::abs(i) % BigInt::BASE);
	i /= BigInt::BASE;
	if (i != 0) this->val.push_back(std::abs(i));
}

// преобразует unsigned int к BigInt
BigInt::BigInt(unsigned int i) {
	this->val.push_back(i % BigInt::BASE);
	i /= BigInt::BASE;
	if (i != 0) this->val.push_back(i);
}

// преобразует signed long к BigInt
BigInt::BigInt(signed long l) {
	if (l < 0) this->neg = true;
	else this->neg = false;
	this->val.push_back(std::abs(l) % BigInt::BASE);
	l /= BigInt::BASE;
	if (l != 0) this->val.push_back(std::abs(l));
}

// преобразует unsigned long к BigInt
BigInt::BigInt(unsigned long l) {
	this->val.push_back(l % BigInt::BASE);
	l /= BigInt::BASE;
	if (l != 0) this->val.push_back(l);
}

// преобразует signed long long к BigInt
BigInt::BigInt(signed long long l) {
	if (l < 0) { this->neg = true; l = -l; }
	else this->neg = false;
	do {
		this->val.push_back(l % BigInt::BASE);
		l /= BigInt::BASE;
	} while (l != 0);
}

// преобразует unsigned long long к BigInt
BigInt::BigInt(unsigned long long l) {
	this->neg = false;
	do {
		this->val.push_back(l % BigInt::BASE);
		l /= BigInt::BASE;
	} while (l != 0);
}

// постфиксный инкремент
const BigInt BigInt::operator ++(int) {
	*this += 1;
	return *this - 1;
}

// префиксный декремент
const BigInt BigInt::operator --() {
	return *this -= 1;
}

// постфиксный декремент
const BigInt BigInt::operator --(int) {
	*this -= 1;
	return *this + 1;
}

// вычитает два числа
const BigInt operator -(BigInt left, const BigInt& right) {
	if (right.neg) return left + (-right);
	else if (left.neg) return -(-left + right);
	else if (left < right) return -(right - left);
	int carry = 0;
	for (size_t i = 0; i < right.val.size() || carry != 0; ++i) {
		left.val[i] -= carry + (i < right.val.size() ? right.val[i] : 0);
		carry = left.val[i] < 0;
		if (carry != 0) left.val[i] += BigInt::BASE;
	}

	left._remove_leading_zeros();
	return left;
}

// вычитает из текущего числа новое
BigInt& BigInt::operator -=(const BigInt& value) {
	return *this = (*this - value);
}

// перемножает два числа
const BigInt operator *(const BigInt& left, const BigInt& right) {
	BigInt result;
	result.val.resize(left.val.size() + right.val.size());
	for (size_t i = 0; i < left.val.size(); ++i) {
		int carry = 0;
		for (size_t j = 0; j < right.val.size() || carry != 0; ++j) {
			long long cur = result.val[i + j] +
				left.val[i] * 1LL * (j < right.val.size() ? right.val[j] : 0) + carry;
			result.val[i + j] = static_cast<int>(cur % BigInt::BASE);
			carry = static_cast<int>(cur / BigInt::BASE);
		}
	}

	result.neg = left.neg != right.neg;
	result._remove_leading_zeros();
	return result;
}

// домножает текущее число на указанное
BigInt& BigInt::operator *=(const BigInt& value) {
	return *this = (*this * value);
}

// сдвигает все разряды на 1 вправо (домножает на BASE)
void BigInt::_shift_right() {
	if (this->val.size() == 0) {
		this->val.push_back(0);
		return;
	}
	this->val.push_back(this->val[this->val.size() - 1]);
	for (size_t i = this->val.size() - 2; i > 0; --i) this->val[i] = this->val[i - 1];
	this->val[0] = 0;
}

// делит два числа
const BigInt operator /(const BigInt& left, const BigInt& right) {
	if (right == 0) throw BigInt::divide_by_zero();
	BigInt b = right;
	b.neg = false;
	BigInt result, current;
	result.val.resize(left.val.size());
	for (long long i = static_cast<long long>(left.val.size()) - 1; i >= 0; --i) {
		current._shift_right();
		current.val[0] = left.val[i];
		current._remove_leading_zeros();
		int x = 0, l = 0, r = BigInt::BASE;
		while (l <= r) {
			int m = (l + r) / 2;
			BigInt t = b * m;
			if (t <= current) {
				x = m;
				l = m + 1;
			}
			else r = m - 1;
		}

		result.val[i] = x;
		current = current - b * x;
	}

	result.neg = left.neg != right.neg;
	result._remove_leading_zeros();
	return result;
}

// делит текущее число на указанное
BigInt& BigInt::operator /=(const BigInt& value) {
	return *this = (*this / value);
}

// возвращает остаток от деления двух чисел
const BigInt operator %(const BigInt& left, const BigInt& right) {
	BigInt result = left - (left / right) * right;
	if (result.neg) result += right;
	return result;
}

// присваивает текущему числу остаток от деления на другое число
BigInt& BigInt::operator %=(const BigInt& value) {
	return *this = (*this % value);
}

// проверяет, является ли текущее число нечетным
bool BigInt::odd() const {
	if (this->val.size() == 0) return false;
	return this->val[0] & 1;
}

// проверяет, является ли текущее число четным
bool BigInt::even() const {
	return !this->odd();
}

// возводит текущее число в указанную степень
const BigInt BigInt::pow(BigInt n) const {
	BigInt a(*this), result(1);
	while (n != 0) {
		if (n.odd()) result *= a;
		a *= a;
		n /= 2;
	}

	return result;
}