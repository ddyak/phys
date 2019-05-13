from DB.DBcontroller import DBcontroller
from DB.DBRole import Role


class DBUsersController(DBcontroller):
    def __init__(self):
        super().__init__()
        self.cursor = self.conn.cursor()
        self.cursor.execute("""CREATE TABLE IF NOT EXISTS clients(
                            ID          INTEGER     PRIMARY KEY   AUTOINCREMENT,
                            LOGIN       TEXT        UNIQUE  NOT NULL ,
                            PASSWORD    TEXT        NOT NULL,
                            FIRSTNAME   TEXT,
                            LASTNAME    TEXT,
                            PHONE       TEXT
                            )""")

        self.cursor.execute("""CREATE TABLE IF NOT EXISTS couriers(
                            ID          INTEGER     PRIMARY KEY   AUTOINCREMENT,
                            LOGIN       TEXT        UNIQUE  NOT NULL ,
                            PASSWORD    TEXT        NOT NULL,
                            FIRSTNAME   TEXT        NOT NULL,
                            LASTNAME    TEXT        NOT NULL,
                            PHONE       TEXT        NOT NULL
                            )""")

    def add_user(self, user):
        if (user.role == Role.client):
            sql = "INSERT INTO clients (LOGIN, PASSWORD, FIRSTNAME, LASTNAME, PHONE) VALUES (?, ?, ?, ?, ?)"
        else:
            sql = "INSERT INTO couriers (LOGIN, PASSWORD, FIRSTNAME, LASTNAME, PHONE) VALUES (?, ?, ?, ?, ?)"
        try:
            self.cursor.execute(sql, (user.login, user.password, user.firstName, user.secondName, user.phone))
        except Exception:
            print("Некорректный ввод, нарушена целостность данных")

        self.conn.commit()

    def get_user(self, role):
        if (role == Role.client):
            self.cursor.execute("""SELECT * FROM clients""")
        else:
            self.cursor.execute("""SELECT * FROM couriers""")
        rows = self.cursor.fetchall()
        for row in rows:
            print(row)
