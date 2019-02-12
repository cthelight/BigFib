#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <ostream>

#define MAX_NUM 1000000000000000000L
#define MAX_NUM_MULT 1000000000L
#define WIDTH_OF_OUTPUT 18

long *doubleAndCopy(long * toCopy, int size);
long *genMultArr(long *arr, int used);

class BigNum
{
public:
	BigNum();
	BigNum(const BigNum &b);
	BigNum(const int &val);
	BigNum(const long &val);
	~BigNum();

	BigNum operator+(const BigNum &b);
	BigNum operator=(const BigNum &b);
	BigNum operator*(const BigNum &b);
	BigNum operator-(const BigNum &b);
	BigNum operator/(const BigNum &b);
	BigNum operator%(const BigNum &b);
	BigNum operator++(int);
	BigNum operator--(int);
	BigNum operator++();
	BigNum operator--();

	BigNum operator+=(const BigNum &b);
	BigNum operator-=(const BigNum &b);

	bool operator>(const BigNum &b);
	bool operator<(const BigNum &b);
	bool operator==(const BigNum &b);
	bool operator <=(const BigNum &b);
	bool operator >=(const BigNum &b);
	bool operator !=(const BigNum &b);


	void print();

	friend BigNum operator+(const int &a, const BigNum &b);
	friend BigNum operator+(const long &a, const BigNum &b);
	friend BigNum operator-(const int &a, const BigNum &b);
	friend BigNum operator-(const long &a, const BigNum &b);
	friend BigNum operator*(const int &a, const BigNum &b);
	friend BigNum operator*(const long &a, const BigNum &b);
	friend BigNum operator/(const int &a, const BigNum &b);
	friend BigNum operator/(const long &a, const BigNum &b);
	friend BigNum operator%(const int &a, const BigNum &b);
	friend BigNum operator%(const long &a, const BigNum &b);


	friend std::ostream &operator<<(std::ostream &os, const BigNum &b);

private:
	long *valArr; //Little Endian!!!
	int size;
	int numUsed;
	bool isPos;
	BigNum(long *vals, int digits);

	BigNum add(const BigNum &a, const BigNum &b); // a - b
	BigNum subtract(const BigNum &a, const BigNum &b);
	BigNum multiply(const BigNum &a, const BigNum &b);
	BigNum divide(const BigNum &a, const BigNum &b);
	int compare(const BigNum &a, const BigNum &b);
};

BigNum::BigNum()
{
	valArr = (long *)malloc(sizeof(long));
	valArr[0] = 0;
	size = 1;
	numUsed = 1;
	isPos = true;

}
BigNum::BigNum(const int &val)
{
	if (val >= 0) {
		valArr = (long *)malloc(sizeof(long));
		valArr[0] = (long)val;
		size = 1;
		numUsed = 1;
		isPos = true;
	}
	else {
		valArr = (long *)malloc(sizeof(long));
		valArr[0] = (long)(-1 * val);
		size = 1;
		numUsed = 1;
		isPos = false;

	}


}


BigNum::BigNum(const long &val)
{
	if (val >= 0) {
		valArr = (long *)malloc(sizeof(long));
		valArr[0] = val;
		size = 1;
		numUsed = 1;
		isPos = true;
	}
	else {
		valArr = (long *)malloc(sizeof(long));
		valArr[0] = (-1 * val);
		size = 1;
		numUsed = 1;
		isPos = false;

	}

}
BigNum::BigNum(const BigNum &b)
{
	valArr = (long *)malloc(sizeof(long) * b.size);
	int i;
	for (i = 0; i < b.numUsed; i++) {
		valArr[i] = b.valArr[i];
	}
	size = b.size;
	numUsed = b.numUsed;
	isPos = b.isPos;

}

BigNum::BigNum(long *vals, int digits) {
	valArr = (long *)malloc(digits * sizeof(long));
	int i;
	for (i = 0; i < digits; i++) {
		valArr[i] = vals[i];
	}
	size = digits;
	numUsed = digits;
	isPos = true;

}


BigNum::~BigNum()
{
	free(valArr);
}


