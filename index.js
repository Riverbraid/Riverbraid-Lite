const fs = require('fs');

function verifyEnvelope(result) {
  const required = ["repo", "status", "timestamp", "phase", "vector_hash"];
  const missing = required.filter(key => !result.hasOwnProperty(key));
  
  if (missing.length > 0) {
    return { status: "fail", message: `Missing fields: ${missing.join(', ')}` };
  }
  return { status: "pass", message: "Envelope valid" };
}

module.exports = { verifyEnvelope };
