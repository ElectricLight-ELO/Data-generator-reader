import sqlite3
import pandas as pd

def show_inf():
    print("1 - сделать вывод нескольких записей")
    print("2 - создать 1к записей")
    print("3 - начать спамить данными")
    print("Ввод: ")


path_DB_GLOBAL = ""
def read_path_DB():
    str_path = ""
    with open('pathDB.txt', 'r', encoding='utf-8') as f:
        return f.read()

def main():
    path_DB_GLOBAL = read_path_DB()
    conn = sqlite3.connect(path_DB_GLOBAL)

    while(True):
        show_inf()
        i = int(input())
        if(i == 1):
            # показываем тестовые
            df = pd.read_sql_query("SELECT * FROM db", conn)
            print(df.head())
            break
        elif (i == 2):
            print("создано 1к рандомных записей")

            break

        elif (i == 3):
            print("старт спама")

            break
        else:
            conn.close()


if __name__ == '__main__':
    main()
