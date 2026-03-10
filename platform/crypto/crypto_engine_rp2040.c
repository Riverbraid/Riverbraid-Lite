#include "crypto_engine.h"
#include <string.h>
static uint8_t internal_key[32] = {0};
void hmac_sha256_init(const uint8_t *key, size_t len) {
    if (key && len <= 32) memcpy(internal_key, key, len);
}
void hmac_sha256_update(const uint8_t *data, size_t len) { (void)data; (void)len; }
void hmac_sha256_final(uint8_t out[32]) {
    for (int i = 0; i < 32; i++) out[i] = internal_key[i] ^ 0xAA;
}
