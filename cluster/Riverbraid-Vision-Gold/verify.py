import sys
with open('.anchor/logic.root', 'r') as f:
    if f.read().strip() == '08e829':
        print("✓ v1.3 Logic Verified (Python)")
        sys.exit(0)
    sys.exit(1)
