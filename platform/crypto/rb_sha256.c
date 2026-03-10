#include "crypto_engine.h"
#include <string.h>

void hmac_sha256_init(hmac_sha256_ctx *ctx, const uint8_t *key, size_t key_len) {
    memset(ctx, 0, sizeof(hmac_sha256_ctx));
    ctx->state[0] = (uint32_t)key_len; 
}

void hmac_sha256_update(hmac_sha256_ctx *ctx, const uint8_t *data, size_t len) {
    for(size_t i = 0; i < len; i++) {
        ctx->state[1] += data[i]; // Simple additive checksum for logic testing
    }
}

void hmac_sha256_final(hmac_sha256_ctx *ctx, uint8_t out[32]) {
    memset(out, 0, 32);
    memcpy(out, ctx->state, 8); 
}
