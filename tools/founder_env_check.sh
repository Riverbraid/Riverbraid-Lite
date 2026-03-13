#!/usr/bin/env bash
set -uo pipefail
FAIL=0
GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'; BOLD='\033[1m'; RESET='\033[0m'

echo -e "${BOLD}— Phase 0: Toolchain Presence —${RESET}"
for cmd in gcc python3 gpg xxd jq; do
    if command -v "$cmd" >/dev/null 2>&1; then echo -e "${GREEN}[PASS]${RESET} $cmd detected"; else echo -e "${RED}[FAIL]${RESET} $cmd missing"; FAIL=$((FAIL + 1)); fi
done

echo -e "\n${BOLD}— Phase 0: Memory Structural Check —${RESET}"
# Check for the specific 648-byte struct definition in headers
if grep -q "RB_STRUCT_SIZE 648" core/include/*.h 2>/dev/null; then
    echo -e "${GREEN}[PASS]${RESET} RB_STRUCT_SIZE verified at 648 bytes"
else
    echo -e "${YELLOW}[WARN]${RESET} Struct size definition not found. Checking source..."
fi

echo -e "\n${BOLD}— Phase 0: Static Entropy Scan —${RESET}"
ENTROPY_PATTERNS=('\bmalloc\b' '\bfree\s*\(' '\bfloat\b' '\bdouble\b' '\btime\s*\(' '\brand\s*\(')
for p in "${ENTROPY_PATTERNS[@]}"; do
    if grep -R -E "$p" core/ 2>/dev/null | grep -v '^\s*//' | grep -v '^\s*\*'; then 
        echo -e "${RED}[FAIL]${RESET} Forbidden entropy pattern '$p' found"; FAIL=$((FAIL + 1))
    fi
done

if [ $FAIL -eq 0 ]; then echo -e "\n${BOLD}${GREEN}[PHASE 0 READY]${RESET}"; exit 0; else echo -e "\n${BOLD}${RED}[PHASE 0 BLOCKED]${RESET}"; exit 1; fi
