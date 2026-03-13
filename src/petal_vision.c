#include "rb_cluster_state.h"
#include <string.h>

/**
 * rb_petal_vision_update
 * Maps incoming raw vision metadata/hash to Petal 7.
 */
int rb_petal_vision_update(rb_cluster_state_t *state, const uint8_t *vision_hash) {
    if (state == NULL || vision_hash == NULL) return -1;

    // Petal 7: Vision slot
    memcpy(state->petals[7].hash, vision_hash, 32);
    state->petals[7].status = 1; // GOLD / Active

    // Explicitly zero reserved bytes
    memset(state->petals[7].reserved, 0, 31);

    return 0;
}
