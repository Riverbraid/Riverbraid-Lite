# Riverbraid v1.4 | Architectural Standard

## 1. Data Structure (The 48-Byte Invariant)
The system state is packed to ensure predictable flash alignment and cryptographic hashing.

| Offset | Type     | Field      | Description                          |
| :----- | :------- | :--------- | :----------------------------------- |
| 0x00   | uint64_t | `step`     | Monotonic transition counter         |
| 0x08   | uint8_t  | `sensors`  | 4-channel 8-bit input snapshot       |
| 0x0C   | uint8_t  | `action`   | Output state (0x00 - 0xFF)           |
| 0x0D   | uint8_t  | `reserved` | 3-byte padding for 16-byte alignment |
| 0x10   | uint8_t  | `hmac_sig` | 32-byte SHA-256 HMAC signature       |

## 2. Integrity Flow
1. **Boot:** System reads flash and verifies `hmac_sig` against `board_id`.
2. **Update:** Sensors are sampled; `step` increments; `action` is calculated.
3. **Seal:** New HMAC is generated over the first 16 bytes of the struct.
4. **Commit:** The 48-byte block is written to persistent flash memory.
