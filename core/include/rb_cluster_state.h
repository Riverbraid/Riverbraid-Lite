#ifndef RB_CLUSTER_STATE_H
#define RB_CLUSTER_STATE_H

/* * RB_STRUCT_SIZE: 648 Bytes
 * This value is mechanically fixed. 
 * Any modification triggers a static_assert failure.
 */
#define RB_STRUCT_SIZE 648
#define RB_PETAL_COUNT 10

typedef struct {
    unsigned char version[4];     // 0-3
    unsigned char sequence[8];    // 4-11
    unsigned char petals[10][60]; // 12-611 (600 bytes)
    unsigned char root_hash[32];  // 612-643
    unsigned char padding[4];     // 644-647
} rb_cluster_state_t;

#endif
