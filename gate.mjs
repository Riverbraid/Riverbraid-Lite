import { spawn, execSync } from 'child_process';

function verify() {
  try {
    console.log('[Riverbraid Gate] Executing Triple-Lock Verification...');
    // Execute the verifier. Fail-closed on any non-zero exit code.
    execSync('node run-vectors.mjs', { stdio: 'inherit', timeout: 15000 });
    return true;
  } catch (e) {
    return false;
  }
}

const args = process.argv.slice(2);
if (args.length === 0) {
  console.error('Usage: node gate.mjs <command> [args...]');
  process.exit(1);
}

if (verify()) {
  console.log('[Riverbraid Gate] AUTH_SUCCESS. Launching capability layer...\n');
  const child = spawn(args[0], args.slice(1), { stdio: 'inherit' });
  child.on('exit', (code) => process.exit(code ?? 0));
} else {
  console.error('\n[Riverbraid Gate] AUTH_FAILURE. Execution blocked by Control Spine.');
  process.exit(1);
}
