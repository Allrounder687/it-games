import socket
import sys

def scan_ports(host, ports):
    print(f"Scanning debug/log ports on {host}...")
    for p in ports:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(1.0)
        try:
            res = s.connect_ex((host, p))
            if res == 0:
                print(f"[OPEN] Port {p} is ACTIVE!")
            else:
                print(f"[CLOSED] Port {p}")
        except Exception as e:
            print(f"[ERROR] Port {p}: {e}")
        finally:
            s.close()

if __name__ == "__main__":
    host = sys.argv[1] if len(sys.argv) > 1 else "192.168.0.208"
    debug_ports = [2121, 9998, 9020, 9021, 9023, 9025, 1337, 3232, 2811, 2812, 10000, 20000]
    scan_ports(host, debug_ports)
