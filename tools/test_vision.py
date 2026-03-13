from tools.bridge_gateway import RBClusterState, rb, monitor, stream_telemetry
import ctypes
import hashlib

def ingest_vision(state, image_path):
    print(f"👁️ Scanning image: {image_path}")
    # In a real app, we'd use PIL/OpenCV here. For now, we hash the file bytes.
    try:
        with open(image_path, "rb") as f:
            digest = hashlib.sha256(f.read()).digest()
    except FileNotFoundError:
        print("⚠️ No image found, using dummy visual noise.")
        digest = hashlib.sha256(b"visual_noise").digest()

    hash_array = (ctypes.c_uint8 * 32).from_buffer_copy(digest)
    
    # Call the C-core Vision Petal update
    rb.rb_petal_vision_update(ctypes.byref(state), hash_array)

if __name__ == "__main__":
    state = RBClusterState()
    state.sequence = 500
    
    ingest_vision(state, "sample_view.jpg")
    monitor(state)
    stream_telemetry(state)