BigNum BigNum::add(const BigNum &a, const BigNum &b) {
	BigNum toReturn;
	free(toReturn.valArr);
	int maxSize = (b.size > a.size) ? b.size : a.size;
	toReturn.valArr = (long *)malloc(sizeof(long) * maxSize);
	toReturn.size = maxSize;
	toReturn.numUsed = b.numUsed;
	toReturn.isPos = b.isPos;
	bool carryOut = false;
	int i;
	int minNumUsed = (b.numUsed < a.numUsed) ? b.numUsed : a.numUsed;
	//std::cout << maxNumUsed << std::endl;
	for (i = 0; i < minNumUsed; ++i) {
		toReturn.valArr[i] = b.valArr[i] + a.valArr[i];

		if (carryOut) {
			++toReturn.valArr[i];
			carryOut = false;

		}

		if (toReturn.valArr[i] > MAX_NUM) {
			carryOut = true;
			toReturn.valArr[i] -= MAX_NUM;
		}

		//toReturn.print();
	}

	for (i = minNumUsed; i < a.numUsed; ++i) {
		if (i >= toReturn.size) {
			toReturn.valArr = doubleAndCopy(toReturn.valArr, toReturn.size);
			toReturn.size = 2 * toReturn.size;
		}
		++toReturn.numUsed;
		toReturn.valArr[i] = a.valArr[i];
		if (carryOut) {
			++toReturn.valArr[i];
			carryOut = false;

		}

		if (toReturn.valArr[i] > MAX_NUM) {
			carryOut = true;
			toReturn.valArr[i] -= MAX_NUM;
		}
	}
	if (carryOut) {
		++toReturn.numUsed;
		if (toReturn.numUsed > toReturn.size) {
			toReturn.valArr = doubleAndCopy(toReturn.valArr, toReturn.size);
			toReturn.size = 2 * toReturn.size;
		}
		toReturn.valArr[toReturn.numUsed - 1] = 1;
	}
	return toReturn;
}


BigNum BigNum::operator+(const BigNum &b) {
	BigNum &ths = *this;
	BigNum toReturn;
	if (isPos && b.isPos) {
		toReturn = add(ths, b);
		toReturn.isPos = true;
	}
	else if (!isPos && !b.isPos) {
		toReturn = add(ths, b);
		toReturn.isPos = false;
	}
	else if (isPos && !b.isPos) {
		BigNum pb(b);
		pb.isPos = true;
		toReturn = ths - pb;
	}
	else if (!isPos && b.isPos) {
		BigNum pb(ths);
		pb.isPos = true;
		toReturn = pb - ths;
	}

	return toReturn;

}
BigNum operator+(const int &a, const BigNum &b) {
	BigNum ba(a);
	return ba + b;
}
BigNum operator+(const long &a, const BigNum &b) {
	BigNum ba(a);
	return ba + b;
}



BigNum BigNum::operator=(const BigNum &b) {
	//BigNum toReturn(b);
	isPos = b.isPos;
	numUsed = b.numUsed;
	size = b.size;
	free(valArr);
	valArr = (long *)malloc(sizeof(long) * b.size);
	int i;
	for (i = 0; i < b.size; ++i) {
		valArr[i] = b.valArr[i];
	}
	//print();
	return *this;
}



void BigNum::print() {

	int i;
	if (!isPos) {
		std::cout << '-';
	}

	std::cout << valArr[numUsed - 1];
	for (i = numUsed - 2; i >= 0; i--) {
		std::cout << std::setfill('0') << std::setw(WIDTH_OF_OUTPUT) << valArr[i];
	}
	std::cout << std::endl;
}

std::ostream &operator<<(std::ostream &os, const BigNum &b) {
	int i;
	if (!b.isPos) {
		os << '-';
	}

	os << b.valArr[b.numUsed - 1];
	for (i = b.numUsed - 2; i >= 0; i--) {
		os << std::setfill('0') << std::setw(WIDTH_OF_OUTPUT) << b.valArr[i];
	}
	return os;
}



long *doubleAndCopy(long * toCopy, int size) {
	long *newCopy = (long*)malloc(sizeof(long) * size * 2);
	int i;
	for (i = 0; i < size; i++) {
		newCopy[i] = toCopy[i];
	}

	free(toCopy);

	return newCopy;
}


BigNum BigNum::multiply(const BigNum &a, const BigNum &b) {
	BigNum toReturn;
	free(toReturn.valArr);

	long *mValArrPsd = genMultArr(b.valArr, b.numUsed);
	long *mValArrThs = genMultArr(a.valArr, a.numUsed);
	int newSize = b.numUsed + a.numUsed + 1;
	long *storeMult = (long*)malloc(2 * newSize * sizeof(long));
	int i, j;


	for (i = 0; i < 2 * newSize; i++) {
		storeMult[i] = 0;
	}


	for (i = 0; i < 2 * b.numUsed; i++) {
		for (j = 0; j < 2 * a.numUsed; j++) {
			storeMult[i + j] += (mValArrPsd[i] * mValArrThs[j]) % MAX_NUM_MULT;
			storeMult[i + j + 1] += (mValArrPsd[i] * mValArrThs[j]) / MAX_NUM_MULT;
		}
	}
	int count = 0;
	for (i = 0; i < newSize * 2; i++) {
		if (storeMult[i] >= MAX_NUM_MULT) {
			int temp = storeMult[i] / MAX_NUM_MULT;
			storeMult[i + 1] += temp;
			storeMult[i] -= (temp * MAX_NUM_MULT);
		}

	}
	//count = newSize * 2;

	toReturn.valArr = (long*)malloc(newSize * sizeof(long));

	toReturn.size = newSize;

	for (i = 0; i < newSize; i++) {
		toReturn.valArr[i] = storeMult[2 * i] + MAX_NUM_MULT * storeMult[2 * i + 1];
		if (toReturn.valArr[i] != 0) {
			count = i;
		}
	}
	toReturn.numUsed = count + 1;
	//toReturn.print();

	free(mValArrThs);
	free(mValArrPsd);
	free(storeMult);
	return toReturn;
}

