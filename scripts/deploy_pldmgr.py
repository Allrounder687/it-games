import json
from ftplib import FTP

def main():
    ftp = FTP()
    ftp.connect("192.168.0.208", 2121)
    ftp.login()
    print("Connected to PS5 FTP.")

    # 1. Create directory /data/pldmgr/payloads/it_games
    try:
        ftp.mkd("/data/pldmgr/payloads/it_games")
        print("Created directory: /data/pldmgr/payloads/it_games")
    except Exception as e:
        print(f"Directory info: {e}")

    # 2. Generate it_games.elf.json manifest
    manifest = {
        "name": "IT Games",
        "filename": "it_games.elf",
        "description": "Native PS5 FreeShop and Homebrew Game Manager.",
        "version": "1.0.0",
        "category": "Games & Homebrew",
        "author": "Achie & Antigravity"
    }
    with open("it_games.elf.json", "w", encoding="utf-8") as f:
        json.dump(manifest, f, indent=2)

    # 3. Upload it_games.elf
    print("Uploading it_games.elf...")
    with open("it_games.elf", "rb") as f:
        ftp.storbinary("STOR /data/pldmgr/payloads/it_games/it_games.elf", f)

    # 4. Upload it_games.elf.json
    print("Uploading it_games.elf.json...")
    with open("it_games.elf.json", "rb") as f:
        ftp.storbinary("STOR /data/pldmgr/payloads/it_games/it_games.elf.json", f)

    # 5. Clean up loose /data/pldmgr/payloads/it_games.elf
    try:
        ftp.delete("/data/pldmgr/payloads/it_games.elf")
        print("Cleaned up loose root file /data/pldmgr/payloads/it_games.elf")
    except Exception:
        pass

    print("\n[SUCCESS] IT Games PayloadManager bundle successfully deployed!")
    ftp.quit()

if __name__ == "__main__":
    main()
