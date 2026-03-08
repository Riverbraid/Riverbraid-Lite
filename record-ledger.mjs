import fs from 'fs';
import path from 'path';
import crypto from 'crypto';

const LEDGER_DIR = './audit-ledger';
if (!fs.existsSync('cluster.audit.json')) process.exit(1);

const auditData = fs.readFileSync('cluster.audit.json', 'utf8');
const timestamp = new Date().toISOString().replace(/[:.]/g, '-');
const logFile = path.join(LEDGER_DIR, `audit_${timestamp}.json`);

fs.writeFileSync(logFile, auditData);
console.log(`✓ Audit recorded to ${logFile}`);

// Update Ledger Root
const logs = fs.readdirSync(LEDGER_DIR).filter(f => f.endsWith('.json')).sort();
const logHashes = logs.map(f => crypto.createHash('sha256').update(fs.readFileSync(path.join(LEDGER_DIR, f))).digest('hex'));

let layer = logHashes;
while (layer.length > 1) {
  const next = [];
  for (let i = 0; i < layer.length; i += 2) {
    const left = layer[i], right = layer[i + 1] || left;
    next.push(crypto.createHash('sha256').update(left + right).digest('hex'));
  }
  layer = next;
}

const root = layer[0] || 'empty';
fs.writeFileSync('ledger.root', root);
console.log(`✓ Ledger Merkle Root Updated: ${root.slice(0, 12)}...`);
