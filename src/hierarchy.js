const HIERARCHY = [
    'Refusal', 'Safety', 'Judicial', 'Crypto', 'Cognition', 
    'Temporal', 'Manifest', 'Action', 'Integration', 'Memory', 
    'Interface', 'Network', 'Vision', 'Audio'
];

function validateHierarchyOrder(stateSources) {
    for (let i = 0; i < stateSources.length - 1; i++) {
        const currentRank = HIERARCHY.indexOf(stateSources[i]);
        const nextRank = HIERARCHY.indexOf(stateSources[i+1]);
        if (currentRank === -1 || nextRank === -1 || currentRank > nextRank) {
            throw new Error(`LITE_INTEGRITY: Hierarchy violation or unknown source.`);
        }
    }
    return true;
}
module.exports = { HIERARCHY, validateHierarchyOrder };
