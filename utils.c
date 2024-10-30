#include "utils.h"
#include "params.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*************************************************
* 4.2.1 CONVERSION AND COMPRESSION ALGORITHMS 
*                  (FIPS 203)
**************************************************/

/*************************************************
* Name:        bits_to_bytes
*
* Description: Converts a bit array (of a length that is a multiple of eight) 
*              into an array of bytes.
*
* Arguments:   - const uint8_t *bits: pointer to input array of bits
*              - size_t bits_len: length of the array of bits
*              - uint8_t *bytes: pointer to output array of bytes
**************************************************/

void bits_to_bytes(const uint8_t *bits, size_t bits_len, uint8_t *bytes) {
    size_t bytes_len = bits_len / 8;
    memset(bytes, 0, bytes_len);

    for (size_t i = 0; i < bits_len; i++) {
        bytes[i / 8] += bits[i] << (i % 8);
    }
}

/*************************************************
* Name:        bytes_to_bits
*
* Description: Performs the inverse of BitsToBytes, converting a byte array into a bit array.
*
* Arguments:   - const uint8_t *bytes: pointer to input array of bytes
*              - size_t bytes_len: length of the array of bytes
*              - uint8_t *bits: pointer to output array of bits
**************************************************/

void bytes_to_bits(const uint8_t *bytes, size_t bytes_len, uint8_t *bits) {
    for (size_t i = 0; i < bytes_len; i++) {
        for (size_t j = 0; j < 8; j++) {
            bits[8*i + j] =  (bytes[i] >> j) & 1;
        }
    }
}

/*************************************************
* Name:        compress
*
* Description: compress an integer modulo q to an integer modulo 2^d
*
* Arguments:   - size_t d: desired compression option (2^d)
*              - uint16_t x: integer modulo q to be compress
**************************************************/

uint16_t compress(size_t d, uint16_t x) {
    uint16_t two_pow_d = 1 << d; // 2^d
    return ((x * two_pow_d + (MLKEM_Q/2)) / MLKEM_Q) % two_pow_d; 
}

/*************************************************
* Name:        decompress
*
* Description: decompress an integer modulo 2^d to an integer modulo q
*
* Arguments:   - size_t d: desired compression option (2^d)
*              - uint16_t y: integer modulo 2^d to be compress
**************************************************/

uint16_t decompress(size_t d, uint16_t y) {
    uint16_t two_pow_d = 1 << d; // 2^d
    return ((y * MLKEM_Q + (two_pow_d >> 1)) / two_pow_d) % MLKEM_Q;
}

/*************************************************
* Name:        encode
*
* Description: Encodes an array of d-bit integers into a byte array 
*              for 1 ≤ 𝑑 ≤ 12
*
* Arguments:   - const uint16_t *F: pointer to input array of d-bit integers
*              - size_t d: size of d-bit integers
*              - uint8_t *B: pointer to output array of bytes
**************************************************/

void encode(const uint16_t *F, size_t d, uint8_t *B) {
    uint8_t *b = malloc(256 * d * sizeof(uint8_t));
    
    for (size_t i=0; i<256; i++){
        uint16_t a = F[i];
        for (size_t j=0; j<d; j++){
            b[i*d + j] = a & 1;
            a = a >> 1; 
        }
    }

    bits_to_bytes(b, 256*d, B);
    free(b);
}

/*************************************************
* Name:        decode
*
* Description: Decodes a byte array into an array of 𝑑-bit integers
*              for 1 ≤ 𝑑 ≤ 12
*
* Arguments:   - const uint8_t *B: pointer to input array of bytes
*              - size_t d: size of the input array
*              - uint16_t *F: pointer to output array of d-bits integers
**************************************************/

void decode(const uint8_t *B, size_t d, uint16_t *F) {
    uint8_t *b = malloc(8 * 32 * d * sizeof(uint8_t));
    bytes_to_bits(B, d, b);

    uint16_t m = (d < 12) ? (1 << d) : MLKEM_Q; // 2^d if d<12 or q if d==12

    for(size_t i=0; i<256; i++){
        uint16_t sum = 0;
        for(size_t j=0; j<d; j++){
            sum += b[i*d + j] * (1 << j);
        }
        F[i] = sum % m; 
    }

    free(b);
}