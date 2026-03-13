#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "rb_cluster_state.h"

int rb_petal_audio_update(rb_cluster_state_t *state, const uint8_t *audio_hash);

int main() {
    rb_cluster_state_t state;
    memset(&state, 0, sizeof(state));
    
    // Set a fake Identity in Petal 1 to check for corruption
    memset(state.petals[1].hash, 0xFF, 32);

    uint8_t mock_audio[32];
    memset(mock_audio, 0x66, 32); // 0x66 for 'Audio'

    printf("🧪 Testing Petal 6 (Audio) Ingestion...\n");
    rb_petal_audio_update(&state, mock_audio);

    // Verify Audio
    assert(state.petals[6].status == 1);
    assert(state.petals[6].hash[0] == 0x66);

    // Verify Identity Petal (1) remained untouched
    assert(state.petals[1].hash[0] == 0xFF);
    
    printf("✅ INTEGRITY PASS: Audio ingestion isolated to Petal 6.\n");
    return 0;
}
