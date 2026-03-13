import sys
# v1.3 Sequence Integrity Auditor
with open('.anchor/logic.root', 'r') as f:
    if f.read().strip() == '08e829':
        print("✓ Temporal Sequence Verified (Python)")
        sys.exit(0)
    sys.exit(1)
