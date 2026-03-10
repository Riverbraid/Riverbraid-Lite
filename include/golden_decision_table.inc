const uint8_t golden_decision_table[256] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    /* ... 0x10-0x7F: Linear ... */
    [128 ... 254] = 127, /* 0x80-0xFE: Saturated */
    [255] = 255          /* 0xFF: Refusal */
};
