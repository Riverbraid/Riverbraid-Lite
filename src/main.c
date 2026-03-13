#include <stdio.h>
#include "pico/stdlib.h"
#include "riverbraid_v1_4.h"
#include "integrity.h"

// Note: Ensure persistence functions are declared if not in riverbraid_v1_4.h
void rb_persistence_save(const rb_cluster_state_t *state);
bool rb_persistence_load(rb_cluster_state_t *state);
void rb_v1_4_init(rb_cluster_state_t *state);

int main() {
    stdio_init_all();
    rb_cluster_state_t state;

    // Small delay to allow USB serial to stabilize
    sleep_ms(2000);

    // Load persisted state or initialize genesis
    if (!rb_persistence_load(&state) || !rb_verify_state_integrity(&state, NULL)) {
        rb_v1_4_init(&state);
    }

    while (true) {
        // Mock 4-channel sensor input (matching the 4-byte array requirement)
        uint8_t sensors[4] = {0x2A, 0x2A, 0x2A, 0x2A};

        rb_v1_4_update(&state, sensors);
        rb_persistence_save(&state);

        // Format: STEP (64-bit), ACTION (8-bit), HMAC (256-bit)
        printf("STEP:%016llu ACTION:%02x HMAC:", state.step, state.action);
        for (int i = 0; i < 32; i++) {
            printf("%02x", state.hmac_sig[i]);
        }
        printf("\n");

        sleep_ms(2000);
    }
}
