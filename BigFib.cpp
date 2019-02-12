#include "BigNum.h"
#include <time.h>

BigNum getFib(int num);

int main(int argc, char *argv[]) {
	if (argc > 1) {
		clock_t start = clock();
		int i = atoi(argv[1]);

		getFib(i).print();

		clock_t end = clock();
		std::cout << "Time to complete: " << (double)(end - start) / CLOCKS_PER_SEC << " s" << std::endl;
	}
	else {
		std::cout << "Enter the Fibonacci to generate as an argument. Aborting..." << std::endl;
	}
	return 0;
}

BigNum getFib(int num) {
	BigNum a(1);
	BigNum b(1);
	BigNum temp(0);

	int i;
	for (i = 0; i < num - 2; i++) {
		temp = a + b;
		b = a;
		a = temp;
	}
	return a;
}