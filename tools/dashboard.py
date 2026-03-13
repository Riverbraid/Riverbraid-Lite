from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO
import socket
import threading
import ctypes
import os
from PIL import Image
import numpy as np
from tools.bridge_gateway import RBClusterState
import subprocess

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")
current_state = RBClusterState()

def udp_listener():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("0.0.0.0", 6480))
    while True:
        data, addr = sock.recvfrom(ctypes.sizeof(RBClusterState))
        if len(data) == ctypes.sizeof(RBClusterState):
            ctypes.memmove(ctypes.addressof(current_state), data, len(data))
            payload = {
                "sequence": current_state.sequence,
                "petals": [
                    {"status": current_state.petals[i].status, "hash": bytes(current_state.petals[i].hash).hex()[:8]}
                    for i in range(10)
                ]
            }
            socketio.emit('state_update', payload)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/upload_vision', methods=['POST'])
def upload_vision():
    if 'file' not in request.files: return "No file", 400
    file = request.files['file']
    path = "web_vision.jpg"
    file.save(path)
    # Run the existing ingestor logic as a subprocess
    subprocess.run(["python3", "-m", "tools.ingest_vision", path])
    return jsonify({"status": "Vision Ingested"})

@app.route('/trigger_feedback', methods=['POST'])
def trigger_feedback():
    # Run the neural feedback logic
    subprocess.run(["python3", "-m", "tools.neural_feedback"])
    return jsonify({"status": "Feedback Loop Triggered"})

if __name__ == "__main__":
    threading.Thread(target=udp_listener, daemon=True).start()
    socketio.run(app, host='0.0.0.0', port=5000)
