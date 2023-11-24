import socket
import os

print('I STARTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT')


socket_path = '/tmp/unix_chat.server'
try:
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
    while True:
        data = connection.recv(1024)
        if not data:
            break
        print('Received data:', data.decode())
        text = data.decode()
        answer = '''<html>
<head><title>U HAVE NEW MESSAGE</title></head>
<body>\n'''
        left = 0 
        right = 0
        while right < len(text):
            if text[right] == '/':
                answer+= '<a>'
                answer+= 'MESSAGE:'
                answer+=text[left:right]
                answer+='/</a>\n'
                right+=1
                left = right
            else:
                right+=1

        answer+= '<a>'
        answer+= 'FROM:'
        answer+=text[left:right]
        answer+='/</a>\n'
        answer+='</pre><hr></body>\n</html>\n'
        # print('html code', answer)

        # response = 'Hello from the server!'
        connection.sendall(answer.encode())
finally:
    connection.close()
    os.unlink(socket_path)