#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <iterator>

using namespace std;

class BigInt
{
	bool neg;
	vector<int> val;
public:

	//Конструкторы BigInt x(to_string(...))
	BigInt() = default;
	BigInt(const char*);
	BigInt(const string);
	BigInt(long long);

	//Конструктор копирования запрещён
	BigInt(const BigInt&);

	//Унарные операторы
	const BigInt operator+ () const;
	const BigInt operator- () const; // -(-x)
	//Операторы сравнения
	bool operator== (const BigInt&) const;
	bool operator== (long long) const;
	bool operator== (string) const;
	bool operator== (const char*) const;
	bool operator< (const BigInt&) const;
	bool operator> (const BigInt&) const;
	bool operator!= (const BigInt&) const;
	bool operator<= (const BigInt&) const;
	bool operator>= (const BigInt&) const;
	

	//Операторы присваивания BigInt x = to_string(...)
	BigInt& operator= (const char*);
	BigInt& operator= (const string);

	//Бинарные операторы
	friend const BigInt operator+ (BigInt, const BigInt&);
	friend const BigInt operator- (BigInt, const BigInt&);

	friend ostream& operator<< (ostream&, const BigInt&);
};