import ctypes
import os
from bridge_gateway import RBClusterState

# Load Library
rb = ctypes.CDLL("./librb_core.so")

# Configure Signatures
rb.rb_state_save.argtypes = [ctypes.POINTER(RBClusterState), ctypes.c_char_p]
rb.rb_state_load.argtypes = [ctypes.POINTER(RBClusterState), ctypes.c_char_p]

# 1. Setup a State
state = RBClusterState()
state.sequence = 999
state.petals[6].status = 1 # Audio is GOLD
FILENAME = b"cluster_snapshot.bin"

print(f"💾 Saving sequence {state.sequence} to disk...")
rb.rb_state_save(ctypes.byref(state), FILENAME)

# 2. Wipe memory
state = RBClusterState() 
print(f"🧹 Memory wiped. Current sequence: {state.sequence}")

# 3. Restore
print(f"📂 Loading from {FILENAME.decode()}...")
rb.rb_state_load(ctypes.byref(state), FILENAME)

# 4. Verify
print(f"✅ Restored Sequence: {state.sequence}")
print(f"✅ Restored Petal 6 Status: {'GOLD' if state.petals[6].status == 1 else 'VOID'}")

if state.sequence == 999 and state.petals[6].status == 1:
    print("\n🎊 PERSISTENCE VERIFIED: The cluster survived the reboot.")
else:
    print("\n❌ PERSISTENCE FAILURE: Structural data lost.")
