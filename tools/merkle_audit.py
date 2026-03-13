import ctypes, hashlib
from tools.bridge_gateway import RBClusterState, rb

def h(prefix, data):
    return hashlib.sha256(bytes([prefix]) + data).digest()

def audit():
    state = RBClusterState()
    if rb.rb_state_load(ctypes.byref(state), b"cluster.bin") != 0: return
    print(f"🔍 Audit Seq: {state.sequence}")
    
    leaves = []
    for i in range(16):
        if i < 10:
            raw = bytes(state.petals[i].hash) if i != 2 else bytes(32)
            leaves.append(h(0x00, raw))
        elif 10 <= i <= 13:
            leaves.append(h(0x00, bytes(32)))
        elif i == 14:
            leaves.append(h(0x03, b"2.0.0"))
        elif i == 15:
            seq_raw = state.sequence.to_bytes(8, 'little') + bytes(24)
            leaves.append(h(0x02, seq_raw))

    layer = leaves
    while len(layer) > 1:
        layer = [h(0x01, layer[i] + layer[i+1]) for i in range(0, len(layer), 2)]
    
    calc, actual = layer[0].hex(), bytes(state.petals[2].hash).hex()
    if calc == actual:
        print(f"✅ VERIFIED: {calc[:16]}")
    else:
        print(f"❌ FAIL\nExpected: {calc[:16]}\nActual:   {actual[:16]}")

if __name__ == "__main__": audit()
