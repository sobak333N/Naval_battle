import socket
import os

print('I STARTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT')


socket_path = '/tmp/unix_error1.server'
try:
    print('error1 ' , socket_path)
    os.unlink(socket_path)
except OSError:
    if os.path.exists(socket_path):
        raise
server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
server.bind(socket_path)
server.listen(1)
print('Server is listening for incoming connections...')
connection, client_address = server.accept()
try:
    # print('Connection from', client_address)
    # while True:
    text = '_'
    while True and text[-1]!='F':
        data = connection.recv(1024)
        if not data:
            break
        print('tmp data:', data.decode())
    # if not data:
        # break
        text += data.decode()
        
    print('Received data:',text)
    answer = '''<html>
<head><title>GAME STARTED!</title></head>
<body>\n'''
    # left = 0 
    # right = 0
    if text[-2] == 'A':
        answer+='<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">U ALLREADY PLAY WITH THIS PLAYER'
        answer+= '</a>'
        answer+='<br>'
    if text[-2] == 'B':
        answer+='<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">THIS PLAYER IS ALLREADY IN GAME WITH ANOTHER PLAYER . WAIT FOR THEIR END</a><br>'
    if text[-2] == 'C':
        answer+='<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">U ALLREADY PLAY WITH ANOTHER PLAYER</a><br>'
    if text[-2] == 'D':
        answer+='<a style="font-family: Arial, sans-serif; font-size: 18px; font-weight: bold; color: blue;">U CANT START GAME WITH YOURSELF !</a><br>'


    answer+='</pre><hr></body>\n</html>\n'
    print('html code', answer)

    # response = 'Hello from the server!'
    connection.sendall(answer.encode())
    # connection.close()
    # os.unlink(socket_path)
finally:
    connection.close()
    os.unlink(socket_path)