#!/bin/bash
set -e
echo "--- INTEGRITY AUDIT ---"
# Ban legacy types
! grep -R "rb_state_t" . --exclude-dir=.git
# Ban entropy sources
! grep -R -E "malloc|free|time\(|rand\(" src/
# Verify 648-byte resonance
gcc -I./include -x c -c - << 'EOT'
#include "rb_cluster_state.h"
_Static_assert(sizeof(rb_cluster_state_t) == 648, "FAIL");
EOT
echo "--- AUDIT PASS ---"
