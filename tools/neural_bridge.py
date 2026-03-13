import ctypes
from tools.bridge_gateway import RBClusterState

def decode_cluster():
    state = RBClusterState()
    with open("cluster.bin", "rb") as f:
        f.readinto(state)
    
    print("\n--- 🧠 NEURAL BRIDGE: CLUSTER DECODE ---")
    petals = {0: "Audio", 1: "Vision", 2: "Ledger", 8: "Action"}
    
    for idx, name in petals.items():
        p = state.petals[idx]
        status = "GOLD" if p.status == 1 else "VOID"
        print(f"[{status}] Petal {idx} ({name}): {bytes(p.hash).hex()[:16]}...")

if __name__ == "__main__":
    decode_cluster()
