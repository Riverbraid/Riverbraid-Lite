#include <stdio.h>
#include <string.h>
#include "rb_cluster_state.h"

int main() {
    rb_cluster_state_t state;
    memset(&state, 0, sizeof(state));
    
    state.sequence = 1;
    
    // Identity Petal (1) - Sealing the Bridge Hash
    // Bridge: ecdb5985747d937eba237b90bce187fae27c28d22a2d2020de27b7929d8c5edb
    uint8_t bridge_hash[32] = {
        0xec, 0xdb, 0x59, 0x85, 0x74, 0x7d, 0x93, 0x7e,
        0xba, 0x23, 0x7b, 0x90, 0xbc, 0xe1, 0x87, 0xfa,
        0xe2, 0x7c, 0x28, 0xd2, 0x2a, 0x2d, 0x20, 0x20,
        0xde, 0x27, 0xb7, 0x92, 0x9d, 0x8c, 0x5e, 0xdb
    };
    memcpy(state.petals[1].hash, bridge_hash, 32);
    state.petals[1].status = 1; // IDENTITY LOCKED

    // Ledger Petal (2) - Initializing
    state.petals[2].status = 1; 
    
    printf("IDENTITY_SEAL_LOCKED\n");
    printf("BRIDGE_FINGERPRINT: %02x%02x%02x%02x...\n", 
            state.petals[1].hash[0], state.petals[1].hash[1], 
            state.petals[1].hash[2], state.petals[1].hash[3]);
    return 0;
}
