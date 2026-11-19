import serial
import time
import sys

try:
    s = serial.Serial('COM5', 115200, timeout=1)
    s.dtr = False
    s.rts = True
    time.sleep(0.05)
    s.rts = False
    time.sleep(0.2)
    s.dtr = False
    s.rts = False
    print("ESP32-S3 Mini TV running with stable DTR/RTS lines.", flush=True)
    while True:
        try:
            line = s.readline()
            if line:
                decoded = line.decode('utf-8', errors='ignore').strip()
                if decoded.startswith("[HEARTBEAT]") or ">>>" in decoded or "Wi-Fi" in decoded:
                    print(decoded, flush=True)
        except Exception as e:
            time.sleep(0.1)
except Exception as e:
    print(f"Error opening COM5: {e}", flush=True)
