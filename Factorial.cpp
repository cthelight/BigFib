#include "BigNum.h"
#include <time.h>

int main(int argc, char *argv[]) {
	if (argc > 1) {
		clock_t start = clock();
		int num = atoi(argv[1]);
		int i;
		BigNum total(1);
		for (i = 0; i < num; ++i) {
			total = total * (i + 1);
		}

		std::cout << total << std::endl;
		clock_t end = clock();
		std::cout << "Time to complete: " << (double)(end - start) / CLOCKS_PER_SEC << " s" << std::endl;
	}
	else {
		std::cout << "Enter numbet to find the factorial of as an argument. Aborting..." << std::endl;
	}
	return 0;
}