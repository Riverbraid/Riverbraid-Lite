#ifndef INTEGRITY_H
#define INTEGRITY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "riverbraid_v1_4.h"

bool rb_verify_state_integrity_with_key(
    const rb_state_t *state,
    const uint8_t *key,
    size_t key_len);

#endif
