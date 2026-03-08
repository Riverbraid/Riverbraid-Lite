import fs from 'fs';
import { execSync } from 'child_process';

const run = (cmd) => {
  try {
    return execSync(cmd, { stdio: 'pipe' }).toString().trim();
  } catch (e) {
    return `FAIL: ${e.message}`;
  }
};

const loadJson = (f) => fs.existsSync(f) ? JSON.parse(fs.readFileSync(f, 'utf8')) : {};

console.log('\nRiverbraid v1.3 | Unified Status Dashboard');
console.log('==========================================');

console.log('\n[INTERNAL SPINE: Identity & Authority]');
console.log(run('node run-vectors.mjs'));

console.log('\n[EXTERNAL CLUSTER: Audit & Reporting]');
const audit = loadJson('cluster.audit.json');
if (Array.isArray(audit)) {
  audit.forEach(e => console.log(`- ${e.repo.padEnd(20)}: ${e.merkle.slice(0, 12)}...`));
} else {
  console.log('No cluster audit found.');
}

console.log('\n[HISTORICAL LEDGER: Root Anchor]');
if (fs.existsSync('ledger.root')) {
  console.log(`Merkle Root: ${fs.readFileSync('ledger.root', 'utf8')}`);
}

console.log('\n==========================================\n');
