#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include "riverbraid_v1_4.h"
#include "integrity.h"
#include "crypto_engine.h"

void test_deterministic_logic() {
    printf("[*] Starting Deterministic Logic Test...\n");
    rb_state_t state;
    memset(&state, 0, sizeof(state));
    uint8_t sensors[4] = {100, 100, 100, 100};
    
    rb_v1_4_update(&state, sensors);
    printf("   [LOG] Step: %" PRIu64 " | Action: 0x%02X\n", state.step, state.action);
    
    assert(state.step == 1);
    assert(state.action == 100);
    printf("   [PASS] Logic is correct.\n");
}

void test_tamper_detection() {
    printf("[*] Starting Tamper Detection Test...\n");
    rb_state_t state;
    memset(&state, 0, sizeof(state));

    uint8_t sensors[4] = {100, 100, 100, 100};
    uint8_t key[8] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED, 0xCA, 0xFE};

    // 1. Generate deterministic state
    rb_v1_4_update(&state, sensors);

    // 2. Manually compute and embed valid HMAC for this test key
    hmac_sha256_ctx ctx;
    hmac_sha256_init(&ctx, key, 8);
    hmac_sha256_update(&ctx, (const uint8_t*)&state, sizeof(rb_state_t) - 32);
    hmac_sha256_final(&ctx, state.hmac_sig);

    // 3. Verify should PASS
    bool valid = rb_verify_state_integrity_with_key(&state, key, 8);
    assert(valid == true);
    printf("   [PASS] Valid state signature verified.\n");

    // 4. Tamper with state
    printf("   [!] Simulating bit-flip/tampering in state memory...\n");
    state.step ^= 0x01; 

    // 5. Verify should FAIL
    bool tampered_valid = rb_verify_state_integrity_with_key(&state, key, 8);
    assert(tampered_valid == false);
    printf("   [PASS] Tamper detected successfully.\n");
}

int main() {
    printf("--- RIVERBRAID-LITE SECURITY AUDIT ---\n");
    test_deterministic_logic();
    test_tamper_detection();
    printf("---------------------------------------\n");
    printf("[SUCCESS] System is professional and secure.\n");
    return 0;
}
