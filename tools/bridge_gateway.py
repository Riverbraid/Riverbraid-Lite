import ctypes
import hashlib
import socket

# 1. Aligned Structural Mapping (Resonance 648B)
class Petal(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("hash", ctypes.c_uint8 * 32),
        ("status", ctypes.c_uint8),
        ("reserved", ctypes.c_uint8 * 31),
    ]

class RBClusterState(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("sequence", ctypes.c_uint64),
        ("petals", Petal * 10),
    ]

# 2. Load the Sovereign Library
rb = ctypes.CDLL("./librb_core.so")

# 3. Callback for SHA256
SHA256_CALLBACK = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.c_size_t, ctypes.POINTER(ctypes.c_uint8))

@SHA256_CALLBACK
def python_sha256(data_ptr, length, out_ptr):
    data = ctypes.string_at(data_ptr, length)
    digest = hashlib.sha256(data).digest()
    ctypes.memmove(out_ptr, digest, 32)

# 4. Configure Function Signatures
rb.rb_petal_audio_update.argtypes = [ctypes.POINTER(RBClusterState), ctypes.POINTER(ctypes.c_uint8)]
rb.rb_petal_action_commit.argtypes = [ctypes.POINTER(RBClusterState), ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint8]
rb.rb_cluster_reseal.argtypes = [ctypes.POINTER(RBClusterState), SHA256_CALLBACK]
rb.rb_state_save_safe.argtypes = [ctypes.POINTER(RBClusterState), ctypes.c_char_p]
rb.rb_state_load.argtypes = [ctypes.POINTER(RBClusterState), ctypes.c_char_p]

# --- UTILITIES ---

def safe_checkpoint(state, filename="cluster.bin"):
    fname_bytes = filename.encode('utf-8')
    res = rb.rb_state_save_safe(ctypes.byref(state), fname_bytes)
    if res == 0:
        print(f"💾 Checkpoint successful: {filename} (Seq: {state.sequence})")
    else:
        print(f"🚨 Checkpoint FAILED: {res}")
    return res

def stream_telemetry(state, target_ip="127.0.0.1", port=6480):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    raw_payload = bytes(state)
    sock.sendto(raw_payload, (target_ip, port))
    # Quiet the output for the heartbeat loop
    # print(f"🚀 Streamed {len(raw_payload)} bytes")

def monitor(state):
    print(f"\n--- BRIDGE AUDIT (Seq: {state.sequence}) ---")
    print(f"Petal 2 (Ledger) Status: {'[GOLD]' if state.petals[2].status == 1 else '[VOID]'}")
    print(f"Petal 8 (Action) Status: {'[GOLD]' if state.petals[8].status == 1 else '[VOID]'}")

if __name__ == "__main__":
    state = RBClusterState()
    monitor(state)
