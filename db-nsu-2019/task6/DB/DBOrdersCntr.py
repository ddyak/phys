from DB.DBcontroller import DBcontroller

class DBOrdersController(DBcontroller):
    def __init__(self):
        super().__init__()
        self.cursor = self.conn.cursor()
        self.cursor.execute("""CREATE TABLE IF NOT EXISTS orders(
                            ID          INTEGER     PRIMARY KEY   AUTOINCREMENT,
                            TIME        TIMESTAMP   DEFAULT CURRENT_TIMESTAMP NOT NULL
                            )""")

        self.cursor.execute("""INSERT INTO orders () VALUES 
                        ()""")

