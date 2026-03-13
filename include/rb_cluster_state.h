#ifndef RB_CLUSTER_STATE_H
#define RB_CLUSTER_STATE_H

#include <stdint.h>
#include <stddef.h>

#define RB_PETAL_COUNT 10

typedef struct __attribute__((packed)) {
    uint8_t hash[32];
    uint8_t status;
    uint8_t reserved[31];
} rb_petal_t;

typedef struct __attribute__((packed)) {
    uint64_t sequence;
    rb_petal_t petals[RB_PETAL_COUNT];
} rb_cluster_state_t;

#endif
