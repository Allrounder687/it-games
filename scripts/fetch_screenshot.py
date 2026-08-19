import os
from ftplib import FTP

def list_files_recursive(ftp, path):
    results = []
    lines = []
    try:
        ftp.retrlines(f"LIST {path}", lines.append)
    except Exception as e:
        return results

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
            results.extend(list_files_recursive(ftp, full_path))
        else:
            try:
                size = int(parts[4]) if len(parts) > 4 and parts[4].isdigit() else 0
            except:
                size = 0
            results.append((full_path, size))
    return results

def main():
    ftp = FTP()
    ftp.connect("192.168.0.208", 2121, timeout=10)
    ftp.login()
    print("Listing files under /user/av_contents via LIST command...")
    
    files = list_files_recursive(ftp, "/user/av_contents")
    print(f"Found {len(files)} files in /user/av_contents:")
    for f, sz in files:
        print(f"  - {f} ({sz} bytes)")

    # Filter image files
    images = [f for f in files if f[0].lower().endswith(('.jpg', '.jpeg', '.png'))]
    if images:
        latest = images[-1][0]
        local_name = "ps5_screen_capture.png" if latest.endswith(".png") else "ps5_screen_capture.jpg"
        print(f"\nDownloading latest screenshot: {latest} -> {local_name}...")
        with open(local_name, "wb") as out:
            ftp.retrbinary(f"RETR {latest}", out.write)
        print("Download complete!")
    else:
        # Also check /data or /user/photo
        print("\nChecking /user/home for media...")
        home_files = list_files_recursive(ftp, "/user/home")
        for f, sz in home_files:
            if f.lower().endswith(('.jpg', '.jpeg', '.png')):
                print(f"  Home image: {f} ({sz} bytes)")

    ftp.quit()

if __name__ == "__main__":
    main()
