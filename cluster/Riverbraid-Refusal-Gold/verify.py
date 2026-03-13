import sys
import os

def check_integrity():
    # Path-Aware Correction: Find the anchor relative to the script location
    base_dir = os.path.dirname(os.path.abspath(__file__))
    anchor_path = os.path.join(base_dir, ".anchor", "logic.root")
    expected_root = "08e829"
    
    if not os.path.exists(anchor_path):
        print(f"FATAL: {anchor_path} not found.")
        sys.exit(1)
        
    with open(anchor_path, "r") as f:
        actual_root = f.read().strip()
        
    if actual_root == expected_root:
        repo_name = os.path.basename(base_dir) or "Riverbraid-Lite"
        print(f"✓ {repo_name}: Python Integrity Verified.")
        sys.exit(0)
    else:
        print(f"FAIL: Logic root mismatch in {base_dir}")
        sys.exit(1)

if __name__ == "__main__":
    check_integrity()
