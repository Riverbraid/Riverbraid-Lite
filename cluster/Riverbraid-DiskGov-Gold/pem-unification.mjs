import crypto from 'crypto';

export function toPEM(hexKey, type = 'public') {
  const buffer = Buffer.from(hexKey, 'hex');
  const format = type === 'public' ? 'spki' : 'pkcs8';
  
  const key = type === 'public' 
    ? crypto.createPublicKey({ key: buffer, format: 'der', type: 'spki' })
    : crypto.createPrivateKey({ key: buffer, format: 'der', type: 'pkcs8' });

  return key.export({ type: format, format: 'pem' });
}

console.log('✓ PEM Unification Module Active');
