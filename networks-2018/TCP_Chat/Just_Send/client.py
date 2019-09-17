import socket
import sys
from threading import Thread

def reciveMessage(sock, message):
    while True:
        message = sock.recv(1000)
        print(message)


serverAddress = ('localhost', 8012)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(serverAddress)
nickname = raw_input()
s.send(bytes(nickname).encode('utf-8'))
data = s.recv(1000)
recvThread = Thread(target=reciveMessage, args=(s, 'hello'))
recvThread.start()
print ('received', data, len(data), 'bytes')

while True:
    nickname = raw_input()
    s.send(bytes(nickname).encode('utf-8'))

s.close()