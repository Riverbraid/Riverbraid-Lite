import ctypes
import hashlib
import os
from google import genai
from google.genai import types
from tools.bridge_gateway import RBClusterState, rb, monitor

def get_ai_decision(image_path, context):
    client = genai.Client(api_key=os.environ.get("GEMINI_API_KEY"))
    
    prompt = f"Analyze this sensory input in the context of Riverbraid State {context}. " \
             "Provide a concise summary of the visual structure and a 32-byte hexadecimal " \
             "intent string for the next cluster action."

    with open(image_path, 'rb') as f:
        image_bytes = f.read()

    response = client.models.generate_content(
        model='gemini-2.0-flash', # Or gemini-3-flash-preview
        contents=[
            types.Part.from_bytes(data=image_bytes, mime_type='image/jpeg'),
            prompt
        ]
    )
    
    print(f"🤖 Gemini Analysis: {response.text[:200]}...")
    # Derive the 32-byte hash from the AI's response text
    return hashlib.sha256(response.text.encode()).digest()

def run_feedback_loop():
    state = RBClusterState()
    if rb.rb_state_load(ctypes.byref(state), b"cluster.bin") != 0:
        print("🚨 Cluster state not found.")
        return

    # Use the last ingested image
    image_path = "web_vision.jpg" if os.path.exists("web_vision.jpg") else "synthetic_vision.jpg"
    
    context = f"Sequence {state.sequence}"
    ai_intent = get_ai_decision(image_path, context)
    
    c_hash = (ctypes.c_uint8 * 32).from_buffer_copy(ai_intent)
    rb.rb_petal_action_commit(ctypes.byref(state), c_hash, 0xA1)
    
    rb.rb_state_save_safe(ctypes.byref(state), b"cluster.bin")
    print(f"✅ Real AI Intent Mined: {ai_intent.hex()[:16]}...")
    monitor(state)

if __name__ == "__main__":
    run_feedback_loop()
