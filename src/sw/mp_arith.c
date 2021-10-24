/*
 * mp_arith.c
 *
 */

#include <stdint.h>

// Calculates res = a + b.
// a and b represent large integers stored in uint32_t arrays
// a and b are arrays of size elements, res has size+1 elements
void mp_add(uint32_t *a, uint32_t *b, uint32_t *res, uint32_t size) {
	uint32_t *inA = a;
	uint32_t *inB = b;
	uint32_t *outRes = res;
	uint64_t c = 0;
	for (int i = 0; i < size; i++) {
		uint64_t resTmp = ((uint64_t) *inA + (uint64_t) *inB + c);

		if (resTmp > 0xFFFFFFFF) {
			c = 1;
		} else {
			c = 0;
		}

		*outRes = (uint32_t) resTmp;

		inA++;
		inB++;
		outRes++;
	}
	*outRes = c;

	return;
}

// Calculates res = a - b.
// a and b represent large integers stored in uint32_t arrays
// a, b and res are arrays of size elements
void mp_sub(uint32_t *a, uint32_t *b, uint32_t *res, uint32_t size) {
	uint32_t *inA = a;
	uint32_t *inB = b;
	uint32_t *outRes = res;
	int64_t c = 0;
	for (int i = 0; i < size; i++) {

		int64_t resTmp = ((int64_t) *inA - (int64_t) *inB + c) % (0x100000000);

		if (resTmp >= 0) {
			c = 0;
		} else {
			c = -1;
		}
		*outRes = (uint32_t) (resTmp);

		inA++;
		inB++;
		outRes++;
	}
	*outRes = c;

	return;
}

// Calculates res = (a + b) mod NresTmp.
// a and b represent operands, N is the modulus. They are large integers stored in uint32_t arrays of size elements
void mod_add(uint32_t *a, uint32_t *b, uint32_t *N, uint32_t *res,
		uint32_t size) {
	uint32_t resTmp[33];
	mp_add(a, b, resTmp, 32);

	uint32_t ext_N[33];
	memcpy(ext_N, N, 128);
	ext_N[32] = 0x00;

	uint32_t comp[33];
	mp_sub(resTmp, ext_N, comp, 33);

	if ((comp[32] & 0x01) == 0) {
		memcpy(res, comp, 128);
	} else {
		memcpy(res, resTmp, 128);
	}

	return;
}

// Calculates res = (a - b) mod N.
// a and b represent operands, N is the modulus. They are large integers stored in uint32_t arrays of size elements
void mod_sub(uint32_t *a, uint32_t *b, uint32_t *N, uint32_t *res,
		uint32_t size) {

	uint32_t resTmp0[33];
	uint32_t resTmp1[33];
	uint32_t ext_N[33];
	uint32_t comp[33];

	memcpy(ext_N, N, 128);
	ext_N[32] = 0x00;

	mp_sub(a, b, resTmp0, 32);

	if (resTmp0[32] == 0) {
		// a >= b

		mp_sub(resTmp0, ext_N, comp, 33);

		if ((comp[32] & 0x01) == 0) {
			memcpy(res, comp, 128);
		} else {
			memcpy(res, resTmp0, 128);
		}

	} else {
		// a < b
		mp_sub(b, a, resTmp1, 32);
		memcpy(ext_N, N, 128);
		ext_N[32] = 0x00;
		mp_sub(resTmp1, ext_N, comp, 32);
		if ((comp[32] & 0x01) == 0) {
			//res > N
			mp_sub(ext_N, comp, res, 32);
		} else {
			mp_sub(ext_N, resTmp1, res, 32);
		}
	}
}


