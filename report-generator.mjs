import fs from 'fs';

const loadJson = (f) => fs.existsSync(f) ? JSON.parse(fs.readFileSync(f, 'utf8')) : null;

const audit = loadJson('cluster.audit.json');
const manifest = loadJson('cluster.manifest.json') || {};

if (!audit) {
  console.error('Error: Run "npm run audit" first.');
  process.exit(1);
}

console.log('\nRiverbraid v1.3 Integrity Report');
console.log('--------------------------------');

audit.forEach(entry => {
  const expected = manifest[entry.repo];
  const status = !expected ? 'UNTRACKED' : (expected === entry.merkle ? 'PASS' : 'DRIFT_DETECTED');
  const color = status === 'PASS' ? '✓' : '✗';
  
  console.log(`${color} ${entry.repo.padEnd(25)} | ${status.padEnd(15)} | ${entry.merkle.slice(0, 12)}...`);
});

console.log('--------------------------------\n');
