import fs from 'fs';
import { verifySpatial } from './enclave.mjs';
import { verifyThreshold } from './custodian.mjs';
import { calculateCoupling } from './heartbeat.mjs';

function readJson(f) { return JSON.parse(fs.readFileSync(f, 'utf8')); }

async function main() {
  const snapshot = readJson('./constitution.snapshot.json');
  const signatures = readJson('./constitution.signature.json');
  const telemetry = readJson('./telemetry.json');

  const spatial = verifySpatial(snapshot.merkleRoot);
  if (!spatial.verified) {
    throw new Error(`SPATIAL_LOCK: mismatch (Computed: ${spatial.computedRoot.slice(0,8)})`);
  }
  
  const auth = verifyThreshold(spatial.computedRoot, signatures.sigs, snapshot.publicKeys, snapshot.threshold);
  if (!auth.authorized) {
    throw new Error(`GOVERNANCE_LOCK: quorum_failure`);
  }

  const health = calculateCoupling(telemetry.internal, telemetry.external);
  console.log(`✓ Spatial & Authority Verified. [Health: ${health.status}]`);
  console.log('✓ SYSTEM STATIONARY');
}

main().catch(err => {
  console.error('FATAL:', err.message);
  process.exit(1);
});
