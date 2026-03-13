#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "riverbraid_v1_4.h"
#include "integrity.h"
#include "crypto_engine.h"

void test_tamper_detection() {
    printf("[*] Starting Tamper Detection Test...\n");
    rb_cluster_state_t state;
    memset(&state, 0, sizeof(state));
    uint8_t key[8] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED, 0xCA, 0xFE};

    state.step = 1;
    state.action = 0x64;

    // SIGNING: Must match integrity.c exactly
    hmac_sha256_ctx ctx;
    hmac_sha256_init(&ctx, key, 8);
    hmac_sha256_update(&ctx, (const uint8_t*)&state.step, sizeof(state.step));
    hmac_sha256_update(&ctx, (const uint8_t*)&state.action, sizeof(state.action));
    hmac_sha256_final(&ctx, state.hmac_sig);

    // 1. Verify should PASS
    bool valid = rb_verify_state_integrity_with_key(&state, key, 8);
    assert(valid == true);
    printf("   [PASS] Valid state signature verified.\n");

    // 2. Tamper and verify FAIL
    printf("   [!] Simulating bit-flip...\n");
    state.step ^= 0x01; 
    bool tampered_valid = rb_verify_state_integrity_with_key(&state, key, 8);
    assert(tampered_valid == false);
    printf("   [PASS] Tamper detected successfully.\n");
}

int main() {
    printf("--- RIVERBRAID-LITE SECURITY AUDIT ---\n");
    test_tamper_detection();
    return 0;
}
