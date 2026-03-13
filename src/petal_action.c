#include "rb_cluster_state.h"
#include <string.h>

/**
 * rb_petal_action_commit
 * Records a deterministic intent or command into Petal 8.
 */
int rb_petal_action_commit(rb_cluster_state_t *state, const uint8_t *action_hash, uint8_t action_code) {
    if (state == NULL || action_hash == NULL) return -1;

    // Petal 8: Action/Intent slot
    memcpy(state->petals[8].hash, action_hash, 32);
    
    // Status 1 (GOLD) indicates a committed action
    state->petals[8].status = 1; 

    // We use the first byte of reserved space for the explicit Action Code
    // (e.g., 0x01 = MOVE, 0x02 = HALT, 0x03 = TRANSMIT)
    state->petals[8].reserved[0] = action_code;

    return 0;
}
