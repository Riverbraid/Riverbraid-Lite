# Riverbraid-Lite: Engineering Design & Navigation

## 1. The Logical Flow
Every update cycle follows a strict, non-branching path to ensure deterministic timing.

1. **Ingress:** Sensor data is normalized.
2. **Transition:** `rb_v1_4_update` maps inputs to an action.
3. **Sealing:** The new state is hashed with the Hardware Unique ID (or mock key in tests).
4. **Verification:** `rb_verify_state_integrity_with_key` checks the "Mechanical Honesty" of the state.

## 2. Directory Map
- `include/`: API Headers (The Contract)
- `src/`: Logic implementation (The Brain)
- `platform/`: Hardware-specific drivers (The Body)
- `tests/`: Security and Logic audits

## 3. Security Invariant
The system REJECTS any state where a bit has been flipped or the HMAC signature does not match the content.