BigNum BigNum::operator*(const BigNum &b) {
	BigNum &ths = *this;
	BigNum toReturn = multiply(ths, b);
	if ((ths.isPos && b.isPos) || (!ths.isPos && !b.isPos)) {
		toReturn.isPos = true;
	}
	else {
		toReturn.isPos = false;
	}

	return toReturn;

}


long *genMultArr(long *arr, int used) {
	int i;
	int newSize = used * 2;
	long *mValArr = (long*)malloc(newSize * sizeof(long));
	for (i = 0; i < used; i++) {
		mValArr[2 * i] = arr[i] % MAX_NUM_MULT;
		mValArr[2 * i + 1] = arr[i] / MAX_NUM_MULT;
	}

	return mValArr;
}









BigNum BigNum::subtract(const BigNum &a, const BigNum &b) {
	BigNum toReturn;
	free(toReturn.valArr);
	int maxNumUsed = (b.numUsed > a.numUsed) ? b.numUsed : a.numUsed;
	toReturn.valArr = (long *)malloc(sizeof(long) * (maxNumUsed + 1));
	toReturn.size = maxNumUsed + 1;
	toReturn.numUsed = maxNumUsed;

	int minNumUsed = (b.numUsed < a.numUsed) ? b.numUsed : a.numUsed;

	int i;
	toReturn.valArr[maxNumUsed] = 0;


	for (i = 0; i < minNumUsed; i++) {
		toReturn.valArr[i] = a.valArr[i] - b.valArr[i];
	}
	for (i = minNumUsed; i < numUsed; i++) {
		toReturn.valArr[i] = valArr[i];
	}
	for (i = minNumUsed; i < b.numUsed; i++) {
		toReturn.valArr[i] = -1 * b.valArr[i];
	}

	for (i = 0; i < maxNumUsed - 1; i++) {
		if (toReturn.valArr[i] < 0) {
			toReturn.valArr[i + 1] --;
			toReturn.valArr[i] += MAX_NUM;
		}
	}



	while (maxNumUsed > 0 && toReturn.valArr[maxNumUsed] == 0) {
		maxNumUsed--;
	}

	toReturn.numUsed = maxNumUsed + 1;

	return toReturn;
}





BigNum BigNum::operator-(const BigNum &b) {
	BigNum &ths = *this;
	BigNum toReturn;
	if (isPos && b.isPos) {
		if (ths > b) {
			toReturn = subtract(ths, b);
			toReturn.isPos = true;
		}
		else if (ths < b) {
			toReturn = subtract(b, ths);
			toReturn.isPos = false;
		}
	}
	else if(!isPos && b.isPos)	{
		BigNum pos(*this);
		pos.isPos = true;
		toReturn = pos + b;
		toReturn.isPos = false;
	}
	else if (isPos && !b.isPos) {
		BigNum pos(b);
		pos.isPos = true;
		toReturn = pos + b;
		toReturn.isPos = true;
	} else if(!isPos && !b.isPos) { 
		BigNum thsPos(ths);
		BigNum bPos(b);
		if (thsPos > bPos) {
			toReturn = subtract(thsPos, bPos);
			toReturn.isPos = false;
		}
		else if (thsPos < bPos) {
			toReturn = subtract(bPos, thsPos);
			toReturn.isPos = true;
		}
	}
	return toReturn;
}

