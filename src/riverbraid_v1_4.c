#include "riverbraid_v1_4.h"
#include "crypto_engine.h"
#include <string.h>

void rb_v1_4_update(rb_state_t *state, const uint8_t sensors[4]) {
    state->step++;
    
    uint32_t sum = 0;
    for(int i=0; i<4; i++) sum += sensors[i];
    state->action = (uint8_t)(sum / 4);

    uint8_t mock_key[8] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED, 0xCA, 0xFE};
    hmac_sha256_ctx ctx;
    hmac_sha256_init(&ctx, mock_key, 8);
    hmac_sha256_update(&ctx, (uint8_t*)state, sizeof(rb_state_t) - 32);
    hmac_sha256_final(&ctx, state->hmac_sig);
}
