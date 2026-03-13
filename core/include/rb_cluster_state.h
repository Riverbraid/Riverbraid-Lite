#include <stdint.h>

/* core/include/rb_cluster_state.h */

// 1. The 48-Byte Identity Floor (The Anchor)
typedef struct {
    uint8_t version[4];
    uint8_t hardware_id[32];
    uint8_t merkle_root[12]; // Total: 48 bytes
} rb_anchor_t;

// 2. The 648-Byte Sovereign Body (The Triad)
typedef struct {
    rb_anchor_t anchor;      // 48 bytes
    uint8_t sensory_data[600]; // Total: 648 bytes
} rb_state_t;

// 3. Static Governance Gates
_Static_assert(sizeof(rb_anchor_t) == 48, "IDENTITY DRIFT: Anchor must be 48 bytes.");
_Static_assert(sizeof(rb_state_t) == 648, "BODY DRIFT: Total state must be 648 bytes.");
