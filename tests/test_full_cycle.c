#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "rb_cluster_state.h"

// Prototypes
int rb_petal_audio_update(rb_cluster_state_t *state, const uint8_t *audio_hash);
int rb_petal_vision_update(rb_cluster_state_t *state, const uint8_t *vision_hash);
int rb_cluster_reseal(rb_cluster_state_t *state, void (*sha256)(const uint8_t*, size_t, uint8_t*));

void mock_sha256(const uint8_t *data, size_t len, uint8_t *out) {
    memset(out, 0, 32);
    for (size_t i = 0; i < len; i++) out[i % 32] ^= data[i];
}

int main() {
    rb_cluster_state_t state;
    memset(&state, 0, sizeof(state));
    state.sequence = 1;

    uint8_t audio_data[32]; memset(audio_data, 0xA1, 32); // Audio pattern
    uint8_t vision_data[32]; memset(vision_data, 0xB2, 32); // Vision pattern

    printf("🚀 Starting Full Sensory Cycle (Seq: 1)...\n");

    // 1. Ingest Audio
    rb_petal_audio_update(&state, audio_data);
    printf("   [INGESTED] Petal 6: Audio\n");

    // 2. Ingest Vision
    rb_petal_vision_update(&state, vision_data);
    printf("   [INGESTED] Petal 7: Vision\n");

    // 3. Reseal
    rb_cluster_reseal(&state, mock_sha256);
    printf("   [RESEALED] New Sequence: %lu\n", state.sequence);

    // 4. Final Verification
    assert(state.sequence == 2);
    assert(state.petals[6].status == 1);
    assert(state.petals[7].status == 1);
    assert(state.petals[2].status == 1); // Ledger updated

    printf("✅ CYCLE COMPLETE: Final Ledger Hash: ");
    for(int i=0; i<4; i++) printf("%02x", state.petals[2].hash[i]);
    printf("...\n");

    return 0;
}
