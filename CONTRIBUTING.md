# Contributing to Riverbraid-Lite
To maintain deterministic safety, all logic changes must:
1. Pass the Host-side Unit Tests (`make unit_tests`).
2. Pass the Security Audit (Tamper Detection).
3. Maintain $O(1)$ time complexity (no dynamic branching).
