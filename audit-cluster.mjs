import fs from 'fs';
import path from 'path';
import crypto from 'crypto';

const CLUSTER_DIR = './cluster';
const EXCLUDES = new Set(['.git', '.gitmodules', 'node_modules', '.DS_Store']);

function collectFiles(rootDir, dir = rootDir) {
  let results = [];
  if (!fs.existsSync(dir)) return [];
  const entries = fs.readdirSync(dir, { withFileTypes: true });
  for (const entry of entries) {
    if (EXCLUDES.has(entry.name)) continue;
    const fullPath = path.join(dir, entry.name);
    const relativePath = path.relative(rootDir, fullPath).split(path.sep).join('/');
    if (entry.isDirectory()) results = results.concat(collectFiles(rootDir, fullPath));
    else if (entry.isFile()) results.push(relativePath);
  }
  return results.sort();
}

function buildMerkleRoot(rootDir, files) {
  const leafHashes = files.map(f => crypto.createHash('sha256').update('RB_LEAF_V1').update(f).update(Buffer.from([0])).update(fs.readFileSync(path.join(rootDir, f))).digest('hex'));
  if (leafHashes.length === 0) return crypto.createHash('sha256').update('RB_LEAF_V1').digest('hex');
  let layer = leafHashes;
  while (layer.length > 1) {
    const next = [];
    for (let i = 0; i < layer.length; i += 2) {
      const left = layer[i], right = layer[i + 1] || left;
      next.push(crypto.createHash('sha256').update('RB_NODE_V1').update(Buffer.from(left + right, 'hex')).digest('hex'));
    }
    layer = next;
  }
  return layer[0];
}

const repos = fs.existsSync(CLUSTER_DIR) ? fs.readdirSync(CLUSTER_DIR).map(r => path.join(CLUSTER_DIR, r)).filter(p => fs.statSync(p).isDirectory()) : [];
const results = repos.map(p => ({ repo: path.basename(p), merkle: buildMerkleRoot(p, collectFiles(p)) }));
fs.writeFileSync('cluster.audit.json', JSON.stringify(results, null, 2));
console.log('✓ Cluster Audit Complete.');
