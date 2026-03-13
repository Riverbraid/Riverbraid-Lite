#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "rb_cluster_state.h"

// Prototypes
int rb_petal_audio_update(rb_cluster_state_t *state, const uint8_t *h);
int rb_petal_vision_update(rb_cluster_state_t *state, const uint8_t *h);
int rb_petal_action_commit(rb_cluster_state_t *state, const uint8_t *h, uint8_t code);
int rb_cluster_reseal(rb_cluster_state_t *state, void (*sha256)(const uint8_t*, size_t, uint8_t*));

void mock_sha256(const uint8_t *data, size_t len, uint8_t *out) {
    memset(out, 0, 32);
    for (size_t i = 0; i < len; i++) out[i % 32] ^= data[i];
}

int main() {
    rb_cluster_state_t state;
    memset(&state, 0, sizeof(state));
    state.sequence = 100;

    uint8_t dummy[32];
    memset(dummy, 0xEE, 32);

    printf("🧪 Testing Sovereign Loop (Perceive -> Act -> Seal)...\n");

    rb_petal_audio_update(&state, dummy);
    rb_petal_vision_update(&state, dummy);
    rb_petal_action_commit(&state, dummy, 0x05);
    
    // This is the missing heartbeat that moves the needle to 101
    int res = rb_cluster_reseal(&state, mock_sha256);
    
    printf("   [RESEAL RESULT]: %d\n", res);
    printf("   [FINAL SEQUENCE]: %lu\n", state.sequence);

    assert(res == 0);
    assert(state.sequence == 101);
    assert(state.petals[8].status == 1);
    assert(state.petals[8].reserved[0] == 0x05);

    printf("✅ SOVEREIGN LOOP PASS: Intent committed and sealed.\n");
    return 0;
}
