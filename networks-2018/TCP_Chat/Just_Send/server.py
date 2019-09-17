import socket
from threading import Thread

def sendMessage(message, clientSockets):

    for clsock in clientSockets:
        clsock.send(bytes(message))
    pass


def reciveMessage(clsock, clientSockets, msg):
    while True:
        msg = clsock.recv(1000)
        if msg:
            sendMessage(msg, clientSockets)
            msg = ''
    pass


clients = {}
clientSockets = []
MAX_CLIENTS = 10
address = ('localhost', 8012)

sendsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(address)
sock.listen(MAX_CLIENTS)


while True:
    conn, addr = sock.accept()
    print(conn)
    data = conn.recv(1000)
    clientSockets.append(conn)
    clients[data] = addr
    print ("client connected from: ", addr)
    conn.send(bytes("connection established!", 'utf8'))
    threadRecv = Thread(target=reciveMessage, args=(conn, clientSockets, 'hello'))
    threadRecv.start()

sock.close()
