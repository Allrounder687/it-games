import sqlite3
import sys
from ftplib import FTP

def sync_itemzflow_db(host="192.168.0.208", port=2121):
    print(f"Connecting to PS5 FTP at {host}:{port}...")
    ftp = FTP()
    ftp.connect(host, port, timeout=10)
    ftp.login()

    local_db = "PS5_Apps.db"
    print("Downloading /data/itemzflow/PS5_Apps.db...")
    with open(local_db, "wb") as f:
        ftp.retrbinary("RETR /data/itemzflow/PS5_Apps.db", f.write)

    conn = sqlite3.connect(local_db)
    c = conn.cursor()

    # Clean stale entries
    c.execute("DELETE FROM itemzflow_ps5_games WHERE TID = 'ITGA00001'")

    # Ensure PPSA99901 exists
    c.execute("""
        INSERT OR REPLACE INTO itemzflow_ps5_games (TID, Title, GM_PATH)
        VALUES ('PPSA99901', 'IT Games', '/data/homebrew/PPSA99901-app0')
    """)
    conn.commit()

    c.execute("SELECT * FROM itemzflow_ps5_games")
    print("Updated Itemzflow games:", c.fetchall())
    conn.close()

    print("Uploading updated PS5_Apps.db to PS5...")
    with open(local_db, "rb") as f:
        ftp.storbinary("STOR /data/itemzflow/PS5_Apps.db", f)

    print("[SUCCESS] Itemzflow database synced successfully!")
    ftp.quit()

if __name__ == "__main__":
    sync_itemzflow_db()
