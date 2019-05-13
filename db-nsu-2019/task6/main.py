from DB.DBcontroller import DBcontroller, Role

class User:
    def __init__(self, role, login, password, firstName=None, secondName=None, phone=None, **kwargs):
        self.role = role
        self.login = login
        self.password = password
        self.firstName = firstName
        self.secondName = secondName
        self.phone = phone


if __name__ == "__main__":
    ddyak5 = User(role=Role.courier, login='ddyakovlev', password='hello', firstName='Che', secondName='Lal', phone='8')
    ddyak = User(role=Role.client, login='ddyakovlev', password='hello')

    db = DBcontroller()
    db.add_user(ddyak)
    db.add_address(14, 'Pirogova')
    db.get_user(Role.client)