#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	union {
		float f_val;
		uint32_t i_val;
	} u_val;

	if (argc != 2) {
		printf("usage: %s 42.7\n", argv[0]);
		return -1;
	}

	u_val.f_val = strtof(argv[1], NULL);

	printf("float=%f\n", u_val.f_val);
	printf("uint=0x%08x\n", u_val.i_val);

	uint32_t i_sign = (u_val.i_val & 0x80000000) >> 31;
	uint8_t i_exp = (u_val.i_val & 0x7F800000) >> 23;
	uint32_t i_frac = (u_val.i_val & 0x007FFFFF);

	printf("encoded:\n");
	printf("  sign=%u\n", i_sign);
	printf("  exp=%d\n", i_exp);
	printf("  frac=%u\n", i_frac);

	float f_frac = 1.0;
	float f_tmp = 0.5;
	for (int i = 22; i >= 0; i--) {
		int i_tmp = (i_frac >> i) & 0x01;
		if (i_tmp) {
			f_frac = f_frac + f_tmp;
		}
		f_tmp = f_tmp / 2;
	}

	printf("decoded:\n");
	printf("  exp=2^%d\n", i_exp - 127);
	printf("  frac=%f\n", f_frac);

	float f_result = powf(-1, i_sign) * f_frac * powf(2, i_exp - 127);
	uint32_t *p_result = (uint32_t *) &f_result;
	printf("result=%f (0x%08x)\n", f_result, *p_result);

	printf("equal? %s\n", f_result == u_val.f_val ? "y" : "n");

	return 0;
}

