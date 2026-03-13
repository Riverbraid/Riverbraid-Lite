import ctypes
import time
import hashlib
from bridge_gateway import RBClusterState, Petal

# Load the library for access to the struct
rb = ctypes.CDLL("./librb_core.so")

def get_state_hash(state):
    """Generates a fingerprint of the entire 648-byte block."""
    return hashlib.sha256(bytes(state)).hexdigest()

def start_watchdog(interval_sec=2):
    print("🛡️ Watchdog Active: Monitoring 648-byte Resonance...")
    
    # Initialize a clean state as our baseline
    state = RBClusterState()
    state.sequence = 1
    
    last_hash = get_state_hash(state)
    last_seq = state.sequence
    
    try:
        while True:
            current_hash = get_state_hash(state)
            
            # If hash changes but sequence didn't, we have Drift
            if current_hash != last_hash:
                if state.sequence == last_seq:
                    print(f"🚨 ALERT: Unauthorized Drift Detected! Hash: {current_hash[:16]}...")
                else:
                    print(f"🔄 Authorized Transition: Sequence {last_seq} -> {state.sequence}")
                
                last_hash = current_hash
                last_seq = state.sequence
            
            time.sleep(interval_sec)
    except KeyboardInterrupt:
        print("\n🛑 Watchdog Stand-down.")

if __name__ == "__main__":
    start_watchdog()
