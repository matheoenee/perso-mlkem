#include "params.h"
#include "utils.h"

#include <stdint.h>
#include <openssl/evp.h>

/*************************************************
* Name:        sample_ntt
*
* Description: Takes a 32-byte seed and two indices as input 
*              and outputs a pseudorandom element of 𝑇𝑞
*
* Arguments:   - const uint8_t *seed: pointer to input array of bytes
*              - uint8_t ind_1: first indice
*              - uint8_t ind_2: second indice 
*              - uint16_t *a: pointer to output array of integers modulo q
**************************************************/

void sample_ntt(const uint8_t *seed, uint8_t ind_1, uint8_t ind_2, uint16_t *a) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();

    // XOF.init()
    if (EVP_DigestInit_ex(ctx, EVP_shake128(), NULL) != 1) {
        fprintf(stderr, "Failed to initialize SHAKE128\n");
        EVP_MD_CTX_free(ctx);
        return;
    }

    // XOF.asborb()
    EVP_DigestUpdate(ctx, seed, 32);
    EVP_DigestUpdate(ctx, &ind_1, 1);
    EVP_DigestUpdate(ctx, &ind_2, 1);

    size_t j = 0;
    uint8_t *C = malloc(3 * 256 * sizeof(uint8_t));

    // XOF.squeeze() - Can be executed only once
    // TRY TO SOLVE THIS PROBLEM
    EVP_DigestFinalXOF(ctx, C, 3 * 256);

    uint16_t d1, d2;

    while (j < 256) {
        d1 = C[3*j] + 256*(C[3*j + 1] % 16);
        d2 = (C[3*j + 1]/16) + 16*C[3*j + 2];

        if(d1 < MLKEM_Q) {
            a[j] = d1;
            j++;
        } 
        if(d2 < MLKEM_Q && j < 256) {
            a[j] = d2;
            j++;
        }
    }

    EVP_MD_CTX_free(ctx);
    free(C);
}

/*************************************************
* Name:        sample_poly_cbd
*
* Description: Takes a seed and eta as inputs and outputs a pseudorandom 
*              sample from the distribution D_eta(𝑅𝑞).
*
* Arguments:   - const uint8_t *seed: pointer to input array of bytes
*              - uint8_t eta: eta parameter
*              - uint16_t *f: pointer to output array of integers modulo q
**************************************************/

void sample_poly_cbd(const uint8_t *seed, uint8_t eta, uint16_t *f) {
    size_t seed_size = 64 * eta; // in bytes

    uint8_t *b = malloc(seed_size * 8 * sizeof(uint8_t));
    bytes_to_bits(seed, seed_size, b);

    uint8_t x, y;

    for(size_t i=0; i<256; i++) {
        x = 0;
        y = 0;
        for(size_t j=0; j<eta; j++) {
            x += b[2*i*eta + j];
            y += b[2*i*eta + eta + j];
        }
        f[i] = (x - y + MLKEM_Q) % MLKEM_Q;
    }

    free(b);
}