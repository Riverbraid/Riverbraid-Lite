#ifndef RB_SHA256_H
#define RB_SHA256_H
#include <stdint.h>
#include <stddef.h>
typedef struct {
    uint32_t state[8];
    uint64_t bitcount;
    uint8_t  buffer[64];
} rb_sha256_ctx;
void rb_sha256_init(rb_sha256_ctx *ctx);
void rb_sha256_update(rb_sha256_ctx *ctx, const uint8_t *data, size_t len);
void rb_sha256_final(rb_sha256_ctx *ctx, uint8_t hash[32]);
#endif
