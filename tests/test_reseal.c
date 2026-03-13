#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "rb_cluster_state.h"

// Mock SHA256 for logic verification (XOR-sum to show change)
void mock_sha256(const uint8_t *data, size_t len, uint8_t *out) {
    memset(out, 0, 32);
    for (size_t i = 0; i < len; i++) {
        out[i % 32] ^= data[i];
    }
}

int rb_cluster_reseal(rb_cluster_state_t *state, void (*sha256)(const uint8_t*, size_t, uint8_t*));

int main() {
    rb_cluster_state_t state;
    memset(&state, 0, sizeof(state));
    state.sequence = 10; // Start at 10

    printf("🧪 Testing Cluster Reseal (Sequence 10 -> 11)...\n");

    int res = rb_cluster_reseal(&state, mock_sha256);
    
    // Verify increment
    assert(res == 0);
    assert(state.sequence == 11);
    
    // Verify Ledger Petal (2) is no longer empty
    assert(state.petals[2].status == 1);
    
    printf("✅ RESEAL PASS: Sequence advanced to %lu and Ledger Petal signed.\n", state.sequence);
    return 0;
}
