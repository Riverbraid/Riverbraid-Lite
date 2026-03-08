export function calculateCoupling(internal, external) {
  if (!Array.isArray(internal) || !Array.isArray(external) || internal.length !== external.length || internal.length < 3) {
    return { healthy: false, status: 'INSUFFICIENT_DATA', r: 0, t: 0 };
  }

  const n = internal.length;
  const muI = internal.reduce((a, b) => a + b, 0) / n;
  const muE = external.reduce((a, b) => a + b, 0) / n;

  let num = 0, dI = 0, dE = 0;
  for (let i = 0; i < n; i++) {
    const di = internal[i] - muI;
    const de = external[i] - muE;
    num += di * de;
    dI += di * di;
    dE += de * de;
  }

  const denom = Math.sqrt(dI * dE);
  if (!Number.isFinite(denom) || denom === 0) return { healthy: false, status: 'DEGENERATE_SIGNAL', r: 0, t: 0 };

  let r = Math.max(-0.99999999, Math.min(0.99999999, num / denom));
  const t = Math.abs(r * Math.sqrt((n - 2) / (1 - r * r)));
  const healthy = t >= 3.291; // 99.9% confidence threshold

  return { healthy, status: healthy ? 'COHERENT' : 'DRIFT_DETECTED', r, t };
}
