#include "rb_cluster_state.h"
#include <string.h>

/**
 * rb_cluster_reseal
 * Hardened state transition. 
 * Allows Petal 8 (Action) to use its first reserved byte for the Action Code.
 */
int rb_cluster_reseal(rb_cluster_state_t *state, void (*sha256)(const uint8_t*, size_t, uint8_t*)) {
    if (state == NULL || sha256 == NULL) return -1;

    // 1. Validate reserved bytes (Integrity Check)
    for (int i = 0; i < RB_PETAL_COUNT; i++) {
        for (int j = 0; j < 31; j++) {
            // EXCEPTION: Petal 8, Reserved Byte 0 is our Action Code.
            if (i == 8 && j == 0) continue; 

            if (state->petals[i].reserved[j] != 0) return -2; // ERR_ENTROPY_DETECTED
        }
    }

    // 2. Increment Sequence
    state->sequence++;

    // 3. Generate the new Cluster Root (Hash of all 648 bytes)
    uint8_t root[32];
    sha256((const uint8_t*)state, 648, root);

    // 4. Commit Root to Ledger (Petal 2)
    memcpy(state->petals[2].hash, root, 32);
    state->petals[2].status = 1;

    return 0;
}
