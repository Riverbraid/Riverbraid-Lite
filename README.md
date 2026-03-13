# Riverbraid-Lite

A sovereign, 648-byte state machine designed for deterministic sensory ingestion and interop.

## 🏛️ The 648-Byte Philosophy
Unlike traditional systems that grow indefinitely, Riverbraid-Lite operates on a **Fixed-Resonance Substrate**. Every state transition is contained within exactly 648 bytes, ensuring:
* **Zero Drift:** Memory layout is immutable and verified by the Bridge.
* **Cryptographic Finality:** Every transition is sealed into a Ledger Petal.
* **Cross-Language Interop:** Seamless mapping between C and Python.

## 🛠️ The Order of Finality
To rebuild the system in a fresh environment:

1. **Build:** `make` - Compiles the core sovereign library.
2. **Verify:** `make test` - Runs the integrity suite (Perceive -> Act -> Seal).
3. **Monitor:** `make monitor` - Launches the Python observation gateway.

## 🛰️ Architecture
* **Petal 1:** Identity (The Genesis Bridge Hash)
* **Petal 2:** Ledger (The Cryptographic Chain)
* **Petal 6:** Audio (Sensory Ingest)
* **Petal 7:** Vision (Sensory Ingest)
* **Petal 8:** Action (Intent & Commands)
