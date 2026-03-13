import ctypes
import hashlib
from tools.bridge_gateway import RBClusterState, rb, monitor

def generate_ai_intent(context_str):
    print(f"🤖 AI is contemplating: '{context_str}'")
    intent_hash = hashlib.sha256(context_str.encode()).digest()
    return intent_hash

def run_feedback_loop():
    state = RBClusterState()
    if rb.rb_state_load(ctypes.byref(state), b"cluster.bin") != 0:
        print("🚨 No cluster state found.")
        return

    # Improved context extraction
    active_sensory = []
    if state.petals[0].status == 1: active_sensory.append("Audio")
    if state.petals[1].status == 1: active_sensory.append("Vision")
    
    context = f"Seq {state.sequence}. Sensory input: {', '.join(active_sensory) if active_sensory else 'None'}."
    ai_intent = generate_ai_intent(context)
    
    c_hash = (ctypes.c_uint8 * 32).from_buffer_copy(ai_intent)
    rb.rb_petal_action_commit(ctypes.byref(state), c_hash, 0xA1)
    
    rb.rb_state_save_safe(ctypes.byref(state), b"cluster.bin")
    print(f"✅ Intent Committed to Petal 8: {ai_intent.hex()[:16]}...")
    monitor(state)

if __name__ == "__main__":
    run_feedback_loop()
