import fs from 'fs';
import path from 'path';
import crypto from 'crypto';

const ROOT_DIR = process.cwd();
const EXCLUDES = new Set([
  '.git', '.gitmodules', 'node_modules', '.rbtmp',
  'custodian_keys_secrets.json', 'constitution.snapshot.json',
  'constitution.signature.json', 'telemetry.json', '.DS_Store'
]);

export function collectFiles(dir) {
  let results = [];
  const entries = fs.readdirSync(dir, { withFileTypes: true });
  for (const entry of entries) {
    if (EXCLUDES.has(entry.name)) continue;
    const fullPath = path.join(dir, entry.name);
    const relativePath = path.relative(ROOT_DIR, fullPath).split(path.sep).join('/');
    if (entry.isDirectory()) results = results.concat(collectFiles(fullPath));
    else if (entry.isFile()) results.push(relativePath);
  }
  return results.sort((a, b) => a.localeCompare(b, 'en'));
}

function hashLeaf(relativePath) {
  const bytes = fs.readFileSync(path.join(ROOT_DIR, relativePath));
  return crypto.createHash('sha256')
    .update('RB_LEAF_V1')
    .update(relativePath)
    .update(Buffer.from([0]))
    .update(bytes)
    .digest('hex');
}

export function buildMerkleRoot(files) {
  const leafHashes = files.map(hashLeaf);
  if (leafHashes.length === 0) return crypto.createHash('sha256').update('RB_LEAF_V1').digest('hex');
  let layer = leafHashes;
  while (layer.length > 1) {
    const nextLayer = [];
    for (let i = 0; i < layer.length; i += 2) {
      const left = layer[i];
      const right = layer[i + 1] || left;
      nextLayer.push(crypto.createHash('sha256')
        .update('RB_NODE_V1')
        .update(Buffer.from(left + right, 'hex'))
        .digest('hex'));
    }
    layer = nextLayer;
  }
  return layer[0];
}

export function verifySpatial(expectedRoot) {
  const files = collectFiles(ROOT_DIR);
  const computedRoot = buildMerkleRoot(files);
  return { verified: computedRoot === expectedRoot, computedRoot, files };
}
