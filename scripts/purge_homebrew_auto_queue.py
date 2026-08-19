import os
from ftplib import FTP

def remove_dir_recursive(ftp, path):
    lines = []
    try:
        ftp.retrlines(f"LIST {path}", lines.append)
    except Exception as e:
        print(f"Failed to list {path}: {e}")
        return

    for line in lines:
        parts = line.split()
        if not parts:
            continue
        name = parts[-1]
        if name in ['.', '..']:
            continue
        is_dir = line.startswith('d')
        full_path = f"{path.rstrip('/')}/{name}"

        if is_dir:
            remove_dir_recursive(ftp, full_path)
            try:
                ftp.rmd(full_path)
                print(f"[FTP] Removed dir: {full_path}")
            except Exception as e:
                print(f"[FTP] Failed to rmd {full_path}: {e}")
        else:
            try:
                ftp.delete(full_path)
                print(f"[FTP] Deleted file: {full_path}")
            except Exception as e:
                print(f"[FTP] Failed to delete {full_path}: {e}")

    try:
        ftp.rmd(path)
        print(f"[FTP] Removed root dir: {path}")
    except Exception as e:
        print(f"[FTP] Failed to rmd {path}: {e}")

def main():
    ftp = FTP()
    # Try port 1337 (Root FTP) or 2121
    try:
        ftp.connect("192.168.0.208", 1337, timeout=5)
        print("Connected to Root FTP on port 1337!")
    except Exception:
        ftp.connect("192.168.0.208", 2121, timeout=5)
        print("Connected to FTP on port 2121!")
    ftp.login()

    # Targets to delete from /data/homebrew to completely stop auto-install
    targets = [
        "/data/homebrew/PPSA99901-app0", # IT Games
        "/data/homebrew/PPSA99902-app0", # PS5 LiveContainer
        "/data/homebrew/PPSA99903-app0", # DOOM (1993)
    ]

    for target in targets:
        print(f"\n--- Purging {target} ---")
        remove_dir_recursive(ftp, target)

    print("\n[SUCCESS] All auto-queuing homebrew directories purged from /data/homebrew/!")
    ftp.quit()

if __name__ == "__main__":
    main()
