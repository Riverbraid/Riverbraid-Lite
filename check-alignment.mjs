import fs from 'fs';
import path from 'path';

const EXPECTED_VERSION = '1.3.0';
const CLUSTER_DIR = './cluster';

function checkRepo(repoName) {
  const pkgPath = path.join(CLUSTER_DIR, repoName, 'package.json');
  if (!fs.existsSync(pkgPath)) return { status: 'MISSING', version: 'N/A' };
  
  try {
    const pkg = JSON.parse(fs.readFileSync(pkgPath, 'utf8'));
    const versionMatch = pkg.version === EXPECTED_VERSION;
    return { 
      status: versionMatch ? 'ALIGNED' : 'VERSION_MISMATCH', 
      version: pkg.version 
    };
  } catch (e) {
    return { status: 'CORRUPT', version: 'N/A' };
  }
}

const clusters = fs.readdirSync(CLUSTER_DIR).filter(f => 
  fs.statSync(path.join(CLUSTER_DIR, f)).isDirectory()
);

console.log(`\nRiverbraid v1.3 | Cluster Alignment Check`);
console.log(`Target Version: ${EXPECTED_VERSION}`);
console.log('------------------------------------------');

let allAligned = true;
clusters.forEach(cluster => {
  const result = checkRepo(cluster);
  const icon = result.status === 'ALIGNED' ? '✓' : '⚠';
  if (result.status !== 'ALIGNED') allAligned = false;
  
  console.log(`${icon} ${cluster.padEnd(25)} | ${result.status.padEnd(16)} | v${result.version}`);
});

console.log('------------------------------------------');
if (allAligned) {
  console.log('✓ ALL SYSTEMS ALIGNED: The Riverbraid is whole.\n');
  process.exit(0);
} else {
  console.log('⚠ ALIGNMENT DRIFT DETECTED: Review cluster versions.\n');
  process.exit(1);
}
