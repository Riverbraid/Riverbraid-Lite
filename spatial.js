const fs = require('fs');
const crypto = require('crypto');
const path = require('path');

function computeSpatialHash(basePath) {
  const repos = fs.readdirSync(basePath).filter(file => 
    fs.statSync(path.join(basePath, file)).isDirectory() && file.startsWith('Riverbraid')
  ).sort();
  
  const hash = crypto.createHash('sha256');
  hash.update(repos.join('|'));
  return hash.digest('hex');
}

module.exports = { computeSpatialHash };
