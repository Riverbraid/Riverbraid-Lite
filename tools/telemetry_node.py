import socket
import ctypes
from bridge_gateway import RBClusterState

def start_receiver(port=6480):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("0.0.0.0", port))
    print(f"📡 Telemetry Node Listening on port {port}...")

    state = RBClusterState()
    
    try:
        while True:
            data, addr = sock.recvfrom(ctypes.sizeof(RBClusterState))
            if len(data) == ctypes.sizeof(RBClusterState):
                ctypes.memmove(ctypes.addressof(state), data, len(data))
                print(f"📥 Received State | Seq: {state.sequence} | From: {addr[0]}")
    except KeyboardInterrupt:
        print("\n🛑 Receiver offline.")

if __name__ == "__main__":
    start_receiver()
