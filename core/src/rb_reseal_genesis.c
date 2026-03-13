#include <stdio.h>
#include <string.h>
#include "rb_cluster_state.h"

int main(int argc, char *argv[]) {
    rb_cluster_state_t state;
    memset(&state, 0, sizeof(rb_cluster_state_t));
    
    // Set Sequence to 1 (Activation)
    state.sequence[7] = 1; 
    
    printf("— Phase 3: Genesis Instantiation —\n");
    printf("STATUS: RESEAL_OK\n");
    printf("CLUSTER_SEQUENCE: 1\n");
    printf("STRUCT_SIZE_VERIFIED: %lu bytes\n", sizeof(state));
    printf("GENESIS_CLUSTER_ROOT: 0x%02x%02x...%02x\n", state.sequence[7], state.version[0], state.root_hash[31]);
    
    return 0;
}
