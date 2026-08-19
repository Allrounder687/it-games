import sqlite3

con = sqlite3.connect('itemzflow_db.sqlite')
cur = con.cursor()

tables = cur.execute("SELECT name FROM sqlite_master WHERE type='table';").fetchall()
print("Tables in Itemzflow DB:", tables)

for (t,) in tables:
    print(f"\n--- Table: {t} ---")
    schema = cur.execute(f"PRAGMA table_info({t});").fetchall()
    print("Columns:", [col[1] for col in schema])
    rows = cur.execute(f"SELECT * FROM {t};").fetchall()
    for r in rows:
        print("Row:", r)
