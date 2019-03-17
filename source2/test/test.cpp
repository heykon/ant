#include "../utilities.h"



int main(void)
{
	char b[16];

	int i = -450000;
	int i2 = 4000000;
	int_to_char_b(b, 2, i);
	int_to_char_b(b, 3, i2);

	int j = char_b_to_int(b, 2);
	int j2 = char_b_to_int(b, 3);
	printf("%d %d", j, j2);

	return 0;
}