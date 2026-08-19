import os
import sys
from ftplib import FTP

def upload_directory(ftp, local_dir, remote_dir):
    print(f"[FTP] Ensuring remote directory: {remote_dir}")
    try:
        ftp.mkd(remote_dir)
    except Exception:
        # Directory may already exist
        pass

    for item in os.listdir(local_dir):
        local_path = os.path.join(local_dir, item)
        remote_path = f"{remote_dir}/{item}"

        if os.path.isdir(local_path):
            upload_directory(ftp, local_path, remote_path)
        else:
            print(f"[FTP] Uploading: {item} -> {remote_path} ({os.path.getsize(local_path)} bytes)")
            with open(local_path, "rb") as f:
                ftp.storbinary(f"STOR {remote_path}", f)

def main():
    host = sys.argv[1] if len(sys.argv) > 1 else "192.168.0.208"
    port = int(sys.argv[2]) if len(sys.argv) > 2 else 2121
    local_app_dir = sys.argv[3] if len(sys.argv) > 3 else "app_pkg/ITGA00001-app0"
    remote_base = "/data/homebrew/ITGA00001-app0"

    print(f"Connecting to PS5 FTP at {host}:{port}...")
    ftp = FTP()
    ftp.connect(host, port, timeout=10)
    ftp.login()
    print("Connected successfully!")

    upload_directory(ftp, local_app_dir, remote_base)

    ftp.quit()
    print("\n[SUCCESS] IT Games PS5 Homebrew App successfully deployed to /data/homebrew/ITGA00001-app0!")

if __name__ == "__main__":
    main()
