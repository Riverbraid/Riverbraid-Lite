import ctypes
from fastapi import FastAPI
from pydantic import BaseModel
from fastapi.responses import FileResponse

app = FastAPI()

class RBState(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("step", ctypes.c_uint64),
        ("action", ctypes.c_uint8),
        ("padding", ctypes.c_uint8 * 7),
        ("hmac_sig", ctypes.c_uint8 * 32),
    ]

class Pulse(BaseModel):
    audio_signal: int
    vision_frame: list[int]

state = RBState()
core = ctypes.CDLL("./build/core_build/libriverbraid_core.so")
core.rb_core_seal_state.restype = ctypes.c_uint8

current_key = (ctypes.c_uint8 * 32)(*[0xDE, 0xAD, 0xBE, 0xEF] + [0]*28)
master_key = (ctypes.c_uint8 * 32)(*[0xAA, 0xBB] + [0]*30)

@app.get("/")
def index():
    return FileResponse("index.html")

@app.post("/pulse")
def pulse(p: Pulse):
    status_input = 1 if p.audio_signal > 255 else 0
    core.rb_core_seal_state(ctypes.byref(state), status_input, current_key)
    return {
        "status": "LOCKED" if state.action == 0xFF else "SEALED",
        "step": state.step
    }

@app.post("/reset")
def reset():
    core.rb_core_seal_state(ctypes.byref(state), 0, master_key)
    return {
        "status": "LOCKED" if state.action == 0xFF else "SEALED",
        "step": state.step
    }