import sqlite3
import pandas as pd
import random
import time

def show_inf():
    print("1 - сделать вывод нескольких записей")
    print("2 - создать 1к записей")
    print("3 - начать спамить данными")
    print("4 - начать сильно спамить данными")
    print("Ввод: ")

def generate_random_record():
    x = round(random.uniform(-100, 100), 2)
    y = round(random.uniform(-100, 100), 2)
    z = round(random.uniform(-100, 100), 2)
    Vx = round(random.uniform(-100, 100), 2)
    Vy = round(random.uniform(-100, 100), 2)
    Vz = round(random.uniform(-100, 100), 2)
    return x, y, z, Vx, Vy, Vz

def reset_table(conn):
    cursor = conn.cursor()
    cursor.execute("DELETE FROM db")
    cursor.execute("DELETE FROM sqlite_sequence WHERE name='db'")
    conn.commit()
    print("Таблица очищена, счётчик id сброшен")

def create_1000_records(conn, reset_first=True):
    if reset_first:
        reset_table(conn)

    cursor = conn.cursor()
    records = [generate_random_record() for _ in range(1000)]
    cursor.executemany("""
    INSERT INTO db (x, y, z, Vx, Vy, Vz)
    VALUES (?, ?, ?, ?, ?, ?)
    """, records)
    conn.commit()
    print("Создано 1к рандомных записей с id от 1 до 1000")

def get_all_ids(conn):
    cursor = conn.cursor()
    cursor.execute("SELECT id FROM db")
    return [row[0] for row in cursor.fetchall()]

def update_batch_records(conn, ids, min_count=100, max_count=200):
    if not ids:
        print("Таблица пуста — сначала создайте записи (пункт 2)")
        return

    batch_size = random.randint(min_count, min(max_count, len(ids)))
    selected_ids = random.sample(ids, batch_size)

    cursor = conn.cursor()
    updates = []
    for record_id in selected_ids:
        x, y, z, Vx, Vy, Vz = generate_random_record()
        updates.append((x, y, z, Vx, Vy, Vz, record_id))

    cursor.executemany("""
        UPDATE db
        SET x = ?, y = ?, z = ?, Vx = ?, Vy = ?, Vz = ?
        WHERE id = ?
    """, updates)
    conn.commit()
    print(f"Обновлено {batch_size} записей")

def spam_data(conn, is_hard, delay=0.5):
    print("Старт спама. Нажмите Ctrl+C для остановки.")
    ids = get_all_ids(conn)
    if not ids:
        print("Таблица пуста — сначала создайте записи (пункт 2)")
        return

    try:
        while True:
            if(is_hard):
                update_batch_records(conn, ids, min_count=300, max_count=700)
            else:
                update_batch_records(conn, ids, min_count=100, max_count=200)
            time.sleep(delay)
    except KeyboardInterrupt:
        print("\nСпам остановлен")

def round_existing_records(conn):
    cursor = conn.cursor()
    cursor.execute("SELECT id, x, y, z, Vx, Vy, Vz FROM db")
    rows = cursor.fetchall()

    for row in rows:
        record_id, x, y, z, Vx, Vy, Vz = row
        cursor.execute("""
            UPDATE db SET x=?, y=?, z=?, Vx=?, Vy=?, Vz=?
            WHERE id=?
        """, (round(x, 2), round(y, 2), round(z, 2),
              round(Vx, 2), round(Vy, 2), round(Vz, 2), record_id))

    conn.commit()
    print(f"Округлено {len(rows)} существующих записей")

path_DB_GLOBAL = ""
def read_path_DB():
    with open('pathDB.txt', 'r', encoding='utf-8') as f:
        return f.read().strip()

def main():
    path_DB_GLOBAL = read_path_DB()
    conn = sqlite3.connect(path_DB_GLOBAL)


    while True:
        show_inf()
        i = int(input())
        if i == 1:
            df = pd.read_sql_query("SELECT * FROM db", conn)
            print(df.head(30))
            break
        elif i == 2:
            create_1000_records(conn, reset_first=True)
            break
        elif i == 3:
            spam_data(conn, False, delay=1)
            break
        elif i == 4:
            spam_data(conn, True, delay=0.3)
            break;
        else:
            conn.close()
            break

if __name__ == '__main__':
    main()

if __name__ == '__main__':
    main()