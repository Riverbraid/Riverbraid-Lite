import fs from 'fs';
import { collectFiles, buildMerkleRoot } from './enclave.mjs';

// Load public keys from the git-ignored secrets file for the initial bootstrap
const secrets = JSON.parse(fs.readFileSync('./custodian_keys_secrets.json', 'utf8'));
const publicKeys = Object.fromEntries(
  Object.entries(secrets).map(([id, pair]) => [id, pair.public])
);

// Compute current spatial identity
const files = collectFiles(process.cwd());
const merkleRoot = buildMerkleRoot(files);

const snapshot = {
  version: '1.3',
  merkleRoot,
  threshold: 2,
  publicKeys
};

fs.writeFileSync('./constitution.snapshot.json', JSON.stringify(snapshot, null, 2));
console.log('✓ Snapshot written with merkleRoot:', merkleRoot);
