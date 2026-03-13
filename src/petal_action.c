#include "rb_cluster_state.h"
#include <string.h>

/**
 * rb_petal_action_commit
 * Hardened to prevent null pointers and ensure fixed-width copy.
 */
int rb_petal_action_commit(rb_cluster_state_t *state, const uint8_t *intent_hash, uint8_t action_code) {
    if (!state || !intent_hash) return -1;

    // Petal 8 is the Action Petal
    rb_petal_t *p = &state->petals[8];

    // Explicitly copy exactly 32 bytes to prevent overflow
    memcpy(p->hash, intent_hash, 32);
    p->status = 1; // Mark as GOLD
    
    // Use the first byte of reserved space for the action code
    p->reserved[0] = action_code;

    return 0;
}
