import crypto from 'crypto';
import fs from 'fs';

const custodians = ['custodianA', 'custodianB', 'custodianC'];
const keys = {};

for (const id of custodians) {
  const { publicKey, privateKey } = crypto.generateKeyPairSync('ed25519');

  keys[id] = {
    public: publicKey.export({ type: 'spki', format: 'der' }).toString('hex'),
    private: privateKey.export({ type: 'pkcs8', format: 'der' }).toString('hex')
  };
}

fs.writeFileSync('custodian_keys_secrets.json', JSON.stringify(keys, null, 2));
console.log("✓ Keys generated and stored in custodian_keys_secrets.json");
