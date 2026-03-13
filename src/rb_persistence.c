#include "rb_cluster_state.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * rb_state_save_safe
 * Writes to a .tmp file and renames it to target.
 * This ensures the main file is never partially written.
 */
int rb_state_save_safe(const rb_cluster_state_t *state, const char *filename) {
    if (state == NULL || filename == NULL) return -1;

    char tmp_name[256];
    snprintf(tmp_name, sizeof(tmp_name), "%s.tmp", filename);

    FILE *f = fopen(tmp_name, "wb");
    if (!f) return -2;

    size_t written = fwrite(state, sizeof(rb_cluster_state_t), 1, f);
    fclose(f);

    if (written != 1) {
        unlink(tmp_name);
        return -3;
    }

    // Atomic move: replaces target only if tmp write succeeded
    return rename(tmp_name, filename);
}

int rb_state_load(rb_cluster_state_t *state, const char *filename) {
    if (state == NULL || filename == NULL) return -1;

    FILE *f = fopen(filename, "rb");
    if (!f) return -2;

    size_t read = fread(state, sizeof(rb_cluster_state_t), 1, f);
    fclose(f);

    return (read == 1) ? 0 : -3;
}
