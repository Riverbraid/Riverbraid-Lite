#include "integrity.h"
#include "crypto_engine.h"
#include <string.h>

bool rb_verify_state_integrity_with_key(const rb_state_t *state, const uint8_t *key, size_t key_len) {
    uint8_t computed_sig[32];
    hmac_sha256_ctx ctx;

    hmac_sha256_init(&ctx, key, key_len);
    
    // Explicitly hash fields to avoid compiler padding issues
    hmac_sha256_update(&ctx, (const uint8_t*)&state->step, sizeof(state->step));
    hmac_sha256_update(&ctx, (const uint8_t*)&state->action, sizeof(state->action));
    
    hmac_sha256_final(&ctx, computed_sig);

    return memcmp(computed_sig, state->hmac_sig, 32) == 0;
}
