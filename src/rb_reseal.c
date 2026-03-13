#include "rb_cluster_state.h"
#include <string.h>

typedef void (*sha256_func)(const uint8_t*, size_t, uint8_t*);

void rb_cluster_reseal(rb_cluster_state_t *state, sha256_func hash_fn) {
    uint8_t layer[16][32];
    uint8_t temp[65];

    // --- Build Leaf Layer ---
    for (int i = 0; i < 16; i++) {
        if (i < 10) {
            uint8_t raw[32];
            if (i == 2) memset(raw, 0, 32);
            else memcpy(raw, state->petals[i].hash, 32);
            temp[0] = 0x00; // Leaf Domain
            memcpy(temp + 1, raw, 32);
            hash_fn(temp, 33, layer[i]);
        }
        else if (i >= 10 && i <= 13) {
            uint8_t zero[32] = {0};
            temp[0] = 0x00; // Padding Leaf
            memcpy(temp + 1, zero, 32);
            hash_fn(temp, 33, layer[i]);
        }
        else if (i == 14) {
            const char *ver = "2.0.0";
            temp[0] = 0x03; // Version Domain
            memset(temp + 1, 0, 32);
            memcpy(temp + 1, ver, strlen(ver));
            hash_fn(temp, 1 + strlen(ver), layer[i]);
        }
        else if (i == 15) {
            uint8_t seq_raw[32] = {0};
            for (int b = 0; b < 8; b++)
                seq_raw[b] = (state->sequence >> (8 * b)) & 0xFF;
            temp[0] = 0x02; // Sequence Domain
            memcpy(temp + 1, seq_raw, 32);
            hash_fn(temp, 33, layer[i]);
        }
    }

    // --- Build Internal Nodes ---
    int nodes = 16;
    while (nodes > 1) {
        for (int i = 0; i < nodes / 2; i++) {
            temp[0] = 0x01; // Internal Node Domain
            memcpy(temp + 1, layer[i*2], 32);
            memcpy(temp + 33, layer[i*2 + 1], 32);
            hash_fn(temp, 65, layer[i]);
        }
        nodes /= 2;
    }

    memcpy(state->petals[2].hash, layer[0], 32);
    state->petals[2].status = 1;
}
