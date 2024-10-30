#include <stdint.h>
#include <stddef.h>

#ifndef UTILS_H
#define UTILS_H

void bits_to_bytes(const uint8_t *bits, size_t bits_len, uint8_t *bytes);

void bytes_to_bits(const uint8_t *bytes, size_t bytes_len, uint8_t *bits);

uint16_t compress(size_t d, uint16_t x);

uint16_t decompress(size_t d, uint16_t y);

void encode(const uint16_t *F, size_t d, uint8_t *B);

void decode(const uint8_t *B, size_t d, uint16_t *F);

#endif