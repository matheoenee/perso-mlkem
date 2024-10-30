#include "utils.h"
#include "sample.h"
#include "params.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SEED_SIZE 32

int main() {
    // Test sampleNTT function
    uint8_t seed[SEED_SIZE] = {0x01, 0xF2, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
                                0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                0x19, 0x10, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20};
    
    uint16_t a[256];
    uint8_t ind1 = 5; // Example index i
    uint8_t ind2 = 10; // Example index j

    sample_ntt(seed, ind1, ind2, a);
    printf("Sample NTT output:\n");
    printf("(");
    for (size_t k = 0; k < 256; k++) { // Display first 10 samples
        printf("%u, ", a[k]);
    }
    printf(")\n");

    // Test sample_poly_cbd function
    uint8_t eta = 3; // Example eta value
    uint16_t f[256];

    sample_poly_cbd(seed, eta, f);
    printf("\nSample poly CBD output:\n");
    for (size_t k = 0; k < 10; k++) { // Display first 10 samples
        printf("f[%zu] = %u\n", k, f[k]);
    }

    return 0;
}
