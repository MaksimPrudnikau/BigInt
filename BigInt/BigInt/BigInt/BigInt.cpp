#pragma once

#include "BigInt.h"

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>

using namespace std;

bool isNumb(string input)
{
	try
	{
		//����� �� ������ �� �������������� � int
		if (input[0] == '-')
		{
			input.erase(0, 1);
		}

		//�������� �� ������� ������ ����� � ������ �����
		if (input[0] == '0' && input.size() != 1)
		{
			throw "Error - 01234 unsupported";
		}

		//�������� �� ��������� � �������� [0,1]
		for (char i : input)
		{
			if ((int)i < 48 || (int)i > 57)
			{
				throw "Error - illegal symbols (not in [0,1]) detected";
			}
		}
	}
	catch (const char* message)
	{
		cout << message << "\n";
		return false;
	}
	return true;
}

BigInt::BigInt(const char* input) : neg(false)
{
	string buff(input);
	assert(isNumb(buff) == true);


	if (buff[0] == '-')
	{
		neg = true;
		buff.erase(0, 1);
	}

	reverse(buff.begin(), buff.end());

	for (char i : buff)
	{
		val.push_back(i - '0');
	}
}

BigInt::BigInt(const string input) : BigInt(input.c_str()) {};

BigInt::BigInt(long long input) : BigInt(to_string(input)) {};

BigInt& BigInt::operator= (const char* input)
{
	string buff(input);
	assert(isNumb(buff) == true);

	val.clear(); //������� ����������� ��������

	if (buff[0] == '-')
	{
		neg = this->neg == true ? false : true;
		buff.erase(0, 1);
	}

	reverse(buff.begin(), buff.end());
	for (char i : buff)
	{
		val.push_back(i - '0');
	}
	return *this;
}

BigInt& BigInt::operator= (string input)
{
	assert(isNumb(input) == true);

	val.clear(); //������� ����������� ��������

	if (input[0] == '-')
	{
		neg = this->neg == true ? false : true;
		input.erase(0, 1);
	}

	reverse(input.begin(), input.end());
	for (char i : input)
	{
		val.push_back(i - '0');
	}
	return *this;
}

BigInt::BigInt(const BigInt& input) : neg(input.neg)
{
	if (input.val != val)
	{
		val = input.val;
	}
}

const BigInt BigInt::operator+ () const
{
	return *this;
}

const BigInt BigInt::operator- () const
{
	BigInt copy = *this;
	copy.neg = !this->neg;
	return copy;
}

bool BigInt::operator== (const BigInt& right) const
{
	if (neg == right.neg)
	{
		if (val == right.val)
		{
			return true;
		}
	}
	return false;
}

bool BigInt::operator== (long long right) const
{
	return *this == BigInt(right) ? true : false;
}

bool BigInt::operator== (string right) const
{
	return *this == BigInt(right) ? true : false;
}

bool BigInt::operator== (const char* right) const
{
	return *this == BigInt(right) ? true : false;
}

bool BigInt::operator< (const BigInt& right) const
{
	//���� ����� �����
	if (neg == right.neg)
	{
		//���� ���������� ���� �����
		if (val.size() == right.val.size())
		{
			//���������� �����������
			for (size_t i = 0; i < val.size(); ++i)
			{
				if (val[i] > right.val[i])
				{
					return false;
				}
			}
			return true;
		}
		else if (val.size() > right.val.size())
		{
			return false;
		}
		else if (val.size() < right.val.size())
		{
			return true;
		}
	}
	//���� ����� ����� �������������, � ������ - �������������
	else if (neg == true && right.neg == false)
	{
		return true;
	}
	//���� ����� ����� �������������, � ������ - �������������
	else if (neg == false && right.neg == true)
	{
		return false;
	}
}

bool BigInt::operator!= (const BigInt& right) const
{
	return !(*this == right);
}

bool BigInt::operator<= (const BigInt& right) const
{
	return (*this < right || *this == right);
}

bool BigInt::operator> (const BigInt& right) const
{
	return !(*this <= right);
}

bool BigInt::operator>= (const BigInt& right) const
{
	return (*this > right || *this == right);
}


const BigInt operator+ (BigInt left, const BigInt& right)
{
	if (!left.neg && !right.neg) // x + y
	{
		int carry = 0; // ���� �������� �� ����������� �������
		for (size_t i = 0; i < max(left.val.size(), right.val.size()); ++i)
		{
			if (i >= left.val.size())
			{
				left.val.push_back(0);
			}
			left.val[i] += carry + (i < right.val.size() ? right.val[i] : 0);
			if (left.val[i] >= 10)
			{
				carry = left.val[i] / 10;
				left.val[i] -= 10;
			}
		}
		return left;
	}
	else if (!left.neg && right.neg) // x + (-y) == -(-x + y)
	{
		return left - (-right);
	}
	else if (left.neg && !right.neg)// -x + y = -(x + (-y))
	{
		return right - (-left);
	}
	else if (left.neg && right.neg) // -x + (-y) == -(x + y)
	{
		return -(-left + (-right));
	}
}


const BigInt operator- (BigInt left, const BigInt& right)
{
	if (!left.neg && !right.neg) // x - y
	{
		if (left < right)
		{
			return -(right - left);
		}

		int carry = 0;
		for (size_t i = 0; i < right.val.size() || carry != 0; ++i) 
		{
			left.val[i] -= carry + (i < right.val.size() ? right.val[i] : 0);
			carry = left.val[i] < 0;
			if (carry != 0)
			{
				left.val[i] += 10;
			}
		}
		return left;
	}
	else if (left.neg && !right.neg) // -x - y = -(x + y)
	{
		return -(-left + right);
	}
	else if (!left.neg && right.neg) // x - (-y) = x + y
	{
		return left + (-right);
	}
	else if (left.neg && right.neg) // -x - (-y) = -x + y
	{
		return -left - (-right);
	}
}

ostream& operator<< (ostream& out, const BigInt& input)
{
	if (!input.val.empty())
	{
		if (input.neg)
		{
			out << "-";
		}
		vector<int> temp = input.val;
		reverse(temp.begin(), temp.end());
		copy(temp.begin(), temp.end(), ostream_iterator<int>(out, ""));
	}
	return out;
}