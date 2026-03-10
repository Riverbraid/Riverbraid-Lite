#ifndef INTEGRITY_H
#define INTEGRITY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "riverbraid_v1_4.h"

// Hardware-bound verification (Uses Pico Unique ID)
bool rb_verify_state_integrity(const rb_state_t *state);

// Manual verification (Used for unit testing/host builds)
bool rb_verify_state_integrity_with_key(
    const rb_state_t *state,
    const uint8_t *key,
    size_t key_len);

#endif
