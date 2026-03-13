#include "rb_cluster_state.h"
#include <string.h>

/**
 * rb_petal_audio_update
 * Maps incoming raw audio metadata/hash to Petal 6.
 */
int rb_petal_audio_update(rb_cluster_state_t *state, const uint8_t *audio_hash) {
    if (state == NULL || audio_hash == NULL) return -1;

    // Petal 6: Audio slot
    memcpy(state->petals[6].hash, audio_hash, 32);
    state->petals[6].status = 1; // GOLD / Active

    // Explicitly zero reserved bytes to prevent entropy leakage
    memset(state->petals[6].reserved, 0, 31);

    return 0;
}
