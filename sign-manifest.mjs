import fs from 'fs';
import crypto from 'crypto';

const secrets = JSON.parse(fs.readFileSync('./custodian_keys_secrets.json', 'utf8'));
const snapshot = JSON.parse(fs.readFileSync('./constitution.snapshot.json', 'utf8'));

// Use Custodian A and B to meet the threshold of 2
const signerIds = ['custodianA', 'custodianB'];
const msgBuffer = Buffer.from(snapshot.merkleRoot, 'hex');

const sigs = signerIds.map((id) => {
  const privateKey = crypto.createPrivateKey({
    key: Buffer.from(secrets[id].private, 'hex'),
    format: 'der',
    type: 'pkcs8'
  });

  return {
    keyId: id,
    signature: crypto.sign(null, msgBuffer, privateKey).toString('hex')
  };
});

fs.writeFileSync('./constitution.signature.json', JSON.stringify({ sigs }, null, 2));
console.log('✓ constitution.signature.json written (Threshold reached: 2/2)');
