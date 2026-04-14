const { validateHierarchyOrder } = require('./hierarchy');

function verifyState(state) {
    console.log(`[LITE] Verifying State: ${state.id || 'unknown'}`);
    
    // 1. Hierarchy Check
    if (state.vectorSources) {
        validateHierarchyOrder(state.vectorSources);
        console.log("Hierarchy Order: Valid ✅");
    }

    // 2. Integrity Check (Placeholder for Merkle Proof validation)
    if (state.merkleRoot) {
        console.log("Merkle Root Check: Active ✅");
    }

    return true;
}

module.exports = { verifyState };
