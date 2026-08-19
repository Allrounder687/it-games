import socket
import struct
import sys

PACKET_MAGIC = 0xFFAABBCC
CMD_VERSION = 0xBD000001
CMD_PROC_LIST = 0xBDAA0001
CMD_PROC_READ = 0xBDAA0003
CMD_CONSOLE_REBOOT = 0xBD000003
CMD_CONSOLE_NOTIFY = 0xBD000004
CMD_CONSOLE_PRINT = 0xBD000005

def send_cmd(s, cmd, data=b""):
    hdr = struct.pack("<III", PACKET_MAGIC, cmd, len(data))
    s.sendall(hdr + data)
    
    resp_hdr = s.recv(8)
    if len(resp_hdr) < 8:
        print("[ps5debug] Incomplete response header:", resp_hdr)
        return None
    magic, status = struct.unpack("<II", resp_hdr)
    if status != 0x40000000: # CMD_SUCCESS
        print(f"[ps5debug] Command failed with status 0x{status:08X}")
        return None
    
    # Read payload length
    len_bytes = s.recv(4)
    if len(len_bytes) == 4:
        length = struct.unpack("<I", len_bytes)[0]
        payload = b""
        while len(payload) < length:
            chunk = s.recv(min(4096, length - len(payload)))
            if not chunk:
                break
            payload += chunk
        return payload
    return b""

def main():
    host = sys.argv[1] if len(sys.argv) > 1 else "192.168.0.208"
    port = int(sys.argv[2]) if len(sys.argv) > 2 else 1337

    print(f"Connecting to ps5debug at {host}:{port}...")
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5.0)
    try:
        s.connect((host, port))
        print("Connected! Querying process list...")

        # Try proc list
        hdr = struct.pack("<III", PACKET_MAGIC, CMD_PROC_LIST, 0)
        s.sendall(hdr)

        res = s.recv(4)
        if len(res) == 4:
            count = struct.unpack("<I", res)[0]
            print(f"Running Processes on PS5: {count}")
            # Each entry is 32 bytes name + 4 bytes pid
            for i in range(count):
                entry = s.recv(36)
                if len(entry) == 36:
                    name = entry[:32].decode('utf-8', errors='replace').rstrip('\0')
                    pid = struct.unpack("<I", entry[32:36])[0]
                    print(f"  PID {pid:4d}: {name}")
    except Exception as e:
        print(f"Debug client error: {e}")
    finally:
        s.close()

if __name__ == "__main__":
    main()
