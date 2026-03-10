import hmac
import hashlib
import struct
import sys

def sign_state(step, action, key_hex):
    try:
        key = bytes.fromhex(key_hex)
        # Pack: Q (uint64), B (uint8) to match C struct memory layout
        data = struct.pack('<QB', step, action)
        signature = hmac.new(key, data, hashlib.sha256).digest()
        return signature.hex()
    except Exception as e:
        return f"Error: {e}"

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Usage: python3 sign_state.py <step> <action> <key_hex>")
        print("Example: python3 sign_state.py 1 100 DEADBEEFFEEDCAFE")
    else:
        print(sign_state(int(sys.argv[1]), int(sys.argv[2]), sys.argv[3]))
