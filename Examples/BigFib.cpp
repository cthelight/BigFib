#include "../BigNum.h"
#include <time.h>
#include <math.h>

BigNum getFib(int num);

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		clock_t start = clock();
		int i = atoi(argv[1]);

		getFib(i).print();

		clock_t end = clock();
		std::cout << "Time to complete: " << (double)(end - start) / CLOCKS_PER_SEC << " s" << std::endl;
	}
	else
	{
		std::cout << "Enter the Fibonacci to generate as an argument. Aborting..." << std::endl;
	}
	return 0;
}

BigNum getFib(int num)
{
	char *steps = (char *)malloc((int)ceil(log2(num) + 1) * sizeof(char));
	int cur = num;
	int count;
	for (count = 0; cur > 1; count++)
	{
		steps[count] = cur % 2;
		cur = cur / 2;
	}
	BigNum k_val = 1;
	BigNum k1_val = 1;
	BigNum k_val_n = 1;
	BigNum k1_val_n = 1;
	BigNum temp(0);

	int i;
	for (i = count - 1; i >= 0; i--)
	{
		k_val_n = k_val * ((2 * k1_val) - k_val);
		k1_val_n = (k1_val * k1_val) + (k_val * k_val);
		k_val = k_val_n;
		k1_val = k1_val_n;
		if (steps[i])
		{
			temp = k_val + k1_val;
			k_val = k1_val;
			k1_val = temp;
		}
	}
	free(steps);
	return k_val;
}
