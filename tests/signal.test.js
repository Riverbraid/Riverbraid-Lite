const { verifyEnvelope } = require('../index');

describe('Phase 3: Signal Integrity', () => {
  test('Accepts a valid result envelope', () => {
    const validResult = {
      repo: "Riverbraid-Core",
      status: "pass",
      timestamp: new Date().toISOString(),
      phase: 1,
      vector_hash: "v1_lexicon_stable"
    };
    expect(verifyEnvelope(validResult).status).toBe("pass");
  });

  test('Fails a result with missing fields (Fail-Closed)', () => {
    const invalidResult = { repo: "Riverbraid-Core", status: "pass" };
    expect(verifyEnvelope(invalidResult).status).toBe("fail");
  });
});
