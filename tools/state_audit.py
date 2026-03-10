import serial, csv
from datetime import datetime

PORT, BAUD = '/dev/ttyACM0', 115200

def run_audit():
    print(f"[*] Monitoring {PORT}...")
    try:
        with serial.Serial(PORT, BAUD, timeout=1) as ser:
            with open('state_log.csv', 'a', newline='') as f:
                writer = csv.writer(f)
                if f.tell() == 0:
                    writer.writerow(['Timestamp', 'Step', 'Action', 'HMAC'])
                while True:
                    line = ser.readline().decode('utf-8', errors='ignore').strip()
                    if line.startswith("STEP:"):
                        p = line.split()
                        s, a, h = p[0].split(':')[1], p[1].split(':')[1], p[2].split(':')[1]
                        writer.writerow([datetime.now().isoformat(), s, a, h])
                        f.flush()
                        print(f"[LOGGED] Step {s} | Action {a}")
    except Exception as e:
        print(f"[!] Error: {e}")

if __name__ == "__main__":
    run_audit()
