#pragma once

#include <vector>
#include <string>

using namespace std;

class BigInt
{
private:
	bool neg;
	vector<int> val;
public:

	//Конструкторы BigInt x(to_string(...))
	BigInt() = default;
	BigInt(const char*);
	BigInt(const string);

	//Конструктор копирования запрещён
	BigInt(const BigInt&) = delete;

	//Операторы присваивания BigInt x = to_string(...)
	BigInt& operator= (const char*);
	BigInt& operator= (const string);

	friend ostream& operator<< (ostream& out, const BigInt&);
};

ostream& operator<< (ostream& out, const BigInt& input)
{
	for each (int i in input.val)
	{
		out << i;
	}
	return out;
}