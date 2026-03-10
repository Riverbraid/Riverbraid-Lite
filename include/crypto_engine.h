#ifndef CRYPTO_ENGINE_H
#define CRYPTO_ENGINE_H

#include <stddef.h>
#include <stdint.h>

/* HMAC SHA-256 Context Structure */
typedef struct {
    uint32_t state[8];
    uint64_t bitcount;
    uint8_t buffer[64];
    uint8_t opad[64];
} hmac_sha256_ctx;

void hmac_sha256_init(hmac_sha256_ctx *ctx,
                      const uint8_t *key,
                      size_t key_len);

void hmac_sha256_update(hmac_sha256_ctx *ctx,
                        const uint8_t *data,
                        size_t len);

void hmac_sha256_final(hmac_sha256_ctx *ctx,
                       uint8_t out[32]);

#endif
