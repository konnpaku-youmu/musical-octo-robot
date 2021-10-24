/*
 * montgomery.c
 *
 */

#include "montgomery.h"
#include "common.h"

static inline void carry_addition(uint32_t *t, uint32_t i, uint64_t C)
{
        uint64_t sum = 0;

        while (C != 0)
        {
                sum = (uint64_t)t[i] + (uint64_t)C;
                C = sum >> 32;
                t[i] = sum & 0x00000000FFFFFFFF;
                i++;
        }

        return;
}

void conditional_sub(uint32_t *u, uint32_t *n, uint32_t *res, uint32_t size)
{
        uint32_t t[size + 1];
        int64_t B = 0;
        int64_t sub;

        for (int i = 0; i <= size; i++)
        {
                sub = (int64_t)u[i] - (int64_t)n[i] - B;

                if (sub >= 0)
                {
                        B = 0;
                }
                else
                {
                        B = 1;
                }
                t[i] = sub & 0x00000000FFFFFFFF;
        }

        if (B == 0)
        {
                memcpy(res, t, size * sizeof(uint32_t));
        }
        else
        {
                memcpy(res, u, size * sizeof(uint32_t));
        }
}

// Calculates res = a * b * r^(-1) mod n.
// a, b, n, n_prime represent operands of size elements
// res has (size+1) elements
void montMul(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *n_prime,
             uint32_t *res, uint32_t size)
{
        // initialize t and m
        uint32_t t[3] = {0, 0, 0};
        uint32_t m[size + 1];
        uint32_t n_ex[size + 1]; // size == 33 is needed in conditional subtraction

        memset(m, 0, (size + 1) * sizeof(uint32_t));
        memset(n_ex, 0, (size + 1) * sizeof(uint32_t));
        memcpy(n_ex, n, size * sizeof(uint32_t));

        uint64_t sum;
        uint64_t C = 0, S = 0;

        for (int i = 0; i < size; i++)
        {
                for (int j = 0; j < i; j++)
                {
                        sum = t[0] + (uint64_t)(a[j]) * (uint64_t)(b[i - j]);
                        C = sum >> 32;
                        S = sum & 0x00000000FFFFFFFF;
                        carry_addition(t, 1, C);
                        sum = S + (uint64_t)(m[j]) * (uint64_t)(n_ex[i - j]);
                        C = sum >> 32;
                        S = sum & 0x00000000FFFFFFFF;
                        t[0] = S;
                        carry_addition(t, 1, C);
                }
                sum = t[0] + (uint64_t)(a[i]) * (uint64_t)(b[0]);
                C = sum >> 32;
                S = sum & 0x00000000FFFFFFFF;
                carry_addition(t, 1, C);
                m[i] = (S * n_prime[0]) & 0x00000000FFFFFFFF;
                sum = S + (uint64_t)(m[i]) * (uint64_t)(n_ex[0]);
                C = sum >> 32;
                S = sum & 0x00000000FFFFFFFF;
                carry_addition(t, 1, C);
                t[0] = t[1];
                t[1] = t[2];
                t[2] = 0;
        }

        for (int i = size; i < 2 * size; i++)
        {
                for (int j = i - size + 1; j < size; j++)
                {
                        sum = (uint64_t)t[0] + (uint64_t)a[j] * (uint64_t)b[i - j];
                        C = sum >> 32;
                        S = sum & 0x00000000FFFFFFFF;
                        carry_addition(t, 1, C);
                        sum = (uint64_t)S + (uint64_t)m[j] * (uint64_t)n_ex[i - j];
                        C = sum >> 32;
                        S = sum & 0x00000000FFFFFFFF;
                        t[0] = S;
                        carry_addition(t, 1, C);
                }
                m[i - size] = t[0];
                t[0] = t[1];
                t[1] = t[2];
                t[2] = 0;
        }

        m[size] = t[0];
        n_ex[size] = 0;

        conditional_sub(m, n_ex, res, size);

        return;
}

void montMulOpt(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *n_prime,
             uint32_t *res, uint32_t size)
{
        // initialize t and m
        uint32_t t[3] = {0, 0, 0};
        uint32_t m[size + 1];
        uint32_t n_ex[size + 1]; // size == 33 is needed in conditional subtraction

        memset(m, 0, (size + 1) * sizeof(uint32_t));
        memset(n_ex, 0, (size + 1) * sizeof(uint32_t));
        memcpy(n_ex, n, size * sizeof(uint32_t));

        uint64_t sum;
        uint64_t C = 0, S = 0;

        for (int i = 0; i < size; i++)
        {
        		START_TIMING
                for (int j = 0; j < i; j++)
                {
                        sum = t[0] + (uint64_t)(a[j]) * (uint64_t)(b[i - j]);
                        C = sum >> 32;
                        S = sum & 0x00000000FFFFFFFF;
                        carry_addition(t, 1, C);
                        sum = S + (uint64_t)(m[j]) * (uint64_t)(n_ex[i - j]);
                        C = sum >> 32;
                        S = sum & 0x00000000FFFFFFFF;
                        t[0] = S;
                        carry_addition(t, 1, C);
                }
        		STOP_TIMING
                sum = t[0] + (uint64_t)(a[i]) * (uint64_t)(b[0]);
                C = sum >> 32;
                S = sum & 0x00000000FFFFFFFF;
                carry_addition(t, 1, C);
                m[i] = (S * n_prime[0]) & 0x00000000FFFFFFFF;
                sum = S + (uint64_t)(m[i]) * (uint64_t)(n_ex[0]);
                C = sum >> 32;
                S = sum & 0x00000000FFFFFFFF;
                carry_addition(t, 1, C);
                t[0] = t[1];
                t[1] = t[2];
                t[2] = 0;
        }

		START_TIMING
        for (int i = size; i < 2 * size; i++)
        {
                for (int j = i - size + 1; j < size; j++)
                {
                        sum = (uint64_t)t[0] + (uint64_t)a[j] * (uint64_t)b[i - j];
                        C = sum >> 32;
                        S = sum & 0x00000000FFFFFFFF;
                        carry_addition(t, 1, C);
                        sum = (uint64_t)S + (uint64_t)m[j] * (uint64_t)n_ex[i - j];
                        C = sum >> 32;
                        S = sum & 0x00000000FFFFFFFF;
                        t[0] = S;
                        carry_addition(t, 1, C);
                }
                m[i - size] = t[0];
                t[0] = t[1];
                t[1] = t[2];
                t[2] = 0;
        }
        STOP_TIMING

        m[size] = t[0];
        n_ex[size] = 0;

        conditional_sub(m, n_ex, res, size);

        return;
}