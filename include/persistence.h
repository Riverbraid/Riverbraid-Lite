#ifndef PERSISTENCE_H
#define PERSISTENCE_H
#include "riverbraid_v1_4.h"
void rb_persistence_save(const rb_state_t *state);
bool rb_persistence_load(rb_state_t *state);
#endif
