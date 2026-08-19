import time
import socket
import struct
import sys
from ftplib import FTP

def test_ps5debug(host, port=1337):
    print(f"[ps5debug] Connecting to debug server on {host}:{port}...")
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(3.0)
        s.connect((host, port))
        print("[ps5debug] Connected! ps5debug wire protocol server is LIVE.")
        
        # CMD_PROC_LIST is typically 0xBD000001 or 0x44424701 depending on protocol
        # Send a ping or read banner
        s.close()
    except Exception as e:
        print(f"[ps5debug] Notice: {e}")

def stream_shadowmount_log(host, port=2121, duration_sec=60):
    print(f"\n[LIVE MONITOR] Streaming real-time logs from PS5 (Duration: {duration_sec}s)...")
    print("----------------------------------------------------------------------")
    print(">>> GO TO YOUR PS5 AND TRY LAUNCHING 'IT GAMES' OR PRESSING BUTTONS <<<")
    print("----------------------------------------------------------------------\n")

    ftp = FTP()
    ftp.connect(host, port, timeout=5)
    ftp.login()

    last_size = 0
    # Get initial size
    try:
        last_size = ftp.size("/data/shadowmount/debug.log") or 0
    except Exception:
        last_size = 0

    # Start from last 2KB if file is large
    start_pos = max(0, last_size - 2048) if last_size > 2048 else 0
    current_pos = start_pos

    start_time = time.time()
    while time.time() - start_time < duration_sec:
        try:
            new_size = ftp.size("/data/shadowmount/debug.log") or 0
            if new_size > current_pos:
                chunks = []
                def callback(data):
                    chunks.append(data)
                
                ftp.retrbinary(f"RETR /data/shadowmount/debug.log", callback, rest=current_pos)
                current_pos = new_size
                text = b"".join(chunks).decode("utf-8", errors="replace")
                for line in text.splitlines():
                    if line.strip():
                        print(f"[PS5 LOG] {line.strip()}")
        except Exception as e:
            # Reconnect on transient drop
            try:
                ftp.close()
                ftp.connect(host, port, timeout=5)
                ftp.login()
            except Exception:
                pass
        time.sleep(1.0)

    try:
        ftp.quit()
    except Exception:
        pass
    print("\n[LIVE MONITOR] Session ended.")

if __name__ == "__main__":
    host = sys.argv[1] if len(sys.argv) > 1 else "192.168.0.208"
    duration = int(sys.argv[2]) if len(sys.argv) > 2 else 45
    test_ps5debug(host, 1337)
    stream_shadowmount_log(host, 2121, duration)
