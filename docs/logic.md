# Riverbraid v1.4 | Decision Engine (Table-Driven)

The system utilizes a deterministic mapping model rather than simple thresholding.

### Logic Mapping:
- **Input:** $S = \{s_0, s_1, s_2, s_3\}$
- **Index Calculation:** $I = \lfloor \frac{\sum S}{4} \rfloor$
- **Resolution:** `state->action = golden_decision_table[I]`

### Why Table-Driven?
This model preserves the **Stationary State invariant**. By using a pre-computed 256-byte table, we ensure $O(1)$ constant-time execution and prevent branching logic, which hardens the firmware against timing attacks and ensures "Mechanical Honesty" in state transitions.
