/*
 * asm_func.h
 *
 *  Created on: May 13, 2016
 *      Author: dbozilov
 */

#ifndef ASM_FUNC_H_
#define ASM_FUNC_H_

#include <stdint.h>

void mov_CS(uint64_t sum, uint32_t* C, uint32_t* S);

// calculate a+b*c with umlal
uint64_t fast_mul(uint32_t a, uint32_t *b, uint32_t *c);

// calculate a+b*c with umlal
uint64_t fast_calc(uint32_t a, uint32_t *b, uint32_t *c, uint32_t *S, uint32_t *t1);

#endif /* ASM_FUNC_H_ */