BigNum BigNum::divide(const BigNum &a, const BigNum &b) {
	

	BigNum toReturn;
	BigNum posa(a);
	posa.isPos = true;
	BigNum posb(b);
	posb.isPos = true;
	if(posb == 0){
		throw("Divide By Zero Error.");
	}
	free(toReturn.valArr);
	int newSize = (posa.numUsed - posb.numUsed + 1);
	toReturn.size = newSize;
	toReturn.numUsed = newSize;
	toReturn.valArr = (long *)malloc((newSize + 1) * sizeof(long));

	int i;

	long j;
	for (i = 0; i < newSize + 1; i++) {
		toReturn.valArr[i] = 0;
	}


	for (i = newSize - 1; i >= 0; i--) {
		for (j = MAX_NUM / 10; j > 0; j = j / 10) {
			while (toReturn * posb <= posa) {
				toReturn.valArr[i] += j;
			}
			toReturn.valArr[i] -= j;
		}
	}
	//(toReturn * b).print();
	//std::cout << (toReturn * posb).valArr[posa.numUsed - 1] << std::endl;
	while (toReturn.valArr[toReturn.numUsed - 1] == 0) {
		toReturn.numUsed--;
	}

	return toReturn;
}



BigNum BigNum::operator/(const BigNum &b) {
	BigNum toReturn = divide(*this, b);
	if (isPos == b.isPos) {
		toReturn.isPos = true;
	}
	else {
		toReturn.isPos = false;
	}
	return toReturn;

}


BigNum operator-(const int &a, const BigNum &b) {
	BigNum bna(a);
	return bna - b;
}
BigNum operator-(const long &a, const BigNum &b) {
	BigNum bna(a);
	return bna - b;
}
BigNum operator*(const int &a, const BigNum &b) {
	BigNum bna(a);
	return bna * b;
}
BigNum operator*(const long &a, const BigNum &b) {
	BigNum bna(a);
	return bna * b;
}
BigNum operator/(const int &a, const BigNum &b) {
	BigNum bna(a);
	return bna / b;
}
BigNum operator/(const long &a, const BigNum &b) {
	BigNum bna(a);
	return bna / b;
}

BigNum operator%(const int &a, const BigNum &b) {
	BigNum bna(a);
	return bna % b;
}
BigNum operator%(const long &a, const BigNum &b) {
	BigNum bna(a);
	return bna % b;
}


BigNum BigNum::operator++(int) {
	BigNum cpy(*this);
	*this = cpy + 1;
	return cpy;
}
BigNum BigNum::operator--(int) {
	BigNum cpy(*this);
	*this = cpy - 1;
	return cpy;
}
BigNum BigNum::operator++() {
	BigNum cpy(*this);
	*this = cpy + 1;
	return *this;
}
BigNum BigNum::operator--() {
	BigNum cpy(*this);
	*this = cpy - 1;
	return *this;
}


BigNum BigNum::operator+=(const BigNum &b) {
	BigNum cpy(*this);
	
	*this = cpy + b;
	return *this;
}
BigNum BigNum::operator-=(const BigNum &b) {
	BigNum cpy(*this);

	*this = cpy - b;
	return *this;
}


//compares IGNORING SIGN!!! -1 if a < b;
int BigNum::compare(const BigNum &a, const BigNum &b) {
	if (a.numUsed < b.numUsed) {
		return -1;
	}
	else if (a.numUsed > b.numUsed) {
		return 1;
	}
	else {
		int i;
		for (i = a.numUsed - 1; i >= 0; i--) {
			if (a.valArr[i] < b.valArr[i]) {
				return -1;
			}
			else if (a.valArr[i] > b.valArr[i]) {
				return 1;
			}
		}
	}

	return 0;
}

BigNum BigNum::operator%(const BigNum &b) {

	return *this - ((*this / b) * b);

}






bool BigNum::operator>(const BigNum &b) {
	BigNum &ths = *this;
	int comp = compare(ths, b);
	if (comp > 0 && ths.isPos && b.isPos) {
		return true;
	}
	else if (comp < 0 && !ths.isPos && !b.isPos) {
		return true;
	}
	else if (ths.isPos && !b.isPos) {
		return true;
	}
	return false;
}
bool BigNum::operator<(const BigNum &b) {
	BigNum &ths = *this;
	int comp = compare(ths, b);
	if (comp < 0 && ths.isPos && b.isPos) {
		return true;
	}
	else if (comp > 0 && !ths.isPos && !b.isPos) {
		return true;
	}
	else if (!ths.isPos && b.isPos) {
		return true;
	}
	return false;
}
bool BigNum::operator <=(const BigNum &b) {
	return *this < b || *this == b;
}
bool BigNum::operator==(const BigNum &b) {
	BigNum &ths = *this;
	int comp = compare(ths, b);
	if (!comp && ths.isPos == b.isPos) {
		return true;
	}
	return false;
}
bool BigNum::operator >=(const BigNum &b) {
	return *this > b || *this == b;
}
bool BigNum::operator !=(const BigNum &b) {
	return !(*this == b);
}






#endif
