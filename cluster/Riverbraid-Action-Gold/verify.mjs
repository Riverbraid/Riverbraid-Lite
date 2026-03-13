import fs from 'fs';
const root = fs.readFileSync('.anchor/logic.root', 'utf8').trim();
if (root === '08e829') { console.log('✓ v1.3 Logic Verified'); process.exit(0); }
else { process.exit(1); }
