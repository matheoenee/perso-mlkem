#include <stdint.h>

#ifndef SAMPLE_H
#define SAMPLE_H

void sample_ntt(const uint8_t *seed, uint8_t i, uint8_t j, uint16_t *a);

void sample_poly_cbd(const uint8_t *seed, uint8_t eta, uint16_t *f);

#endif