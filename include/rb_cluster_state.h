#ifndef RB_CLUSTER_STATE_H
#define RB_CLUSTER_STATE_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define RB_PETAL_COUNT 10

typedef struct __attribute__((packed)) {
    uint8_t hash[32];      // 32 bytes
    uint8_t status;        // 1 byte
    uint8_t reserved[31];  // must remain zero
} rb_petal_t;

typedef struct __attribute__((packed)) {
    uint64_t sequence;                   // 8 bytes
    rb_petal_t petals[RB_PETAL_COUNT];   // 640 bytes
} rb_cluster_state_t;

/* Mechanical Invariants */
_Static_assert(sizeof(rb_petal_t) == 64, "Petal size drift");
_Static_assert(sizeof(rb_cluster_state_t) == 648, "Cluster must be 648 bytes");
_Static_assert(offsetof(rb_cluster_state_t, petals) == 8, "Petals misaligned");

#endif
