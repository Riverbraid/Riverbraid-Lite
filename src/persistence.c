#include "riverbraid_v1_4.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "crypto_engine.h"
#include <string.h>

#define FLASH_TARGET_OFFSET (2048 * 1024 - FLASH_SECTOR_SIZE)

void rb_persistence_save(const rb_state_t *state) {
    rb_state_t state_to_save = *state;
    pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);

    hmac_sha256_init(board_id.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES);
    hmac_sha256_update((const uint8_t*)&state_to_save, sizeof(rb_state_t) - 32);
    hmac_sha256_final(state_to_save.hmac_sig);

    uint8_t buffer[FLASH_PAGE_SIZE];
    memset(buffer, 0, FLASH_PAGE_SIZE);
    memcpy(buffer, &state_to_save, sizeof(rb_state_t));

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(FLASH_TARGET_OFFSET, buffer, FLASH_PAGE_SIZE);
    restore_interrupts(ints);
}

bool rb_persistence_load(rb_state_t *state) {
    const uint8_t *flash_target_contents = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);
    if (flash_target_contents[0] == 0xFF && flash_target_contents[1] == 0xFF) return false;
    memcpy(state, flash_target_contents, sizeof(rb_state_t));
    return true;
}
