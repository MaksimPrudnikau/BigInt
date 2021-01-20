#pragma once

#include "BigInt.h"

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

bool isNumb(string input)
{
	try
	{
		//Минус не влияет на принадлежность к int
		if (input[0] == '-')
		{
			input.erase(0, 1);
		}

		//Проверка на наличие лишних нулей в начале числа
		if (input[0] == '0' && input.size() != 1)
		{
			throw "Error - 01234 unsupported";
		}

		//Проверка на вхождение в диапазон [0,1]
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

	for each (char i in buff)
	{
		val.push_back(i - '0');
	}
}

BigInt::BigInt(const string input) : BigInt(input.c_str()) {};

BigInt& BigInt::operator= (const char* input)
{
	string buff(input);
	assert(isNumb(buff) == true);

	val.clear(); //Очистка предыдущего значения

	if (buff[0] == '-')
	{
		neg = this->neg == true ? false : true;
		buff.erase(0, 1);
	}

	for each (char i in buff)
	{
		val.push_back(i - '0');
	}
	return *this;
}

BigInt& BigInt::operator= (string input)
{
	assert(isNumb(input) == true);

	val.clear(); //Очистка предыдущего значения

	if (input[0] == '-')
	{
		neg = this->neg == true ? false : true;
		input.erase(0, 1);
	}

	for each (char i in input)
	{
		val.push_back(i - '0');
	}
	return *this;
}