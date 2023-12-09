import socket
import os

print('I STARTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT')


socket_path = '/tmp/unix_link.server'
try:
    print('new_link ' , socket_path)
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
    answer= '''<html>
<head><title>LINK</title></head>
<body bgcolor="#333">\n'''
    # left = 0 
    # right = 0
    left = 0
    right = 0
    while text[right]!=' ':
        right+=1

    host = text[left+1:right]
    host = host[:-2] + '/'
    left = right
    right+=1

    while text[right]!='F':
        right+=1

    name = text[left+1:right]
    answer+='<a style="font-family: Arial, sans-serif; font-size: 20px; font-weight: bold; color: rgba(238, 234, 9, 0.808);">There is your link !</a><br>\n'
    answer+='<a style="font-family: Arial, sans-serif; font-size: 16px; font-weight: bold; color: rgba(238, 234, 9, 0.808);">TO START GAME WITH SOMEONE ENTER HIS NAME IN FIRST FIELD AND "start" IN SECOND FIELD</a><br>\n'
    answer+='<a style="font-family: Arial, sans-serif; font-size: 16px; font-weight: bold; color: rgba(238, 234, 9, 0.808);">TO DO MOVE ENTER NAME OF YOUR OPPONENT AND "m00" WHER 00 ARE COORDINATES</a><br>\n'
    answer+='<a style="font-family: Arial, sans-serif; font-size: 16px; font-weight: bold; color: rgba(238, 234, 9, 0.808);">TO CHAT WITH SOMEONE ENTER NAME AND MESSAGE</a><br>\n'
    answer+='<p><a href='
    # answer+=host
    answer+='/'
    answer+=name
    answer+='>YOUR LINK!</a></p><br>\n'

    answer+='</pre><hr></body>\n</html>\n'
    print('html code', answer)

    # response = 'Hello from the server!'
    connection.sendall(answer.encode())
    # connection.close()
    # os.unlink(socket_path)
finally:
    connection.close()
    os.unlink(socket_path)