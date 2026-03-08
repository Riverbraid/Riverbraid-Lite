import crypto from 'crypto';

export function verifyThreshold(messageHash, signatures, publicKeys, threshold) {
  const seen = new Set();
  let validCount = 0;
  const msgBuffer = Buffer.from(messageHash, 'hex');

  for (const sigObj of signatures) {
    if (!sigObj || typeof sigObj.keyId !== 'string' || typeof sigObj.signature !== 'string') continue;
    if (seen.has(sigObj.keyId)) continue;
    seen.add(sigObj.keyId);

    const pubKey = publicKeys[sigObj.keyId];
    if (!pubKey) continue;

    const ok = crypto.verify(
      null,
      msgBuffer,
      crypto.createPublicKey({ key: Buffer.from(pubKey, 'hex'), format: 'der', type: 'spki' }),
      Buffer.from(sigObj.signature, 'hex')
    );

    if (ok) validCount += 1;
  }
  return { authorized: validCount >= threshold, validCount, threshold };
}
