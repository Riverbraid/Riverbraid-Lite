import ctypes
import hashlib

# 1. Structural Mapping (Verified 648B)
class Petal(ctypes.Structure):
    _pack_ = 1
    _fields_ = [("hash", ctypes.c_uint8 * 32), ("status", ctypes.c_uint8), ("reserved", ctypes.c_uint8 * 31)]

class RBClusterState(ctypes.Structure):
    _pack_ = 1
    _fields_ = [("sequence", ctypes.c_uint64), ("petals", Petal * 10)]

# 2. Define the Callback Type for SHA256
# void (*sha256)(const uint8_t*, size_t, uint8_t*)
SHA256_CALLBACK = ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_uint8), ctypes.c_size_t, ctypes.POINTER(ctypes.c_uint8))

@SHA256_CALLBACK
def python_sha256(data_ptr, length, out_ptr):
    data = ctypes.string_at(data_ptr, length)
    digest = hashlib.sha256(data).digest()
    ctypes.memmove(out_ptr, digest, 32)

# 3. Load Library and Configure Signatures
rb = ctypes.CDLL("./librb_core.so")
rb.rb_petal_audio_update.argtypes = [ctypes.POINTER(RBClusterState), ctypes.POINTER(ctypes.c_uint8)]
rb.rb_petal_action_commit.argtypes = [ctypes.POINTER(RBClusterState), ctypes.POINTER(ctypes.c_uint8), ctypes.c_uint8]
rb.rb_cluster_reseal.argtypes = [ctypes.POINTER(RBClusterState), SHA256_CALLBACK]

def monitor(state):
    print(f"\n--- BRIDGE AUDIT (Seq: {state.sequence}) ---")
    print(f"Petal 2 (Ledger) Status: {'[GOLD]' if state.petals[2].status == 1 else '[VOID]'}")
    if state.petals[2].status == 1:
        print(f"Ledger Hash: {bytes(state.petals[2].hash).hex()[:16]}...")

# --- EXECUTION ---
state = RBClusterState()
state.sequence = 200

# Ingest & Commit
digest = hashlib.sha256(b"INIT_SIGNAL").digest()
rb.rb_petal_audio_update(state, (ctypes.c_uint8 * 32).from_buffer_copy(digest))
rb.rb_petal_action_commit(state, (ctypes.c_uint8 * 32)(*[0xEE]*32), 0x07)

print("🧪 Triggering Reseal via Python Callback...")
rb.rb_cluster_reseal(state, python_sha256)

monitor(state)
