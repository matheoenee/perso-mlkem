#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "params.h"

#define BITS_LEN 16   // Test length for bit array
#define BYTES_LEN (BITS_LEN / 8)
#define ARRAY_LEN 256 // Test length for F array

int main() {
    // Test for bits_to_bytes and bytes_to_bits
    printf("Testing bits_to_bytes and bytes_to_bits...\n");

    uint8_t bits[BITS_LEN] = {1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0};
    uint8_t bytes[BYTES_LEN];
    uint8_t recovered_bits[BITS_LEN];

    bits_to_bytes(bits, BITS_LEN, bytes);
    bytes_to_bits(bytes, BYTES_LEN, recovered_bits);

    printf("Original bits:     ");
    for (size_t i = 0; i < BITS_LEN; i++) {
        printf("%d", bits[i]);
    }
    printf("\nRecovered bits:    ");
    for (size_t i = 0; i < BITS_LEN; i++) {
        printf("%d", recovered_bits[i]);
    }
    printf("\n\n");

    // Test for compress and decompress
    printf("Testing compress and decompress...\n");

    uint16_t x = 1234; // Arbitrary value within range
    size_t d = 10; // Compression level
    uint16_t compressed = compress(d, x);
    uint16_t decompressed = decompress(d, compressed);

    printf("Original value:    %u\n", x);
    printf("Compressed value:  %u\n", compressed);
    printf("Decompressed value: %u\n\n", decompressed);

    // Test for encode and decode
    printf("Testing encode and decode...\n");

    uint16_t F[ARRAY_LEN];
    uint8_t B[ARRAY_LEN * d / 8]; // Adjust size for byte array
    uint16_t recovered_F[ARRAY_LEN];

    // Fill F with example data
    for (size_t i = 0; i < ARRAY_LEN; i++) {
        F[i] = i % (1 << d); // Example values within d-bit limit
    }

    encode(F, d, B);
    decode(B, d, recovered_F);

    // Display sample data to verify encoding and decoding
    printf("Original F (sample):     ");
    for (size_t i = 0; i < 10; i++) {
        printf("%u ", F[i]);
    }
    printf("\nRecovered F (sample):    ");
    for (size_t i = 0; i < 10; i++) {
        printf("%u ", recovered_F[i]);
    }
    printf("\n");

    // Compare arrays for testing correctness
    int match = 1;
    for (size_t i = 0; i < ARRAY_LEN; i++) {
        if (F[i] != recovered_F[i]) {
            match = 0;
            break;
        }
    }

    if (match) {
        printf("\nAll functions passed the tests successfully!\n");
    } else {
        printf("\nError: Mismatch found in encode-decode test.\n");
    }

    return 0;
}
