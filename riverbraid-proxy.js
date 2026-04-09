#!/usr/bin/env node
const express = require('express');
const crypto = require('crypto');
const fs = require('fs');
const axios = require('axios');

const app = express();
app.use(express.json());

const CONSTITUTION_PATH = process.env.CONSTITUTION_PATH || './constitution.threshold.json';
const STATE_FILE = process.env.STATE_FILE || './proxy.state.json';
const LLM_ENDPOINT = process.env.LLM_ENDPOINT || 'https://api.openai.com/v1/chat/completions';
const LLM_API_KEY = process.env.LLM_API_KEY;
const PORT = process.env.PORT || 8080;

if (!fs.existsSync(CONSTITUTION_PATH)) {
    fs.writeFileSync(CONSTITUTION_PATH, JSON.stringify({
        threshold: 0,
        keys: [],
        genesis_root: "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
    }, null, 2));
}

const constitution = JSON.parse(fs.readFileSync(CONSTITUTION_PATH, 'utf8'));
let state = { step: 0, root: constitution.genesis_root };
if (fs.existsSync(STATE_FILE)) state = JSON.parse(fs.readFileSync(STATE_FILE, 'utf8'));

function saveState() { fs.writeFileSync(STATE_FILE, JSON.stringify(state, null, 2)); }
function sha256(str) { return crypto.createHash('sha256').update(str, 'utf8').digest('hex'); }

app.post('/v1/chat/completions', async (req, res) => {
    try {
        const delta_hash = sha256(JSON.stringify(req.body, Object.keys(req.body).sort()));
        const response = await axios.post(LLM_ENDPOINT, req.body, {
            headers: { 'Authorization': `Bearer ${LLM_API_KEY}`, 'Content-Type': 'application/json' }
        });
        
        const llmResponse = response.data;
        const newRoot = sha256(state.root + delta_hash + (state.step + 1).toString());
        
        state.step++;
        state.root = newRoot;
        saveState();
        res.json(llmResponse);
    } catch (err) {
        res.status(500).json({ error: 'PROXY_ERROR', detail: err.message });
    }
});

app.listen(PORT, () => console.log(`Riverbraid Lite listening on ${PORT}`));
