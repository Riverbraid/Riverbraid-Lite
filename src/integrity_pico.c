#include "pico/unique_id.h"
#include "integrity.h"
#include "crypto_engine.h"

bool rb_verify_state_integrity(const rb_state_t *state) {
    pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);

    // Use the 8-byte (64-bit) Unique ID as the HMAC Key
    // This binds the software state to the physical silicon.
    return rb_verify_state_integrity_with_key(
        state, 
        board_id.id, 
        PICO_UNIQUE_BOARD_ID_SIZE_BYTES
    );
}
